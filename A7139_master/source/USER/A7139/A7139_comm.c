#include "A7139_comm.h"
#include "A7139.h"
#include "SN8F5708.h"
#include "uart.h"
#include "led.h"
#include "cmd.h"

uint8_t idata rf_state_a7139 = RF_STATE_A7139_IDLE;
uint8_t idata a7139_irq_status = 0;

uint16_t idata crc_check_code = 0;
uint8_t idata crc_check_code_l = 0;
uint8_t idata crc_check_code_h = 0;

extern uint8_t idata timer0_flag;
extern uint8_t idata timer1_flag;
uint8_t idata bufSend[]=	{"A7139"};
uint8_t idata bufRecv[RF_RECV_BUF_LEN_MAX]={0};

uint8_t idata string_test[] = {"hellow !"};
uint8_t idata i = 0;

extern uint8_t idata UartRxFlag;
extern uint8_t idata RxBuf[RF_RECV_BUF_LEN_MAX];
uint8_t xdata a7139_tx[RF_RECV_BUF_LEN_MAX]={0};

extern uint8_t xdata per_second_flag;
extern xdata uint8_t rf_tx_valid_flag;

//定义二维数组，存放向各节点要发送的数据，注意第3字节为节点地址，节点接收到数据后需比对节点地址
// uint8_t xdata slave_cmd[10][RF_RECV_BUF_LEN_MAX] = {{0xaa,0xbb,0x0a,0x00,0x00,0xfe,0x38},
//														 {0xaa,0xbb,0x02,0x00,0x00,0x3c,0xb9},
//														 {0xaa,0xbb,0x03,0x00,0x00,0xfc,0xe8},
//														 {0xaa,0xbb,0x04,0x00,0x00,0x3d,0x59},
//														 {0xaa,0xbb,0x05,0x00,0x00,0xfd,0x08},
//														 {0xaa,0xbb,0x06,0x00,0x00,0xfd,0xf8},
//														 {0xaa,0xbb,0x07,0x00,0x00,0x3d,0xa9},
//													 	 {0xaa,0xbb,0x08,0x00,0x00,0x3e,0x99},
//														 {0xaa,0xbb,0x09,0x00,0x00,0xfe,0xc8},
//														 {0xaa,0xbb,0x0a,0x00,0x00,0xfe,0x38}};
 
														 
//INT1 interrupt function
void INT1Interrupt(void) interrupt ISRInt1 //
{ //IE0 clear by hardware
	a7139_irq_status = 1;	
}

//a7139 transmit a packet
void a7139_tx_packet(uint8_t *s,uint8_t n)
{
		A7139_StrobeCmd(CMD_STBY);
		delay_ms(10);
	  //send a tx packet
//		A7139_WriteFIFO(s,7);
		A7139_WriteFIFO(s,n);
		A7139_StrobeCmd(CMD_TX);
		delay_ms(10);//For some perform faster MCU, need time to wait at least more than 10 millisecond
		rf_state_a7139 =  RF_STATE_A7139_TX;
}

int a7139_master()
{
	  int ret = TRUE;
	  int i = 0;
	
		//send a tx packet per 100ms
//		 if(per_second_flag == 1)
//		 {
//			 per_second_flag = 0;
//				 
//	     if(i < 10)
//			 {
//					a7139_tx_packet(slave_cmd[i],RF_RECV_BUF_LEN_MAX);
//					i++;
//	  	 }
//		 	 else
//			 {
//					i = 0;
//			 }
//		 }
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
				
		  		crc_check_code = Get_CRC_Check_Code(bufRecv,sizeof(bufRecv)-2);
					crc_check_code_l = 0xff & crc_check_code;
					crc_check_code_h = 0xff & (crc_check_code>>8);
				  if((crc_check_code_h == bufRecv[5])&&(crc_check_code_l == bufRecv[6]))  //CRC 校验,高字节在前
					{
						if(bufRecv[2] == 0x0a)//节点地址
						{
							uart_send_string(bufRecv,sizeof(bufRecv));//debug 
							toggle_led_red;
						}
					}
					break;
				case RF_STATE_A7139_TX:
					//tx completed
				  toggle_led_blue;//debug			
					A7139_StrobeCmd(CMD_RX); //enter RX 
					delay_ms(10);//For some perform faster MCU, need time to wait at least more than 10 millisecond
					rf_state_a7139 = RF_STATE_A7139_RX;				
					break;
			}

			a7139_irq_status = 0;
		 }
		 //uart received a packet
		 if(rf_tx_valid_flag == 1)
		 {
			 a7139_tx_packet(a7139_tx,RF_RECV_BUF_LEN_MAX);
			 
			 rf_tx_valid_flag = 0;
		 }
	 return ret;
}