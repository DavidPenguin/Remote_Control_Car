/*
 * car.c
 *
 * Created: 11/18/2015 4:00:05 PM
 * Author : David
 */ 

#include <avr/io.h>
#include "scheduler.h"

enum direction{none, right, left, up, down, up_right, up_left, down_right, down_left};
	
int drive(int state)
{
	switch(state)	
	{
		case -1:
			state = none;
			break;
			
		case none:
			state = up;
			break;
			
		case up:
			state = down;
			break;
			
		case down:
			state = none;
			break;
	}
	
	switch(state)
	{
		case none:
			PORTA = PORTB = 00;
			break;
		
		case up:
			PORTA = PORTB = 01;
			break;
		
		case down:
			PORTA = PORTB = 10;
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
	
	tasks[1].state = 1;
	tasks[1].period = 50;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &drive;
	
	TimerSet(1);
	TimerOn();

    /* Replace with your application code */
    while (1) 
    {
    }
}

