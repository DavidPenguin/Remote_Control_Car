#include <avr/io.h>
#include "scheduler.h"
#include "lcd.h"

unsigned short input;
unsigned short input2;

void A2D_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Set_A2D_Pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	//Allow channel to stabilize static
	//unsigned char i = 0;
	//for(i = 0; i < 15; i++)
	//{
	//asm("nop");
	//}
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int LR = 0;		// 1 = left, 2 = right
enum state_LR{init_LR, nothing_LR, right, left};
int LR_move(int state)
{
	Set_A2D_Pin(0);
	unsigned int i = 0;
	input = ADC;
	
	switch(state)
	{
		case init_LR:
			state = nothing_LR;
			break;
		
		case nothing_LR:
		case right:
		case left:
			input = ADC;
			if(input > 600)
				state = left;
			else if(input < 424)
				state = right;
			else
				state = nothing_LR;
			break;
		
		
		default:
			state = init_LR;
	}
	
	switch(state)
	{
		case nothing_LR:
			LR = 0;
			break;
		
		case right:
			LR = 1;
			break;
		
		case left:
			LR = 2;
			break;
	}
	
	return state;
}

int UD = 0; // 1 = up, 2 = down
enum state_UD{init_UD, nothing_UD, up, down};
int UD_move(int state)
{
	Set_A2D_Pin(1);
	unsigned int i = 0;
	input2 = ADC;
	
	switch(state)
	{
		case init_UD:
			state = nothing_UD;
			break;
		
		case up:
		case down:
		case nothing_UD:
			input2 = ADC;
			if(input2 > 600)
				state = down;
			else if(input2 < 400)
				state = up;
			else
				state = nothing_UD;
			break;
		
		default:
			state = init_UD;
	}
	
	switch(state)
	{
		case nothing_UD:
			UD = 0;
			break;
		
		case up:
			UD = 1;
			break;
		
		case down:
			UD = 2;
			break;
	}

	return state;
}

int display(int state)
{
	if(LR == 0 && UD == 0)
		LCD_DisplayString(1, "No Direction");
	else if(LR == 0 && UD == 1)
		LCD_DisplayString(1, "Up");
	else if(LR == 0 && UD == 2)
		LCD_DisplayString(1, "Down");
	else if(LR == 1 && UD == 0)
		LCD_DisplayString(1, "Left");
	else if(LR == 1 && UD == 1)
		LCD_DisplayString(1, "Up Left");
	else if(LR == 1 && UD == 2)
		LCD_DisplayString(1, "Down Left");
	else if(LR == 2 && UD == 0)
		LCD_DisplayString(1, "Right");
	else if(LR == 2 && UD == 1)
		LCD_DisplayString(1, "Up Right");
	else if(LR == 2 && UD == 2)
		LCD_DisplayString(1, "Down Right");
		
	return state;
}

int main(void)
{
	PORTA = 0x0F; DDRA = 0xF0;
	PORTD = 0x00;
	DDRD = 0xFF;
	A2D_init();
	LCD_init();
	
	tasksNum = 3;
	task function[3];
	tasks = function;
	
	tasks[0].state = init_LR;
	tasks[0].period = 50;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &LR_move;
	
	tasks[1].state = init_UD;
	tasks[1].period = 50;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &UD_move;
	
	tasks[2].state = 1;
	tasks[2].period = 200;
	tasks[2].elapsedTime = tasks[1].period;
	tasks[2].TickFct = &display;
	
	TimerSet(1);
	TimerOn();
	/* Replace with your application code */
	while (1)
	{}
}