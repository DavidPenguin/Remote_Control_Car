#include <avr/io.h>	
#include "scheduler.h"

int position_lr = 0;

void left_turn()
{
	PORTA = 0x05;
	
	if(position_lr < 2)
	{
		PORTB = 0x03;
		position_lr++;
	}
	else
	{		
		PORTB = 0x05;
	}
}

enum direction{none, up, down, left, right, up_right, up_left, down_right, down_left};

int drive(int state)
{
	switch(state)
	{
		case -1:
			state = none;
			break;
		
		case none:
			state = left;
			break;
		
		case up:
			state = down;
			break;
		
		case down:
			state = left;
			break;
			
		case left:
			state = left;
			break;
			
		//case right:
		//	state = none;
		//	break;
		
		default:
			state = none;
	}
	switch(state)
	{
		case none:
			PORTA = PORTB = 0x01;	//001
			break;
		
		case up:
			PORTA = PORTB = 0x05;	//101
			break;
		
		case down:
			PORTA = PORTB = 0x03;	//011
			break;
			
		case left:
			left_turn();
			break;
	}
	
	return state;
}

int main(void)
{
	PORTA = PORTB = 0x00;
	DDRA = DDRB = 0xFF;
	
	tasksNum = 1;
	task function[1];
	tasks = function;
	
	tasks[0].state = -1;
	tasks[0].period = 500;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &drive;
	
	TimerSet(1);
	TimerOn();
	/* Replace with your application code */
	while (1)
	{}
}
