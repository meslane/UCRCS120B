/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: Button Incrementing/Decrementing
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=8i_wlpfSHQE
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef enum {START, RELEASE, PRESS}State;
State state = START;

unsigned char seq[] = {0x2A, 0x15, 0x38, 0x07};
unsigned char ptr = 0;

void Tick() {
	switch(state) { /* transitions */
		case START:
			state = RELEASE;
			break;
		case RELEASE:
			if ((PINA & 0x01) == 0x00) {
				if (ptr < 3) {
					ptr++;
				} else {
					ptr = 0;
				}
				
				PORTB = seq[ptr];
				
				state = PRESS;
			}
			break;
		case PRESS:
			if ((PINA & 0x01) == 0x01) {
				state = RELEASE;
			}
			break;
	}
}

int main(void) {
	DDRA = 0x00; /* input */
	DDRB = 0xFF; /* output */
	PORTB = 0x00;
	
	PORTB = seq[ptr];
    while (1) {
		Tick();
    }
}