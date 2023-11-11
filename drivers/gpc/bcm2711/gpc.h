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
enum gpc_state {
    IDLE,
    PENDING_CONFIRMATION,
    PREP_TX,
    ASSERT_TA
};

/* book-keeping */
void init_gpc();

/* transmit related */
int send(char *c);
int putc(char c);
void transmitb(int bit);

/* testing our revised functions */
void _send(char *c);
void parse_state();

#endif