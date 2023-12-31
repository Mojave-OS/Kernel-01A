#ifndef _KERN_MEMLAYOUT_H
#define _KERN_MEMLAYOUT_H

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

// function for manipulation of mem registers without read 
// perms
volatile void mem_regw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
);

// function for manipulation of mem registers that allow for RW.
volatile void mem_regrw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
);

#endif