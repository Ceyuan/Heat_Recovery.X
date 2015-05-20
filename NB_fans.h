/***************************************
 * NinjaBean Fan Speed Control Functions
 * File:    NB_Time.h
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 17/03/2014
 *
 * Process and output appropriate values for Triac, PWM and relay control
 */
#include <xc.h>
#ifndef NB_FANS_H
#define	NB_FANS_H
#define TRIAC_MAX_COUNT 320
#define TRIAC_ZERO_DELAY 50
#define PWM1    LATAbits.LATA6
#define PWM2    LATBbits.LATB5
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* NB_FANS_H */

int Triac_Counter(int speed, int maxCount, int zeroDelay);

int PWM_Counter(int speed, int invert);