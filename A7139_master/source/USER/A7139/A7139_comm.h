#ifndef __A7139_COMM_H__
#define __A7139_COMM_H__

#include "stdint.h"

#define RF_STATE_A7139_RX   1
#define RF_STATE_A7139_TX   2
#define RF_STATE_A7139_IDLE 3

#define RF_RECV_BUF_LEN_MAX   ( 7 )

void a7139_tx_packet(uint8_t *s,uint8_t n);
int a7139_master();

#endif //__A7139_COMM_H__