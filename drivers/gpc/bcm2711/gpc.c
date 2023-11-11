#include "gpc.h"
#include "gpio/bcm2711/gpio.h"

#define MIN_SAMPLE_COUNT 3

/* buffers & friends */
char recvbf[BUFF_MAX_RECV];
char sendbf[BUFF_MAX_SEND];

/* book-keeping */
enum gpc_state state;
unsigned int si; 					// sendbf index
unsigned int ci; 					// character index
unsigned int need_sent;

static void init_pins_for_gpc() {
	gpio_func(PIN_TX, GPIO_FUNC_OUT);
	gpio_func(PIN_TA, GPIO_FUNC_OUT);
	gpio_func(PIN_IA, GPIO_FUNC_OUT);

	gpio_clear(PIN_TX);
	gpio_clear(PIN_TA);
	gpio_clear(PIN_IA);

	gpio_pull(PIN_RX, GPIO_PULLD);
	gpio_pull(PIN_RD, GPIO_PULLD);
	gpio_pull(PIN_ID, GPIO_PULLD);
	
	gpio_func(PIN_RX, GPIO_FUNC_IN);
	gpio_func(PIN_RD, GPIO_FUNC_IN);
	gpio_func(PIN_ID, GPIO_FUNC_IN);
} 

void init_gpc() 
{
	init_pins_for_gpc();
	state = IDLE;
}

int send(char *c) {
	for (; *c != '\0'; c++) {
		putc(*c);
	}

	return 0x0;
}

/**
 * @brief Sends a character, which is 1 byte (8 bits).
 * 
 * @param c character to send 
 */
int putc(char c) {
	unsigned int i = 0; 
	for (; i < sizeof(char) * 8; i++) {
		transmitb((unsigned int) (c) & 0b1);
		c = c >> 1; 
	}

	return 0;
}

/**
 * @brief Will transmit a bit via the transmit line, currently doesn't
 * check for a read declaration by the other endpoint. Will then wait
 * for it's RD line to be set to high.
 * 
 * @param bit bit to send over the wire
 */
void transmitb(int bit) {
	if ((bit & 0b1))
		gpio_set(PIN_TX);
	else gpio_clear(PIN_TX);

	gpio_set(PIN_TA);

	/* now we are waiting for a falling edge */
	while (!sample_pin(PIN_RD, MIN_SAMPLE_COUNT));
	while (sample_pin(PIN_RD, MIN_SAMPLE_COUNT));
	
	gpio_clear(PIN_TX);
	gpio_clear(PIN_TA);
}

static void goto_idle(int *reading) {
	switch (*reading) {
	case 0b01:
		if (need_sent > 0) {
			gpio_set(PIN_TA);
			state = PENDING_CONFIRMATION;
			return;
		}
	}
}

static void goto_prep_tx(int *reading) {
	char *c;
	switch (*reading) {
	case 0b00:
		c = &sendbf[si % need_sent];
		if ((unsigned int) (*c) & (0b1 << ci))
			gpio_set(PIN_TX);
		else 
			gpio_clear(PIN_TX);
		
		if (++ci == 8) {
			si++;
			ci = 0;
		}		
		state = ASSERT_TA;
		return;
	default:
		break;
	}

	// we did not pass the test (i.e., receiver may be trying
	// to send before us)
	gpio_clear(PIN_TA); // may be a logic issue here
	state = IDLE;
	return;
}

static void goto_pending_confirm(int *reading) {
	unsigned int i = 0, count = 0;
	switch (*reading) {
		case 0b00:
			for (; i < 2; i++) {
				count += sample_pin(PIN_RD, MIN_SAMPLE_COUNT);
				count += sample_pin(PIN_ID, MIN_SAMPLE_COUNT);
			}

			if (count == 0) {
				// we have confirmed our send, so we will now 
				// go into the prep state
				state = PREP_TX;
				goto_prep_tx(reading);
				return;
			}
			break;
		default:
			break;
	}

	// we did not pass the test (i.e., receiver may be trying
	// to send before us)
	gpio_clear(PIN_TA);
	state = IDLE;
	return;
}

static void goto_assert_ta(int *reading) {
	switch (*reading) {
	case 0b00:
		gpio_clear(PIN_IA);
		return;
	default:
		state = IDLE;
		break;
	}

	// we did not pass the test (i.e., receiver may be trying
	// to send before us)
	gpio_clear(PIN_TA); // may be a logic issue here
	state = IDLE;
	return;
}

static int _strlen(char *c) {
	int count = 0, i = 0;
	while (c[i] != '\0') {
		count++;
	}

	return count;
}

void _send(char *c) {
	int i = 0;
	int len = _strlen(c);

	for (; i < len && need_sent < BUFF_MAX_SEND; i++) {
		sendbf[need_sent++] = c[i];
	}
}

/* rewrite */
void parse_state() {
	int reading = sample_pin(PIN_ID, MIN_SAMPLE_COUNT);
	reading |= (sample_pin(PIN_RD, MIN_SAMPLE_COUNT) << 1);

	switch (state) {
	case IDLE:
		goto_idle(&reading);
		break;
	case PENDING_CONFIRMATION:
		goto_pending_confirm(&reading);
		break;
	case PREP_TX:
		goto_prep_tx(&reading);
		break;
	case ASSERT_TA:
		goto_assert_ta(&reading);
		break;
	} 
}
