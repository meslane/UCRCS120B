#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRB = 0xFE;
    PIND = 0x00;
    PINB = 0x00;
    PORTB = 0x00;
    while(1) {
        unsigned short pd = PIND & 0x00FF;
        unsigned short weight = (pd * 2);
        if (PINB == 0x01) {
            weight++;
        }
        PORTB = 0;
        if (weight >= 70) {
            PORTB |= 0x02;
        }
        else if (weight < 70 && weight > 5) {
            PORTB |= 0x04;
        }
    }
}
