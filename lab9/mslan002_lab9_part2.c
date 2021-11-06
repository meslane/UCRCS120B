/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: Note increment/decrement
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/lt-2CBqbYjM
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

void set_PWM(double frequency) {
	static double current_frequency;
	
	if (frequency != current_frequency) {
		if (!frequency) {TCCR3B &= 0x08;}
		else {TCCR3B |= 0x03;}
			
		if (frequency < 0.954) {
			OCR3A = 0xFFFF;
		}
		else if (frequency > 31250) {
			OCR3A = 0x0000;
		}
		else {
			OCR3A = (short) (8000000 / (128 * frequency)) - 1;
		}
		
		TCNT3 = 0;
		current_frequency = frequency;
	}	
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

double noteTable[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char noteIndex = 0;

unsigned char mute = 0; //0 = sound on, else sound off

typedef enum {START, IDLE, PRESS}State;
State state = START;
	
void Tick() {
	switch(state) { /* transitions */
		case START:
			state = IDLE;
			break;
		case IDLE:
			switch(PINA) {
				case 0x01:
					if (noteIndex > 0) {
						noteIndex--;
					}
					break;
				case 0x02:
					if (noteIndex < 7) {
						noteIndex++;
					}
					break;
				case 0x04:
					mute = ~mute;
					break;
			}
			state = PRESS;
			break;
		case PRESS:
			if (PINA == 0) {
				state = IDLE;
			}
			break;
	}
	
	if (mute == 0) {
		set_PWM(noteTable[noteIndex]);
	} else {
		set_PWM(30000);
	}
}

int main(void){
	DDRB = 0xFF;
	DDRA = 0xF8;
	PORTA = 0x00;
	
	TimerSet(200);
	TimerOn();
	
	PWM_on();
   
    while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
}