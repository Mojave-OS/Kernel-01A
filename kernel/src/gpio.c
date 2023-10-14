#include "memlayout.h"
#include "gpio.h"

static inline unsigned int gpio_bank_sel(unsigned int pin, unsigned int max) {
    return (unsigned int) (pin / max);
}

volatile pin_t gpio(unsigned int pin) {
    unsigned int *regbase = GPIO_ADDR(GPIO_REG_LEV0);
    unsigned int levelreg = gpio_bank_sel(pin, 32);
    
    // selects the register base depending on the pin
    switch (levelreg) {
    case 1:
        regbase = GPIO_ADDR(GPIO_REG_LEV1);
        break;
    
    default:
        break;
    }

    return *regbase;
}

void gpio_set(unsigned int pin) {
    unsigned int shiftam = (pin) % 32;
    unsigned int *regbase = GPIO_ADDR(GPIO_REG_SET0);
    switch (gpio_bank_sel(pin, 32)) {
        case 1:
            regbase = GPIO_ADDR(GPIO_REG_SET1);
            break;
        default:
            break;
    }

    *regbase = (1 << shiftam);
}

void gpio_clear(unsigned int pin) {
    unsigned int shiftam = (pin) % 32;
    unsigned int *regbase = GPIO_ADDR(GPIO_REG_CLR0);
    switch (gpio_bank_sel(pin, 32)) {
        case 1:
            regbase = GPIO_ADDR(GPIO_REG_CLR1);
            break;
        default:
            break;
    }

    *regbase = (1 << shiftam);
}

void gpio_func(unsigned int pin, unsigned int func) {
    unsigned int shiftam = (pin % 10) * 3;
    unsigned int *regbase = GPIO_ADDR(GPIO_REG_FSEL0);
    switch (gpio_bank_sel(pin, 10)) {
        case 1:
            regbase = GPIO_ADDR(GPIO_REG_FSEL1);
            break;
        case 2:
            regbase = GPIO_ADDR(GPIO_REG_FSEL2);
            break;
        case 3:
            regbase = GPIO_ADDR(GPIO_REG_FSEL3);
            break;
        case 4:
            regbase = GPIO_ADDR(GPIO_REG_FSEL4);
            break;
        case 5:
            regbase = GPIO_ADDR(GPIO_REG_FSEL5);
            break;
        default:
            break;
    }
    
    *regbase = *regbase | ((func & 0x7) << shiftam);
}