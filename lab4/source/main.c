#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

typedef enum {START}State;
State state = START;

void Tick() {
    switch(state) { /* transitions */
        case START:
            break;
    }

    switch(state) { /* actions */
        case START:
            break;
    }
}

int main(void) {
    while(1) {
        Tick();
    }
    return 0;
}
