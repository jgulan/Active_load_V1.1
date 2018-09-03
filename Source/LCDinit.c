#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"

	/*
Command list:
0x01	Clear display screen
0x30	8 bit, 1 line
0x38	8 bit, 2 line
0x20	4 bit, 1 lines
0x28	4 bit, 2 line
0x06	Entry mode
0x08	Display off, cursor off
0x0E	Display on, Cursor on
0x0C	Display on, Cursor off
0x0F	Display on, Cursor blinking
0x18	Shift entire display lft
0x1C	Shift entire display right
0x10	Move cursor left by on character
0x14	Move cursor right by on character
0x80	Force cursor o beginning of 1st row
0xC0	Force cursor to beginning of 2nd row
*/

/* ----------LCD functions--------- */

void initLCD(void)
{
	LCDsendCmd(0x03);				//initialize LCD
	delayms(2);
	LCDsendCmd(0x02);
	delayms(1);
	LCDsendCmd(0x28);				//enable 5x7 mode for charactersž
	delayms(1);
	LCDsendCmd(0x0C);				//display ON, cursor ON
	delayms(1);
	LCDsendCmd(0x01);				//clear display
	delayms(1);
	LCDsendCmd(0x80);				//move cursor to beginning of first line
	delayms(1);
	LCDsendCmd(0x06);
}

void LCDsendNibble(char nibble)
{
	LPC_GPIO0->DATA 		&= ~(GPIO0_DATA_MASK);											//clear previous data on pins DB4, DB5, DB7
	LPC_GPIO1->DATA			&= ~(GPIO1_DATA_MASK);											//clear previous data on pin DB6
	LPC_GPIO0->DATA			|= (((nibble >> 0) & 1) << LCD_PIO0_DB4);		//set data pins
	LPC_GPIO0->DATA			|= (((nibble >> 1) & 1) << LCD_PIO0_DB5);
	LPC_GPIO1->DATA			|= (((nibble >> 2) & 1) << LCD_PIO1_DB6);
	LPC_GPIO0->DATA			|= (((nibble >> 3) & 1) << LCD_PIO0_DB7);
}

void LCDsendCmd(char cmd)
{
	LCDsendNibble((cmd >> 4) & 0x0F);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_RS);			//send LOW puls to Rgister Select for command 
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_EN);			//high to low pulse on enable pin
	delay(100);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_EN);
	delay(100);
	
	LCDsendNibble(cmd & 0x0F);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_RS);			//send LOW puls to Rgister Select
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_EN);			//high to low pulse on enable pin
	delay(100);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_EN);
	delay(100);
	
}

void LCDsendData(char data){
	LCDsendNibble((data >> 4) & 0x0F);
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_RS);			//send HIGH puls to Rgister Select for data 
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_EN);			//high to low pulse on enable pin
	delay(100);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_EN);
	delay(100);
	
	LCDsendNibble(data & 0x0F);
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_RS);			//send HIGH puls to Rgister Select
	LPC_GPIO0->DATA			|= (1 << LCD_PIO0_EN);			//high to low pulse on enable pin
	delay(100);
	LPC_GPIO0->DATA			&= ~(1 << LCD_PIO0_EN);
	delay(100);
}

void LCDsendString(char *string)
{
	while(*string != '\0')
	{
		LCDsendData(*string);
		string++;
	}
}



