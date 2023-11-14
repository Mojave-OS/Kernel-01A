#include "gpc.h"
#include "gpio/bcm2711/gpio.h"

#define MIN_SAMPLE_COUNT 3
#define NINPUT_PINS 2

/* configuration related */
#define NBTS_MAX (1024 << 1) // max chars buffer holds

/* buffering realted */
int buffer[NBTS_MAX];
int start = 0, end = 0;

static void delay(int cycles) {
	for (volatile unsigned int j = 0; j < (cycles); j++) {
		__asm__ volatile ("nop");
	}
}

/**
 * @brief Will conditionally tranfer if the state is equal to the inputs.
 * 
 */
#define TRANSFER(expected, handler, shift) \
	if ((inputs >> shift) == (expected | 0)) { \
		state = handler; \
		return; \
	} \

/**
 * @brief Forces the state to be a certain value, will change if it doesn't
 * match.
 * 
 */
#define TRANSFER_FORCE(expected, handler, shift) \
	if ((inputs >> shift) != (expected | 0)) { \
		state = handler; \
		return; \
	} \

static void idle(int inputs);
static void enqueue_tx(int inputs);
static void assert_ta(int inputs);
static void cleanup_tx(int inputs);

/* inputs */
int INPUT_PINS[NINPUT_PINS];

/* state handling */
void (*state)(int);

/**
 * @brief Reads the inputs and provides the state
 * of our state machine in an integer representation
 * as such: RD | ID | buffer has bits to send; represented as 
 * one or zero.
 * 
 * @return int state of the machine
 */
static int read_inputs(void)
{
	int reading = 0;

	for (unsigned int i = 0; i < NINPUT_PINS; i++) {
		// keep in mind that we expect the reading to be in the following
		// format: RD | ID | buffer_count > 0 (that's why we have this << )
		reading |= (sample_pin(INPUT_PINS[i], MIN_SAMPLE_COUNT) << (NINPUT_PINS - i));
	}
	reading |= (((end - start) > 0) & 0b1); // paranoia check
	return reading;
}

/**
 * @brief Prepare a bit to be sent on the "wire", by setting / clearing
 * the TX Pin.
 * 
 * @param inputs state from pins.
 */
static void enqueue_tx(int inputs) {
	gpio_clear(PIN_TA);
	gpio_clear(PIN_TX);

	/* handle state transfers for unexpected cases */
	TRANSFER_FORCE(0b011, idle, 0); // if rd
	gpio_clear(PIN_IA);

	/* handle enqueue operations */
	unsigned int c = (buffer[start / 8] >> (start % 8)) & 0b1;
	if (c)
		gpio_set(PIN_TX);

	/* manually setting the state here */
	state = assert_ta;
}

/**
 * @brief Asserts the TA Pin (i.e., sets it to 1). This is required
 * for our state machine.
 * 
 * @param inputs 
 */
static void assert_ta(int inputs) {
	gpio_set(PIN_TA);

	/* handle state transitions for unwanted cases */
	TRANSFER_FORCE(0b00, cleanup_tx, 1);
}

/**
 * @brief Will clear the TX Buffer's last sent bit, and will
 * increment the start bit. Also clears the TA Pin.
 * 
 * @param inputs 
 */
static void cleanup_tx(int inputs) {
	/* clear the bit we just sent */
	
	// this clears out the bit we just sent from the buffer
	// because it can lead to errors in character interpretation
	// since we or the char with the buffer
	buffer[start / 8] &= (~0 ^ (0b1 << (start % 8)));

	/* set up tx state bits for next enqueue */
	start = start + 1;
	state = idle;

	/* handle idle operations */
	gpio_clear(PIN_TA);
}

/**
 * @brief To be implemented.
 * 
 * @param inputs 
 */
static void tx_incoming(int inputs) {
	state = idle;
}

/**
 * @brief Default state for the state machine. 
 * 
 * @param inputs 
 */
static void idle(int inputs) {
	gpio_set(PIN_IA);

	/* handle state transfers */
	TRANSFER(0b011, enqueue_tx, 0);
	TRANSFER(0b010, idle, 0);
}

/**
 * @brief "Driver" function.
 * 
 */
void exec() {
	int inputs = read_inputs();
	state(inputs);
	delay(1000);
}

/* book-keeping */
void init_gpc() {
	/* set up the buffers */
	start = 0;
	end = 0;

	INPUT_PINS[0] = PIN_RD;
	INPUT_PINS[1] = PIN_ID;

	/* set up the pins pud */
	gpio_pull(PIN_RX, GPIO_PULLF);
	gpio_pull(PIN_RD, GPIO_PULLF);
	gpio_pull(PIN_ID, GPIO_PULLF);

	/* set up the pin functions */
	gpio_func(PIN_RX, GPIO_FUNC_IN);
	gpio_func(PIN_RD, GPIO_FUNC_IN);
	gpio_func(PIN_ID, GPIO_FUNC_IN);
	gpio_func(PIN_TX, GPIO_FUNC_OUT);
	gpio_func(PIN_TA, GPIO_FUNC_OUT);
	gpio_func(PIN_IA, GPIO_FUNC_OUT);

	/* set the output pins to their default state (i.e., low) */
	gpio_clear(PIN_TX);
	gpio_clear(PIN_TA);
	gpio_set(PIN_IA);

	/* set the initial state */
	state = idle;
}

/**
 * @brief Puts a bit in the buffer.
 * 
 * @param b 
 * @return int 
 */
int putb(int b) {
	if (end == NBTS_MAX * 8)
		return -1;

	buffer[end / 8] |= ((b & 0b1) << (end % 8));
	end = end + 1;
	return 0x0;
}

/**
 * @brief Puts a character in the buffer.
 * 
 * @param c 
 * @return int 
 */
int putc(char c) {
	for (unsigned int i = 0; i < 8; i++) {
		if (putb(((c >> i) & 0b1)) == -1)
			return -1;
	}
	
	return 0;
}

/**
 * @brief Puts a string in the buffer.
 * 
 * @param c 
 * @return int 
 */
int puts(char *c) {
	for (; *c != '\0'; c++) {
		if (putc(*c) == -1)
			return -1;
	}

	return 0;
}

/**
 * @brief Checks if the buffer is empty.
 * 
 * @return int 
 */
int buffer_empty() {
	return (end - start) <= 0;
}

/**
 * @brief Flushes the buffer; effectively removing anything in it.
 * 
 */
void buffer_flush() {
	for (unsigned int i = 0; i < NBTS_MAX; i++) {
		buffer[i] = 0;
	}

	start = 0;
	end = 0;
}
