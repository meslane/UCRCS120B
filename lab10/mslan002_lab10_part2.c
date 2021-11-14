/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: Concurrent state machines pt. 2
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/Ao6RlnZkbZU
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

int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(100);
	TimerOn();
    
	unsigned char threeLEDPeriod = 3;
	unsigned char blinkLEDPeriod = 10;
	
	unsigned char i = 1;
	
    while (1) {
		if (i % threeLEDPeriod == 0) {
			Tick_ThreeLEDs();
		}
		if (i % blinkLEDPeriod == 0) {
			Tick_BlinkLED();
		}
		Tick_CombineLEDs();
		
		while (!TimerFlag);
		TimerFlag = 0;
		
		i = (i < 31) ? (i + 1) : 1;
    }
}