#include "LPC11xx.h"
#include "math.h"

#include "PIOinit.h"
#include "PWMinit.h"
#include "LCDinit.h"
#include "ADCinit.h"
#include "Functions.h"
#include "PIreg.h"

	volatile double error, integral = 0;

int PIregulator(double setpoint, double current)
{
	int PWM = 0;
	error = setpoint - current;										//error = setpoint curent - current current
	integral = integral + error;
		
	if (integral > ADCmaxValue)							//integral max limit
		integral = ADCmaxValue;
		
	PWM = (Kp * error) + (Ki * integral);
		
	if (PWM > ADCmaxValue)			//output max limit
		PWM = ADCmaxValue;
	else if (PWM < 0)						//output min limit
		PWM = 0;
		
	if(setpoint < 0.01)							//if setpiont is less than 0.01A, turn off MosFET
		PWM = 0;
		
	PWM = ADCmaxValue - PWM;														//invert PWM value because duty cycle is inverted
	
	return PWM;
}
