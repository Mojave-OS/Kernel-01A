#include "gpc.h"
#include "gpio/bcm2711/gpio.h"

#define MIN_SAMPLE_COUNT 3
#define NINPUT_PINS 2
#define NBTS_MAX (1 << 15)

#define BIT_BUFFER_INDX(bts) (bts / 8)
#define B2S_INDX(bts) (bts % 8)

int buffer[NBTS_MAX];
int bts = 0;
int current_bit = 0;

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

static int read_inputs(void)
{
	int reading = 0;

	for (unsigned int i = 0; i < NINPUT_PINS; i++) {
		// keep in mind that we expect the reading to be in the following
		// format: RD | ID | buffer_count > 0 (that's why we have this << )
		reading |= (sample_pin(INPUT_PINS[i], MIN_SAMPLE_COUNT) << (NINPUT_PINS - i));
	}
	reading |= ((bts > 0) & 0b1); // paranoia check
	return reading;
}

/**
 * @brief Prepare a bit to be sent on the "wire".
 * 
 * @param inputs state from pins.
 */
static void enqueue_tx(int inputs) {
	gpio_clear(PIN_TA);
	gpio_clear(PIN_TX);

	/* handle state transfers for unexpected cases */
	TRANSFER_FORCE(0b011, idle, 0); // if rd
	gpio_clear(PIN_IA);

	int i = BIT_BUFFER_INDX(current_bit);
	int j = B2S_INDX(current_bit);

	/* handle enqueue operations */
	unsigned int c = (buffer[i % NBTS_MAX] >> j) & 0b1;
	if (c)
		gpio_set(PIN_TX);

	/* manually setting the state here */
	state = assert_ta;
}

static void assert_ta(int inputs) {
	gpio_set(PIN_TA);

	/* handle state transitions for unwanted cases */
	TRANSFER_FORCE(0b00, cleanup_tx, 1);
}

static void cleanup_tx(int inputs) {
	/* clear the bit we just sent */
	int i = BIT_BUFFER_INDX(current_bit);
	int j = B2S_INDX(current_bit);

	// this clears out the bit we just sent from the buffer
	// because it can lead to errors in character interpretation
	// since we or the char with the buffer
	(buffer[i % NBTS_MAX]) &= (~0 ^ (0b1 << j));

	/* set up tx state bits for next enqueue */
	bts--; 
	current_bit = (current_bit + 1) % (NBTS_MAX * 8);
	state = idle;
}

static void tx_incoming(int inputs) {
	state = idle;
}

static void idle(int inputs) {
	/* handle idle operations */
	gpio_clear(PIN_TA);
	gpio_set(PIN_IA);

	/* handle state transfers */
	TRANSFER(0b001, tx_incoming, 0);
	TRANSFER(0b000, tx_incoming, 0);
	TRANSFER(0b011, enqueue_tx, 0);
	TRANSFER(0b010, idle, 0);
}

void exec() {
	int inputs = read_inputs();
	state(inputs);
}

/* book-keeping */
void init_gpc() {
	/* set up the buffers */
	bts = 0;
	current_bit = 0;

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

void putb(int b) {
	int i = BIT_BUFFER_INDX(bts);
	int j = B2S_INDX(bts);
	buffer[i % NBTS_MAX] |= ((b & 0b1) << j);
	bts++;
}

void putc(char c) {
	for (unsigned int i = 0; i < 8; i++) {
		putb((c & (0b1 << i)) >> i);
	}
}

void printg(char *c) {
	for (; *c != '\0'; c++) {
		putc(*c);
	}
}

int bits_to_send() {
	return bts;
}
