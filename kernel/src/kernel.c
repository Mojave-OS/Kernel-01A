#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"
#include "led/led.h"
#include "strings/strings.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();

    char destination[32];
    unsigned int args[1];
    args[0] = 123456789;

    sprintf(&destination[0], "\nCount: %d\n", (void *)(args));
    puts(destination);

    while (1) {
        exec();
    }

    panic();
}