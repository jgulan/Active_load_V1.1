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

char main_menu[4][16] = {"Pulse width:","Peak current:","Duty cycle:"};
char width_menu[9][8] = {"1ms", "2ms", "5ms", "10ms", "20ms", "50ms", "100ms", "500ms", "1000ms"};
char peak_menu[9][8] = {"0,1A", "0,2A", "0,5A", "1A", "2A", "5A", "8A", "9A", "10A" };
char cycle_menu[13][5] ={"1%", "2%", "5%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"};

int width[9] = {1, 2, 5, 10 ,20 ,50, 100, 500, 1000};								//pulse width in ms
float peak[9] = {0.1, 0.2, 0.5, 1, 2, 5, 8, 9, 10};									//current pak in A
int cycle[13] = {1, 2, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};	//duty cycle in %

int menu_pointer = 0;
int width_pointer = 0;
int peak_pointer = 0;
int cycle_pointer = 0;
int current_menu_pointer = 0;
int preState = 0;

int menu_pointerMAX = (4-1);
int current_menu_pointerMAX = 0;


char currentSens[8];			//strings for numbers to display
char voltageSens[8];
char currentSet[8];
char powerString[8];

float adFloatBuffer[3];
int widthSetpoint;
int cycleSetpoint;
extern float peakCurrentSetpoint;

void LCDmenuPrint(void)
{
	LCDsendCmd(0x01);							//clear display
	LCDsendCmd(0x80);							//move cursor to beginning of first line
	delayms(2);
	LCDsendString(main_menu[menu_pointer]);
		
	LCDsendCmd(0xC0);							//new line
	switch(menu_pointer)
	{
		case 0:
			if(preState != 0)
				current_menu_pointer = 0;
			preState = 0;
			width_pointer = current_menu_pointer;
			widthSetpoint = width[width_pointer];
			LCDsendString(width_menu[width_pointer]);
			current_menu_pointerMAX = (9-1);
			break;
		case 1:
			if(preState != 1)
				current_menu_pointer = 0;
			preState = 1;
			peak_pointer = current_menu_pointer;
			adFloatBuffer[1] = peak[peak_pointer];				//current setpoint
			peakCurrentSetpoint = peak[peak_pointer];
			LCDsendString(peak_menu[peak_pointer]);
			current_menu_pointerMAX = (9-1);
			break;
		case 2:
			if(preState != 2)
				current_menu_pointer = 0;
			preState = 2;
			cycle_pointer = current_menu_pointer;
			cycleSetpoint = cycle[cycle_pointer];
			LCDsendString(cycle_menu[cycle_pointer]);
			current_menu_pointerMAX = (13-1);
			break;
		case 3:
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
		break;
		default:
			break;
			
	}	
}
