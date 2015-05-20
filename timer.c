#include "Timer.h"
#include <p24Fxxxx.h>

volatile uint32_t systemTick = 0;
	
void initTimer(void) {
	
	// Setup timer 1 to generate an interrupt at 10ms intervals
	// used for timeouts and delays
	
	T1CONbits.TON = 1;		// Starts 16-bit Timer1
	T1CONbits.TSIDL = 1;	// Discontinue module operation when device enters Idle mode
	T1CONbits.TCKPS = 0b11;	// 1:256 	T1CONbits.TCS = 0;		// Internal clock (FOSC/2)
	
	PR1 = 625; // (10 milliseconds) / ((1 / (16 MHz)) * 256)
	
	// Enable interrupt
	IEC0bits.T1IE = 1;
	
}

uint32_t sysTick(void) {
	
	uint32_t retVal;
	
	// Disable interrupt for read
	IEC0bits.T1IE = 0;
	retVal = systemTick;
	IEC0bits.T1IE = 1;
	
	return retVal;
	
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
	
	// Increment global counter
	systemTick++;
	
	// Clear the flag
	IFS0bits.T1IF = 0;
}
