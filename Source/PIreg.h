#define ADCmaxValue 		255.0f

#define Kp							10						//PI proportional constant
#define Ki							1							//PI integral constant

#ifndef PIREG_INCLUDED
#define PIREG_INCLUDED

int PIregulator(double setpoint, double current);

#endif
