/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: Note playing
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/BlmQoSbQVbw
 */

#include <avr/io.h>

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

int main(void){
	DDRB = 0x40;
	DDRA = 0xF8;
	PORTA = 0x00;
	
	PWM_on();
   
    while (1) {
		switch(PINA) {
			case 0x01:
				set_PWM(261.63);
				break;
			case 0x02:
				set_PWM(293.66);
				break;
			case 0x04:
				set_PWM(329.63);
				break;
			default:
				set_PWM(30000);
				break;
		}
    }
}