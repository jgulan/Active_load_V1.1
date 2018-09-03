#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"

void initADC(void)
{
	LPC_SYSCON->PDRUNCFG			&= ~(0x1<<4);				//power up ADC
	LPC_SYSCON->SYSAHBCLKCTRL	|= (1<<13);					//bit6 enables clock for ADC
	
	LPC_IOCON->R_PIO1_0				=0x02;							//set pin to ADC mode
	LPC_IOCON->R_PIO1_1				=0x02;
	LPC_IOCON->R_PIO1_2				=0x02;
	
	LPC_ADC->CR								= 0x010B0E;					//BURST mode ON, CLKDIV = B so clock for ADC is 12Mhz/(CLKDIV+1), SEL = E to select ADC 1, 2, 3 1110
	LPC_ADC->INTEN						&=~(1<<8);					//must be set 0 in BURST mode
	LPC_ADC->CR								&=~(7<<24);					//when BURST bit is 1, START bits must be 000;
}
