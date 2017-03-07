

#include "clock.h"
#include "SN8F5708.H"
#include "8051def.h"
#include "led.h"

#define MAX_TICKS (~((clock_time_t)0) / 2)
/*---------------------------------------------------------------------------*/
/* Do NOT remove the absolute address and do NOT remove the initialiser here */
//__xdata __at(0x0000) unsigned long timer_value = 0; // TODO
static volatile unsigned long timer_value = 0;

static volatile  __data clock_time_t count = 0; /* Uptime in ticks */
static volatile  __data clock_time_t seconds = 0; /* Uptime in secs */


#define T0Mode0 (0 << 0) //T0 mode0, 13-bit counter
#define T0Mode1 (1 << 0) //T0 mode1, 16-bit counter
#define T0Mode2 (2 << 0) //T0 mode2, 8-bit auto-reload counter
#define T0Mode3 (3 << 0) //T0 mode3, T0 two 8-bit counter/T1 no flag
#define T0GATE (8 << 0) //T0 gating clock by INT0
#define T0ClkFcpu (0 << 0) //T0 clock source from Fcpu/12
#define T0ClkExt (4 << 0) //T0 clock source from Fosc or FRTC
#define T0ExtFosc (0 << 4) //T0 clock source from Fosc
#define T0ExtFRTC (8 << 4) //T0 clock source from FRTC
#define T1Mode0 (0 << 4) //T1 mode0, 13-bit counter
#define T1Mode1 (1 << 4) //T1 mode1, 16-bit counter
#define T1Mode2 (2 << 4) //T1 mode2, 8-bit auto-reload counter
#define T1Mode3 (3 << 4) //T1 mode3, T1 stop
#define T1GATE (8 << 4) //T1 gating clock by INT1
#define T1ClkFcpu (0 << 4) //T1 clock source from Fcpu/12
#define T1ClkExt (4 << 4) //T1 clock source from Fosc

void clock_init(void)
{
	//CLKSEL = 0x04; // set fcpu = fosc / 8
  //CLKCMD = 0x69;
	// T1 mode1, clock source from Fosc
	TMOD |= T1Mode1 | T1ClkFcpu;
	// Timer 1 clock = Fcpu/12 = 2.67MHz
	//TCON0 = 0x05;
	// T1_Initial
	// 32M/12/20833 = 128
	// 65536-20833=0xAE9F
	//TMOD &= 0xBF;
	TH1 = 0xAE;
	TL1 = 0x9F;
	TCON  |= 0x40;//enable timer1
	IEN0  |= 0x88;//enable timer1 interrupt
}

void clock_isr(void) interrupt ISRTimer1 //0x1B
{ 
	ET1=0;
	
	++count;
	if((count % CLOCK_CONF_SECOND) == 0) {
    ++seconds;
		toggle_led_blue;
  }
	TH1 = 0xAE;
	TL1 = 0x9F;
  ET1=1;
}

/*---------------------------------------------------------------------------*/
/**
 * One delay is about 0.6 us, so this function delays for len * 0.6 us
 */
void
clock_delay(unsigned int len)
{
  unsigned int i;
  for(i = 0; i< len; i++) {
    ASM(nop);
  }
}
/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of ~8 ms (a tick)
 */
void
clock_wait(clock_time_t i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < i);
}
/*---------------------------------------------------------------------------*/
CCIF clock_time_t
clock_time(void)
{
  return count;
}
/*---------------------------------------------------------------------------*/
CCIF unsigned long
clock_seconds(void)
{
  return seconds;
}
