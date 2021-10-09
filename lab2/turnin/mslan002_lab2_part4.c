/*    Your Name & E-mail: mslan002@ucr.edu
 *    Lab Section: 023
 *    Assignment: Lab #2  Exercise #4 
 *    Exercise Description: [optional - include for your own benefit]
 *    
 *    I acknowledge all content contained herein, excluding template 
 *     or example code, is my own original work.
 *
 *    Demo Link: <URL>
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

int main(void) {
    unsigned char totalWeight;
    unsigned char difference;

    while(1) {
        totalWeight = PINA + PINB + PINC;

	if (PINA >= PINC) {
            difference = PINA - PINC;
	}
	else {
	    difference = PINC - PINA;
	}

	if (totalWeight >= 141) {
            PORTD |= 0x01;
	}
	else {
            PORTD &= 0xFE; /* 1111 1110 */
	}

	if (difference > 80) {
            PORTD |= 0x02;
	}
	else {
	    PORTD &= 0xFD; /* 1111 1101 */
	}

	PORTD &= 0x03; /* 0000 0011 */
	PORTD |= ((totalWeight & 0xFC) >> 2);
    }

    return 1;
}
