#include "gpio.h"

unsigned int *GPIO_SETS[2];
unsigned int *GPIO_CLEARS[2];
unsigned int *GPIO_LEVS[2];
unsigned int *GPIO_FSELS[6];
unsigned int *GPIO_PLS[4];
unsigned int *GPIO_GPEDS[2];
unsigned int *GPIO_GPRENS[2];
unsigned int *GPIO_GPFENS[2];

static void delay(int amount) 
{
    for (unsigned int s = 0; s < amount; s++) {
        for (volatile unsigned int j = 0; j < (500); j++) {
            __asm__ volatile ("nop");
        }
    }
}

void init_gpio_map() {
    GPIO_SETS[0] = GPIO_ADDR(GPIO_REG_SET0);
    GPIO_SETS[1] = GPIO_ADDR(GPIO_REG_SET1);

    GPIO_CLEARS[0] = GPIO_ADDR(GPIO_REG_CLR0);
    GPIO_CLEARS[1] = GPIO_ADDR(GPIO_REG_CLR1);
    
    GPIO_LEVS[0] = GPIO_ADDR(GPIO_REG_LEV0);
    GPIO_LEVS[1] = GPIO_ADDR(GPIO_REG_LEV1);

    GPIO_FSELS[0] = GPIO_ADDR(GPIO_REG_FSEL0);
    GPIO_FSELS[1] = GPIO_ADDR(GPIO_REG_FSEL1);
    GPIO_FSELS[2] = GPIO_ADDR(GPIO_REG_FSEL2);
    GPIO_FSELS[3] = GPIO_ADDR(GPIO_REG_FSEL3);
    GPIO_FSELS[4] = GPIO_ADDR(GPIO_REG_FSEL4);
    GPIO_FSELS[5] = GPIO_ADDR(GPIO_REG_FSEL5);

    GPIO_PLS[0] = GPIO_ADDR(GPIO_REG_PL0);
    GPIO_PLS[1] = GPIO_ADDR(GPIO_REG_PL1);
    GPIO_PLS[2] = GPIO_ADDR(GPIO_REG_PL2);
    GPIO_PLS[3] = GPIO_ADDR(GPIO_REG_PL3);

    GPIO_GPEDS[0] = GPIO_ADDR(GPIO_REG_GPEDS0);
    GPIO_GPEDS[1] = GPIO_ADDR(GPIO_REG_GPEDS1);

    GPIO_GPRENS[0] = GPIO_ADDR(GPIO_REG_GPREN0);
    GPIO_GPRENS[1] = GPIO_ADDR(GPIO_REG_GPREN1);

    GPIO_GPFENS[0] = GPIO_ADDR(GPIO_REG_GPFEN0);
    GPIO_GPFENS[1] = GPIO_ADDR(GPIO_REG_GPFEN1);
}

static volatile void mem_regw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    *address = (value << shiftam);
}

static volatile void mem_regrw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    volatile unsigned int lhs = (*address >> (shiftam + granularity));
    *address = *address & ~(1 << shiftam);
    *address = *address | (value << shiftam) | (lhs << (shiftam + granularity));
}

// selects the proper memory mapped register in a register bank (i.e.,
// a contigious group of registers) based on the granularity of the 
// data and the pin we are targeting.
//
// the pin % 57 may be unneccesarry but prevents a 
// case where we choose a pin > the provided number of regs in the bank
// causing a buffer overflow - in theory.
unsigned int *gpio_bank_sel(
    unsigned int pin,
    unsigned int granularity,
    unsigned int **banks
) {
    return banks[(((pin % 57) * granularity) / 32)];
}

unsigned int *gpio_bank_sel_max(
    unsigned int pin,
    unsigned int granularity,
    unsigned int **banks,
    unsigned int max
) {
    return banks[(((pin % 57) * granularity) / max)];
}

volatile pin_t gpio(unsigned int pin) {
    return *gpio_bank_sel(pin, 1, GPIO_LEVS);
}

volatile void gpio_set(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_SETS);
    mem_regw(1, regbase, 1, (pin % 32));
}

volatile void gpio_clear(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_CLEARS);
    mem_regw(1, regbase, 1, (pin % 32));
}

volatile void gpio_func(unsigned int pin, unsigned int func) {
    unsigned int *regbase = gpio_bank_sel_max(pin, 3, GPIO_FSELS, 30);
    mem_regrw(3, regbase, func, (pin % 10));
}

volatile void gpio_pull(unsigned int pin, unsigned int pullv) {
    unsigned int *regbase = gpio_bank_sel(pin, 2, GPIO_PLS);
    mem_regrw(2, regbase, pullv, (pin % 16));
}

volatile int gpio_lev(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_LEVS);
    return (*regbase >> (pin % 32)) & 0b1;
}

volatile int gpio_event(unsigned int pin) 
{
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_GPEDS);
    return (*regbase >> (pin % 32)) & 0b1;
}

volatile void gpio_falling_edge(unsigned int pin) 
{
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_GPFENS);
    mem_regw(1, regbase, 1, (pin % 32));
}

volatile void gpio_rising_edge(unsigned int pin) 
{
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_GPRENS);
    mem_regw(1, regbase, 1, (pin % 32));
}

volatile void gpio_clear_ev(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_GPEDS);
    mem_regw(1, regbase, 1, (pin % 32));
}

unsigned int sample_pin(int pin, int min_sample)
{
    unsigned int state_input = 0;
    int count = 0, last = -1;
    while (count < min_sample) {
        state_input = gpio_lev(pin);
        if (state_input == last) {
            count++;
        } else {
            last = state_input;
            count = 0;
        }

        delay(10);
    }

    return state_input;
}