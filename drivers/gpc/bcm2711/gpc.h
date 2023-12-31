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
int putb(int b);
int putc(char c);
int puts(char *c);

/* testing our revised functions */
int buffer_empty();
void buffer_flush();
void exec();

#endif