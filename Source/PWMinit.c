#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"

void initPWM(void)
{
	LPC_IOCON->PIO0_8					= (LPC_IOCON->PIO0_8 & ~(0x3FF)) |0x2;		//set pin for PWM use with counter
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);																//enables clock for 16 bit counter (CT16B0)
	LPC_TMR16B0->PR						= 0x0;																		//prescaler value
	LPC_TMR16B0->MCR					= 0x10;																		//set for reset when counter matches MR1
	LPC_TMR16B0->EMR					|= 0x20;																	//set pin on HIGH on match
	LPC_TMR16B0->CCR					= 0;																			//set to timer mode
	LPC_TMR16B0->PWMC					= 0x1;																		//set TR16B0 to PWM mode
	LPC_TMR16B0->MR1					= (CPUfreq / PWMfreq);										//set value for period
	LPC_TMR16B0->MR0					= 0x0;																		//set value for duty cycle
	LPC_TMR16B0->TCR					|= 0x3;																		//enable and reset counter
	LPC_TMR16B0->TCR					&= ~(0x2);																//clear reset bit
}
