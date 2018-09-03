#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"


#define ADCmaxValue 		255.0f
#define V_REF						3.3f
#define R_shunt					0.007f
#define Isens_R1				47000.0f
#define Isens_R2				1000.0f
#define Vsens_R1				47000.0f
#define Vsens_R2				1000.0f


#define Kp							20						//PI proportional constant
#define Ki							1							//PI integral constant

#define LCDrefreshRate	100						//number of executions before LCD is refreshed

int counter = 0;		//Counter for LCD refrshing

int main ()
{
	initGPIO();
	initADC();
	initPWM();
	SYSTICKinit();
	initLCD();
	
	delayms(100);
	
	LCDsendString("**Active  load**");

	delayms(2000);
	
	char currentSens[8];			//strings for numbers to display
	char voltageSens[8];
	char currentSet[8];
	char powerString[8];
	
	int AD1, AD2, AD3;										//ADC values
	float AD1f, AD2f, AD3f;								//ADC float values
	float powerWatt;
	
	//PI regulator
	double error, integral = 0, PWM;
	
	while(1)
	{
		while((LPC_ADC->DR[1] < 0x7FFFFFFF));											//wait for flag "done" to be set
		AD1 = (LPC_ADC->DR[1] & 0xFFC0)>>8;												//voltag sensor data is stored in bits 15:6
		AD1f = AD1 / ADCmaxValue;																	//voltage sensor float vale
		AD1f = AD1f * V_REF *((Vsens_R1 + Vsens_R2) / Vsens_R2);
		floatToStr(AD1f, voltageSens, 2);
		
		while((LPC_ADC->DR[2] < 0x7FFFFFFF));											//wait for flag "done" to be set
		AD2 = (LPC_ADC->DR[2] & 0xFFC0)>>8;												//data is stored in bits 15:6
		AD2f = (ADCmaxValue - AD2) / ADCmaxValue;									//current setpoint
		AD2f = AD2f * 10;
		floatToStr(AD2f, currentSet, 2);
		
		while((LPC_ADC->DR[3] < 0x7FFFFFFF));																	//wait for flag "done" to be set
		AD3 = (LPC_ADC->DR[3] & 0xFFC0)>>8;																		//data is stored in bits 15:6
		AD3f = AD3 / ADCmaxValue;																							//currrent sensor
		AD3f = AD3f * V_REF * (1/R_shunt) * (Isens_R2/(Isens_R2+Isens_R1));	
		floatToStr(AD3f, currentSens, 2);
		
		powerWatt = AD1f * AD3f;														//P = U * I
		floatToStr(powerWatt, powerString, 2);
		
		//PI regulator
		
		error = AD2f - AD3f;										//error = setpoint curent - current current
		integral = integral + error;
		
		if (integral > ADCmaxValue)							//integral max limit
			integral = ADCmaxValue;
		
		PWM = (Kp * error) + (Ki * integral);
		
		if (PWM > ADCmaxValue)			//output max limit
			PWM = ADCmaxValue;
		else if (PWM < 0)						//output min limit
			PWM = 0;
		
		if(AD2f < 0.01)							//if setpiont is less than 0.01A, turn off MosFET
			PWM = 0;
		
		PWM = ADCmaxValue - PWM;														//invert PWM value because duty cycle is inverted
		
		LPC_TMR16B0->MR0 = (((float)PWM)/ADCmaxValue) * (CPUfreq / PWMfreq);	//duty cycle
		
		if (counter == LCDrefreshRate)
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
