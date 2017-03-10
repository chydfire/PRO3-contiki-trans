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
#include "A7139_comm.h"

//rtimer_clock_t rt_now = 0;
//struct rtimer rt;


//void
//rt_callback(void)
//{
//	printf("rtimer call back\n");
//	toggle_led_red;
//	rt_now = RTIMER_NOW();
//	rtimer_set(&rt, rt_now + 65530UL, 1,(void *) rt_callback, &rt_now);
//}
int main(void)
{
	uint16_t id = 0;
	WDTR = 0x5A;            // clear watchdog if watchdog enable
	led_init();             //led initial
	clock_init();
	uart_init();            //uart initial 57600
	rtimer_init();
	//rt_now = RTIMER_NOW();
	//rtimer_set(&rt, rt_now + RTIMER_ARCH_SECOND*1UL, 1,(void *) rt_callback, &rt_now);
	//a7139 initial
	clock_delay_ms(10);
	init_spi_a7139();
	clock_delay_ms(10);
	
	if(A7139_Init(475.001f) != 0)
	{
		printf("a7139 initial fail!\n");
	}
	
	A7139_StrobeCmd(CMD_RX); //enter RX
	clock_delay_ms(10);
	
	
	while(TRUE)
	{
		  a7139_master();
		  clock_delay_ms(10);
		  cmd_master();
	}
	
	return 0;
}
