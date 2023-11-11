#include "gpc.h"
#include "gpio/bcm2711/gpio.h"

#define MIN_SAMPLE_COUNT 3
#define NINPUT_PINS 2

int buf = 0;

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
#define TRANSFER_FORCE(expected, handler) \
	if ((inputs >> shift) != (expected | 0)) { \
		state = handler; \
		return; \
	} \

/* inputs */
int INPUT_PINS[NINPUT_PINS];
int buffer_count;

/* state handling */
void (*state)(int);

/* book-keeping */
void init_gpc() {
	buffer_count = 0;
	INPUT_PINS[0] = PIN_RD;
	INPUT_PINS[1] = PIN_ID;
}

static int read_inputs(void)
{
	int reading = 0;

	for (unsigned int i = 0; i < NINPUT_PINS; i++) {
		// keep in mind that we expect the reading to be in the following
		// format: RD | ID | buffer_count > 0 (that's why we have this << )
		reading |= (sample_pin(INPUT_PINS[i]) << (NINPUT_PINS - i));
	}
	reading |= ((buffer_count > 0) & 0b1); // paranoia check
	return reading;
}

/* declare handlers for our states */
/**
 * @brief Prepare a bit to be sent on the "wire".
 * 
 * @param inputs state from pins.
 */
void enqueue_tx(int inputs) {
	gpio_clear(PIN_IA);
	gpio_clear(PIN_TA);

	/* handle state transfers for unexpected cases */
	TRANSFER_FORCE(0b011, idle, 0); // if rd

	/* handle enqueue operations */
	unsigned int c = buf & 0b1;

	buffer_count--;
}

void assert_ta(int inputs) {

}

void tx_incoming(int inputs) {

}

void read_rx(int inputs) {

}

void idle(int inputs) {
	/* handle idle operations */
	gpio_clear(PIN_TA);
	gpio_set(PIN_IA);

	/* handle state transfers */
	TRANSFER(0b001, tx_incoming, 0);
	TRANSFER(0b000, tx_incoming, 0);
	TRANSFER(0b011, enqueue_tx, 0);
	TRANSFER(0b010, idle, 0);
}