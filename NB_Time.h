/* 
 * File:    NB_Time.h
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 20/11/2014
 */

#include <xc.h>
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 32000000
#endif
#define TIMER1_MAX_TICK 4800000 //2.5 minutes
#define TIMER1_BUFFER   (TIMER1_MAX_TICK/10)
extern unsigned long tick_us1;

typedef unsigned long NB_TIMER_;

/**
 * Function for timing critical things
 * max timer time = 5 minutes
 *
 * @param timer4 blocks of time(250us) to add to current timer
 * @return what the counter will reach before time out
 */
NB_TIMER_ Tick24BitTimer(NB_TIMER_ timer4);

/**
 * Function for timing critical things
 *
 * @param timeout timer tick required for timeout
 * @return 0 if timer has not ended, 1 when timer is complete
 */
unsigned int CheckTick24Timer(NB_TIMER_ timeout);

void SetClockSpeed();

