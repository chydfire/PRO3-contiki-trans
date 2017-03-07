
#include "uart.h"
/*---------------------------------------------------------------------------*/

char
putchar(char c)
{
  uart_writeb((char) c);
  return c;
}