#ifndef __DRIVERS_MU_H
#define __DRIVERS_MU_H

/**
 * @brief This is a mini-uart driver for the BCM2711 Chip. It makes not 
 * assumptions about the development environment. 
 *
 * @author Diego Alba, DAlba-sudo
 *
 */

/* memory related definitions */
#define AUX_BASE 0xFE215000
#define AUX_ADDR(offset) ((unsigned int *)(AUX_BASE + offset))

/* register offsets */
#define AUX_IRQ 0x0
#define AUX_ENABLES 0x04
#define AUX_MU_IO_REG 0x40
#define AUX_MU_IER_REG 0x44
#define AUX_MU_IIR_REG 0x48
#define AUX_MU_LCR_REG 0x4c
#define AUX_MU_MCR_REG 0x50
#define AUX_MU_LSR_REG 0x54
#define AUX_MU_MSR_REG 0x58
#define AUX_MU_SCRATCH 0x5c
#define AUX_MU_CNTL_REG 0x60
#define AUX_MU_STAT_REG 0x64
#define AUX_MU_BAUD_REG 068

/* operations */
void mu_enable();
void mu_read(char *c);
void mu_write(char *c);
void mu_interrupt(int enable);
void mu_clear_dlab();
void mu_enable_tx();
void mu_enable_rx();
void mu_baud(int baud);

int mu_data_size(int size);
int mu_fifo_size();

#endif