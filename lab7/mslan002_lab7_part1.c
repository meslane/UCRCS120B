/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: SynchSM incrementing/decrementing with LCD
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/H904MX4oLbU
 */

#include <avr/io.h>
#include "io.h"
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

typedef enum {START, BOTHUP, PRESS0, PRESS1, PRESSBOTH}State;
State state = START;
State prev = START;

unsigned char holdTime = 0;
unsigned char score = 0x07;

void incB() {
	if (score < 9) {
		score++;
	}
}

void decB() {
	if (score > 0) {
		score--;
	}
}

void Tick() {
	
	switch(state) { /*transitions */
		case START:
			state = BOTHUP;
			break;
		case BOTHUP:
		case PRESS0:
		case PRESS1:
			if ((PINA & 0x03) == 0x01) {
				state = PRESS0;
			}
			else if ((PINA & 0x03) == 0x02) {
				state = PRESS1;
			}
			else if ((PINA & 0x03) == 0x03) {
				state = PRESSBOTH;
			}
			else if ((PINA & 0x03) == 0x00) {
				state = BOTHUP;
			}
			break;
		case PRESSBOTH:
			if ((PINA & 0x03) == 0x00) {
				state = BOTHUP;
			}
			break;
		default:
			break;
	}
	
	switch(state) { /* actions */
		case BOTHUP:
			holdTime = 0;
			break;
		case PRESS0:
			if (prev != PRESS0) {
				holdTime = 0;
				incB();
			}
			else if (holdTime == 10) {
				holdTime = 0;
				incB();
			}
			holdTime++;
			break;
		case PRESS1:
			if (prev != PRESS1) {
				holdTime = 0;
				decB();
			}
			else if (holdTime == 10) {
				holdTime = 0;
				decB();
			}
			holdTime++;
			break;
		case PRESSBOTH:
			holdTime = 0;
			score = 0;
			break;
		default:
			break;
	}
	
	prev = state;
}

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
	DDRA = 0x00;
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	TimerSet(100);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	
    while (1) {
		Tick();
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}