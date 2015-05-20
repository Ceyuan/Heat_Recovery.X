/******************************************************************************************/
// Hardware I2C single master routines for PIC16F877
// for HI-TEC PIC C COMPILER.
//
// i2c_init  - initialize I2C functions
// i2c_start - issue Start condition
// i2c_repStart- issue Repeated Start condition
// i2c_stop  - issue Stop condition
// i2c_read(x) - receive unsigned char - x=0, don't acknowledge - x=1, acknowledge
// i2c_write - write unsigned char - returns ACK
//
/******************************************************************************************/

void i2c_init()
{
    TRISC3=1;           // set SCL and SDA pins as inputs
    TRISC4=1;

    SSPCON = 0x38;      // set I2C master mode
    SSPCON2 = 0x00;

    file://SSPADD = 0x6;     // 400kHz bus with 10MHz xtal - use 0x0C with 20MHz xtal
    SSPADD = 10;            // 100k at 4Mhz clock

    STAT_CKE=1;     // use I2C levels      worked also with '0'
    STAT_SMP=1;     // disable slew rate control  worked also with '0'

    PSPIF=0;      // clear SSPIF interrupt flag
    BCLIF=0;      // clear bus collision flag
}

/******************************************************************************************/

void i2c_waitForIdle()
{
 while (( SSPCON2 & 0x1F ) | STAT_RW ) {}; // wait for idle and not writing
}

/******************************************************************************************/

void i2c_start()
{
 i2c_waitForIdle();
 SEN=1;
}

/******************************************************************************************/

void i2c_repStart()
{
 i2c_waitForIdle();
 RSEN=1;
}

/******************************************************************************************/

void i2c_stop()
{
 i2c_waitForIdle();
 PEN=1;
}

/******************************************************************************************/

int i2c_read( unsigned char ack )
{
 unsigned char i2cReadData;

 i2c_waitForIdle();

 RCEN=1;

 i2c_waitForIdle();

 i2cReadData = SSPBUF;

 i2c_waitForIdle();

 if ( ack )
  {
  ACKDT=0;
  }
 else
  {
  ACKDT=1;
  }
  ACKEN=1;               // send acknowledge sequence

 return( i2cReadData );
}

/******************************************************************************************/

unsigned char i2c_write( unsigned char i2cWriteData )
{
 i2c_waitForIdle();
 SSPBUF = i2cWriteData;

 return ( ! ACKSTAT  ); // function returns '1' if transmission is acknowledged
}

/******************************************************************************************/


