#include "timer.h"
unsigned int idata timer0_count = 0;
unsigned char idata timer0_flag = 0;
unsigned int idata timer1_count = 0;
unsigned char idata timer1_flag = 0;

//timer0 init 25ms
void timer0_init_25ms(void)
{
	TH0   = 0x2F; // 65536-53333 = 122203 = 0x2fAB
	TL0   = 0xAB;
	TMOD  |= 0x01;//Timer0 mode1;时钟源 Fcpu/12
	TCON0 = 0x00;
	TCON  |= 0x10;//enable timer0
	IEN0  |= 0x82;//enable timer0 interrupt
}
//timer0 interrupt function
void T0Interrupt(void) interrupt ISRTimer0 // 0x0B
{// TF0 clear by hardware
	if(timer0_count >= 10)//250ms
	{
		timer0_count = 0;
		timer0_flag  = 1;
		
	}
	else
	{
		timer0_count++;
	}
}

//timer1_init_1ms
void timer1_init_25ms(void)
{

//  TH1   = 0x97; // 65536-26666 = 38870 = 0x97D6
//	TL1   = 0xD6;
	TH1   = 0x00; //26666 = 0x97D6
	TL1   = 0x00;
	TMOD  |= 0x10;  //Timer1 mode1;时钟源 Fcpu/12
	TCON0 = 0x00;
	TCON  |= 0x40;//enable timer1
	IEN0  |= 0x88;//enable timer1 interrupt
}
//timer1 interrupt function
void T1Interrupt(void) interrupt ISRTimer1 
{// TF1 clear by hardware
	if(timer1_count >= 3)//100ms
	{
		timer1_count = 0;
		timer1_flag  = 1;
	
	}
	else
	{
		timer1_count++;	
	}
}

void timer_init(void)
{
//	timer0_init_25ms();
	timer1_init_25ms();
}
