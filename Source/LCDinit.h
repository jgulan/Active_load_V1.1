#define GPIO0_CONTROL_MASK ((1<<LCD_PIO0_RS)|(1<<LCD_PIO0EN))										//Mask for clearing control pins
#define GPIO0_DATA_MASK ((1<<LCD_PIO0_DB4)|(1<<LCD_PIO0_DB5)|(1<<LCD_PIO0_DB7))	//Mask for clearing data pins on PIO0
#define GPIO1_DATA_MASK (1<<LCD_PIO1_DB6)																				//Mask for clearing data pins on PIO1

#ifndef LCDINIT_INCLUDED
#define LCDINIT_INCLUDED

	void initLCD(void);
	void LCDsendNibble(char nibble);
	void LCDsendCmd(char cmd);
	void LCDsendData(char data);
	void LCDsendString(char *string);

#endif
