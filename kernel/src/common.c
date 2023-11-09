#include "common.h"
#include "gpio/bcm2711/gpio.h"

/**
 * @brief This is a method that is used to declare to the user
 * that we have entered a panic'd state. 
 * 
 */

int _panic_ready;

void panic() {
    if (!_panic_ready) {
        gpio_pull(COMMON_PANIC_PIN, GPIO_PULLF);
        gpio_func(COMMON_PANIC_PIN, GPIO_FUNC_OUT);
        _panic_ready = 0x1;
    }

    gpio_set(COMMON_PANIC_PIN);
    while (1);
}

void delay(int seconds) {
    for (unsigned int s = 0; s < seconds; s++) {
        for (volatile unsigned int j = 0; j < (500); j++) {
            __asm__ volatile ("nop");
        }
    }
}