/***************************************
 * Putpose: Stand-alone Heat Exchange unit controls
 * PIC16F1783
 * Author: Shane Wu
 * Company: SIMX LTD NZ
 * Created: 15/12/2014
*/
//#define SENSOR_READ_INTERVAL   160000//5 seconds
#define SENSOR_READ_INTERVAL   64000//2 seconds
#define ADC_INTERVAL    64000//2 seconds
#define LED3 LATAbits.LATA3
#define LED4 LATAbits.LATA4

#include <stdio.h>
#include <stdlib.h>
#include "NB_Time.h"
#include "NB_PINS.h"
#include "delay.h"
#include "rht03.h"
#include "NB_ADC.h"
#include "NB_fans.h"
//#include "RHT05_Sensor.h"
#include "HD44780.h"
// PIC16F1783 Configuration Bit Settings
#include <xc.h>
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 32000000
#endif
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC =       INTOSC      // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE =       OFF         // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE =      OFF         // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE =      ON          // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP =         OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD =        OFF         // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN =      OFF         // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN =   OFF         // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO =       OFF         // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN =      ON          // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT =        OFF         // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN =     OFF         // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN =      OFF          // PLL Enable (4x PLL disabled)
#pragma config STVREN =     ON          // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV =       LO          // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR =      OFF         // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP =        ON          // Low-Voltage Programming Enable (Low-voltage programming enabled)
/*
 * 
 */

unsigned long tick_us1 =   0;
volatile uint32_t sensorReadTimer;
volatile uint32_t ledFlashTimer;

char rhString[16];
char tempString[16];
int fanspeed    =   0;
int TriacDelay  =   0;
int PWMDC  =   19;  //duty cycle
int triacCounter    =   0;
int PWMCounter  =   0;

int ADC_intF    =   1;  //ADC interrupt flag
unsigned long potDivider   =   0;

/******************************************************************************
 * TODO: Configure all EEPROM Data on program
 *****************************************************************************/

