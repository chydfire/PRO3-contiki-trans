/*
 * Copyright (c) 2010, Loughborough University - Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Hardware-dependent functions used to support the
 *         contiki rtimer module.
 *
 *         clock_init() has set our tick speed prescaler already, so we
 *         are ticking with 500 kHz freq.
 *
 *         Contiki typedefs rtimer_clock_t as unsigned short (16bit)
 *         It thus makes sense to use the 16bit timer (Timer 1)
 *
 *         This file contains an ISR and must reside in the HOME bank
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#include "rtimer.h"
#include <SN8F5708.h>


#define T2ClkFcpu (1 << 0) //T2 clock from Fcpu
#define T2ClkPin (2 << 0) //T2 clock from T2 pin
#define T2ClkGate (3 << 0) //T2 clock from Fcpu with T2 pin gating
#define T2Fcpu12 (0 << 7) //T2 clock = Fcpu/12
#define T2Fcpu24 (1 << 7) //T2 clock = Fcpu/24
#define T2RLMode0 (2 << 3) //T2 reload mode0 = auto-reload
#define T2RLMode1 (3 << 3) //T2 reload mode1 = T2RL falling edge trigger
#define ComMode0 (0 << 2) //Compare mode = directly method
#define ComMode1 (1 << 2) //Compare mode = indirectly output method
#define T2COM0EdNE (0 << 6) //T2COM0 interrupt edge = no equle CRC
#define T2COM0EdE (1 << 6) //T2COM0 interrupt edge = equle CRC
#define T2COM0En (2 << 0) //T2COM0 compare funcion enable
#define T2COM1En (2 << 2) //T2COM1 compare funcion enable
#define T2COM2En (2 << 4) //T2COM2 compare funcion enable
#define T2COM3En (2 << 6) //T2COM3 compare funcion enable


#define RT_MODE_COMPARE() do { CCEN |= T2COM1En; } while(0)
#define RT_MODE_CAPTURE() do { CCEN &= ~T2COM1En; } while(0)

unsigned char xdata rtimer_cycle=0;
unsigned char xdata rtimer_cycle_cnt=0;
/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{	
	// T2_Initial
	//P11 = 1;//GPIO P1.0 input mode
	TH2 = 0x00;
	TL2 = 0x00;
	CRCH = 0x00;
	CRCL = 0x00;
	CCH1 = 0xC0;
	CCL1 = 0x00;
	// T2 clock from Fcpu/24 with T2 pin gating
	// Reload mode1 = T2RL falling edge trigger
	// Compare mode = directly method
	// T2COM0 interrupt trigger = equle CRC
	T2CON |= T2ClkFcpu | T2Fcpu24 | T2RLMode0 | ComMode1;
	// Compare function T2COM0/1/2/3 enable
	CCEN |= T2COM1En;
	
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
  /* Switch to capture mode before writing T1CC1x and
   * set the compare mode values so we can get an interrupt after t */
	CCL1 = (unsigned char)t;
  CCH1 = (unsigned char)(t >> 8);
  RT_MODE_COMPARE();

  /* Turn on compare mode interrupt */
  //TF2C1 = 0;
  ET2C1 = 1;
	
}
/*---------------------------------------------------------------------------*/
/* avoid referencing bits, we don't call code which use them */
#pragma save
//#if CC_CONF_OPTIMIZE_STACK_SIZE
//#pragma exclude bits
//#endif
void
rtimer_isr(void) interrupt ISRCom1
{
  ET2C1 = 0; /* Ignore Timer 1 Interrupts */
	++rtimer_cycle_cnt;
	if(rtimer_cycle_cnt==rtimer_cycle)
  {
		rtimer_run_next();
		rtimer_cycle_cnt=0;
	}
  ET2C1 = 1; /* Acknowledge Timer 1 Interrupts */
}
#pragma restore
