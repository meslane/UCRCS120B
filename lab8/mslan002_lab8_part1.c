/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: ADC w/ pot
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/8-fCdxhZmy8
 */

#include <avr/io.h>

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
		PORTD = (char) ((potValue >> 8) & 0x03);
		PORTB = (char) (potValue & 0x00FF);
    }
}