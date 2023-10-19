#ifndef _KERN_GPIO_H
#define _KERN_GPIO_H

/* GPIO Func Values */
#define GPIO_FUNC_OUT           0x1
#define GPIO_FUNC_IN            0x0

/* GPIO Pull Values */
#define GPIO_PULLD              0x2 // pull down
#define GPIO_PULLU              0x1 // pull up
#define GPIO_PULLF              0x0 // floating
#define GPIO_RESERVED           0x3 // reserved

typedef unsigned int pin_t;
typedef unsigned int *memreg;

void init_gpio_map();

/* GPIO Pin Read Operations */

// Returns the actual value of the pin via
// the pin level register.
volatile pin_t gpio(unsigned int pin);
unsigned int *gpio_bank_sel_max(
    unsigned int pin,
    unsigned int granularity,
    unsigned int **banks,
    unsigned int max
);

unsigned int *gpio_bank_sel(
    unsigned int pin,
    unsigned int granularity,
    unsigned int **banks
);

/* GPIO Pin Write Operations */

// sets the pin as high, pin func must be set to 
// output beforehand.
volatile void gpio_set(unsigned int pin);

// sets the pin as low, pin func must be set to 
// output beforehand.
volatile void gpio_clear(unsigned int pin);

// sets the pin's pull up/down resistor
volatile void gpio_pull(unsigned int pin, unsigned int pullv);
volatile void gpio_func(unsigned int pin, unsigned int func);

#endif