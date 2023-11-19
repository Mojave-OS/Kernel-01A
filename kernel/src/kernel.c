#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"
#include "led/led.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();

    led_init(21, 1);
    led_init(20, 0);
    led_init(16, 1);

    while (1) {
        led_toggle(21);
        led_toggle(20);
        led_toggle(16);

        delay(1);
    }

    panic();
}