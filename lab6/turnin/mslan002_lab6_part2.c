/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: SynchSM part 2
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/0Z52io1vUpQ
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

typedef enum {START, B0, B1A, B2, B1B, PRESS, HOLD, REL}State;
State state = START;
State prior = START;
State ret = START;

void Tick() {
	if ((PINA & 0x01) == 0x01 && (state < PRESS)) { /* assign button press */
		state = PRESS;
	}
	
	switch(state) { /* transitions */
		case START:
			state = B0;
			break;
		case B0:
			state = B1A;
			break;
		case B1A:
			state = B2;
			break;
		case B2:
			state = B1B;
			break;
		case B1B:
			state = B0;
			break;
		case PRESS:
			ret = prior;
			state = HOLD;
			break;
		case HOLD:
			if ((PINA & 0x01) == 0x00) {
				state = REL;
			}
			break;
		case REL:
			if ((PINA & 0x01) == 0x01) {
				state = ret;
			}
			break;
	}
	
	switch(state) { /* actions */
		case B0:
			PORTB = 0x01;
			break;
		case B1A:
		case B1B:
			PORTB = 0x02;
			break;
		case B2:
			PORTB = 0x04;
			break;
		case PRESS:
			break;
		case HOLD:
			break;
		case REL:
			break;
		default:
			PORTB = 0x00;
			break;
	}
	
	prior = state;
}

int main(void) {
	DDRA = 0x00;
    DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(300);
	TimerOn();
	
	
    while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}