#ifndef __CMD_H__
#define __CMD_H__

#include "stdint.h"

#define CMD_BUF_LEN_MAX    ( 32 )
#define CMD_NAME_LEN_MAX   ( 8 )
#define CMD_GET_LINE_DELAY ( 10 )

#define CMD_FLAG_CNTL_Q    ( 0x11 )
#define CMD_FLAG_CNTL_S    ( 0x13 )
#define CMD_FLAG_DEL       ( 0x7F )
#define CMD_FLAG_BACKSPACE ( 0x08 )
#define CMD_FLAG_CR        ( 0x0D )
#define CMD_FLAG_LF        ( 0x0A )
#define CMD_FLAG_ESC       ( 0x1B )
#define CMD_FLAG_ZERO		   ( 0x00 )

/* Command definitions structure. */
typedef struct cmd_type 
{
   char name[ CMD_NAME_LEN_MAX ];
   void ( *func )( char *exec );
}CMD_TYPE;

//return TRUE or FALSE
extern int getline(char * lp, uint32_t n);
extern int strtoul(char *s);
extern char *getentry(char *pSrc, char **ppNext);
extern void cmdline(void);

/*---------------------------------------------------------------------------*/

#endif //__CMD_H__