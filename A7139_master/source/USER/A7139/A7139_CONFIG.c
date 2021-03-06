#include "A7139_CONFIG.h"

#if CONFIG_DATA_RATE == 2

const uint16_t CODE A7139Config[]=
{
//	0x0021,             //[0x00] System Clock register
	0x0220,             //[0x00] System Clock register      Data Rate = 50KHZ  wxq test
	0x0A24,             //[0x01] PLL I register
	0xB805,             //[0x02] PLL II register
	0x0000,             //[0x03] PLL III register
	0x0A20,             //[0x04] PLL IV register
	0x0024,             //[0x05] PLL V register
	0x0000,             //[0x06] PLL VI register
	0x0011,             //[0x07] Crystal register
	0x0000,		     //PAGE A
	0x0000,		     //PAGE B
//	0x18D4,             //[0x0A] RX I register
		0x18D0,             //[0x0A] RX I register
	0x7009,             //[0x0B] RX II register
	0xC400,             //[0x0C] ADC register
	0x0800,             //[0x0D] Pin Control register
	0x4845,             //[0x0E] Calibration register
	0x20C0              //[0x0F] Mode Control register
};

const uint16_t CODE A7139Config_PageA[]= 
{
	0x1706,             //[0x08][Page0] TX I register
	0x0000,             //[0x08][Page1] WOR I register
	0x0000,             //[0x08][Page2] WOR II register
	0x1107,             //[0x08][Page3] RF Current register
	0x8170,             //[0x08][Page4] Power Manage register
	0x0201,             //[0x08][Page5] AGC RSSI Threshold register
	0x400F,             //[0x08][Page6] AGC Control register
	0x2AC0,             //[0x08][Page7] AGC Control II register
	0x0045,             //[0x08][Page8] GPIO register
	0xD281,             //[0x08][Page9] CKO register
	0x0004,             //[0x08][Page10] VCO Current register
	0x0825,             //[0x08][Page11] Channel Group I register
	0x0127,             //[0x08][Page12] Channel Group II register
	0x003F,             //[0x08][Page13] FIFO register
	0x155F,             //[0x08][Page14] CODE register
	0x0000              //[0x08][Page15] WCAL register
};

const uint16_t CODE A7139Config_PageB[]=
{
	0x037F,             //[0x09][Page0] TX II register
//	0x8400,             //[0x09][Page1] IF I register
		0x8200,             //[0x09][Page1] IF I register
	0x0000,             //[0x09][Page2] IF II register
	0x0000,             //[0x09][Page3] ACK register
	0x0000              //[0x09][Page4] ART register
};

#elif CONFIG_DATA_RATE == 10

const uint16_t CODE A7139Config[]=		//470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1221,		//SYSTEM CLOCK register,
	0x0A24,		//PLL1 register,
	0xB805,		//PLL2 register,	470.001MHz
	0x0000,		//PLL3 register,
	0x0B30,		//PLL4 register,
	0x0024,		//PLL5 register,
	0x0000,		//PLL6 register,
	0x0011,		//CRYSTAL register,
	0x0000,		//PAGEA,
	0x0000,		//PAGEB,
	0x18D4,		//RX1 register, 	IFBW=100KHz, ETH=1	
	0x7009,		//RX2 register, 	by preamble
	0x4400,		//ADC register,	   	
	0x0800,		//PIN CONTROL register,		Use Strobe CMD
	0x4845,		//CALIBRATION register,
	0x20C0		//MODE CONTROL register, 	Use FIFO mode
};

const uint16_t CODE A7139Config_PageA[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1706,		//TX1 register, 	Fdev = 37.5kHz

	0x0000,		//WOR1 register,
	0x0000,		//WOR2 register,
	0x1107,		//RFI register, 	Enable Tx Ramp up/down  
	0x8170,		//PM register,		CST=1
	0x0201,		//RTH register,
//	0x40CF,		//AGC1 register,    由于这2个寄存器的配置值导致A7139初始化不成功，修改后可以成功通讯。	
//	0x6FC0,		//AGC2 register, 
	0x400F,		//AGC1 register,	
	0x2FC0,		//AGC2 register

	0x0045,		//GIO register, 	GIO2=WTR, GIO1=FSYNC
	0xD281,		//CKO register
	0x0004,		//VCB register,
	0x0825,		//CHG1 register, 	480MHz
	0x0127,		//CHG2 register, 	500MHz
	0x003F,		//FIFO register, 	FEP=63+1=64bytes
	0x155F,		//CODE register, 	Preamble=4bytes, ID=4bytes
	0x0000		//WCAL register,
};

