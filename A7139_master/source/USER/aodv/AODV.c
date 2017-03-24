/*****************************************************************************
 *
 * Copyright (C) 2017 TaDunTech
 *
 * Authors: Cheny
 *          
 *
 *****************************************************************************/
#include "AODV.h"
#include "A7139_comm.h"
#include "clock.h"
#include "stdio.h"
#include "uart.h"
#include "led.h"

RREQ xdata rreq_msg_buf;
RREQ_LIST xdata rreq_list_buf[RREQ_LIST_BUF_LEN_MAX];
RREP xdata rrep_msg_buf;
ROUTE_LIST xdata route_list_buf[ROUTE_LIST_BUF_LEN_MAX];

uint8_t xdata rreq_list_length = 0;
uint8_t xdata route_list_length = 0;
uint8_t xdata route_success_flag = 0;

uint8_t rreq_init(RREQ *rreq_msg)
{
	  rreq_msg-> type = AODV_RREQ_TYPE;
    rreq_msg-> hopcnt = 0;
    rreq_msg-> from_addr = NODE_ADDR;
    rreq_msg-> rreq_id = 0;
	  rreq_msg-> dest_addr = 0;
	  rreq_msg-> dest_seqno = 0;
    rreq_msg-> src_addr = NODE_ADDR;
    rreq_msg-> src_seqno = 0;

    return 0;
}

uint8_t rreq_create(RREQ *rreq_msg, uint16_t dest_addr)
{   
	  rreq_msg-> rreq_id = rreq_msg-> rreq_id + 1;
    rreq_msg-> dest_addr = dest_addr;
    rreq_msg-> dest_seqno = 0;
    rreq_msg-> src_seqno = rreq_msg-> src_seqno + 1;
	
    return 0;
}

uint8_t rreq_send(RREQ *rreq_msg)
{
	/*broadcast(&rreq_msg)*/
	a7139_tx_packet((uint8_t *)rreq_msg, RREQ_LEN);
	//a7139_tx_packet(test_rreq_buf, RREQ_LEN);
	return 0;
}

uint8_t rreq_retrans(RREQ *rreq_msg)
{
	rreq_msg-> hopcnt += 1;
	rreq_msg-> from_addr = NODE_ADDR;
	rreq_send(rreq_msg);

	return 0;
}

uint8_t update_rreq_list(RREQ *rreq_msg)
{
	int i = 0;

	while(i < rreq_list_length)
	{
		if((rreq_list_buf[i].src_addr) == (rreq_msg-> src_addr))
			break;
		i++;
	}
	if(i < rreq_list_length)
	{
		if((rreq_msg-> rreq_id) > (rreq_list_buf[i].rreq_id))
		{
			rreq_list_buf[i].rreq_id = rreq_msg-> rreq_id;
			return 1;
		}
		else
		  return 0;
	}
	else
	{
		rreq_list_buf[i].src_addr = rreq_msg-> src_addr;
		rreq_list_buf[i].rreq_id = rreq_msg-> rreq_id;
		rreq_list_length++;
		return 1;
	}
}

uint8_t rrep_init(RREP *rrep_msg)
{
	  rrep_msg-> type = AODV_RREP_TYPE;
    rrep_msg-> from_addr = NODE_ADDR;
    rrep_msg-> dest_addr = NODE_ADDR;
    rrep_msg-> dest_seqno = 0;

    return 0;
}

uint8_t rrep_create(RREQ *rreq_msg, RREP *rrep_msg)
{	
    rrep_msg-> hopcnt = rreq_msg-> hopcnt;
    rrep_msg-> dest_seqno += 1;
    rrep_msg-> src_addr = rreq_msg->src_addr;

    return 0;
}

uint8_t rrep_send(RREP *rrep_msg, uint16_t to_addr)
{
	rrep_msg-> from_addr = NODE_ADDR;
	rrep_msg-> to_addr = to_addr;
	a7139_tx_packet((uint8_t *)rrep_msg, RREP_LEN);
	
	return 0;
}

uint8_t search_in_routelist(uint16_t dest_addr)
{
	int i = 0;

	while(i < route_list_length)
	{
		if(route_list_buf[i].dest_addr == dest_addr)
			break;
		i++;
	}

	return i;
}

uint8_t update_prehop_route_list(RREQ *rreq_msg)
{
	int result;

	result = search_in_routelist(rreq_msg-> dest_addr);
	route_list_buf[result].prehop_addr = rreq_msg-> from_addr;
	if(result < route_list_length)
	{
		return result;
	}
	else
	{
		route_list_buf[result].src_addr = rreq_msg-> src_addr;
		route_list_buf[result].dest_addr = rreq_msg-> dest_addr;
		route_list_length++;
		return route_list_length;
	}
}

uint8_t update_nexthop_route_list(RREP *rrep_msg)
{
	int result;

	result = search_in_routelist(rrep_msg-> dest_addr);
	route_list_buf[result].nexthop_addr = rrep_msg-> from_addr;
	if(result < route_list_length)
	{
		return result;
	}
	else
	{
		route_list_buf[result].src_addr = rrep_msg-> src_addr;
		route_list_buf[result].dest_addr = rrep_msg-> dest_addr;
		route_list_length++;
		return route_list_length;
	}

	return result;
}

uint8_t recv_rreq_process(RREQ *rreq_msg)
{
	int result;

	if(NODE_ADDR != rreq_msg-> src_addr)                                
	{
		if(update_rreq_list(rreq_msg))
		{
			result = update_prehop_route_list(rreq_msg);	
			if(NODE_ADDR == rreq_msg-> dest_addr)
			{
				rrep_create(rreq_msg, &rrep_msg_buf);
				rrep_send(&rrep_msg_buf, route_list_buf[result].prehop_addr);
			}
			else
			{
				rreq_retrans(rreq_msg);
			}
		}
	}
	
	return 0;
}

