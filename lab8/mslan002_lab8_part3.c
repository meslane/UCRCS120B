/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: ADC w/ binary light
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/BpFYZhnkvy8
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
	unsigned short potValue;
	
    while (1) {
		potValue = ADC;
		if (potValue >= (MAX / 2)) {
			PORTD = 0x01;
		}
		else {
			PORTD = 0x00;
		}
    }
}