/**
 * @brief           Functions for Setting pin states and functions
 * @file            NB_PINS.c
 * @author          SIMX LTD NEW ZEALAND
 * @compiler        MPLAB XC8 compiler
 *
 **********************************************************************
 * Software License Agreement
 *
 * The software supplied herewith is owned by SIMX, and is
 * protected under applicable copyright laws. The software supplied herewith is
 * intended and supplied to you, the Company customer, for use solely and
 * exclusively on products manufactured by SIMX. The code may
 * be modified and can be used free of charge for commercial and non commercial
 * applications. All rights are reserved. Any use in violation of the foregoing
 * restrictions may subject the user to criminal sanctions under applicable laws,
 * as well as to civil liability for the breach of the terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN 'AS IS' CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE
 * COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 **********************************************************************
 * File History
 *
 * 2015-02-19, Shane Wu:
 *    - Initial version
 *********************************************************************/
#include "NB_PINS.h"

/*********************************************************************
 * Write to register associated with PIC pins
 * 1. Write to LAT to change to input/output    DIO
 * 2. Write to PORT to change output state      OUTPUT
 * 3. Write to WPU to enable/disable pull up    WPU
 * 4. Write to ANSEL to enable Analog           ANSEL
 * 5....
 * 6...
 * 7...
 * 
 * @param PinNum Pin number on the pic, see data sheet
 * @param Action what properties to change on the pin e.g. LAT, TRIS, etc
 * @param set value to write to the register bit
 *
 * return 0 = successful, 1 = failed to execute
 *********************************************************************/
unsigned int ConfigPin (int PinNum, int Action, int set){
    int temp = 1;
    switch (Action){
        case DIO    :
            temp    =   ConfigDIO(PinNum, set);
            break;
        case OUTPUT :
            temp    =   ConfigOutput(PinNum, set);
            break;
        case IOWPU  :
            temp    =   ConfigIOWPU(PinNum, set);
            break;
//        case ANSEL  :
//            break;
    }
    return  temp;
}

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable configure it
 * as digital IO
 * otherwise return an error message
 *
 * @param PinNum Pin number on the pic, see data sheet
 * @param set value to write to the TRIS register bit
 *
 * return 0 = successful, 1 = failed to execute
 *********************************************************************/
unsigned int ConfigDIO(int PinNum, int set){
    unsigned char temp=0b1;
    //Pin is RAx
    if (PinNum>=2&&PinNum<=10&&PinNum!=8){
        if (PinNum==10){
            PinNum  =   8;
        }
        temp    <<=   (PinNum-2);
        
        if (set == 1){  //set bit high
            TRISA |=    temp;
        }
        else{
            TRISA &=    (temp^=0xFF);
        }
    }

    //Pin is RBx
    else if (PinNum>=21&&PinNum<=28){
        temp    <<=   (PinNum-21);

        if (set == 1){  //set bit high
            TRISB |=    temp;
        }
        else{
            TRISB &=    (temp^=0xFF);
        }
    }

    //Pin is RCx
    else if (PinNum>=11&&PinNum<=18){
        temp    <<=   (PinNum-11);

        if (set == 1){  //set bit high
            TRISC |=    temp;
        }
        else{
            TRISC &=    (temp^=0xFF);
        }
    }

    //Invalid Pin Number
    else{
        return  1;
    }
    return  0;
}

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable change its state
 * otherwise return an error message
 *
 * @param PinNum Pin number on the pic, see data sheet
 * @param set value to write to the LAT register bit
 *
 * return 0 = successful, 1 = failed to execute
 *********************************************************************/
