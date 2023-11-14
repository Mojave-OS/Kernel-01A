#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();

    /* initializing a debug pin */
    gpio_func(4, GPIO_FUNC_OUT);
    gpio_clear(4);

    delay(3);

    while (1) {
        if (buffer_empty()) {
            buffer_flush();
            if (puts("Cha Cha Motherfucker\n") == -1) {
                gpio_set(4);
            } else {
                gpio_clear(4);
            }
        }
        exec();
    }
    

    delay(4);
    panic();
}