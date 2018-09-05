#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

	void delayms(int miliseconds);
	void SysTick_Handler(void);
	void initSYSTICK(void);
	void reverse(char *str, int len);
	int intToStr(int x, char str[], int d);
	void floatToStr(float n, char *res, int afterpoint);
	void delay(int count);
	void delayus(int useconds);
		
#endif
