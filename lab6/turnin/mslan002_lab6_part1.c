/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: SynchSM part 1
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/z0jH9aJWi-s
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

typedef enum {START, B0, B1, B2}State;
State state = START;

void Tick() {
	switch(state) { /* transitions */
		case START:
			state = B0;
			break;
		case B0:
			state = B1;
			break;
		case B1:
			state = B2;
			break;
		case B2:
			state = B0;
			break;	
	}
	
	switch(state) { /* actions */
		case B0:
			PORTB = 0x01;
			break;
		case B1:
			PORTB = 0x02;
			break;
		case B2:
			PORTB = 0x04;
			break;
		default:
			PORTB = 0x00;
			break;
	}
}

int main(void) {
    DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	
    while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}