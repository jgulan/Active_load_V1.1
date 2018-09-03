#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"

void initGPIO(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL	|= (1<<6);																//bit6 enables clock for GPIO
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);																//bit 16 enables the clock to the IOCON block (for modifying IOCON registers)
	LPC_GPIO0->DIR						|= (1<<1)|(1<<2)|(1<<6)|(1<<9)|(1<<11);		//set pin 1, 2, 6, 9, 11 to output
	LPC_IOCON->R_PIO0_11			|= (1<<0);																//bit 0 enabls PIO0_11
	LPC_GPIO1->DIR						|= (1<<6);																//set pin 11 to output
}

