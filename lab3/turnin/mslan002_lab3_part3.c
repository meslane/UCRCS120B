#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

int main(void) {
    while(1) {
        unsigned char fuelLevel = PINA & 0x0F;
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
