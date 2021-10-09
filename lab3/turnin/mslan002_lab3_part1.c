#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif   

int main(void) {
    while(1) {
        unsigned char oneCount = 0;

	for(unsigned char i = 0; i < 8; i++) {
           oneCount += (PINA >> i) & 0x01;
	   oneCount += (PINB >> i) & 0x01;
	}

        PORTC = oneCount;
    }

    return 0;
}
