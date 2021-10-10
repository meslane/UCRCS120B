#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

typedef enum {START, LOCKED, PRESSPOUND, RELPOUND, UNLOCKED}State;
State state = START;

void Tick() {
     switch(state) { /* transitions */
        case START:
            state = LOCKED;
            break;
        case LOCKED:
            if ((PINA & 0x07) == 0x04) { /* press pound */
                state = PRESSPOUND;
            }
            else {
                state = LOCKED;
            }
            break;
        case PRESSPOUND:
            if ((PINA & 0x04) == 0) { /* release pound */
                state = RELPOUND;
            }
            else if ((PINA & 0x07) != 0x04) {
                state = LOCKED;
            }
            break;
        case RELPOUND:
            if ((PINA & 0x07) == 0x02) { /* press Y */
                state = UNLOCKED;
            }
            else if ((PINA & 0x07) != 0x00) {
                state = LOCKED;
            }
            break;
        case UNLOCKED:
            if (PINA == 0x80) { /* press PA7 */
                state = LOCKED;
            }
            break;
    }

    switch(state) { /* actions */
        case START:
        case LOCKED:
        case PRESSPOUND:
        case RELPOUND:
            PORTB = 0x00; /* locked */
            break;
        case UNLOCKED:
            PORTB = 0x01; /* unlocked */
            break;
    }
}

int main(void) {
    while(1) {
        Tick();
        PORTC = state;
    }
    return 0;
}
