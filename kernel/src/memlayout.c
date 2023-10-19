#include "memlayout.h"

volatile void mem_regw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    *address = (value << shiftam);
}

volatile void mem_regrw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    volatile unsigned int lhs = (*address >> (shiftam + granularity));
    *address = *address & ~(1 << shiftam);
    *address = *address | (value << shiftam) | (lhs << (shiftam + granularity));
}