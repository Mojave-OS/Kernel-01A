/**/
#define GPFSEL4 0x10
#define GPSET1  0x20
#define GPCLR1  0x2c
#define GPLEV1  0x38

#define GPFUNC_INPUT        0x0
#define GPFUNC_OUTPUT       0x1

#define DELAY_COUNT (1 << 15)

/* Definitions for Pin Related Helpers */
#define PIN_MASK(pin) (0x1 << (pin % 32))

typedef unsigned int *register_t;
typedef unsigned int pin_t;

pin_t static gpio(register_t mreg) {
    return *mreg;
}

void static volatile gpio_set(pin_t pin, pin_t state, register_t mreg) {
    *mreg = PIN_MASK(pin) | state;
}

void static volatile gpio_clear(pin_t pin, pin_t state, register_t mreg) {
    *mreg = PIN_MASK(pin) | state;
}

void static volatile gpio_func(pin_t pin, unsigned int func, register_t mreg) {
    volatile pin_t state = *mreg;
    *mreg = (func << ((pin % 32) * 3)) | state;
}

void delay(int count) {
    while (--count > 0) {

    }
}

void kernel_main() {
    // set the pin as an input
    gpio_func(42, GPFUNC_OUTPUT, (register_t) GPFSEL4);
    pin_t state_set = gpio((register_t) GPLEV1);

    // loop blink
    while (1) {
        // turn on the ACT_LED
        gpio_set(42, state_set, (register_t) GPSET1);

        // sleep for some time so we can see it 
        // blink
        delay(DELAY_COUNT);

        // turn off the ACT_LED
        state_set = gpio((register_t) GPLEV1);
        gpio_clear(42, state_set, (register_t) GPCLR1);
    }
}