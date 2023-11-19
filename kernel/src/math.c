#include "math/defs.h"

unsigned int pow(int base, int exp) {
    int result = 1;
    if (exp == 0)
        return result;

    result *= 10;

    for (int i = 0; i < (exp - 1); i++) {
        result *= base;
    }

    return result;
}