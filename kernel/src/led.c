#include "led/led.h"

void led_init(int pin, int state) {
    gpio_func(pin, GPIO_FUNC_OUT);
    gpio_clear(pin);

    if (state) 
        led_set(pin);
}

void led_set(int pin) {
    gpio_set(pin);
}

void led_clear(int pin) {
    gpio_clear(pin);
}

void led_toggle(int pin) {
    if (gpio_lev(pin))
        led_clear(pin);
    else led_set(pin);
}