const uint16_t CODE A7139Config_PageB[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
//	0x037F,		//TX2 register,
	0x0301,		//TX2 register,   wxq test
	0x8400,		//IF1 register, 	Enable Auto-IF, IF=200KHz
	0x0000,		//IF2 register,
	0x0000,		//ACK register,
	0x0000		//ART register,
};

//wxq add 修改之后470模块初始化成功。以上代码针对433模块？
//const u16 CODE A7139Config[]=		//470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//    0x1221,		//SYSTEM CLOCK register,
//	0x0A21,		//PLL1 register,
//	0xDA05,		//PLL2 register,	433.301MHz
//	0x0000,		//PLL3 register,
//	0x0A20,		//PLL4 register,
//	0x0024,		//PLL5 register,
//	0x0000,		//PLL6 register,
//	0x0011,		//CRYSTAL register,
//	0x0000,		//PAGEA,
//	0x0000,		//PAGEB,
//	0x18D4,		//RX1 register, 	IFBW=100KHz	
//	0x7009,		//RX2 register, 	by preamble
//	0x4000,		//ADC register,	   	
//	0x0800,		//PIN CONTROL register,		Use Strobe CMD
//	0x4C45,		//CALIBRATION register,
//	0x20C0		//MODE CONTROL register, 	Use FIFO mode
//};

//const u16 CODE A7139Config_PageA[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//	0xF706,		//TX1 register, 	Fdev = 37.5kHz
//	0x0000,		//WOR1 register,
//	0xF800,		//WOR2 register,
//	0x1107,		//RFI register, 	Enable Tx Ramp up/down  
//	0x8170,		//PM register,		CST=1
//	0x0201,		//RTH register,
//	0x400F,		//AGC1 register,	
//	0x2AC0,		//AGC2 register, 
//	0x0045,		//GIO register, 	GIO2=WTR, GIO1=FSYNC
//	0xD181,		//CKO register
//	0x0004,		//VCB register,
//	0x0A21,		//CHG1 register, 	430MHz
//	0x0022,		//CHG2 register, 	435MHz
//	0x003F,		//FIFO register, 	FEP=63+1=64bytes
//	0x1507,		//CODE register, 	Preamble=4bytes, ID=4bytes
//	0x0000		//WCAL register,
//};

//const u16 CODE A7139Config_PageB[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//	0x0337,		//TX2 register,
//	0x8400,		//IF1 register, 	Enable Auto-IF, IF=200KHz
//	0x0000,		//IF2 register,
//	0x0000,		//ACK register,
//	0x0000		//ART register,
//};

#elif CONFIG_DATA_RATE == 50//DR=50,IFBW=50

const uint16_t CODE A7139Config[]=
{
	0x0220,             //[0x00] System Clock register
	0x0A24,             //[0x01] PLL I register
	0xB805,             //[0x02] PLL II register
	0x0000,             //[0x03] PLL III register
	0x0A20,             //[0x04] PLL IV register
	0x0024,             //[0x05] PLL V register
	0x0000,             //[0x06] PLL VI register
	0x0001,             //[0x07] Crystal register
	0x0000,		     //[0x08] PAGE A
	0x0000,	          //[0x07] PAGE B
	0x18D0,             //[0x0A] RX I register
	0x7009,             //[0x0B] RX II register
	0x4400,             //[0x0C] ADC register
	0x0800,             //[0x0D] Pin Control register
	0x4845,             //[0x0E] Calibration register
	0x20C0              //[0x0F] Mode Control register
};

const uint16_t CODE A7139Config_PageA[]= 
{
	0xF606,             //[0x08][Page0] TX I register
	0x0000,             //[0x08][Page1] WOR I register
	0xF800,             //[0x08][Page2] WOR II register
	0x1107,             //[0x08][Page3] RF Current register
	0x0170,             //[0x08][Page4] Power Manage register
	0x0201,             //[0x08][Page5] AGC RSSI Threshold register
	0x400F,             //[0x08][Page6] AGC Control register
	0x2AC0,             //[0x08][Page7] AGC Control II register
	0x0045,             //[0x08][Page8] GPIO register
	0xD281,             //[0x08][Page9] CKO register
	0x0004,             //[0x08][Page10] VCO Current register
	0x0825,             //[0x08][Page11] Channel Group I register
	0x0127,             //[0x08][Page12] Channel Group II register
	0x003F,             //[0x08][Page13] FIFO register
	0x155F,             //[0x08][Page14] CODE register
	0x0000              //[0x08][Page15] WCAL register
};

const uint16_t CODE A7139Config_PageB[]=
{
  0x037F,             //[0x09][Page0] TX II register
	0x8200,             //[0x09][Page1] IF I register
	0x0000,             //[0x09][Page2] IF II register
	0x0000,             //[0x09][Page3] ACK register
	0x0000              //[0x09][Page4] ART register
};

