#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"


#define V_REF						3.3f
#define R_shunt					0.007f
#define Isens_R1				47000.0f
#define Isens_R2				1000.0f
#define Vsens_R1				47000.0f
#define Vsens_R2				1000.0f

#define LCDrefreshRate	100						//number of executions before LCD is refreshed

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
	
	char currentSens[8];			//strings for numbers to display
	char voltageSens[8];
	char currentSet[8];
	char powerString[8];
	
	int adReadBuffer[3];
	float adFloatBuffer[3];
	float powerWatt;
	
	int PWM;
	int counter = 0;		//Counter for LCD refrshing
		
	while(1)
	{
		NVIC_EnableIRQ(ADC_IRQn);											//enable ADC interrupt
		readADC(adReadBuffer);
		
		/*voltage sensor*/														
		adFloatBuffer[0] = adReadBuffer[0] /ADCmaxValue;														
		adFloatBuffer[0] = adFloatBuffer[0] * V_REF *((Vsens_R1 + Vsens_R2) / Vsens_R2);
		floatToStr(adFloatBuffer[0], voltageSens, 2);
		
		/*current setpoint*/
		adFloatBuffer[1] = (ADCmaxValue - adReadBuffer[1]) /ADCmaxValue;						
		adFloatBuffer[1] = adFloatBuffer[1] * 10;
		floatToStr(adFloatBuffer[1], currentSet, 2);
		
		/*currrent sensor*/
		adFloatBuffer[2] = adReadBuffer[2] /ADCmaxValue;														
		adFloatBuffer[2] = adFloatBuffer[2] * V_REF * (1/R_shunt) * (Isens_R2/(Isens_R2+Isens_R1));	
		floatToStr(adFloatBuffer[2], currentSens, 2);
		
		powerWatt = adFloatBuffer[0] * adFloatBuffer[2];											//P = U * I
		floatToStr(powerWatt, powerString, 2);
		
		PWM = PIregulator(adFloatBuffer[1], adFloatBuffer[2]);								//calculate PWM duty cycle
		
		LPC_TMR16B0->MR0 = (((float)PWM)/ADCmaxValue) * (CPUfreq / PWMfreq);	//set duty cycle
		
		if (counter == LCDrefreshRate)	//refresh LCD every LCDrefreshrate times
		{
			LCDsendCmd(0x01);							//clear display
			LCDsendCmd(0x80);							//move cursor to beginning of first line
			delayms(2);
			LCDsendString("Iset:");
			LCDsendString(currentSet);
			
			LCDsendCmd(0x14);							//shift cursor right by one character
			LCDsendString("I:");
			LCDsendString(currentSens);
			
			LCDsendCmd(0xC0);							//new line
			LCDsendString("U:");
			LCDsendString(voltageSens);
			
			LCDsendCmd(0x14);							//shift cursor right by one character
			LCDsendString("P:");
			LCDsendString(powerString);
			
			counter = 0;
		}
		counter ++;
	}
}
