#include "memlayout.h"
#include "gpio.h"

void delay(int seconds) {
    for (unsigned int s = 0; s < seconds; s++) {
        for (volatile unsigned int j = 0; j < (1000000); j++) {
            __asm__ volatile ("nop");
        }
    }
}

void kernel_main() {
    unsigned int PIN_COUNT = 2;
    unsigned int PINS[PIN_COUNT];
    PINS[0] = 42;
    PINS[1] = 21;
    
    /* initialize the gpio map */
    init_gpio_map();

    /* set the pins to not use any pull resistors */
    for (unsigned int i = 0; i < PIN_COUNT; i++) {
        gpio_pull(PINS[i], GPIO_PULLF);
        gpio_func(PINS[i], GPIO_FUNC_OUT);
    }
    // gpio_func(21, GPIO_FUNC_IN);

    /* set and clear pins with delay */
    while (1) {
        /* set the pins */
        for (unsigned int i = 0; i < PIN_COUNT; i++) {
            gpio_set(PINS[i]);
        }
        // gpio_pull(21, GPIO_PULLD);
        delay(1);
        
        /* set the pins */
        for (unsigned int i = 0; i < PIN_COUNT; i++) {
            gpio_clear(PINS[i]);
        }
        // gpio_pull(21, GPIO_PULLU);
        delay(1);
    }
}