/**
 * @brief           Functions for RHT05 Temperature and Humidity Sensor.
 * @file            RHT05_Sensor.c
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
#define THIS_IS_RHT05_SENSOR_C
#include "RHT05_Sensor.h"
typedef struct RHT_SENSOR_
{
    unsigned char   byte[5];
    NB_TIMER_    SensorBusyTimer;
    NB_TIMER_    PowerOffTimer;
    unsigned int    SensorError;
    union {
        struct{
            unsigned char PowerInvert   : 4;
            unsigned char PowerReset    : 1;
            unsigned char NoPowerSwitch : 1;
            unsigned char SensorPower   : 1;
            unsigned char SensorBusy    : 1;
        }flags;
        unsigned char flagsValue;
    };
}RHT_SENSOR_;

RHT_SENSOR_ sens;

/*********************************************************************
 * Set Power Reset pin and type
 * When the sensors are not reading properly or is locked up disconnect
 * the power to the sensor for 2 seconds and then reconnect.
 * Do not read sensor when power is off
 *
 * @param PwrSwitchPin address of pin to be set up as the switch
 * e.g. TRISAbits.TRISA0
 * Input 0 to turn off
 *
 * @param PwrSwitchInv  0 = set low to switch off,
 *                      1 = set high to switch off
 *********************************************************************/
void RHT05Init(unsigned int PwrSwitchPin, unsigned int PwrSwitchInv){
    //Set up Power pin if it is not 0
    sens.flags.SensorPower     =   1;
    if (PwrSwitchPin != 0){
        sens.flags.NoPowerSwitch    =   ConfigPin(PwrSwitchPin, DIO, 0);
        sens.flags.SensorPower      =   !ConfigPin(PwrSwitchPin, OUTPUT, 1);
        sens.flags.PowerInvert      =   PwrSwitchInv;
    }
    else{
        sens.flags.NoPowerSwitch        =   1;
    }
    //expire after 2s
    sens.SensorBusyTimer  =   Tick24BitTimer(POWER_OFF_TIMER);
}


/*********************************************************************
 * Configure pins for sensor reading, set as Digital input and pull up
 * resistor enabled
 *
 * @param SensorPins address of pin to be connected to sensor signal
 * wire
 *********************************************************************/
void Add_Sensor_port(unsigned int SensorPin){
    int temp    =   ConfigPin(SensorPin, DIO, 1);
    temp    =   ConfigPin(SensorPin, IOWPU, 1);

    //TODO: Set up timer 'SensTimer' to expire at least 35us afterwards
    //for now put 40us delay
}

/*********************************************************************
 * Check Status of sensor timer
 * When the sensor timer has finished and power is on set as not busy
 * When the timer is still counting down exit the function
 * When the sensors are not being powered:
 *      Power timer still ticking - exit the function
 *      Power timer finished - turn on sensor and set up power on time of 2s
 ********************************************************************/
void CheckSensorStatus(unsigned int PwrSwitchPin){
    if (sens.flags.PowerReset){
        //reset power to sensors and start power off timer
        if (!sens.flags.NoPowerSwitch){
            ConfigPin(PwrSwitchPin, OUTPUT, sens.flags.PowerInvert);
        }
        sens.flags.PowerReset   =   0;
        sens.flags.SensorPower  =   0;
        sens.PowerOffTimer  =   Tick24BitTimer(POWER_OFF_TIMER);
    }
    if (CheckTick24Timer(sens.PowerOffTimer)){
        if ((sens.flags.SensorPower==0)&&(!sens.flags.NoPowerSwitch)){
            RHT05Init(PwrSwitchPin, sens.flags.PowerInvert);    //turn on power to sensors
            return;
        }
        if (CheckTick24Timer(sens.SensorBusyTimer)){
            sens.flags.SensorBusy   =   0;
            return;
        }
        else{
            sens.flags.SensorBusy   =   1;
            return;
        }
    }
    return;
}

/*********************************************************************
 * Function for checking if the sensor port is high or low
 * @param SensorPin Pin number on the PIC to check
 * @param state state to check for (High or low)
 * @return  1: timed out, Pin did not reach desired state
 *          0: Successful
 *********************************************************************/
int WaitForSensor (unsigned int SensorPin, unsigned int times, unsigned int state){
//    while (ReadPin(SensorPin)!= state ){
//        
//    }
    for(unsigned int i=0;i<times;i++){
        __delay_us(1);
//        if (ReadCPin(SensorPin)==state){
        if(PORTCbits.RC0==state){
            return 0;
        }
    }
    return 1;
}

