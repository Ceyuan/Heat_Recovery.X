/***************************************
 * NinjaBean Fan Speed Control Functions
 * File:    NB_Time.c
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 17/03/2014
 * 
 * Process and output appropriate values for Triac, PWM and relay control
 */

#include "NB_fans.h"

/**
 * Converts percentage fan speed into counter format for use with interrupt
 * timer. Assume AC source is sinusoidal and 50Hz.
 * 
 * @param speed percentage of max fan speed (out of 100)
 * @param maxWidth  number of counts for 10ms (half a cycle)
 * @param zeroDelay number of counts between zero detection and actual zero crossing
 * @return counter for triac firing delay
 */
int Triac_Counter(int speed, int maxCount, int zeroDelay){
    if((speed<60)&&(speed!=0)){
        speed   =   60;
    }
    return ((100-speed)*maxCount/100 + zeroDelay);
}
/**
 * 10 speed PWM signal generated depending on fan speed
 * @param speed percentage of max fan speed (out of 100)
 * @param invert 0: 0V = off, 10V = max speed
 *               1: 0V = max speed, 10V = off
 * @return counter for pwm duty cycle (0->10)
 */
int PWM_Counter(int speed, int invert){
    if((speed<30)&&(speed>5)){
        speed   =   30;
    }
    if(invert){
        return ((100-speed)/5);
    }
    return (speed/5);
}