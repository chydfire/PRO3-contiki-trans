
#include "uart.h"

xdata uint8_t RxBuf[UART_BUF_LEN_MAX] = {0};
xdata uint8_t RxData = 0;
xdata uint8_t UartRxFlag = 0;
xdata uint8_t recv_head=0;
xdata uint8_t recv_tail=0;

//uart init
void uart_init(void)
{
	P05  = 1;			//UTX pin set high
	P0M |= 0x20;  //UTX is output,URX is input
	
	SM0 = 0;
	SM1 = 1;			//mode 1:8-bit uart
	REN0 = 1;			//uart reception enable
	
  switch (Baudrate) 
  {
        case BR_9600:
            S0RELH = 0x03;
            S0RELL = 0x98;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 9615
            BD = 1;
            break;
        case BR_19200:
            S0RELH = 0x03;
            S0RELL = 0xCC;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 19230
            BD = 1;
            break;
        case BR_38400:
            S0RELH = 0x03;
            S0RELL = 0xE6;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 38461
            BD = 1;
            break;
        case BR_57600:
            S0RELH = 0x03;
            S0RELL = 0xEF;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 58823
            BD = 1;
            break;
        case BR_115200:
            S0RELH = 0x03;
            S0RELL = 0xF7;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 111111
            BD = 1;
            break;
        default:
            // br_9600
            S0RELH = 0x03;
            S0RELL = 0x98;      // for baudrate gen

            PCON |= 0x80;       // SMOD = 1;    
                                // baudrate : 9615
            BD = 1;
            break;
  }
	
	ES0 = 1;   //enable uart interrupt
	EAL = 1;   //enable golbal interrupt
}

//uart interrupt function
/*************************************************************************************
uart接收命令帧格式（下行）
起始字节 + 同步字节 + 节点地址 + 数据区（2个字节） + CRC校验码高字节 + CRC校验码低字节
0xaa        0xbb      (1 -100)
**************************************************************************************/
void SYSUartInterrupt(void) interrupt ISRUart  // Vector @  0x23
{
    if(RI0)
		{
			  register uint8_t next = 0;
			
        RI0 = 0;                // Clear receiver flag
        RB80 = 0;

	      next = (recv_head + 1) % UART_BUF_LEN_MAX;
		    if( next != recv_tail )
		    {
			      RxBuf[recv_head] = S0BUF;
			      recv_head = next;
		    }
	  }
}

//UART send one byte function
void uart_send_byte(unsigned char txData)
{
    S0BUF = txData;

	  while(!TI0);       // wait for end of transmit

    if (TI0) 
		{
        TI0 = 0;
    }
}
//UART send string function
void uart_send_string(unsigned char *s,unsigned char n)
{
   unsigned char idata i = 0;
	 for(i=0;i<n;i++)
	 {
		uart_send_byte(s[i]);
	 }
}
//发送一个字符串
void Prints(unsigned char *pd)
{
	while((*pd) != '\0')
	{
  	uart_send_byte(*pd);
		pd++;
	}
}

unsigned int Get_CRC_Check_Code(unsigned char *s,unsigned int n)
{
 unsigned char idata i,j;
 unsigned int  idata CRC_Code = 0xFFFF;
 for(i=0;i<n;i++)
 {
     CRC_Code ^= s[i];
     for(j=0;j<8;j++)
     {
        if(CRC_Code&1)
        {
         CRC_Code >>= 1;
         CRC_Code ^= 0xA001;
        }
        else
        {
         CRC_Code >>= 1;
        }
     }
 }
 return CRC_Code;
}


void
uart_writeb(char byte)
{
    S0BUF = byte;

	  while(!TI0);       // wait for end of transmit

    if (TI0) 
		{
        TI0 = 0;
    }
}

char
putchar(char c)
{
  uart_writeb((char) c);
  return c;
}

char getchar(void)
{
	register uint8_t p = 0;
	
	if(recv_tail != recv_head)
	{
	    p = RxBuf[recv_tail];
	    recv_tail = (recv_tail + 1) % UART_BUF_LEN_MAX;
	}
	
	return (char)p;
}

uint8_t avalible(void)
{
	uint8_t cnt = 0;
	
	if(recv_tail < recv_head)
	{
		 cnt = recv_head - recv_tail;
	}
	else if(recv_tail > recv_head)
	{
		 cnt = UART_BUF_LEN_MAX - recv_tail + recv_head;
	}
	
	return cnt;
}