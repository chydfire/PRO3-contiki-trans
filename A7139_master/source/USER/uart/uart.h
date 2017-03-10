

#ifndef _UART_H
#define _UART_H
#include <SN8F5708.h>
#include <stdio.h>

#define BR_9600    1
#define BR_19200   2
#define BR_38400   3
#define BR_57600   4
#define BR_115200  5
#define Baudrate   BR_115200

#define UART_BUF_LEN_MAX    ( 32 )  //receive buffer 32 byte



void uart_init(void);
void uart_send_byte1(uint8_t txData);
void uart_send_string1(uint8_t *s,unsigned char n);
void uart_writeb(char byte);
char putchar(char c);
uint8_t uart_getchar (void);
//unsigned int Get_CRC_Check_Code(unsigned char *s,unsigned int n);

#endif
