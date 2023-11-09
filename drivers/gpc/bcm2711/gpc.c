#include "gpc.h"
#include "gpio/bcm2711/gpio.h"

#define MIN_SAMPLE_COUNT 3

/* buffers & friends */
char recvbf[BUFF_MAX_RECV];
char sendbf[BUFF_MAX_SEND];

/* book-keeping */
enum gpc_state state;
unsigned int ri;
unsigned int si;

static void init_pins_for_gpc() {
	gpio_func(PIN_TX, GPIO_FUNC_OUT);
	gpio_func(PIN_TA, GPIO_FUNC_OUT);

	gpio_pull(PIN_RX, GPIO_PULLF);
	gpio_pull(PIN_RD, GPIO_PULLF);
	
	gpio_func(PIN_RX, GPIO_FUNC_IN);
	gpio_func(PIN_RD, GPIO_FUNC_IN);
} 

void init_gpc() 
{
	init_pins_for_gpc();
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



