#ifndef __DRIVERS_GPC_H
#define __DRIVERS_GPC_H

#define BUFF_MAX_RECV   (100)
#define BUFF_MAX_SEND   (100)
#define PIN_TX          2
#define PIN_RX          3
#define PIN_TA          21
#define PIN_RD          20
#define PIN_ID          16
#define PIN_IA          17

/* state related things */
extern void (*state)(int);

/* book-keeping */
void init_gpc();

/* transmit related */
void putb(int b);
void putc(char c);
void printg(char *c);

/* testing our revised functions */
int bits_to_send();
void exec();

#endif