/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: LCD
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/P-0kA9y6Cv8
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>

/* helpers */
unsigned char GetBit(unsigned char var, unsigned char pos) {
	return ((var & (1 << pos)) != 0) ? 1 : 0;
}

/* timer code */
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A  = 125;
	TIMSK1 = 0x02;
	TCNT1  = 0x00;
	
	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
/* end timer code */

/* begin scheduler code */
typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;
/* end scheduler code */

/* begin get key task */
/* col 1 2 3 4
 *   |________
 *  1| 1 2 3 A
 *  2| 4 5 6 B
 *  3| 7 8 9 C
 *  4| * 0 # D
*/
unsigned char GetKeypadKey() {
	PORTC = 0xEF; //column 1
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('1');}
	if (GetBit(PINC, 1) == 0) {return('4');}
	if (GetBit(PINC, 2) == 0) {return('7');}
	if (GetBit(PINC, 3) == 0) {return('*');}
		
	PORTC = 0xDF; //column 2
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('2');}
	if (GetBit(PINC, 1) == 0) {return('5');}
	if (GetBit(PINC, 2) == 0) {return('8');}
	if (GetBit(PINC, 3) == 0) {return('0');}
	
	PORTC = 0xBF; //column 3
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('3');}
	if (GetBit(PINC, 1) == 0) {return('6');}
	if (GetBit(PINC, 2) == 0) {return('9');}
	if (GetBit(PINC, 3) == 0) {return('#');}

	PORTC = 0x7f; //column 4
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('A');}
	if (GetBit(PINC, 1) == 0) {return('B');}
	if (GetBit(PINC, 2) == 0) {return('C');}
	if (GetBit(PINC, 3) == 0) {return('D');}
	
	return('\0');
}

enum GetKey_States {GetKey_START, GetKey_GET};
unsigned char keyResult = '\0';

int Tick_GetKey(int state) {
	switch(state) { //transitions
		case GetKey_START:
			state = GetKey_GET;
			break;
		case GetKey_GET:
			break;
		default:
			state = GetKey_START;
			break;
	}
	
	switch(state) { //actions
		case GetKey_START:
			break;
		case GetKey_GET:
			keyResult = GetKeypadKey();
			break;
	}
	
	return state;
}
/* end get key task */

/* begin text scroll task */
enum Scroll_States {Scroll_START, Scroll_A, Scroll_B};
	
int Tick_Scroll(int state) {
	switch(state) { //transitions 
		case Scroll_START:
			state = Scroll_A;
			break;
		case Scroll_A:
			state = Scroll_B;
			break;
		case Scroll_B:
			state = Scroll_A;
			break;
		default: 
			state = Scroll_START;
			break;
	}
	
	switch(state) {
		case Scroll_START:
			break;
		case Scroll_A:
			LCD_ClearScreen();
			LCD_DisplayString(1, "CS120B is Legend...");
			break;
		case Scroll_B:
			LCD_ClearScreen();
			LCD_DisplayString(1, "wait for it DARY!");
			break;
	}
	
	return state;
}

int main(void){
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF;
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	
	static task GetKey, Scroll;
	task *tasks[] = {&GetKey, &Scroll};
	const unsigned short numTasks = 2;
	
	const char start = -1;
	
	//getkey task
	GetKey.state = start;
	GetKey.period = 50;
	GetKey.elapsedTime = GetKey.period;
	GetKey.TickFct = &Tick_GetKey;
	
	//scroll task
	Scroll.state = start;
	Scroll.period = 2000;
	Scroll.elapsedTime = Scroll.period;
	Scroll.TickFct = &Tick_Scroll;
	
	const unsigned short GCD = 50;
	
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	
	unsigned short i;
    while (1) {
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime >= tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		
	
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}