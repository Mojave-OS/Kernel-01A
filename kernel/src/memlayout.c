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