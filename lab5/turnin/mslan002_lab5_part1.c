/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: Fuel Meter
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=m_ReDZ6qmSY
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; /* input */
	DDRC = 0xFF; /* output */
	PORTC = 0x00;
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD; 
	MCUCR = tmp; 
	MCUCR = tmp; 
	
	while(1) {
		unsigned char fuelLevel = ~PINA & 0x0F;
		unsigned char out = 0x00;
		 
		if (fuelLevel >= 1) {
			out |= 0x20;
		}
		if (fuelLevel >= 3) {
			out |= 0x10;
		}
		if (fuelLevel >= 5) {
			out |= 0x08;
		}
		if (fuelLevel >= 7) {
			out |= 0x04;
		}
		if (fuelLevel >= 10) {
			out |= 0x02;
		}
		if (fuelLevel >= 13) {
			out |= 0x01;
		}
		if (fuelLevel <= 4) {
			out |= 0x40;
		}
        if ((PINA & 0x70) == 0x30) { /* seatbelt sign */
            out |= 0x80;
        }

		PORTC = out;
	}
}
