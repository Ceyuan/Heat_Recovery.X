/* 
 * File:   RHT05_Sensor.h
 * Author: Ceyuan
 *
 * Created on 19 February 2015, 11:42 AM
 */

#ifndef RHT05_SENSOR_H
#define	RHT05_SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* RHT05_SENSOR_H */

#include <xc.h>
#include "NB_PINS.h"
#include "NB_Time.h"

#define POWER_OFF_TIMER 64000   //2SECONDS

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
 * @param PwrSwitchInv 0 = set low to switch off,
 * 1 = set high to switch off
 *********************************************************************/
void RHT05Init(unsigned int PwrSwitchPin, unsigned int PwrSwitchInv);

/*********************************************************************
 * Configure pins for sensor reading, set as Digital input and pull up
 * resistor enabled
 *
 * @param SensorPins address of pin to be connected to sensor signal
 * wire
 *********************************************************************/
void Add_Sensor_port(unsigned int SensorPin);

/*********************************************************************
 * Check Status of sensor timer
 * When the sensor timer has finished and power is on set as not busy
 * When the timer is still counting down exit the function
 * When the sensors are not being powered:
 *      Power timer still ticking - exit the function
 *      Power timer finished - turn on sensor and set up power on time of 2s
 ********************************************************************/
void CheckSensorStatus(unsigned int PwrSwitchPin);

/*********************************************************************
 * Function for checking if the sensor port is high or low
 * @param SensorPin Pin number on the PIC to check
 * @param state state to check for (High or low)
 * @return  1: timed out, Pin did not reach desired state
 *          0: Successful
 *********************************************************************/
int WaitForSensor (unsigned int SensorPin, unsigned int times, unsigned int state);

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
unsigned int RHT05Read(unsigned int SensorPin);

/*********************************************************************
 * Decode and output Sensor Reading Function
 * reutrn RH*10
 *********************************************************************/
int RHT05_RH();

/*********************************************************************
 * Decode and output Sensor Reading Function
 * return T*10
 *********************************************************************/
int RHT05_T();

int Read_RHT05_Byte(unsigned int byte_No, unsigned int SensorPin);