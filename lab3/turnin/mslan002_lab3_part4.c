#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

int main(void) {
    while(1) {
        unsigned char inp = PINA;
        PORTC = 0x00 | ((inp & 0x0F) << 4);
        PORTB = 0x00 | ((inp & 0xF0) >> 4);
    }
}
