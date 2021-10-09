#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

typedef enum {START, BOTHUP, PRESS0, PRESS1, BOTHDOWN}State;
State state = START;

void Tick() {
    switch(state) { /* transitions */
        case START:
            state = BOTHUP;
            break;
        case BOTHUP:
            if ((PINA & 0x03) == 0x03) {
                state = BOTHDOWN;
                PORTC = 0;
            }
            else if ((PINA & 0x02) == 0x02) {
                state = PRESS1;
                if (PORTC > 0) {
                    PORTC -= 1;
                }
            }
            else if ((PINA & 0x01) == 0x01) {
                state = PRESS0;
                if (PORTC < 9) {
                    PORTC += 1;
                }
            }
            break;
        case PRESS0:
            if ((PINA & 0x03) == 0x03) {
                state = BOTHDOWN;
                PORTC = 0;
            }
            else if ((PINA & 0x02) == 0x02) {
                state = PRESS1;
                if (PORTC > 0) {
                    PORTC -= 1;
                } 
            }
            else if ((PINA & 0x01) == 0x00) {
                state = BOTHUP; 
            }
            break;
        case PRESS1:
            if ((PINA & 0x03) == 0x03) { 
                state = BOTHDOWN;
                PORTC = 0;
            }
            else if ((PINA & 0x01) == 0x01) {
                state = PRESS0;
                if (PORTC < 9) {
                    PORTC += 1;
                }
            }
            else if ((PINA & 0x02) == 0x00) {
                state = BOTHUP;
            }
            break;
        case BOTHDOWN:
            if ((PINA & 0x03) == 0x00) {
                state = BOTHUP;
            }
            else if ((PINA & 0x02) == 0x02) {
                state = PRESS1;
            }
            else if ((PINA & 0x01) == 0x01) {
                state = PRESS0;
            }
            break;
    }
}

int main(void) {
    PORTC = 7;
    while(1) {
        Tick();
    }
    return 0;
}
