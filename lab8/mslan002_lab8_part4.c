/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #4
 *	Exercise Description: ADC w/ light meter
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/O2wl-cYePQM
 */

#include <avr/io.h>

#define MIN 63
#define MAX 911

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    ADC_init();
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	unsigned short light;
	
    while (1) {
		PORTB = 0x00;
		light = ADC;
		
		if (light > MIN) {
			PORTB |= 0x01;
		}
		
		for (unsigned int i = 1; i < 8; i++) {
			if (light >= ((i * MAX) / 8)) {
				PORTB |= (0x01 << i);
			}
		}
    }
}