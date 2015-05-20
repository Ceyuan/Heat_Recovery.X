#include "rht03.h"

#define SET_BIT(a, mask) a |= mask
#define CLEAR_BIT(a, mask) a &= ~(mask)

#define DATA1_MASK (1<<0)
#define DATA2_MASK (1<<1)
#define DATA3_MASK (1<<2)
#define DATA4_MASK (1<<3)
#define DATA5_MASK (1<<4)
#define DATA6_MASK (1<<5)
#define DATA7_MASK (1<<6)

//#define DATA PORTCbits.RC0
//#define DATA_DIR TRISCbits.TRISC0
//
//#define DATA1 PORTCbits.RC1
//#define DATA1_DIR TRISCbits.TRISC1

#define LED2 LATAbits.LATA7
uint8_t RHT_data[5];

int16_t rh[NUMBER_OF_SENSORS];
int16_t temp[NUMBER_OF_SENSORS];

RHT_ERR_t RHT_start(char port) {

	int timeout;
	int mask;

	// Host send start signal by pulling line low (1ms minimum)
	switch (port) {
		case 0: mask = DATA1_MASK; break;
		case 1: mask = DATA2_MASK; break;
		case 2: mask = DATA3_MASK; break;
		case 3: mask = DATA4_MASK; break;
		case 4: mask = DATA5_MASK; break;
		case 5: mask = DATA6_MASK; break;
		case 6: mask = DATA7_MASK; break;
		default: break;
	}

	CLEAR_BIT(TRISC, mask);
	CLEAR_BIT(PORTC, mask);
	__delay_ms(5);

	// Host releases line (pulls up) and wait 20-40us for RHT response
	SET_BIT(TRISC, mask);
	SET_BIT(PORTC, mask);
	__delay_us(40);

	// RHT pulls low for 80us
	if ((PORTC & mask) != mask) {

		timeout = 128;	// approx 80us
		while((PORTC & mask) != mask && timeout-- > 0) ;
		if (timeout < 0) {
			return ERR_START_TIMEOUT_1;
		}

		timeout = 128;	// approx 80us
		while((PORTC & mask) == mask && timeout-- > 0) ;
		if (timeout < 0) {
			return ERR_START_TIMEOUT_2;
		}

		return ERR_OK;

	} else {
		return ERR_NO_RESPONSE;
	}

}

RHT_ERR_t RHT_readByte(uint8_t* out, char port)
{
	char i;
	int timeout;
	char tempBit;
	char tempByte = 0;

	int mask;

	// Host send start signal by pulling line low (1ms minimum)
	switch (port) {
		case 0: mask = DATA1_MASK; break;
		case 1: mask = DATA2_MASK; break;
		case 2: mask = DATA3_MASK; break;
		case 3: mask = DATA4_MASK; break;
		case 4: mask = DATA5_MASK; break;
		case 5: mask = DATA6_MASK; break;
		case 6: mask = DATA7_MASK; break;
		default: break;
	}

	for(i = 0; i < 8; i++)
	{
		timeout = 256;
		while(((PORTC & mask) != mask) && timeout-- > 0);   //check for 1

		__delay_us(27);
		if((PORTC & mask) == mask)
		{
			tempBit = 1;
			LED2 = 1;
		} else{
		tempBit = 0;
                LED2    =   0;
                }

		timeout = 256;
		while(((PORTC & mask) == mask) && timeout-- > 0);   //check for 0
		if(timeout < 0)
		{
			LED2 = 0;
			return ERR_READ_BYTE_TIMEOUT;
		}

		// Accumulate bits in byte
		tempByte <<= 1;
		tempByte |= tempBit;
	}

	LED2 = 0;
	*out = tempByte;
	return ERR_OK;
}

RHT_ERR_t RHT_readData(char port) {

	int i;
	uint8_t checksum = 0;
	RHT_ERR_t error = 0;

	// Send start signal
	error = RHT_start(port);

	if (error == 0) {

		// Get data
		for (i = 0; i < 5; i++) {
			error = RHT_readByte(&RHT_data[i], port);
			if (error) {
//				return error;
                            return i+1;
			}
		}

		// Calculate checksum on received data
		for (i = 0; i < 4; i++) {
			checksum += RHT_data[i];
		}

		// Check calculated checksum against received checksum
		if (checksum != RHT_data[4]) {
			return ERR_CHECKSUM;
		}

	}

	rh[port] = RHT_data[0] << 8;
	rh[port] |= RHT_data[1];

	temp[port] = RHT_data[2] << 8;
	temp[port] |= RHT_data[3];

	return error;
}
