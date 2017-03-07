
#include "A7139_spi.h"


//A7139引脚配置：
//SCS->P2.0, SCK->P1.7, GPIO2->P2.2/INT1, GPIO1->P2.1/INT0, SDIO->P1.6, CKO->P1.5(GPIO2就是IRQ引脚)	
 
 //a7139中断管脚配置:GPIO2->P2.2/INT1
void init_irq_a7139()
{
	P2M  &= 0xFB;		//P2.2 input mode
//	P2UR |= 0x04;   //enable P2.2 pull-up resister
	P2UR &= 0xFB;   //disable P2.2 pull-up resister

	PEDGE = 0x08;   //INT1 trigger is falling edge
  IEN0 |= 0x84;   //enable INT1;enable all interrupt 
}
//SCS->P2.0, SCK->P1.7, GPIO2->P2.2/INT1, GPIO1->P2.1/INT0, SDIO->P1.6, CKO->P1.5
void init_spi_a7139(void)
{

	//GPIO2->P2.2/INT1  pull-up input
	P2M  &= 0xFB;		    //P2.2 input mode
	P2UR |= 0x04;       //enable P2.2 pull-up resister
	//GPIO1->P2.1/INT0  pull-up input
	P2M  &= 0xFD;		    //P2.1 input mode
	P2UR |= 0x02;       //enable P2.1 pull-up resister
	
	//SCS->P2.0, SCK->P1.7 push-pull output
	P2M  |= 0x01;		    //P2.0 output mode
	P2UR &= 0xFE;       //disable P2.0 pull-up resister

	
	P1M  |= 0x80;		    //P1.7 output mode
	P1UR &= 0x7F;       //disable P1.7 pull-up resister
	P1OC &= 0x7E;       //disable open drain
	
	//SDIO->P1.6  push-pull by default(two-way)
	P1M  |= 0x40;		    //P1.6 output mode
	P1UR &= 0xBF;       //disable P1.6 pull-up resister

	
	//CKO->P1.5 floating input
	P1M  &= 0xDF;		    //P1.5 input mode
	P1UR &= 0xDF;       //disable P1.6 pull-up resister
	
	init_irq_a7139();
	
	cs_high_a7139();
}

//a7139 chip select
void cs_low_a7139(void)
{
	P20 = 0;
}
void cs_high_a7139(void)
{
	P20 = 1;
}


//SCS->P2.0, SCK->P1.7, GPIO2->P2.2/INT1, GPIO1->P2.1/INT0, SDIO->P1.6, CKO->P1.5(GPIO2就是IRQ引脚)	
void SIO_Mode(u8 mod)
{
	if(mod == INPUT)
	{
		P1M  &= 0xBF;	      //SIO pull-up input or floating input
		P1UR |= 0x40;       //enable P1.6 pull-up resister
//		P1UR &= 0xBF; //disable P1.6 pull-up resister
	}
	else
	{
    //SIO push-pull output
		P1M  |= 0x40;		    //P1.6 output mode
		P1UR &= 0xBF;       //disable P1.6 pull-up resister
		P1OC &= 0xBE;       //disable open drain
	}
}

void SPIx_WriteByte(u8 dat)
{
    u8 i;
    for(i=0; i<8; i++)
    {
        if(dat & 0x80)
            SIO_OUT(HIGH);
        else
            SIO_OUT(LOW);
        delay_us(1);
        SCK_OUT(HIGH);
        delay_us(1);
        SCK_OUT(LOW);
        dat<<=1;
    }
}

void SPIx_WriteWord(u16 wrd)
{
    u8 i;
    for(i=0; i<16; i++)
    {
        if(wrd & 0x8000)
            SIO_OUT(HIGH);
        else
            SIO_OUT(LOW);
        SCK_OUT(HIGH);
        delay_us(1);
        SCK_OUT(LOW);
        wrd<<=1;
    }
}

u8 SPIx_ReadByte(void)
{
	u8 i,tmp=0;
	SIO_Mode(INPUT);
	for(i=0; i<8; i++)
	{
        if(SIO_IN())
            tmp = (tmp << 1) | 0x01;
        else
            tmp = tmp << 1;
        SCK_OUT(HIGH);
        delay_us(1);
        SCK_OUT(LOW);
     }
	SIO_Mode(OUTPUT);
	return tmp;
}

u16 SPIx_ReadWord(void)
{
	u8  i;
	u16 tmp=0;
	SIO_Mode(INPUT);	
	for(i=0; i<16; i++)
	{
        if(SIO_IN())
            tmp = (tmp << 1) | 0x01;
        else
            tmp = tmp << 1;
        SCK_OUT(HIGH);
        delay_us(1);
        SCK_OUT(LOW);
    }
	SIO_Mode(OUTPUT);    
    return tmp;
}
//delay us*n
void delay_us(unsigned char n)
{
   unsigned int i;
    // init value
    i = 0;
    for (i=0; i<n; i++)
	  {
			_nop_();    _nop_();
    }
}
//delay 1ms*n
void delay_ms(unsigned int n)
{
    unsigned int i, j;

    // init value
    i = 0;
    j = 0;

    for (i=0; i<n; i++) {
        for (j=0; j<220; j++) {
            _nop_();    _nop_();
            _nop_();    _nop_();
            _nop_();    _nop_();
            _nop_();    _nop_();
        }
    }
}








