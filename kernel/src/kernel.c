#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();
    
    puts("Hello, welcome to the GPIO-Communication Protocol!\n");
    puts("o7\n");

    while (1) {
        exec();
    }
    

    delay(4);
    panic();
}