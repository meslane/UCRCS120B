/*    Your Name & E-mail: mslan002@ucr.edu
 *    Lab Section: 023
 *    Assignment: Lab #2  Exercise #1 
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
    unsigned char test;

    while(1) {
        test = PINA & 0x03;
        
	switch(test) {
	    case 0x00:
                PORTB &= 0x00;
		break;
	    case 0x01:
		PORTB |= 0x01;
		break;
	    case 0x02:
	    case 0x03:
		PORTB &= 0x00;
		break;
	}
    }    
}
