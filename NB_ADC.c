/***************************************
 * NinjaBean ADC Function
 * File:    NB_Time.c
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 15/12/2014
 */

#include "NB_ADC.h"

//Set up ADC enabling bits
void init_adc(void)
{
    
    ADCON2bits.CHSN     = 0b1111;//set negative input to ADNREF
    ADCON1bits.ADPREF   = 0b00; // set v+ reference to Vdd
    ADCON1bits.ADNREF   = 0b0;  // set v- reference to Vss
    ADCON0bits.ADRMD    = 0b0;  // Output 12-bit
    ADCON1bits.ADFM     = 0b1;  // 2's complement output: bit 0-11 is value, 12-15 is sign(0=positive)
//    ADCON1bits.ADCS     = 0b010;// use Fosc/32 for clock source
//    ADCON1bits.ADCS     = 0b001;// use Fosc/8
    ADCON1bits.ADCS     = 0b110;// use Fosc/64
    ADCON0bits.ADON     = 0b1;  // turn on the ADC
}

unsigned short adc_convert(unsigned short pChannel, char notSigned)
{
    unsigned char temp;
    ADCON0bits.CHS      = pChannel;  // select the given positive channel
    ADRESH = 0b00000000;
    ADRESL = 0b00000000;
    ADCON0bits.GO       = 0b1;      // start the conversion
    while(ADCON0bits.GO_nDONE);         // wait for the conversion to finish
//    LATAbits.LATA1 ^= 1;
//    PIR1bits.ADIF       = 0b0;      //clear interrupt flag
    if (notSigned){
        temp = ADRESH & 0b00001111;   //get rid of the sign
    }
    return (temp<<8)|ADRESL;      // return the result
}