/*********************************************************************
 * Read sensor Function. Make sure port is high.
 * Set to output and set port low. 2ms delay and write high to port.
 * Set port as input and sample 41 reads. 1st bit is high and is discarded.
 * wait for low then high. Wait 35us and check bit, 0 = low, 1 = high.
 * Repeat above.
 *
 * Check data validity. Checksum(byte 4) is the lower byte of the sum
 * of RH and T bytes
 *
 * Power off sensors when the sensors are not reading properly, allow
 * 5 errors before reset. Do not read sensors until 2 seconds after
 * reset.
 *
 * return 0 process completed, 1 incomplete
 *********************************************************************/
unsigned int RHT05Read(unsigned int SensorPin){
    if(sens.flags.SensorBusy||(!sens.flags.SensorPower)){
        return 1;
    }
    
//TESTING////////////////////
LATAbits.LATA3^=1;
//TESTING////////////////////

    ConfigPin(SensorPin, DIO, 1);       //set pin as input
    if(ReadPin(SensorPin)){
        ConfigPin(SensorPin, DIO, 0);   //set pin as output
        ConfigPin(SensorPin, OUTPUT, 0);    //set pin low
        __delay_ms(2);
        ConfigPin(SensorPin, OUTPUT, 1);    //set pin high
        Add_Sensor_port(SensorPin);
        if(WaitForSensor(SensorPin, 100, 0)){//wait for Pin to go low (100us max)
            goto SENSOR_ERR;
        }
        if(WaitForSensor(SensorPin, 100, 1)){//wait for Pin to go high (100us max)
            goto SENSOR_ERR;
        }
        if(WaitForSensor(SensorPin, 100, 0)){//wait for Pin to go low (100us max)
            goto SENSOR_ERR;
        }
        if(WaitForSensor(SensorPin, 100, 1)){//wait for Pin to go high (100us max)
            goto SENSOR_ERR;
        }
        /*********************************
         * Repeat the following 40 times
         * first time discard result
         ********************************/
        //wait for Pin to go low (100us max)
        //wait for Pin to go high (100us max)
        //wait for 30us and check if pin is high or low
        //High = 1, low = 0
        for(unsigned int i=0;i<8;i++){
            if(Read_RHT05_Byte(i, SensorPin)){
                goto SENSOR_ERR;
            }
        }
    }
    else{ goto SENSOR_ERR;}
    //check sum
    unsigned int checksum    =   0;
    for (unsigned int i=4;i>0;i--){
        checksum    +=  (sens.byte[i]&0xFF);
    }
    if(sens.byte[4]!=(checksum&0xFF)){

        LATAbits.LATA5  ^=  1;
        goto SENSOR_ERR;
    }
    sens.SensorError    =   0;
    return 0;
    SENSOR_ERR:

        LATAbits.LATA4  ^=  1;
    sens.SensorError++;
    if (sens.SensorError>5){
        sens.flags.PowerReset   =   1;
        sens.SensorError        =   0;
    }
    return 1;
}
/*********************************************************************
 * Decode and output Sensor Reading Function
 * return RH(integer) RH(decimal)
 *********************************************************************/
int RHT05_RH(){
    //CHANGED
//    if (sens.SensorError){
//        return 1;
//    }
    int RH = 0;
    RH  =   (sens.byte[4]&0xFF)<<8;
    RH  |=  (sens.byte[3]&0xFF);

//    RH  =   sens.byte[3];
    return RH;
}
/*********************************************************************
 * Decode and output Sensor Reading Function
 * return T(integer) T(decimal)
 *********************************************************************/
int RHT05_T(){
    //CHANGED
//    if (sens.SensorError){
//        return 0;
//    }
    int temp = 0;
    temp  =   (sens.byte[2]&0xFF)<<8;
    temp  |=  (sens.byte[1]&0xFF);
    return temp;
}

// Load 1 byte of data at a time
int Read_RHT05_Byte(unsigned int byte_No, unsigned int SensorPin){
    sens.byte[byte_No]  &   0x00;
    for (int i=0; i<8;i++){
        if(WaitForSensor(SensorPin, 100, 0)){
            return 1;
        }
        if(WaitForSensor(SensorPin, 100, 1)){
            return 1;
        }

//        while(PORTCbits.RC0&&timeout-- > 0);
//        while((!PORTCbits.RC0)&&timeout-- > 0);
        __delay_us(30);
        sens.byte[byte_No]  <<= 1;
//                sens.byte[(i-1)/8]  |=  PORTCbits.RC0;
        sens.byte[byte_No]  |=  PORTCbits.RC0;

            //CHANGED TO SEE how many times code excuted before failing
//                sens.byte[3]    =   i;
        
    }
    return 0;
}