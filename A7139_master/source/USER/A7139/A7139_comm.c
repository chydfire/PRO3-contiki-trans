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


uint8_t idata bufSend[]=	{"A7139"};
uint8_t idata bufRecv[RF_RECV_BUF_LEN_MAX]={0};

uint8_t idata string_test[] = {"hellow !"};
uint8_t idata i = 0;

extern uint8_t idata UartRxFlag;
extern uint8_t idata RxBuf[RF_RECV_BUF_LEN_MAX];
uint8_t xdata a7139_tx[RF_RECV_BUF_LEN_MAX]={0};

extern uint8_t xdata per_second_flag;
extern xdata uint8_t rf_tx_valid_flag;

//�����ά���飬�������ڵ�Ҫ���͵����ݣ�ע���3�ֽ�Ϊ�ڵ��ַ���ڵ���յ����ݺ���ȶԽڵ��ַ
 uint8_t xdata slave_cmd[10][RF_RECV_BUF_LEN_MAX] = {{0xaa,0xbb,0x01,0x00,0x00,0x3c,0x49},
														 {0xaa,0xbb,0x02,0x00,0x00,0x3c,0xb9},
														 {0xaa,0xbb,0x03,0x00,0x00,0xfc,0xe8},
														 {0xaa,0xbb,0x04,0x00,0x00,0x3d,0x59},
														 {0xaa,0xbb,0x05,0x00,0x00,0xfd,0x08},
														 {0xaa,0xbb,0x06,0x00,0x00,0xfd,0xf8},
														 {0xaa,0xbb,0x07,0x00,0x00,0x3d,0xa9},
													 	 {0xaa,0xbb,0x08,0x00,0x00,0x3e,0x99},
														 {0xaa,0xbb,0x09,0x00,0x00,0xfe,0xc8},
														 {0xaa,0xbb,0x0a,0x00,0x00,0xfe,0x38}};
 
														 
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
int a7139_master()
{
	  int ret = TRUE;
	  uint8_t rx_tmp = 0;
//	  static int i = 0;
//	   uint8_t tmp[RF_RECV_BUF_LEN_MAX] = {0xaa,0x07,0x0a,0x00,0x01,0x02,0x03,0x04,0x05,0xE0};
	
		//send a tx packet per 100ms
//		 if(per_second_flag == 1)
//		 {
//			 per_second_flag = 0;
//				 a7139_tx_packet(tmp,RF_RECV_BUF_LEN_MAX);
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
				
				  rx_tmp=bufRecv[1];
				
				  if(bufRecv[rx_tmp+2]==chkSumCalc(&bufRecv[1],rx_tmp+1))  //CRC У��,���ֽ���ǰ
					{
						if(bufRecv[2] == 0x0a)//�ڵ��ַ
						{
							uart_send_string1(bufRecv,sizeof(bufRecv));//debug 
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
