#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();

    while (1) {
        send("Hello, kernel!\n");
    }

    panic();
}