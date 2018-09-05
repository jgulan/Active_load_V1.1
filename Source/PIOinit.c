#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"
#include "menuFunctions.h"
#include "converting.h"

volatile int brojac = 0;
extern int menu_pointer;
extern int width_pointer;
extern int peak_pointer;
extern int cycle_pointer;
extern int current_menu_pointer;
extern int current_menu_pointerMAX;
extern int menu_pointerMAX;

int vrati_brojac(void)
{
	return brojac;
}

void initGPIO(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL	|= (1<<6);																//bit6 enables clock for GPIO
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);																//bit 16 enables the clock to the IOCON block (for modifying IOCON registers)
	LPC_GPIO0->DIR						|= (1<<1)|(1<<2)|(1<<6)|(1<<9)|(1<<11);		//set pin 1, 2, 6, 9, 11 to output
	LPC_IOCON->R_PIO0_11			|= (1<<0);																//bit 0 enabls PIO0_11
	LPC_GPIO1->DIR						|= (1<<6);																//set pin 11 to output
		

	LPC_GPIO0->DIR						&=~(1<<4);						//set pins 4, 5 to input			  UP
	LPC_GPIO0->DIR						&=~(1<<5);																					//DOWN
	LPC_GPIO0->IS							&=~(1<<4)&(1<<5);						//set pins 4, 5 sensitive to edge
	LPC_GPIO0->IBE						&=~(1<<4)&(1<<5);						//interrupt is controled through register GPIO0IEV
	LPC_GPIO0->IEV						&= ~(1<<4)&(1<<5);						//interrupt on rising edge
	LPC_GPIO0->IE							|= (1<<4)|(1<<5);						//interrupt is not masked
	NVIC_EnableIRQ(EINT0_IRQn);
	
	LPC_GPIO1->DIR						&=~(1<<7);									//set pin 7 to input		//ENTER
	LPC_GPIO1->IS							&=~(1<<7);						//set pins 4, 5 sensitive to edge
	LPC_GPIO1->IBE						&=~(1<<7);						//interrupt is controled through register GPIO0IEV
	LPC_GPIO1->IEV						|=(1<<7);						//interrupt on rising edge
	LPC_GPIO1->IE							|= (1<<7);						//interrupt is not masked
	NVIC_EnableIRQ(EINT1_IRQn);
	
}

void PIOINT0_IRQHandler(void)
{
	switch(LPC_GPIO0->MIS & 0xFFF){
		case(0x10):																//UP
			LPC_GPIO0->IC |=(1<<4);
			{}{}																		//2 NOPs for two clocks
			if(!(LPC_GPIO0->DATA & (1<<5)))					//BACK
			{	
				menu_pointer--;
				if(menu_pointer < 0)
					menu_pointer = 0;
			}
			else 
			{
				current_menu_pointer++;
				if(current_menu_pointer > current_menu_pointerMAX)
					current_menu_pointer = current_menu_pointerMAX;
			}
			break;
		case(0x20):																//DOWN
			LPC_GPIO0->IC |=(1<<5);
			{}{}
			if(!(LPC_GPIO0->DATA & (1<<4)))
				{	
				menu_pointer--;
				if(menu_pointer < 0)
					menu_pointer = 0;
				}
				else 
				{
					current_menu_pointer--;
					if(current_menu_pointer < 0)
						current_menu_pointer = 0;
				}
			break;
		
		
	}
}

void PIOINT1_IRQHandler(void)
{
	if(LPC_GPIO1->MIS & 0x80)
	{
		LPC_GPIO1->IC |=(1<<7);
		{}{}
		menu_pointer++;
		if(menu_pointer > menu_pointerMAX)
			menu_pointer = menu_pointerMAX;
	}
}
