#include "cmd.h"
#include "uart.h"
#include "clock.h"
#include "A7139_comm.h"
#include <ctype.h>
#include <string.h>

xdata uint8_t rf_tx_valid_flag = 0;	

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

static void cmd_tx (char *par) 
{
	rf_tx_valid_flag = 1;
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
	"TX", cmd_tx,
  };
	
#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))

//xdata char in_line[CMD_BUF_LEN_MAX] = { 0 };
xdata uint8_t in_line[CMD_BUF_LEN_MAX] = { 0 };

extern uint8_t xdata a7139_tx[RF_RECV_BUF_LEN_MAX];

int getline(uint8_t * lp, uint8_t n)
{
    uint8_t cnt = 0;
    uint8_t c;
	  uint8_t pos = 0;
	  uint8_t head1 = 0;
	
	  if( avalible() > 0 )
		{
			do 
			{
					//clock_delay(CMD_GET_LINE_DELAY);
					c = uart_getchar();
					switch(c) 
					{
						   case CMD_FLAG_ZERO:
								   if( 1 == head1 )
									 {
										 return (TRUE);
									 }
							 break;
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
							 case CMD_FLAG_PKT_HEAD_1:
								  head1 = 1;
							    *lp = c;
							    a7139_tx[pos] = c;
							    pos++;
									lp++;                          /* increment line pointer         */
									cnt++;
								  break;
							 case CMD_FLAG_PKT_HEAD_2:
								  if( 1 == head1 )
									{
										 uint8_t i = 0;
										
										 *lp = c;
										 a7139_tx[pos] = c;
										 pos++;
										 lp++;                          /* increment line pointer         */
										 cnt++;
										
										 for(i = 0; i < 5; i++)  // RF_RECV_BUF_LEN_MAX - 2
										 {
											  *lp = uart_getchar();
											  a7139_tx[pos] = *lp;
										    pos++;
											  lp++;                          /* increment line pointer         */
										    cnt++; 
										 }
										 
										 cmd_tx(NULL);
										 
										 return (TRUE);
									}
									
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
	 }
		
   *lp = 0;    
                            /* mark end of string             */
   return (TRUE);
}


#if 0
int getline(char * lp, uint32_t n)
{
    uint32_t cnt = 0;
    char c;

	  uint8_t head1 = 0;
	
	  if( avalible() > 0 )
		{
			do 
			{
					//clock_delay(CMD_GET_LINE_DELAY);
					c = getchar();
				  putchar(c);
				  putchar(" ");
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
							 case CMD_FLAG_PKT_HEAD_1:
								  head1 = 1;
							    *lp = c;
									lp++;                          /* increment line pointer         */
									cnt++;
								  break;
							 case CMD_FLAG_PKT_HEAD_2:
								  if( 1 == head1 )
									{
										 uint8_t i = 0;
										
										 *lp = c;
										 lp++;                          /* increment line pointer         */
										 cnt++;
										
										 for(i = 0; i < 5; i++)  // RF_RECV_BUF_LEN_MAX - 2
										 {
											  *lp = getchar();
											  lp++;                          /* increment line pointer         */
										    cnt++; 
										 }
										 
										 cmd_tx(NULL);
										 
										 return (TRUE);
									}
									
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
	 }
		
   *lp = 0;    
                            /* mark end of string             */
   return (TRUE);
}

#endif

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

int cmd_master(void)
{
	 int ret = TRUE;
	 char *sp=NULL,*cp=NULL,*next=NULL;
   int8_t i;
	 unsigned char u = 0;
   //printf (intro);                            /* display example info        */
   //printf (help);
   //cmd[0].func(NULL);	//?????recv??
      //printf (PROMPT);                     /* display prompt              */
      /* get command line input      */
	  ret = getline (in_line, CMD_BUF_LEN_MAX);
		if( FALSE == ret ) 
		{
			printf("get line failed\n");
		}
		else
		{
			//printf("getline %s\n", in_line);
			sp = getentry (&in_line[0], &next);
			if (*sp != 0) 
			{
				 for (cp = sp; *cp && (*cp != ' '); cp++) {
				 *cp = toupper (*cp);                 /* command to upper-case       */
				}
				 
				for (i = 0; i < CMD_COUNT; i++) 
				{
					 if (0 == strcmp (sp, (const char *)&cmd[i].name)) 
					 {
							cmd[i].func (next);                  /* execute command function    */
					    break;
					 }
					 
				}
				
				if (i == CMD_COUNT) {
					ret = FALSE;
					printf ("\ncmd error\n");
				}
			}
		}
		
		return ret;
}