#elif CONFIG_DATA_RATE == 100//DR=100,IFBW=100

const uint16_t CODE A7139Config[]=
{
	0x0021,             //[0x00] System Clock register
	0x0A24,             //[0x01] PLL I register
	0xB805,             //[0x02] PLL II register
	0x0000,             //[0x03] PLL III register
	0x0A20,             //[0x04] PLL IV register
	0x0024,             //[0x05] PLL V register
	0x0000,             //[0x06] PLL VI register
	0x0001,             //[0x07] Crystal register
	0x0000,		     //PAGE A
	0x0000,		     //PAGE B
	0x18D4,             //[0x0A] RX I register
	0x7009,             //[0x0B] RX II register
	0x4400,             //[0x0C] ADC register
	0x0800,             //[0x0D] Pin Control register
	0x4845,             //[0x0E] Calibration register
	0x20C0              //[0x0F] Mode Control register
};

const uint16_t CODE A7139Config_PageA[]= 
{
	0xF706,             //[0x08][Page0] TX I register
	0x0000,             //[0x08][Page1] WOR I register
	0x0000,             //[0x08][Page2] WOR II register
	0x1107,             //[0x08][Page3] RF Current register
	0x0170,             //[0x08][Page4] Power Manage register
	0x0201,             //[0x08][Page5] AGC RSSI Threshold register
	0x400F,             //[0x08][Page6] AGC Control register
	0x2AC0,             //[0x08][Page7] AGC Control II register
	0x0045,             //[0x08][Page8] GPIO register
	0xD281,             //[0x08][Page9] CKO register
	0x0004,             //[0x08][Page10] VCO Current register
	0x0825,             //[0x08][Page11] Channel Group I register
	0x0127,             //[0x08][Page12] Channel Group II register
	0x003F,             //[0x08][Page13] FIFO register
	0x155F,             //[0x08][Page14] CODE register
	0x0000              //[0x08][Page15] WCAL register
};

const uint16_t CODE A7139Config_PageB[]=
{
	0x037F,             //[0x09][Page0] TX II register
	0x8400,             //[0x09][Page1] IF I register
	0x0000,             //[0x09][Page2] IF II register
	0x0000,             //[0x09][Page3] ACK register
	0x0000              //[0x09][Page4] ART register
};

#elif CONFIG_DATA_RATE == 150//DR=150,IFBW=150

const uint16_t CODE A7139Config[]=
{
	0x007B,             //[0x00] System Clock register
	0x0A24,             //[0x01] PLL I register
	0xB805,             //[0x02] PLL II register
	0x0000,             //[0x03] PLL III register
	0x0A20,             //[0x04] PLL IV register
	0x0024,             //[0x05] PLL V register
	0x0000,             //[0x06] PLL VI register
	0x0003,             //[0x07] Crystal register
	0x0000,
	0x0000,
	0x18D8,             //[0x0A] RX I register
	0x7009,             //[0x0B] RX II register
	0x4400,             //[0x0C] ADC register
	0x0800,             //[0x0D] Pin Control register
	0x4845,             //[0x0E] Calibration register
	0x20C0              //[0x0F] Mode Control register
};

const uint16_t CODE A7139Config_PageA[]= 
{
	0xF709,             //[0x08][Page0] TX I register
	0x0000,             //[0x08][Page1] WOR I register
	0x0000,             //[0x08][Page2] WOR II register
	0x1107,             //[0x08][Page3] RF Current register
	0x0170,             //[0x08][Page4] Power Manage register
	0x0201,             //[0x08][Page5] AGC RSSI Threshold register
	0x400F,             //[0x08][Page6] AGC Control register
	0x2AC0,             //[0x08][Page7] AGC Control II register
	0x4045,             //[0x08][Page8] GPIO register
	0xD281,             //[0x08][Page9] CKO register
	0x0004,             //[0x08][Page10] VCO Current register
	0x0825,             //[0x08][Page11] Channel Group I register
	0x0127,             //[0x08][Page12] Channel Group II register
	0x003F,             //[0x08][Page13] FIFO register
	0x155F,             //[0x08][Page14] CODE register
	0x0000,             //[0x08][Page15] WCAL register
};

const uint16_t CODE A7139Config_PageB[]=
{
	0x037F,             //[0x09][Page0] TX II register
	0x8600,             //[0x09][Page1] IF I register
	0x0000,             //[0x09][Page2] IF II register
	0x0000,             //[0x09][Page3] ACK register
	0x0000              //[0x09][Page4] ART register
};

#endif