unsigned int ConfigOutput(int PinNum, int set){
    unsigned char temp=0b1;
    //Pin is RAx
    if (PinNum>=2&&PinNum<=10&&PinNum!=8){
        if (PinNum==10){
            PinNum  =   8;
        }
        temp    <<=   (PinNum-2);

        if (set == 1){  //set bit high
            LATA |=    temp;
        }
        else{
            LATA &=    (temp^=0xFF);
        }
    }

    //Pin is RBx
    else if (PinNum>=21&&PinNum<=28){
        temp    <<=   (PinNum-21);

        if (set == 1){  //set bit high
            LATB |=    temp;
        }
        else{
            LATB &=    (temp^=0xFF);
        }
    }

    //Pin is RCx
    else if (PinNum>=11&&PinNum<=18){
        temp    <<=   (PinNum-11);

        if (set == 1){  //set bit high
            LATC |=    temp;
        }
        else{
            LATC &=    (temp^=0xFF);
        }
    }

    //Invalid Pin Number
    else{
        return  1;
    }
    return  0;
}

/*********************************************************************
 * Function for configuring pin as digital IO
 * checks pin to be configured and if it is applicable configure it
 * otherwise return an error message
 *
 * @param PinNum Pin number on the pic, see data sheet
 * @param set value to write to the  WPU register bit
 *
 * return 0 = successful, 1 = failed to execute
 *********************************************************************/
unsigned int ConfigIOWPU(int PinNum, int set){
    unsigned char temp=0b1;
    //Pin is RAx
    if (PinNum>=2&&PinNum<=10&&PinNum!=8){
        if (PinNum==10){
            PinNum  =   8;
        }
        temp    <<=   (PinNum-2);

        if (set == 1){  //set bit high
            WPUA |=    temp;
        }
        else{
            WPUA &=    (temp^=0xFF);
        }
    }

    //Pin is RBx
    else if (PinNum>=21&&PinNum<=28){
        temp    <<=   (PinNum-21);

        if (set == 1){  //set bit high
            WPUB |=    temp;
        }
        else{
            WPUB &=    (temp^=0xFF);
        }
    }

    //Pin is RCx
    else if (PinNum>=11&&PinNum<=18){
        temp    <<=   (PinNum-11);

        if (set == 1){  //set bit high
            WPUC |=    temp;
        }
        else{
            WPUC &=    (temp^=0xFF);
        }
    }

    //Invalid Pin Number
    else{
        return  1;
    }
    return  0;
}
/*********************************************************************
 * Function for reading a port value
 * checks pin to be configured and if it is applicable configure it
 * otherwise return an error message
 *
 * @param PinNum Pin number on the pic, see data sheet
 *
 * return port value
 *********************************************************************/
unsigned int ReadCPin(unsigned int PinNum){
    unsigned char temp=0b1;
//    //Pin is RAx
//    if (PinNum>=2&&PinNum<=10&&PinNum!=8){
//        if (PinNum==10){
//            PinNum  =   8;
//        }
//        temp    <<=   (PinNum-2);
//        temp    &=  PORTA;
//    }
//
//    //Pin is RBx
//    else if (PinNum>=21&&PinNum<=28){
//        temp    <<=   (PinNum-21);
//        temp    &=  PORTB;
//    }
//
//    //Pin is RCx
//    else if (PinNum>=11&&PinNum<=18){
    if (PinNum>=11&&PinNum<=18){
        temp    <<=   (PinNum-11);
        temp    &=  PORTC;
        return  temp;
    }
    return  0;
//    return  temp;
}
unsigned int ReadPin(unsigned int PinNum){
    unsigned char temp=0b1;
    //Pin is RAx
    if (PinNum>=2&&PinNum<=10&&PinNum!=8){
        if (PinNum==10){
            PinNum  =   8;
        }
        temp    <<=   (PinNum-2);
        temp    &=  PORTA;
    }

    //Pin is RBx
    else if (PinNum>=21&&PinNum<=28){
        temp    <<=   (PinNum-21);
        temp    &=  PORTB;
    }

    //Pin is RCx
    else if (PinNum>=11&&PinNum<=18){
//    if (PinNum>=11&&PinNum<=18){
        temp    <<=   (PinNum-11);
        temp    &=  PORTC;
//        return  temp;
    }
//    return  0;
    return  temp;
}