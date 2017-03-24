/*****************************************************************************
 *
 * Copyright (C) 2017
 *
 * Authors: Cheny,
 *          
 *
 *****************************************************************************/
#ifndef _AODV_H
#define _AODV_H

#include "stdint.h"

#define AODV_RREQ_TYPE  0xFA 
#define AODV_RREP_TYPE  0xFB
#define AODV_DATA_TYPE  0xFC

#define RREQ_LEN  (14)
#define RREP_LEN  (12)

#define RREQ_LIST_BUF_LEN_MAX  (10)
#define ROUTE_LIST_BUF_LEN_MAX  (10)
#define AODV_DATA_BUF_LEN_MAX  (10)
#define AODV_DATA_BUF_LEN_BASE  (10)

  
typedef struct{
    uint8_t  type;
    uint8_t  hopcnt;
    uint16_t from_addr;
    uint16_t rreq_id;
    uint16_t dest_addr;
    uint16_t dest_seqno;
    uint16_t src_addr;
    uint16_t src_seqno;
} RREQ;

typedef struct 
{
    uint16_t src_addr;
    uint16_t rreq_id;
} RREQ_LIST;

typedef struct {
    uint8_t  type;
    uint8_t  hopcnt;
    uint16_t from_addr;
    uint16_t to_addr;
    uint16_t dest_addr;
    uint16_t dest_seqno;
    uint16_t src_addr;
} RREP;

typedef struct {
    uint16_t prehop_addr;
    uint16_t nexthop_addr;
    uint16_t src_addr;
    uint16_t dest_addr;
    uint16_t lifetime;
} ROUTE_LIST;

typedef struct {
    uint8_t  type;
	  uint8_t  len;
    uint16_t from_addr;
    uint16_t to_addr;
    uint16_t src_addr;
    uint16_t dest_addr;
    uint8_t  aodv_data_buf[AODV_DATA_BUF_LEN_MAX];
} AODV_DATA;



uint8_t search_in_routelist(uint16_t dest_addr);
uint8_t recv_rreq_process(RREQ *rreq_msg);
uint8_t recv_rrep_process(RREP *rrep_msg);
uint8_t aodv_init(void);
uint8_t aodv_data_send(AODV_DATA *aodv_data, uint16_t to_addr);
uint8_t aodv_data_pkg(uint8_t *dat, uint8_t len, uint16_t dest_addr, AODV_DATA *p);
uint8_t aodv_data_tx(uint8_t *dat, uint8_t len, uint16_t dest_addr);
uint8_t recv_aodv_data_process(AODV_DATA *aodv_data);

//uint8_t msg_type_to_rreq(uint8_t *msg, RREQ *p);
//uint8_t msg_type_to_rrep(uint8_t *msg, RREP *p);
//uint8_t msg_type_to_aodv_data_type(uint8_t *msg, AODV_DATA *p);

#endif				/* AODV_H */
