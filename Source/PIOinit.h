#define LCD_PIO0_RS	6				//LCD Register select
#define LCD_PIO0_EN	1				//LCD Enable

														//LCD data pins
#define LCD_PIO0_DB4	2
#define LCD_PIO0_DB5	9
#define LCD_PIO1_DB6	6
#define LCD_PIO0_DB7	11

#ifndef PIOINIT_INCLUDED
#define PIOINIT_INCLUDED

void initGPIO(void);

#endif
