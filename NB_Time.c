/***************************************
 * NinjaBean Time Functions
 * File:    NB_Time.c
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 20/11/2014
 */

#include "NB_Time.h"

int timer24overflow=0;
/***************************************
 * Set Clock Speed to be 8MHz
 * */
void SetClockSpeed(){

    OSCCONbits.SCS=0;
    OSCCONbits.IRCF= 0b1111;    //16M Hz
    OSCCONbits.SPLLEN = 1;      //Multiplier on = 32M Hz
    return;
}

/**
 * Function for timing critical things
 * max timer time = 5 minutes
 *
 * @param timer4 blocks of time(125us) to add to current timer
 * @return what the counter will reach before time out
 */
NB_TIMER_ Tick24BitTimer(NB_TIMER_ timer4){
    NB_TIMER_ temp=0;
    temp    =   timer4 + tick_us1;
    if((temp&0x7FFFFF)>TIMER1_MAX_TICK){
        temp    -=  TIMER1_MAX_TICK;
        temp    ^=  0x800000;
    }
//    if (temp>=1200000){
//        timer24overflow =   1;
//        timer4  =   temp-1200000;
//    }
//    else{
//        timer24overflow =   0;
//    }
    return temp;
}

/**
 * Function for timing critical things
 * 
 * @param timeout timer tick required for timeout
 * @return 0 if timer has not ended, 1 when timer is complete
 */
unsigned int CheckTick24Timer(NB_TIMER_ timeout){
//
//    if ((timeout>(TIMER1_MAX_TICK-TIMER1_BUFFER))&&(tick_us1<timeout/3)){
//        if(timeout<TIMER1_MAX_TICK){
//            timeout =   0;
//        }
//        else{
//            timeout -=  TIMER1_MAX_TICK;
//        }
//    }
    if(((timeout&0x7FFFFF)<(tick_us1&0x7FFFFF))&&((timeout&0x800000)==(tick_us1&0x800000))){
        return 1;
    }
    return 0;
}
