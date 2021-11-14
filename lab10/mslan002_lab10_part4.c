/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #10  Exercise #4
 *	Exercise Description: Concurrent state machines pt. 4
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/X0357dy1GUQ
 */

#include <avr/io.h>
#include <avr/interrupt.h>

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

/* state machine code */
unsigned char threeLEDOut;
unsigned char blinkLEDOut;
unsigned char tickPeriod = 1;

typedef enum {START, UP, A0DOWN, A1DOWN, WAIT}State;
State state = START;

void Tick_ThreeLEDs() {
	static unsigned char i = 0;
	threeLEDOut = (1 << i);
	i = (i < 2) ? (i + 1) : 0; //increment i if < 2, set to zero else
}

void Tick_BlinkLED() {
	static unsigned char i = 0;
	if (i == 0) {
		blinkLEDOut = 0x08;
	}
	else {
		blinkLEDOut = 0x00;
	}
	
	i = ~i;
}

void Tick_CombineLEDs() {
	PORTB = (0x0F & (threeLEDOut | blinkLEDOut));
}

void Tick_AdjustFreq() {
	switch(state) { /* transitions */
		case START:
			state = UP;
			break;
		case UP:
			if ((PINA & 0x01) == 0x01) {
				state = A0DOWN;
			}
			if ((PINA & 0x02) == 0x02) {
				state = A1DOWN;
			}
			break;
		case A0DOWN: //freq up
			if (tickPeriod >= 2) {
				tickPeriod = tickPeriod / 2;
			}
			state = WAIT;
			break;
		case A1DOWN: //freq down
			if (tickPeriod == 1) {
				tickPeriod++;
			}
			else if (tickPeriod < 128) {
				tickPeriod = tickPeriod * 2;
			}
			state = WAIT;
			break;
		case WAIT:
			if ((PINA & 0x03) == 0x00) {
				state = UP;
			}
			break;
	}
}

void Tick_ToggleSpeaker() {
	static unsigned char i = 0;
	static unsigned char count = 0;
	
	if ((PINA & 0x04) == 0x04) {
		if (i > 0) {
			PORTB |= 0x10;
		}
		else  {
			PORTB &= 0xEF;
		}
		
		count++;
		if (count == tickPeriod) {
			i = ~i;	
			count = 0;
		}
	}
	else {
		PORTB &= 0xEF; //speaker off
	}
}

int main(void) {
	DDRA = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
    
	unsigned long threeLEDPeriod = 300;
	unsigned long blinkLEDPeriod = 1000;
	
	unsigned long i = 1;
	
    while (1) {
		if (i % threeLEDPeriod == 0) {
			Tick_ThreeLEDs();
		}
		if (i % blinkLEDPeriod == 0) {
			Tick_BlinkLED();
		}
		Tick_CombineLEDs();
		Tick_AdjustFreq();
		Tick_ToggleSpeaker();
		
		while (!TimerFlag);
		TimerFlag = 0;
		
		i = (i < 300000) ? (i + 1) : 1;
    }
}