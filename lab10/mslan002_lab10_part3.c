/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: Concurrent state machines pt. 3
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/UyS-7ELLPCA
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

void Tick_ToggleSpeaker() {
	static unsigned char i = 0;
	
	if ((PINA & 0x04) == 0x04) {
		if (i > 0) {
			PORTB |= 0x10;
		}
		else  {
			PORTB &= 0xEF;
		}
		
		i = ~i;	
	}
	else {
		PORTB &= 0xEF; //speaker off
	}
}

int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(2);
	TimerOn();
    
	unsigned long threeLEDPeriod = 150;
	unsigned long blinkLEDPeriod = 500;
	
	unsigned long i = 1;
	
    while (1) {
		if (i % threeLEDPeriod == 0) {
			Tick_ThreeLEDs();
		}
		if (i % blinkLEDPeriod == 0) {
			Tick_BlinkLED();
		}
		Tick_CombineLEDs();
		Tick_ToggleSpeaker();
		
		while (!TimerFlag);
		TimerFlag = 0;
		
		i = (i < 75000) ? (i + 1) : 1;
    }
}