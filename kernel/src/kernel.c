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
    gpio_func(40, GPIO_FUNC_OUT);
    
    while (1) {
        gpio_set(40);
        gpio_clear(40);

        delay(1);
    }
}