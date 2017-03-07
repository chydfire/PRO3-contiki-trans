

#ifndef _A7139_SPI_H
#define _A7139_SPI_H

#include <SN8F5708.h>
#include <intrins.h> //for _nop_

#define LOW           0
#define HIGH          (!LOW)
//#define FALSE         0
//#define TRUE		  (!FALSE)	
#define OUTPUT		  0
#define INPUT		  (!OUTPUT)

////a7139 function------------------------------
//#define SIO_OUT(x)	((x)>0?((GPIOC->BSRR)|=(1<<11)) :(GPIOC->BRR|=(1<<11)))  //write SIO pin value
//#define SIO_IN()	(((GPIOC->IDR)>>11)&0x0001) 				  	   //read  SIO pin value

//#define SCS_OUT(x)  ((x)>0?((GPIOD->BSRR)|=(1<<2)):((GPIOD->BRR)|=(1<<2))) //write SCS pin value
//#define SCK_OUT(x)	((x)>0?((GPIOC->BSRR)|=(1<<12)):((GPIOC->BRR)|=(1<<12))) //write SCK pin value

//#define CKO_IN()	(((GPIOC->IDR)>>4)&0x0001) //read CKO  pin value
//#define GIO1_IN()	(((GPIOC->IDR)>>10)&0x0001)   //read GIO1(PC10) pin value
//#define GIO2_IN()	(((GPIOE->IDR)>>3)&0x0001)   //read GIO2(PE3) pin value

//a7139 function------------------------------
#define SIO_OUT(x)	((x)>0?(P16=1) :(P16=0))  //write SIO pin value
#define SIO_IN()	(P16 & 1) 				  	   //read  SIO pin value

#define SCS_OUT(x)  ((x)>0?(P20=1):(P20=0)) //write SCS pin value
#define SCK_OUT(x)	((x)>0?(P17=1):(P17=0)) //write SCK pin value

#define CKO_IN()	(P15 & 1) //read CKO  pin value
#define GIO1_IN()	(P21 & 1)   //read GIO1(PC10) pin value
#define GIO2_IN()	(P22 & 1)   //read GIO2(PE3) pin value

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

#ifndef u32
#define u32 unsigned long
#endif


#ifndef Uint8
#define Uint8 unsigned char
#endif

#ifndef Uint16
#define Uint16 unsigned int
#endif

#ifndef Uint32
#define Uint32 unsigned long
#endif

void init_irq_a7139();
void init_spi_a7139(void);
void cs_low_a7139(void);
void cs_high_a7139(void);

void SIO_Mode(u8 mod);
void SPIx_WriteByte(u8 dat);
void SPIx_WriteWord(u16 wrd);
u8 SPIx_ReadByte(void);
u16 SPIx_ReadWord(void);

void delay_us(unsigned char n);
void delay_ms(unsigned int n);

#endif












