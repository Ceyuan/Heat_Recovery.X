/*
 * File:    NB_ADC.h
 * Author:  Shane Wu
 * Company: SIMX LTD NZ
 * Created: 15/12/2014
 */

#include <xc.h>

void init_adc(void);
unsigned short adc_convert(unsigned short pChannel, char not_signed);