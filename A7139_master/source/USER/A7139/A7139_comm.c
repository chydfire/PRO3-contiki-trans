#include "A7139_comm.h"
#include "A7139.h"
#include "SN8F5708.h"
#include "uart.h"
#include "led.h"
#include "cmd.h"
#include "AODV.h"

uint8_t xdata rf_state_a7139 = RF_STATE_A7139_IDLE;
uint8_t xdata a7139_irq_status = 0;



uint8_t xdata bufSend[]=	{"A7139"};
uint8_t xdata bufRecv[RF_RECV_BUF_LEN_MAX]={0};


extern uint8_t xdata RxBuf[RF_RECV_BUF_LEN_MAX];
uint8_t xdata a7139_tx[RF_RECV_BUF_LEN_MAX]={0};

extern xdata uint8_t rf_tx_valid_flag;

xdata uint8_t rf_retx_valid_flag = 0;

#define BLACK_LIST_LEN  (3)
uint16_t xdata black_list_buf[BLACK_LIST_LEN]={1,4,5};
extern uint8_t route_success_flag;
extern ROUTE_LIST route_list_buf[ROUTE_LIST_BUF_LEN_MAX];
														 
//INT1 interrupt function
void INT1Interrupt(void) interrupt ISRInt1 //
{ //IE0 clear by hardware
	a7139_irq_status = 1;	
}

//a7139 transmit a packet
void a7139_tx_packet(uint8_t *s,uint8_t n)
{
		A7139_StrobeCmd(CMD_STBY);
		clock_delay_ms(10);
	  //send a tx packet
//		A7139_WriteFIFO(s,7);
		A7139_WriteFIFO(s,n);
		A7139_StrobeCmd(CMD_TX);
		clock_delay_ms(10);//For some perform faster MCU, need time to wait at least more than 10 millisecond
		rf_state_a7139 =  RF_STATE_A7139_TX;
}
uint8_t chkSumCalc( const uint8_t * pData, uint8_t len )
{
    uint8_t chksum = 0;
    uint8_t i = 0;

    for( i = 0; i < len; i++ )
    {
        chksum += pData[i];
    }

    chksum = ~chksum + 1;
    return chksum;
}

uint8_t in_black_list(uint16_t from_addr)
{
	int i = 0;
	
	while(i<BLACK_LIST_LEN)
	{
		if(black_list_buf[i] == from_addr)
			break;
		i++;
	}
	if(i<BLACK_LIST_LEN)
		return 1;
	else
		return 0;
}

int a7139_master()
{
	  int ret = TRUE;
	  uint8_t rx_tmp = 0;

		 //A7139 interrupt
		 if(a7139_irq_status == 1)
		 {
			switch(rf_state_a7139)
			{
				case RF_STATE_A7139_IDLE:
					A7139_ReadFIFO(bufRecv,sizeof(bufSend));
					A7139_StrobeCmd(CMD_RX);
					rf_state_a7139 = RF_STATE_A7139_RX;
					break;
				case RF_STATE_A7139_RX:
					A7139_ReadFIFO(bufRecv,sizeof(bufRecv));
					A7139_StrobeCmd(CMD_RX);
				
				  if(!in_black_list((bufRecv[2]<<8)+bufRecv[3]))
					{
						switch(bufRecv[0])
						{ 
							case AODV_RREQ_TYPE:
								recv_rreq_process((RREQ *)bufRecv);
								break;
							case AODV_RREP_TYPE:
									if(NODE_ADDR == ((bufRecv[4]<<8)+bufRecv[5]))
										recv_rrep_process((RREP *)bufRecv);
								break;
							case AODV_DATA_TYPE:
									if(NODE_ADDR == ((bufRecv[4]<<8)+bufRecv[5]))
										recv_aodv_data_process((AODV_DATA *)bufRecv);
								break;
						}
					}
				  
					break;
				case RF_STATE_A7139_TX:
					//tx completed
				  blink_led_blue;//debug			
					A7139_StrobeCmd(CMD_RX); //enter RX 
					clock_delay_ms(10);//For some perform faster MCU, need time to wait at least more than 10 millisecond
					rf_state_a7139 = RF_STATE_A7139_RX;				
					break;
			}

			a7139_irq_status = 0;
		 }
		 if(rf_retx_valid_flag)
		{
			clock_delay_ms(1000);
			a7139_tx_packet(bufRecv,bufRecv[1]+3);
			rf_retx_valid_flag = 0;
		}
		 
		 //uart received a packet
		 if(rf_tx_valid_flag == 1)
		 {
			 //a7139_tx_packet(a7139_tx,a7139_tx[1]+3);
			 aodv_data_tx(&a7139_tx[4], a7139_tx[1]-2, a7139_tx[2]);
			 rf_tx_valid_flag = 0;
		 }
		 if(route_success_flag)
		 {
			 uint8_t result;
			 AODV_DATA p;
			 
			 result = search_in_routelist(a7139_tx[2]);
			 aodv_data_pkg(&a7139_tx[4], a7139_tx[1]-2, a7139_tx[2], &p);
			 aodv_data_send(&p, route_list_buf[result].nexthop_addr);
			 uart_send_string1(&p, p.len+AODV_DATA_BUF_LEN_BASE);
			 route_success_flag = 0;
		 }
	 return ret;
}
