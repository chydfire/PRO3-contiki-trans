
#include "led.h"


void led_init(void)
{	 
  P2M  |= 0xC0;		//P2.6 P2.7 output mode
	P4UR &= 0x3F;   //disable pull-up resister	 
	P26   = 1;      //P2.6 output high level	
	P27   = 1;      //P2.7 output high level
}









