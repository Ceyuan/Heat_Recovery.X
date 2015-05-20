/*
 *	Delay functions
 *	See delay.h for details
 *
 *	Make sure this code is compiled with full optimization!!!
 */

#include	"delay.h"


void
DelayMs(unsigned char cnt)
{
#if	_XTAL_FREQ <= 2MHZ
	do {
		DelayUs(996);
	} while(--cnt);
#endif

#if    _XTAL_FREQ > 2MHZ
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
		} while(--i);
	} while(--cnt);
#endif
}

void
DelaySec(char sec)
{
   char loop;
   for(loop=0;loop<=sec*4;loop++)
	   {
     DelayMs(250);
      }
}
