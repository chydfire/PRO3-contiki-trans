#ifndef __A7139_COMM_H__
#define __A7139_COMM_H__

#include "stdint.h"

#define RF_NODE_ID          0x0a
#define RF_STATE_A7139_RX   1
#define RF_STATE_A7139_TX   2
#define RF_STATE_A7139_IDLE 3

#define RF_RECV_BUF_LEN_MAX   ( 40)

void a7139_tx_packet(uint8_t *s,uint8_t n);
int a7139_master();
uint8_t chkSumCalc( const uint8_t * pData, uint8_t len );

#endif //__A7139_COMM_H__