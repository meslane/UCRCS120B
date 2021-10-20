/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: Button Incrementing/Decrementing
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=oKV6I_V_IYg
 */


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef enum {START, BOTHUP, PRESS0, PRESS1, BOTHDOWN}State;
State state = START;

void Tick() {
    unsigned int temp = ~PINA;
    
	switch(state) { /* transitions */
		case START:
		state = BOTHUP;
		break;
		case BOTHUP:
		if ((temp & 0x03) == 0x03) {
			state = BOTHDOWN;
			PORTC = 0;
		}
		else if ((temp & 0x02) == 0x02) {
			state = PRESS1;
			if (PORTC > 0) {
				PORTC -= 1;
			}
		}
		else if ((temp & 0x01) == 0x01) {
			state = PRESS0;
			if (PORTC < 9) {
				PORTC += 1;
			}
		}
		break;
		case PRESS0:
		if ((temp & 0x03) == 0x03) {
			state = BOTHDOWN;
			PORTC = 0;
		}
		else if ((temp & 0x02) == 0x02) {
			state = PRESS1;
			if (PORTC > 0) {
				PORTC -= 1;
			}
		}
		else if ((temp & 0x01) == 0x00) {
			state = BOTHUP;
		}
		break;
		case PRESS1:
		if ((temp & 0x03) == 0x03) {
			state = BOTHDOWN;
			PORTC = 0;
		}
		else if ((temp & 0x01) == 0x01) {
			state = PRESS0;
			if (PORTC < 9) {
				PORTC += 1;
			}
		}
		else if ((temp & 0x02) == 0x00) {
			state = BOTHUP;
		}
		break;
		case BOTHDOWN:
		if ((temp & 0x03) == 0x00) {
			state = BOTHUP;
		}
		else if ((temp & 0x02) == 0x02) {
			state = PRESS1;
		}
		else if ((temp & 0x01) == 0x01) {
			state = PRESS0;
		}
		break;
	}
}

int main(void) {
	DDRA = 0x00; /* input */
	DDRC = 0xFF; /* output */
	PORTC = 0x00;
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	PORTC = 7;
	while(1) {
		Tick();
	}
	return 0;
}
