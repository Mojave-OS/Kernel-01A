#include "memlayout.h"
#include "gpio.h"

void delay(int seconds) {
    for (unsigned int s = 0; s < seconds; s++) {
        for (volatile unsigned int j = 0; j < (1000000 * 2.5); j++) {
            __asm__ volatile ("nop");
        }
    }
}

void kernel_main() {
    unsigned int PIN_COUNT = 3;
    unsigned int PINS[PIN_COUNT];
    PINS[0] = 40;
    PINS[1] = 38;
    PINS[2] = 36;
    
    /* initialize the gpio map */
    init_gpio_map();

    /* initialize gpio functions */
    gpio_func(40, GPIO_FUNC_OUT);
    gpio_func(38, GPIO_FUNC_OUT);
    gpio_func(36, GPIO_FUNC_OUT);

    /* set and clear pins with delay */
    while (1) {
        /* set the pins */
        for (unsigned int i = 0; i < PIN_COUNT; i++) {
            gpio_set(PINS[i]);
        }

        delay(2);
        
        
        /* set the pins */
        for (unsigned int i = 0; i < PIN_COUNT; i++) {
            gpio_clear(PINS[i]);
        }
    }
}