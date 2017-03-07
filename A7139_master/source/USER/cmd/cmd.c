#include "cmd.h"
#include "uart.h"
#include "clock.h"
#include <ctype.h>
#include <string.h>

/* Display prompt string */
code char *PROMPT =
   "\n#> "
   ;
/* Copyright string */
code char *COPYRIGHT =
 "\n"
 "SARI51 Copyright 2012-22"
   ;
/* Local constants */
code char *intro=
 "\n"
// " SARI51 Command Line Interface"
 "SARI51 CLI"
   ;
code char *help = 
 "\n"
     "+==== Command ===+= Function =============+\n"
//   "|INFO            | Display system info    |\n"
//   "|HELP or ?       | Display this help      |\n"
//   "|----------------+------------------------|\n"
//   "|LED [n] [op]    | Turn on/off led        |\n"
     "|TRAC            | Tracing Adjust Radio   |\n"
//   "|XRAM [addr] <va>| Read/Write XRAM        |\n"
//   "|REG  [reg] <va> | Read/Write SPI Register|\n"
     "|FREQ <va>       | Get/Set Frequency      |\n"
     "|COLC <va>       | Get/Set COLC           |\n"
     "|DIFF <va>       | Get/Set DesiredOffset  |\n"
     "|RATE <va>       | Get/Set RF Baudrate    |\n"
     "|TXPW <va>       | Get/Set RF TX Power    |\n"
//   "|SEND            | Wireless Send Frame    |\n"
//   "|RECV            | Wireless Receive Frame |\n"
     "|LOAD            | Read Flash Settings    |\n"
     "|SAVE            | Save to Flash          |\n"
     "+----------------+------------------------+"
   ;

static void cmd_help (char *par) 
{
	par=par;
	printf(help);
	return;
}

code CMD_TYPE cmd[] = {
//	"RECV", cmd_recv,
//	"XRAM", cmd_xram,
//	"REG",	cmd_reg,
	//"FREQ", cmd_freq,
	//"COLC", cmd_colc,
	//"DIFF", cmd_diff,
	////"RATE", cmd_rate,
	//"TXPW", cmd_txpower,
//	"SEND", cmd_send,
	//"LED",  cmd_led,
	//"TRAC", cmd_trace,
	//"LOAD", cmd_load,
	//"SAVE", cmd_save,
	//"INFO", cmd_info,
	"HELP", cmd_help,
	//"?",    cmd_help
  };
	
#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))
	
char in_line[CMD_BUF_LEN_MAX] = { 0 };

int getline(char * lp, uint32_t n)
{
    uint32_t cnt = 0;
    char c;
	
    do 
		{
   	    //clock_delay(CMD_GET_LINE_DELAY);
        c = getchar();
        switch(c) 
				{
						 case CMD_FLAG_ZERO:
						 case CMD_FLAG_CNTL_Q:   /* ignore Control S/Q             */
						 case CMD_FLAG_CNTL_S:
						 break;
						
						 case CMD_FLAG_BACKSPACE:
						 case CMD_FLAG_DEL:
								if (cnt == 0) {
									 break;
								}
								cnt--;                         /* decrement count                */
								lp--;                          /* and line pointer               */
								putchar (0x08);                /* echo backspace                 */
								putchar (' ');
								putchar (0x08);
								break;
						 case CMD_FLAG_ESC:
								*lp = 0;                       /* ESC - stop editing line        */
								return (FALSE);
						 case CMD_FLAG_CR:                          /* CR - done, stop editing line   */
								*lp = c;
								lp++;                          /* increment line pointer         */
								cnt++;                         /* and count                      */
								c = CMD_FLAG_LF;
						 default:
							  *lp = c;
						    //putchar (c); 
								putchar (*lp);             /* echo and store character       */
						    
								lp++;                          /* increment line pointer         */
								cnt++;                         /* and count                      */
								break;
      }
   } while (cnt < n - 2  &&  c != CMD_FLAG_LF);     /* check limit and CR             */
   *lp = 0;    
                            /* mark end of string             */
   return (TRUE);
}

int strtoul(char *s)
{
	int ret;
	int radix = 10;
	int negative = 0;
	int i;
	ret = 0;
	if(*s == '-') 
	{
		negative = 1;
		s++;
	}
	else if(*s == '0')
	{
		s++;
		if(*s == 'x')
		{
			s++;
			radix = 0x10;
		}
	}
	while (*s) {
		if (*s >= '0' && *s <= '9')
			i = *s - '0';
		else if (*s >= 'a' && *s <= 'f')
			i = *s - 'a' + 0xa;
		else if (*s >= 'A' && *s <= 'F')
			i = *s - 'A' + 0xa;
		else
			break;
		if(i >= radix) break;
		ret = (ret * radix) + i;
		s++;
	}
	return negative?(-ret):ret;
}

char *getentry(char *cp, char **ppNext)
{
	char *sp, lfn = 0, sep_ch = ' ';
   if (cp == NULL) {                          /* skip NULL pointers          */
      *ppNext = cp;
      return (cp);
   }
   for ( ; *cp == ' ' || *cp == '\"'; cp++) { /* skip blanks and starting  " */
      if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
      *cp = 0;
   }
   for (sp = cp; *sp != CMD_FLAG_CR && *sp != CMD_FLAG_LF; sp++) {
      if ( lfn && *sp == '\"') break;
      if (!lfn && *sp == ' ' ) break;
   }
   for ( ; *sp == sep_ch || *sp == CMD_FLAG_CR || *sp == CMD_FLAG_LF; sp++) {
      *sp = 0;
      if ( lfn && *sp == sep_ch) { sp ++; break; }
   }
   *ppNext = (*sp) ? sp : NULL;                /* next entry                  */
   return (cp);
}

void cmdline(void)
{
	 char *sp=NULL,*cp=NULL,*next=NULL;
   int8_t i;
	 unsigned char u = 0;
	char c;
   //printf (intro);                            /* display example info        */
   //printf (help);
   //cmd[0].func(NULL);	//?????recv??
   while (TRUE) {
      //printf (PROMPT);                     /* display prompt              */
      /* get command line input      */
		 #if 0
		  do
			{
				c = getchar();
				if( 0 != c )
				{
					putchar( c );
				}
			}while( 0 != c );
			#endif
			#if 1
      if (getline (in_line, CMD_BUF_LEN_MAX) == FALSE) {
				printf("get line failed\n");
         continue;
      }
//			for(u = 0; u < 32; u++){
//			putchar(in_line[u]);
//			}
//			putchar("\n");
			printf("getline %s\n", in_line);
			#endif
			#if 1
      sp = getentry (&in_line[0], &next);
      if (*sp == 0) {
         continue;
      }
      for (cp = sp; *cp && (*cp != ' '); cp++) {
         *cp = toupper (*cp);                 /* command to upper-case       */
      }
      for (i = 0; i < CMD_COUNT; i++) {
         if (strcmp (sp, (const char *)&cmd[i].name)) {
            continue;
         }
         cmd[i].func (next);                  /* execute command function    */
         break;
      }
      if (i == CMD_COUNT) {
        printf ("\ncmd error\n");
      }
			#endif
   }
}