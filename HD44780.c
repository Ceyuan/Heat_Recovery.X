/******************************************************************************
*                          Basic display functions                            *
******************************************************************************/

#include "HD44780.h"

void LCD_Clock(){
	LCD_EN=1;
	__delay_us(LCD_CK_DEL);
	LCD_EN=0;
}

void LCD_Write(unsigned char c){
	LCD_DB7 = (c >> 7) & 0x01;
	LCD_DB6 = (c >> 6) & 0x01;
	LCD_DB5 = (c >> 5) & 0x01;
	LCD_DB4 = (c >> 4) & 0x01;
	#if LCD_DL == 0   //
		LCD_Clock();  // 4-bit only
	#endif            //
	LCD_DB3 = (c >> 3) & 0x01;
	LCD_DB2 = (c >> 2) & 0x01;
	LCD_DB1 = (c >> 1) & 0x01;
	LCD_DB0 = (c >> 0) & 0x01;
	LCD_Clock();
	__delay_ms(1);
}

void LCD_Initialize(){
	LCD_RS=0;
	LCD_RW=0;
	LCD_EN=0;
	
	__delay_ms(20);
	LCD_DB7 = 0;
	LCD_DB6 = 0;
	LCD_DB5 = 1;
	LCD_DB4 = 1;
	LCD_Clock();
	
	__delay_ms(5);
	LCD_DB7 = 0;
	LCD_DB6 = 0;
	LCD_DB5 = 1;
	LCD_DB4 = 1;
	LCD_Clock();
        
        __delay_us(150);
	LCD_DB7 = 0;
	LCD_DB6 = 0;
	LCD_DB5 = 1;
	LCD_DB4 = 1;
	LCD_Clock();
	
	LCD_DB7 = 0;
	LCD_DB6 = 0;
	LCD_DB5 = 1;
	LCD_DB4 = 0;
	LCD_Clock();

	// FUNCTION SET				| 0  0  1  DL N  F  —  — |
	// BIN_OR_BIT_MASK:			{ 0  0  1  0  0  0  0  0 }
	// HEX_OR_BIT_MASK:			0x20
	LCD_Write((((LCD_DL<<4)&0x10)+((LCD_N<<3)&0x08)+((LCD_F <<2)&0x04)) | 0x20);
	
//	// CURSOR OR DISPLAY SHIFT	| 0  0  0  1  SC RL —  — |
//	// BIN_OR_BIT_MASK:			{ 0  0  0  1  0  0  0  0 }
//	// HEX_OR_BIT_MASK:			0x10
//	LCD_Write((((LCD_SC<<3)&0x08)+((LCD_RL<<3)&0x04)) | 0x10);
	
	// DISPLAY OFF CONTROL          | 0  0  0  0  1  D  C  B |
	// BIN_OR_BIT_MASK:			{ 0  0  0  0  1  0  1  1 }
	// HEX_OR_BIT_MASK:			0x08
	LCD_Write((((LCD_C<<1)&0x02)+(LCD_B&0x01)) | 0x08);

	// CLEAR LCD DISPLAY
	LCD_Write(0x01);
	
	// ENTRY MODE SET			| 0  0  0  0  0  1  ID S |
	// BIN_OR_BIT_MASK:			{ 0  0  0  0  0  1  0  0 }
	// HEX_OR_BIT_MASK:			0x04
	LCD_Write((((LCD_ID<<1)&0x02)+(LCD_S&0x01)) | 0x04);
	
	__delay_ms(10);

        // DISPLAY ON CONTROL	| 0  0  0  0  1  D  C  B |
	// BIN_OR_BIT_MASK:			{ 0  0  0  0  1  1  1  1 }
	// HEX_OR_BIT_MASK:			0x08
	LCD_Write((((LCD_D<<2)&0x04)+((LCD_C<<1)&0x02)+(LCD_B&0x01)) | 0x08);
}

/******************************************************************************
*                          Extended display functions                         *
******************************************************************************/

void LCD_WriteNumber(unsigned long num){
	LCD_RS = 1;
	if (num == 0) LCD_Write('0');
	else {
		static char numstr[10];
		char CurPos=0;
		while (num != 0){
			numstr[CurPos] = (num%10) + '0';
			num /= 10;
			CurPos++;
		}
		
		while (CurPos--) LCD_Write(numstr[CurPos]);
	}
        LCD_RS = 0;
}

void LCD_WriteString(const char * s){
	LCD_RS = 1;
	while(*s){
		if (*s != '\n') LCD_Write(*s);
		else LCD_GoTo(0x40);
		
		s++;
	}
        LCD_RS = 0;
}

void LCD_Clear(void){
	LCD_RS = 0;
	LCD_Write(0x01);
	LCD_RS = 1;
	__delay_ms(2);
}

void LCD_Home(void){
	LCD_RS = 0;
	LCD_Write(0x02);
	LCD_RS = 1;
	__delay_ms(2);
}

void LCD_GoTo(unsigned char P){
	LCD_RS = 0;
	LCD_Write(0x80+P);
	LCD_RS = 1;
	__delay_ms(2);
}
