#include "gpio/bcm2711/gpio.h"
#include "gpc/bcm2711/gpc.h"
#include "common.h"

void kernel_main() {
    /* initialize the gpio map */
    init_gpio_map();
    init_gpc();
    
    exec();
    buffer_flush();
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");
    printg("Hello!\n");   
    printg("Hello!\n");
    printg("Hello!\n");

    while (1) {       
        exec();
    }
    

    delay(4);
    panic();
}