#ifndef __KERN_COMMON_H
#define __KERN_COMMON_H

// a gpio pin that we will wire to signal 
// panics
#define COMMON_PANIC_PIN        26

/**
 * @brief This is a file where common methods, declarations, and other 
 * information will live for the kernel.
 * 
 */

void panic();
void delay(int);

#endif