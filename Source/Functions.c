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

volatile int msTicks = 0;
volatile int widthTicks = 0;
volatile int dutyCycleTicks = 0;


void initSYSTICK(void)
{
	SysTick->LOAD  = (uint32_t)(120 - 1UL);                         // set reload register
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); // set Priority for Systick Interrupt
  SysTick->VAL   = 0UL;                                             // Load the SysTick Counter Value
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;    // Enable SysTick IRQ and SysTick Timer
}

void delayms(int miliseconds)									//must be less than 1380ms
{
	msTicks = 0;
	SysTick->VAL  &= 0x0;												//Reset_IRQn System Tick counter and COUNTFLAG in CTRL register
	while((msTicks/100) < miliseconds);
}

void delayus(int useconds)
{
	msTicks = 0;
	SysTick->VAL  &= 0x0;												//Reset_IRQn System Tick counter and COUNTFLAG in CTRL register
	while((msTicks)*10 < useconds);
}

void SysTick_Handler(void)
{
	msTicks++;
	widthTicks++;
//	dutyCycleTicks++;

}

void delay(int count)
{
	for(int i = 0; i < count; i++);
}

void reverse(char *str, int len)
{
	int i = 0, j = len-1, temp;
	while (i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i=0;
	while (x)
	{
		str[i++] = (x % 10) + '0';
		x = x/10;
	}
	while(i<d)
		str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

void floatToStr(float n, char *res, int afterpoint)
{
	int ipart = (int)n;
	
	float fpart = n - (float)ipart;
	
	int i = intToStr(ipart, res, 1);
	
	if(afterpoint != 0)
	{
		res[i] = '.';
		fpart = fpart * pow(10, afterpoint);
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
