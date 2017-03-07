/************************************************/
//MCU: SN8F5708
//PRJ: A7139
//DATE:2016-5-27
/************************************************/

#include <SN8F5708.h>
#include <intrins.h> //for _nop_
#include "A7139.h"
#include "clock.h"
#include "uart.h"
#include "led.h"
#include "rtimer.h"
#include "cmd.h"

rtimer_clock_t rt_now = 0;
struct rtimer rt;


void
rt_callback(void)
{
	printf("rtimer call back\n");
	toggle_led_red;
	rt_now = RTIMER_NOW();
	rtimer_set(&rt, rt_now + 65530UL, 1,(void *) rt_callback, &rt_now);
}
int main(void)
{
	
	WDTR = 0x5A;            // clear watchdog if watchdog enable
	led_init();             //led initial
	clock_init();
	uart_init();            //uart initial 57600
	rtimer_init();
	printf("test printf\n");
	rt_now = RTIMER_NOW();
	rtimer_set(&rt, rt_now + RTIMER_ARCH_SECOND*1UL, 1,(void *) rt_callback, &rt_now);
	cmdline();
	
	return 0;
}


#if 0

#include <SN8F5708.h>
#include <intrins.h> //for _nop_
#include "A7139.h"
#include "timer.h"
#include "uart.h"
#include "led.h"

void a7139_tx_packet(unsigned char *s,unsigned char n);

#define RF_STATE_A7139_RX   1
#define RF_STATE_A7139_TX   2
#define RF_STATE_A7139_IDLE 3
u8 idata rf_state_a7139 = 1;
u8 idata a7139_irq_status = 0;

u16 idata crc_check_code = 0;
u8 idata crc_check_code_l = 0;
u8 idata crc_check_code_h = 0;

extern u8 idata timer0_flag;
extern u8 idata timer1_flag;
u8 idata bufSend[]=	{"A7139"};
u8 idata bufRecv[7]={0};

u8 idata string_test[] = {"hellow !"};
u8 idata i = 0;

extern u8 idata UartRxFlag;
extern u8 idata RxBuf[7];
u8 xdata a7139_tx[7]={0};

//定义二维数组，存放向各节点要发送的数据，注意第3字节为节点地址，节点接收到数据后需比对节点地址
 u8 xdata slave_cmd[10][7] = {{0xaa,0xbb,0x01,0x00,0x00,0x3c,0x49},
														 {0xaa,0xbb,0x02,0x00,0x00,0x3c,0xb9},
														 {0xaa,0xbb,0x03,0x00,0x00,0xfc,0xe8},
														 {0xaa,0xbb,0x04,0x00,0x00,0x3d,0x59},
														 {0xaa,0xbb,0x05,0x00,0x00,0xfd,0x08},
														 {0xaa,0xbb,0x06,0x00,0x00,0xfd,0xf8},
														 {0xaa,0xbb,0x07,0x00,0x00,0x3d,0xa9},
													 	 {0xaa,0xbb,0x08,0x00,0x00,0x3e,0x99},
														 {0xaa,0xbb,0x09,0x00,0x00,0xfe,0xc8},
														 {0xaa,0xbb,0x0a,0x00,0x00,0xfe,0x38}};

void main(void)
{
	WDTR = 0x5A;            // clear watchdog if watchdog enable
	
	//led initial
	led_init();
	
	//uart initial 57600
	uart_init();
	
	//a7139 initial
	delay_ms(10);
	init_spi_a7139();
	delay_ms(10);
	if(A7139_Init(475.001f) != 0)
	{
		close_led_red; //a7139 initial fail! close led 
	}
	else
	{	  
		open_led_red; //a7139 initial ok!  open led
	}
	A7139_StrobeCmd(CMD_RX); //enter RX 
	delay_ms(10);//For some perform faster MCU, need time to wait at least more than 10 millisecond
//	rf_state_a7139 = RF_STATE_A7139_RX;
	rf_state_a7139 = RF_STATE_A7139_IDLE;
	
	//timer initial
	timer_init();
	
	 while(1)
	 {
			//send a tx packet per 100ms
		 if(timer1_flag == 1)
		 {
			 timer1_flag = 0;
				 
	     if(i < 10)
			 {
					a7139_tx_packet(slave_cmd[i],7);
					i++;
	  	 }
		 	 else
			 {
					i = 0;
			 }
		 }
		 
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
		 if(UartRxFlag == 1)
		 {
//			 crc_check_code = Get_CRC_Check_Code(RxBuf,5);
//			 crc_check_code_l = 0xff & crc_check_code;
//			 crc_check_code_h = 0xff & (crc_check_code>>8);
//			 uart_send_byte(crc_check_code_h);	//debug
//			 uart_send_byte(crc_check_code_l);
//			 if((RxBuf[5] == crc_check_code_h)&&(RxBuf[6] == crc_check_code_l))  //CRC 校验,高字节在前
//			 {
//					toggle_led_red;

//				  if(i < 10)
//					{
//						a7139_tx_packet(slave_cmd[i],7);
//						i++;
//					}
//					else
//					{
//						i = 0;
//					}
//			 }
//			 
//			 UartRxFlag = 0;
		 }
	 }
}


//INT1 interrupt function
void INT1Interrupt(void) interrupt ISRInt1 //
{ //IE0 clear by hardware
	a7139_irq_status = 1;	
}

//a7139 transmit a packet
void a7139_tx_packet(unsigned char *s,unsigned char n)
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

#endif
	


