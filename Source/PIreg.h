#define ADCmaxValue 		255.0f

#define Kp							20						//PI proportional constant
#define Ki							3							//PI integral constant

#ifndef PIREG_INCLUDED
#define PIREG_INCLUDED

int PIregulator(double setpoint, double current);

#endif
