#ifndef __RHT03_H
#define __RHT03_H

#include <stdint.h>
#include <xc.h>
#include "NB_Time.h"

#define NUMBER_OF_SENSORS 4
typedef enum {
	ERR_OK = 0,
	ERR_NO_RESPONSE,
	ERR_START_TIMEOUT_1,
	ERR_START_TIMEOUT_2,
	ERR_READ_BYTE_TIMEOUT,
	ERR_CHECKSUM
} RHT_ERR_t;

RHT_ERR_t RHT_start(char port);
RHT_ERR_t RHT_readByte(uint8_t* out, char port);
RHT_ERR_t RHT_readData(char port);
extern uint8_t RHT_data[5];

extern int16_t rh[NUMBER_OF_SENSORS];
extern int16_t temp[NUMBER_OF_SENSORS];

#endif
