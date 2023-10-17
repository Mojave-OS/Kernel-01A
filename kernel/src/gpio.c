#include "memlayout.h"
#include "gpio.h"

unsigned int *GPIO_SETS[2];
unsigned int *GPIO_CLEARS[2];
unsigned int *GPIO_LEVS[2];
unsigned int *GPIO_FSELS[6];
unsigned int *GPIO_PLS[4];

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
}

// selects the proper memory mapped register in a register bank (i.e.,
// a contigious group of registers) based on the granularity of the 
// data and the pin we are targeting.
//
// the % (57 * granularity / 32) may be unneccesarry but prevents a 
// case where we choose a pin > the provided number of regs in the bank
// causing a buffer overflow - in theory.
static inline unsigned int *gpio_bank_sel(
    unsigned int pin,
    unsigned int granularity,
    unsigned int **banks
) {
    return banks[((pin * granularity) / 32) % (57 * granularity / 32)];
}

volatile pin_t gpio(unsigned int pin) {
    return *gpio_bank_sel(pin, 1, GPIO_LEVS);
}

void gpio_set(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_SETS);
    mem_regw(1, regbase, 1, (pin % 32));
}

void gpio_clear(unsigned int pin) {
    unsigned int *regbase = gpio_bank_sel(pin, 1, GPIO_CLEARS);
    mem_regw(1, regbase, 1, (pin % 32));
}

void gpio_func(unsigned int pin, unsigned int func) {
    unsigned int *regbase = gpio_bank_sel(pin, 3, GPIO_FSELS);
    mem_regw(3, regbase, func,(3 * (pin % 10)));
}


void gpio_pull(unsigned int pin, unsigned int pullv) {
    unsigned int *regbase = gpio_bank_sel(pin, 2, GPIO_PLS);
    mem_regw(2, regbase, pullv, (pin % 16));
}