#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

typedef enum {START, ON0, PRESS0, ON1, PRESS1}State;
State state = START;

void Tick() {
    switch(state) { /* transitions */
        case START:
            state = ON0;
            break;
        case ON0:
            if ((PINA & 0x01) == 0x01) {
                state = PRESS0;
            }
            break;
        case PRESS0:
            if ((PINA & 0x01) == 0x00) {
                state = ON1;
            }
            break;
        case ON1:
            if ((PINA & 0x01) == 0x01) {
                state = PRESS1;
            }
            break;
        case PRESS1:
            if ((PINA & 0x01) == 0x00) {
                state = ON0;
            }
            break;
    }

    switch(state) { /* actions */
        case START:
            break;
        case ON0:
            PORTB = 0x01;
            break;
        case PRESS0:
            PORTB = 0x02;
            break;
        case ON1:
            PORTB = 0x02;
            break;
        case PRESS1:
            PORTB = 0x01;
            break;
    }
}

int main(void) {
    while(1) {
        Tick();
    }
    return 0;
}
