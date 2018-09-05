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

#define LCDrefreshRate	100						//number of executions before LCD is refreshed

int counter = 0;		//Counter for LCD refrshing
extern int adReadBuffer[3];

int main ()
{
	initGPIO();
	initADCinterrupt();
	initPWM();
	initSYSTICK();
	initLCD();
	
	delayms(100);
	LCDsendString("**Active  load**");
	delayms(1000);

	while(1)
	{
		NVIC_EnableIRQ(ADC_IRQn);											//enable ADC interrupt
		readADC(adReadBuffer);
		convert();
		if (counter == LCDrefreshRate)	//refresh LCD every LCDrefreshrate times
		{
			LCDmenuPrint();
			counter = 0;
		}
		counter ++;
	}
}
