#ifndef __KERN_LED_H
#define __KERN_LED_H

#include "gpio/bcm2711/gpio.h"

/**
 * @brief Convenience function that initializes a pin to be used
 * as an LED.
 * 
 * @param pin pin to use as LED. 
 */
void led_init(int pin, int state);

/**
 * @brief Toggles LED depending on the state of the pin.
 * 
 * @param pin pin to use as LED.
 */
void led_toggle(int pin);

/* these functions wrap the gpio driver for clarity */
void led_set(int pin);
void led_clear(int pin);

#endif