int main(void) {
    NB_TIMER_ sensorDelay   =   Tick24BitTimer(SENSOR_READ_INTERVAL);
    NB_TIMER_ ADCDelay   =   Tick24BitTimer(ADC_INTERVAL);
    unsigned int i   =   0;
    char sensorPort = 0;
//    int temperature[NUMBER_OF_SENSORS];
//    int rHumidity[NUMBER_OF_SENSORS];
    RHT_ERR_t error;
    //Pin Setup
    TRISBbits.TRISB = 0b11100001;         // 1=input 0=output
    TRISAbits.TRISA = 0b00100000;
    ANSELBbits.ANSELB   =   0b00000000;
    ANSELAbits.ANSELA   =   0b00000000;
//    TRISBbits.TRISB0    = 0b1;  // set pin 21 as input
//    ANSELBbits.ANSB0    = 0b1;  // set pin 21 as analog
//    TRISBbits.TRISB1    = 0b1;  // set pin 22 as input
//    ANSELBbits.ANSB1    = 0b1;  // set pin 22 as analog

//    TRISBbits.TRISB2    = 0b1;  // set pin 23 as input
    ANSELAbits.ANSA5    = 0b1;  // set pin 7 as analog
//    TRISBbits.TRISB3    = 0b1;  // set pin 24 as input
//    ANSELBbits.ANSB3    = 0b1;  // set pin 24 as analog
//    TRISBbits.TRISB4    = 0b1;  // set pin 25 as input
//    ANSELBbits.ANSB4    = 0b1;  // set pin 25 as analog

//    TRISA = 255;            //set all pins as input
//    TRISAbits.TRISA0 = 0; // set pin2 as output (LED1)
//    TRISAbits.TRISA1 = 0; // set pin3 as output (LED2)
//    TRISAbits.TRISA3 = 0; // set pin3 as output (LED4)
//    TRISAbits.TRISA4 = 0; // set pin6 as output (LED5)
//    TRISAbits.TRISA5 = 0; // set pin7 as output (LED6)

    //Timer0 and Interrupt setup
    OPTION_REGbits.TMR0CS = 0;               // Timer increments on instruction clock
    OPTION_REGbits.TMR0SE = 0;              //Timer increments on low-to-high transition of T0CKI pin
    OPTION_REGbits.INTEDG = 1;              //Interrupt on rising edge 
    INTCONbits.TMR0IE = 1;               // Enable interrupt on TMR0 overflow
    OPTION_REGbits.PSA  =1;             //Disable Prescaler

    init_adc();

    SetClockSpeed();        //8Mhz internal clock
    __delay_ms(50);
    LCD_Initialize();

    INTCONbits.INTE = 1;               // enable the external interrupt
    INTCONbits. GIE = 1;                // Global interrupt enable

    int speed   =   0;
    unsigned int error_counter  =   0;
    unsigned char sensor_er_no = 50;
    //Main Loop
    while(1){
        /**********************************************************************
         * Read Sensors Every 2 seconds
         *********************************************************************/
        speed   =   (potDivider*100/4095);
         TriacDelay    =   Triac_Counter(speed, TRIAC_MAX_COUNT, TRIAC_ZERO_DELAY);
         PWMDC =   PWM_Counter(speed, 0);
//         PWMDC  =   19;

         if(CheckTick24Timer(sensorDelay)){
//            //Toggle led for debugging
//            CheckSensorStatus(18);
//            for(i=0;i<NUMBER_OF_SENSORS;i++){
//                if(RHT05Read(RHT05Pin[i])){
//                    break;
//                }
//                else{
//                    temperature[i]  =   RHT05_T();
//                    rHumidity[i]    =   RHT05_RH();
//                    break;//REMOVE AFTER SENSOR CODE IS WORKING
//                }
//            }
//            __delay_ms(100);
//            sensorDelay   =   Tick24BitTimer(SENSOR_READ_INTERVAL);
//            //CHANGED
//            LCD_Clear();
//            i++;
//            if (i>1000){
//                i=0;
//            }
//            LCD_WriteNumber(RHT05_T());
//            LCD_GoTo(0x40);
//            LCD_WriteNumber(RHT05_RH());
////            LCDDelay =   Tick24BitTimer(DISPLAY_INTERVAL);
//        }
            for(sensorPort=0;sensorPort<NUMBER_OF_SENSORS;sensorPort++){
            error = 0;
			error = RHT_readData(sensorPort);
//                        sensorPort++;
//                        if(sensorPort>=2){
//                            sensorPort = 1;
//                        }
			if (error == ERR_OK) {
				// OK, get new reading in 2 seconds
				sensorDelay   =   Tick24BitTimer(SENSOR_READ_INTERVAL);

				// Clear error LEDs
//				LED3 = 0;
//				LED4 = 0;

				PORTC = 0;
                                
                                LCD_Clear();
//                                sprintf(rhString, "%d %d\n", temp[0], temp[1] );
                                sprintf(rhString, "%d %d\n", speed, sensor_er_no );
				LCD_WriteString(rhString);

				if (temp[0] < 0) {
					temp[0] *= -1;
					sprintf(tempString, "T:  -%d.%d\r\n", temp[0] / 10, temp[0] % 10);
				} else {
//					sprintf(tempString, "%d %d", temp[2],rh[3]);

					sprintf(tempString, "%d %d", temp[0], error_counter);
				}
				LCD_WriteString(tempString);

			} else if (error == ERR_CHECKSUM) {
				// Checksum error, retry in 2 second
				LCD_Clear();
				sensorDelay   =   Tick24BitTimer(SENSOR_READ_INTERVAL);
                                sensor_er_no    =   sensorPort*3;
                                error_counter++;
                                break;
			} else {
				// General error, retry in 250ms
//				LCD_Clear();
				sensorDelay   =   Tick24BitTimer(SENSOR_READ_INTERVAL);
                                sensor_er_no    =   error;
                                error_counter++;
                                break;
			}
                        __delay_ms(100);
            }
            // Print RH&T on LCD
//             sensor_er_no   =   20;
        }

        /**********************************************************************
         * Logic For Evolve Upgrade / Synergy
         * Choose operating mode with DIP Switches
         * 8 Modes 4way DIP Switch
         * 1 AN and 1 Digital Pin
         * Digital Pin  =   Synergy or Positive
         * AN 000        =   no H/T no Summer
         * AN 001        =   Summer, no H/T
         * AN 010        =   H/T, no Summer
         * AN 100       =   Summer and H/T
         *********************************************************************/

        /**********************************************************************
         * Positive Pressure Ventilation Rate Formula
         * (Desired - Indoor)*(Outside - Indoor)*0.4+65-abs(Desired - Outside)*4
         *********************************************************************/

        /**********************************************************************
         * LCD Display Used For Debugging
         *********************************************************************/
        //CHANGED
//        if(CheckTick24Timer(sensorDelay)){
//            LCD_Clear();
//            i++;
//            if (i>1000){
//                i=0;
//            }
//            LCD_WriteNumber(RHT05_T());
//            LCD_GoTo(0x40);
//            LCD_WriteNumber(RHT05_RH());
//            LCDDelay =   Tick24BitTimer(DISPLAY_INTERVAL);
//        }
        /**********************************************************************
         * ADC used for testing triac and PWM - to be removed
         * Check every second
         *********************************************************************/
        if(CheckTick24Timer(ADCDelay)){
            ADC_intF    =   1;
            ADCDelay   =   Tick24BitTimer(ADC_INTERVAL);
        }
    }

    return 0;
}

void interrupt   tc_int  (void)        // interrupt function

{

        if(INTCONbits.T0IF && INTCONbits.T0IE){// if timer flag & enabled is set
            TMR0 -= 250;               // reload the timer - 31.25us per interrupt
            INTCONbits.T0IF = 0;                  // clear the interrupt flag
            tick_us1++;
            if ((tick_us1&0x7FFFFF)>=TIMER1_MAX_TICK){
                tick_us1    -=  TIMER1_MAX_TICK;
                tick_us1    ^=  0x00800000; //toggle ms bit
            }
            if(triacCounter<=0){
                LED3    =   1;
            }
            else{
                triacCounter--;
            }
            if(PWMCounter>=PWMDC){
                PWM1    =   0;
            }
            else{
                PWM1    =   1;
            }
            if(PWMCounter++>18){
                PWMCounter  =   0;
            }
        }

        if(INTCONbits.INTF && INTCONbits.INTE){// if INT flag & enable is set
            INTCONbits.INTF =   0;              //clear interrupt flag
            LED3    =  0;
            triacCounter    =   TriacDelay;
        }

        if(ADC_intF){   //ADC - Remove Later
            ADC_intF    =   0;
            potDivider =   adc_convert(AN_RA5, 1);
        }
}