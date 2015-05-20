/***************************************
 * NinjaBean Pin layout and numbers
 * File:    NB_Time.c
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 20/11/2014
 */


#include <xc.h>

/* ADCON pin and numbers*/
#define AN_RA0      0
#define AN_RA1      1
#define AN_RA2      2
#define AN_RA3      3
#define AN_RA5      4
#define AN_RB2      8
#define AN_RB3      9
#define AN_RB1      10
#define AN_RB4      11
#define AN_RB0      12
#define AN_RB5      13

/* Pin Function define used for ConfigPin()*/
#define DIO         1
#define OUTPUT      2
#define IOWPU       3
//#define ANSEL       4

/*********************************************************************
 * Write to register associated with PIC pins
 * 1. Write to LAT to change to input/output    DIO
 * 2. Write to PORT to change output state      OUTPUT
 * 3. Write to WPU to enable/disable pull up    WPU
 * 4. Write to ANSEL to enable Analog           ANSEL
 * 5....
 * 6...
 * 7...
 *********************************************************************/
unsigned int ConfigPin (int PinNum, int Action, int set);

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable configure it
 * as digital IO
 * otherwise return an error message
 *********************************************************************/
unsigned int ConfigDIO(int PinNum, int set);

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable change its state
 * otherwise return an error message
 *********************************************************************/
unsigned int ConfigOutput(int PinNum, int set);

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable configure it
 * otherwise return an error message
 *********************************************************************/
unsigned int ConfigIOWPU(int PinNum, int set);

/*********************************************************************
 * Function for reading a port value
 * checks pin to be configured and if it is applicable configure it
 * otherwise return an error message
 *
 * @param PinNum Pin number on the pic, see data sheet
 *
 * return port value
 *********************************************************************/
unsigned int ReadCPin(unsigned int PinNum);
unsigned int ReadPin(unsigned int PinNum);