#ifndef __KERN_STRING_H
#define __KERN_STRING_H

#define SPRINTF_BUFFER_MAX 512

/**
 * @brief Converts an array of arguments and a format stringh
 * to a string.
 * 
 * @param dest Destination buffer to write the string to.
 * @param fmt Format string.
 * @param args Argument List.
 * @return int number of bits written
 */
int sprintf(char *dest, char *fmt, void *args);

#endif