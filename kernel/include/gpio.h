#ifndef _KERN_GPIO_H
#define _KERN_GPIO_H

#define GPIO_FUNC_OUT           0x1
#define GPIO_FUNC_IN            0x0

typedef unsigned int pin_t;
typedef unsigned int *memreg;

/* GPIO Pin Read Operations */

// Returns the actual value of the pin via
// the pin level register.
volatile pin_t gpio(unsigned int pin);

/* GPIO Pin Write Operations */

// sets the pin as high, pin func must be set to 
// output beforehand.
void gpio_set(unsigned int pin);

// sets the pin as low, pin func must be set to 
// output beforehand.
void gpio_clear(unsigned int pin);

// sets the pin's pull up/down resistor
void gpio_pullu(unsigned int pin);
void gpio_pulld(unsigned int pin);

// floating state 
void gpio_pullf(unsigned int pin); 

void gpio_func(unsigned int pin, unsigned int func);

#endif