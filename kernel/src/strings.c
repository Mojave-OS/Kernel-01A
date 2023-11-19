#include "strings/strings.h"
#include "math/defs.h"

enum SPRINTF_STATE {
    COPY,
    REPLACE,
    IDENTIFYING,
};

static int find_depth(int n) {
    if ((n / 10) == 0) {
        return 0;
    }

    return find_depth(n/10) + 1;
}

/**
 * @brief Handles an integer argument for sprintf and appends to the dest.
 * 
 * @param dest destination buffer where string is expected.
 * @param di destination index.
 * @param num number as argument to %d.
 */
static void sprintf_handle_uint(char *dest, unsigned int *di, void *args) {
    unsigned int num = ((unsigned int *)(args))[0];
    unsigned int depth = find_depth(num);
    unsigned int power = pow(10, depth);

    while (power > 0) {
        // we do mod 57 to restrain the output encoding to only ascii 
        // numbers (aruably not needed since we divide by power, but...)
        *(dest + *di) = (char) (((num / power) + 48)); 
        num = num - ((num / power) * power);
        power /= 10;
        *di = *di + 1;
    }
}

static void sprintf_handle_hex(char *dest, unsigned int *di, void *args) {
    // to be implemented
}

static void sprintf_append(char *dst, unsigned int *di, char c) {
    dst[*di] = c;
    *di = *di + 1;
}

int sprintf(char *dest, char *fmt, void *args) {
    unsigned int di = 0;
    unsigned int va_index = 0;
    
    for (char *c = fmt; *c != '\0'; c++) {
        if (*c == '%') {
            switch (*(c + 1)) {
            case 'd':
                sprintf_handle_uint(dest, &di, (args + va_index));
                va_index += sizeof(unsigned int);
                c++;
                break;

            default:
                sprintf_append(dest, &di, *c);
                break;
            }

        } else {
            sprintf_append(dest, &di, *c);
        }
    }

    if (*(dest + di) != '\0') {
        dest[di] = '\0';
    }
    return 0;
}
