 /*===========================================================================
 Written in HI-TEC PIC C COMPILER                                       8.6.01

 by

 Michael Alon - MICROCHIP FAE
 My E-mail michael@elina-micro.co.il
 www.elina-micro.co.il
 Tel: 972-3-6498543/4
 Fax: 972-3-6498745
 Elina Micro LTD .ISRAEL

 this program is using the hardware i2c on the pic 16f877 to write

 to 24LC01B configured for a x8 org  I used the  PICDEM 2 demo board from MICROCHIP


 ===========================================================================

 the project contains 2 files:

 877_i2c.h this is the header file all functions are in it include it in your project

 877_i2c.c this is a demo program to illustrate using this header


  to use the files :

  init_i2c();               Call before the other functions are used

  write_ext_eeprom(a, d);   Write the byte "d"  to the address" a"

  d = read_ext_eeprom(a);   Read the byte "d" from the address" a"

 ===========================================================================

 the demo program is writing 10  numbers (0 to 10) to 10 first pleases in the eeprom memory

 and showing it on leds on PORTB on PICDEM 2,

 after it lights all the leds once to indicate moving to read state,

 and then it lights on the leds of the address it is going to read, waiting a while

 and sends to leds the data it read from that address an so on,  10 times for  the 10 read addresses

 (attention!! THE ADDRESS AND THE DATA IT READ should be the SAME if not check the pin connection or

 the 820 ohm pull up resistors).

 I used 24LC01B that was on the board:

 pin 1 to 4 connected to VSS
 pin 5 "SDA" connected to  RC4 PIN 23 on PIC (do not forget to connect to this point a 820 ohm pull up resistor to VCC)
 pin 6 "SCL" connected to  RC3 PIN 18 on PIC (do not forget to connect to this point a 820 ohm pull up resistor to VCC)
 pin 7 "WP"  connected to  VSS
 pin 8 connected to VCC

 you can watch the eeprom connection drawings in PICDEM 2 MANUEL on microchip site WWW.MICROCHIP.COM

 the PICDEM 2 is using 2 pull up resistors of 820 ohm on the board.


*/

/******************************************************************************************/

#include <pic.h>
#include "Delay.h"
#include "Delay.C"
#include  "i2cSlave.h"

/******************************************************************************************/

void write_ext_eeprom(unsigned char address, unsigned char data)
 {
   i2c_start();
   i2c_write(0xa0);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
   DelayMs(11);
}

/******************************************************************************************/

unsigned char read_ext_eeprom(unsigned char address)
{
   unsigned char data;

   i2c_start();
   i2c_write(0xa0);
   i2c_write(address);
   i2c_repStart();
   i2c_write(0xa1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}

/******************************************************************************************/

void main()
{
 unsigned char i=0;

 PORTB=0x00;    // port
 PORTC=0x00;    // port

 TRISB=0x00;    //portb out
 TRISC=0x00;    //portc out

 i2c_init();    // init i2c

 while(i<=10)   /* write 10 numbers to eeprom */
  {
  PORTB=i;                  // the value of i on leds i is the curent address and also the data
  write_ext_eeprom(i,i);   // it to eeprom
  i++;
  DelayMs(255);
  }


 PORTB=0xff;         // some leds
 DelaySec(1);
 PORTB=0x00;
 DelaySec(1);

 i=0;

 while(i<=10)
  {
  PORTB=i;                 // the address will read from the EEprom on the leds
  DelayMs(255);
  PORTB=0x00;      // leds off
  DelayMs(255);
  PORTB=read_ext_eeprom(i);   // the memory and show it on leds,it shoaled be the same like the address
  DelayMs(255);
  i++;
  }
}
/******************************************************************************************/