uint8_t recv_rrep_process(RREP *rrep_msg)
{
	uint8_t result;

	result = update_nexthop_route_list(rrep_msg);
	if(NODE_ADDR == rrep_msg-> src_addr)
		route_success_flag = 1;
	else
		rrep_send(rrep_msg, route_list_buf[result].prehop_addr);

	return 0;
}

uint8_t aodv_init(void)
{
	rreq_init(&rreq_msg_buf);
	rrep_init(&rrep_msg_buf);
	
	return 0;
}

uint8_t aodv_data_send(AODV_DATA *aodv_data, uint16_t to_addr)
{
	aodv_data-> from_addr = NODE_ADDR;
	aodv_data-> to_addr = to_addr;
	a7139_tx_packet((uint8_t *)aodv_data, aodv_data-> len + AODV_DATA_BUF_LEN_BASE);
	
	return 0;
}

uint8_t aodv_data_pkg(uint8_t *dat, uint8_t len, uint16_t dest_addr, AODV_DATA *p)
{
	int i;
	
	p-> type = AODV_DATA_TYPE;
  p-> from_addr = NODE_ADDR;
  p-> src_addr = NODE_ADDR;
  p-> dest_addr = dest_addr;
  p-> len = len;
	for(i=0;i<len;i++)
	{
		p-> aodv_data_buf[i] = *(dat + i);
	}
	
	return 0;
}

uint8_t aodv_data_tx(uint8_t *dat, uint8_t len, uint16_t dest_addr)
{
	uint8_t result;
	uint8_t rrep_overtime_flag = 0;
	uint16_t time_rreq_send;
	AODV_DATA p;

	result = search_in_routelist(dest_addr);
		if(result < route_list_length)
		{
			/*broadcast(&data, NODE_ADDR, *(route_list_buf + result).nexthop, NODE_ADDR, dest_addr);*/
			aodv_data_pkg(dat, len, dest_addr, &p);
			aodv_data_send(&p, route_list_buf[result].nexthop_addr);
			
			return TRUE;
		}
		else
		{
			rreq_create(&rreq_msg_buf, dest_addr);
			rreq_send(&rreq_msg_buf);
			uart_send_string1(&rreq_msg_buf , RREQ_LEN);
		}


//		time_rreq_send = clock_time();
//		while(!route_success_flag)
//		{
//			if(clock_time() == (time_rreq_send + 3*CLOCK_CONF_SECOND))
//			{
//				rrep_overtime_flag = 1;
//				break;
//			}
//		}
//		if(rrep_overtime_flag)
//		{
//			printf("Tx failed as rrep overtime\n");
//			return FALSE;
//		}
//		else
//		{
//			/*broadcast(&data, NODE_ADDR, *(route_list_buf + result).nexthop, NODE_ADDR, dest_addr);*/
//			aodv_data_pkg(dat, len, dest_addr, &p);
//			aodv_data_send(&p, route_list_buf[result].nexthop_addr);
//			return TRUE;
//		}		
}

uint8_t recv_aodv_data_process(AODV_DATA *aodv_data)
{
	uint8_t result;

	if(NODE_ADDR == aodv_data-> dest_addr)
	{
		toggle_led_red;
		printf("Recv from NODE-%s:\n", aodv_data-> src_addr);
		uart_send_string1(aodv_data-> aodv_data_buf, aodv_data-> len);
	}
	else
	{
			aodv_data_tx(aodv_data-> aodv_data_buf, aodv_data-> len, aodv_data-> dest_addr);	
	}
	
	return 0;
}


//uint8_t msg_type_to_rreq(uint8_t *msg, RREQ *p)
//{
//	  p-> type = *msg;
//    p-> hopcnt = *(msg+1);
//    p-> from_addr = *(msg+2)<<8 + *(msg+3);
//    p-> rreq_id = *(msg+4)<<8 + *(msg+5);
//    p-> dest_addr = *(msg+6)<<8 + *(msg+7);
//    p-> dest_seqno = *(msg+8)<<8 + *(msg+9);
//    p-> src_addr = *(msg+10)<<8 + *(msg+11);
//    p-> src_seqno = *(msg+12)<<8 + *(msg+13);
//	
//	return 0;
//}
//uint8_t msg_type_to_rrep(uint8_t *msg, RREP *p)
//{
//		p-> type = *msg;
//    p-> hopcnt = *(msg+1);
//    p-> from_addr = *(msg+2)<<8 + *(msg+3);
//    p-> to_addr = *(msg+4)<<8 + *(msg+5);
//    p-> dest_addr = *(msg+6)<<8 + *(msg+7);
//    p-> dest_seqno = *(msg+8)<<8 + *(msg+9);
//    p-> src_addr = *(msg+10)<<8 + *(msg+11);
//	
//	return 0;
//}
//uint8_t msg_type_to_aodv_data_type(uint8_t *msg, AODV_DATA *p)
//{
//	  int i;  
//	
//	  p-> type = *msg;
//	  p-> len = *(msg+1);
//    p-> from_addr = *(msg+2)<<8 + *(msg+3);
//    p-> to_addr = *(msg+4)<<8 + *(msg+5);
//    p-> src_addr = *(msg+6)<<8 + *(msg+7);
//    p-> dest_addr = *(msg+8)<<8 + *(msg+9);
//	  for(i=0;i<p-> len;i++)
//		{
//			p-> aodv_data_buf[i] = *(msg+9+i);
//		}
//		
//		return 0;
//}