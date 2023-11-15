#ifndef __DRIVERS_GPIO_H
#define __DRIVERS_GPIO_H

/* GPIO Func Values */
#define GPIO_FUNC_OUT           0x1
#define GPIO_FUNC_IN            0x0

/* GPIO Pull Values */
#define GPIO_PULLD              0x2 // pull down
#define GPIO_PULLU              0x1 // pull up
#define GPIO_PULLF              0x0 // floating
#define GPIO_RESERVED           0x3 // reserved

/* GPIO Related Memory Items */
#define GPIO_REG_BASE 0xfe200000
#define GPIO_ADDR(reg) ((unsigned int *) (GPIO_REG_BASE + reg))

// GPIO Related Registers and their offsets
#define GPIO_REG_SET0                   0x1c
#define GPIO_REG_SET1                   0x20
#define GPIO_REG_CLR0                   0x28
#define GPIO_REG_CLR1                   0x2c
#define GPIO_REG_LEV0                   0x34
#define GPIO_REG_LEV1                   0x38
#define GPIO_REG_FSEL0                  0x0
#define GPIO_REG_FSEL1                  0x4
#define GPIO_REG_FSEL2                  0x8
#define GPIO_REG_FSEL3                  0xc
#define GPIO_REG_FSEL4                  0x10
#define GPIO_REG_FSEL5                  0x14
#define GPIO_REG_PL0                    0xe4
#define GPIO_REG_PL1                    0xe8
#define GPIO_REG_PL2                    0xec
#define GPIO_REG_PL3                    0xf0
#define GPIO_REG_GPEDS0                 0x40
#define GPIO_REG_GPEDS1                 0x44
#define GPIO_REG_GPREN0                 0x4c
#define GPIO_REG_GPREN1                 0x50
#define GPIO_REG_GPFEN0                 0x58
#define GPIO_REG_GPFEN1                 0x5c
#define GPIO_REG_GPHEN0                 0x64
#define GPIO_REG_GPHEN1                 0x68
#define GPIO_REG_GPLEN0                 0x70
#define GPIO_REG_GPLEN1                 0x74

/* sample related vars */
#define GPIO_SAMPLE_DELAY 10            // unit is cycles 

/* memreg related */
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

// gets the input level of the pin
volatile int gpio_lev(unsigned int pin);

// sets the pin's pull up/down resistor
volatile void gpio_pull(unsigned int pin, unsigned int pullv);
volatile void gpio_func(unsigned int pin, unsigned int func);

volatile int gpio_event(unsigned int pin);
volatile void gpio_falling_edge(unsigned int pin);
volatile void gpio_rising_edge(unsigned int pin);
volatile void gpio_clear_ev(unsigned int pin);

/* some functions that build on the gpio primitives */
unsigned int sample_pin(int pin, int min_sample);


#endif
