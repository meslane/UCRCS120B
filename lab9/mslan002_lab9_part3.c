/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: Melody player
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/pptQUIMu_wc
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
//                     C0       D1      E2      F3      G4      A5      B6     C7
double noteTable[9] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 30000};

unsigned char mute = 0; //0 = sound on, else sound off

typedef enum {START, IDLE, PRESS, PLAY, WAIT}State;
State state = START;

//unsigned char song[12] = {1, 3, 5, 4, 4, 2, 4, 1, 5, 7, 1, 1};
unsigned char song[] = {5, 8, 2, 8, 7, 6, 5, 8, 5, 6, 7, 5, 4, 5, 2, 8, 5, 8, 2, 8, 7, 6, 5, 8, 0, 1, 6, 4, 5, 2, 5};
//gourmet race from kirby

void playSong() {
	TimerSet(250);
	TimerOn();
	PWM_on();
	
	for (unsigned int i = 0; i < sizeof(song); i++) {
		set_PWM(noteTable[song[i]]);
		while (!TimerFlag);
		TimerFlag = 0;	
	}
	
	PWM_off();
	TimerOff();
}
	
void Tick() {
	switch(state) { /* transitions and actions */
		case START:
			state = IDLE;
			break;
		case IDLE:
			if ((PINA & 0x01) == 0x01) {
				state = PRESS;
			}
			break;
		case PRESS:
			if ((PINA & 0x01) == 0x00) {
				state = PLAY;
			}
			break;
		case PLAY:
			playSong();
			state = WAIT;
			break;
		case WAIT:
			if (PINA == 0x00) {
				state = IDLE;
			}
			break;
	}
}

int main(void){
	DDRB = 0xFF;
	DDRA = 0xF8;
	PORTA = 0x00;
	
	PWM_on();
   
    while (1) {
		Tick();
		//while (!TimerFlag);
		//TimerFlag = 0;
    }
}