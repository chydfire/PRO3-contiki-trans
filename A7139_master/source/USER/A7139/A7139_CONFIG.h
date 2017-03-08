/*********************************************/
#ifndef __A7139_CONGFIG_H__
#define __A7139_CONGFIG_H__
#include <SN8F5708.h>
#include <intrins.h> //for _nop_

/*********************************************/
//wxq add
//typedef   u32    Uint32;
//typedef   u16    Uint16;
//typedef   u8     Uint8;
#ifndef Uint8
#define Uint8 unsigned char
#endif

#ifndef Uint16
#define Uint16 unsigned short
#endif

#ifndef Uint32
#define Uint32 unsigned long
#endif

/*********************************************/
//#define CONFIG_DATA_RATE 10
#define CONFIG_DATA_RATE 100  
#define PLATEFORM_C51    0
/*********************************************/
#if  PLATEFORM_C51 == 1
	#define DATA  data
	#define IDATA idata
	#define CODE  code
	#define XDATA xdata
#else
	#define DATA
	#define IDATA
	#define CODE
	#define XDATA
#endif
/********************************************/
extern const Uint16 CODE A7139Config[];
extern const Uint16 CODE A7139Config_PageA[];
extern const Uint16 CODE A7139Config_PageB[];
#endif
/*********************************************/
