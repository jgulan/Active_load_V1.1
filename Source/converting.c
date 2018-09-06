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

#define V_REF						3.3f
#define R_shunt					0.007f
#define Isens_R1				47000.0f
#define Isens_R2				1000.0f
#define Vsens_R1				47000.0f
#define Vsens_R2				1000.0f

extern float adFloatBuffer[3];

int adReadBuffer[3];
float powerWatt;

int PWM = 0;

extern char currentSens[8];			//strings for numbers to display
extern char voltageSens[8];
extern char currentSet[8];
extern char powerString[8];

extern int widthSetpoint;
extern int cycleSetpoint;

float peakCurrentSetpoint;

extern volatile int widthTicks;
extern volatile int dutyCycleTicks;

void convert(void)
{
	adFloatBuffer[0] = adReadBuffer[0] /ADCmaxValue;														
	adFloatBuffer[0] = adFloatBuffer[0] * V_REF *((Vsens_R1 + Vsens_R2) / Vsens_R2);
	floatToStr(adFloatBuffer[0], voltageSens, 2);
		
	/*current setpoint*/
	floatToStr(peakCurrentSetpoint, currentSet, 2);

	/*currrent sensor*/
	adFloatBuffer[2] = adReadBuffer[2] /ADCmaxValue;														
	adFloatBuffer[2] = adFloatBuffer[2] * V_REF * (1/R_shunt) * (Isens_R2/(Isens_R2+Isens_R1));	
	floatToStr(adFloatBuffer[2], currentSens, 2);
		
	powerWatt = adFloatBuffer[0] * adFloatBuffer[2];											//P = U * I
	floatToStr(powerWatt, powerString, 2);
	//floatToStr(adFloatBuffer[1], powerString, 2);
	
	/*  dutyCycleTicks is added every 10us, widthSetpoint is in ms and cycleSetpoint is in %  */
	if((widthTicks * 10) >= (widthSetpoint*10*cycleSetpoint))
	{
		adFloatBuffer[1] = 0;
		if((widthTicks * 10) >= (widthSetpoint*1000))
		{
			widthTicks = 0;
			adFloatBuffer[1] = peakCurrentSetpoint;
		}
	}
	
	PWM = PIregulator(adFloatBuffer[1], adFloatBuffer[2]);								//calculate PWM duty cycle
		
	LPC_TMR16B0->MR0 = (((float)PWM)/ADCmaxValue) * (CPUfreq / PWMfreq);	//set duty cycle
}
