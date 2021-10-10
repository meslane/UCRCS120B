#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

typedef enum {START, LOCKED, UNLOCKED}State;
State state = START;

unsigned char code[4] = {4, 1, 2, 1}; /* 4 1 2 1 */
unsigned char ix = 0;

void Tick() {   
     static unsigned char prevInput = 0x00;     

     switch(state) { /* transitions */
        case START:
            state = LOCKED;
            break;
        case LOCKED:
            if (PINA != 0x00 && PINA != prevInput && PINA == code[ix]) {
                if (ix < 3) {
                    ix++;
                }
                else if (ix == 3) {
                    state = UNLOCKED;
                    ix = 0;
                }
            }
            else if (PINA != 0x00 && PINA != prevInput && PINA != code[ix]) {
                ix = 0; /* reset if failed */
            }
            break;
        case UNLOCKED:
            if (PINA != 0x00 && PINA != prevInput && PINA == code[ix]) {
                if (ix < 3) {
                    ix++;
                }
                else if (ix == 3) {
                    state = LOCKED;
                    ix = 0;
                }
            }
            else if (PINA == 0x80) {
                state = LOCKED;
                ix = 0;
            }
            else if (PINA != 0x00 && PINA != prevInput && PINA != code[ix]) {
                ix = 0; /* reset if failed */
            }
            break;
    }

    switch(state) {
        case START:
        case LOCKED:
            PORTB = 0;
            break;
        case UNLOCKED:
            PORTB = 1;
            break;
    }

    prevInput = PINA;
}

int main(void) {
    while(1) {
        Tick();
        PORTC = state;
    }
    return 0;
}
