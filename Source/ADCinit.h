#ifndef ADCINIT_INCLUDED
#define ADCINIT_INCLUDED

	void initADC(void);
	void initADCinterrupt(void);
	void ADC_IRQHandler(void);
	void readADC(int *buffer);

#endif

