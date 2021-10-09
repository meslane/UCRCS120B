/*    Your Name & E-mail: mslan002@ucr.edu
 *    Lab Section: 023
 *    Assignment: Lab #2  Exercise #2 
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
    unsigned char cnt;

    while(1) {
	cnt = 4;
        for(unsigned char i = 0; i < 4; i++) {
            cnt -= ((PINA >> i) & 0x01);
	}	
	PORTC = cnt;
    }
}
