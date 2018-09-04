#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"

volatile int adBuffer[3];

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

void initADCinterrupt(void)
{
	LPC_SYSCON->PDRUNCFG			&= ~(0x1<<4);				//power up ADC
	LPC_SYSCON->SYSAHBCLKCTRL	|= (1<<13);					//bit6 enables clock for ADC
	
	LPC_IOCON->R_PIO1_0				=0x02;							//set pin to ADC mode
	LPC_IOCON->R_PIO1_1				=0x02;
	LPC_IOCON->R_PIO1_2				=0x02;
	
	LPC_ADC->CR								= 0x010B0E;					//BURST mode ON, CLKDIV = B so clock for ADC is 12Mhz/(CLKDIV+1), SEL = E to select ADC 1, 2, 3 1110
	LPC_ADC->INTEN						&=~(1<<8);					//must be set 0 in BURST mode
	NVIC_EnableIRQ(ADC_IRQn);
	LPC_ADC->INTEN						|=(0x0E);					//set pins for ADC 1, 2, 3 ca generate interrupt
	LPC_ADC->CR								&=~(7<<24);					//when BURST bit is 1, START bits must be 000;
}

void ADC_IRQHandler(void)
{
	for(int i = 0; i < 3; i++)
	{
		adBuffer[i] = (LPC_ADC->DR[(i+1)]& 0xFFC0)>>8;
	}
	NVIC_DisableIRQ(ADC_IRQn);
}

void readADC(int *buffer)
{
	for(int i = 0; i < 3; i++)
	{
		*buffer = adBuffer[i];
		buffer++;
	}
}
