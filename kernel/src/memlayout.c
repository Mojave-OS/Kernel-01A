#include "memlayout.h"

void mem_regw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    *address = (value << shiftam);
}

void mem_regrw(
    unsigned int granularity,
    unsigned int *address,
    unsigned int value,
    unsigned int index
) {
    unsigned int shiftam = (index * granularity);
    *address = *address | (value << shiftam);
}