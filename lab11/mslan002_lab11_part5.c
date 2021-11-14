/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: Keypad + LCD game
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/G2s-TLaCq1Y
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>

/* helpers */
unsigned char GetBit(unsigned char var, unsigned char pos) {
	return ((var & (1 << pos)) != 0) ? 1 : 0;
}

/* col 1 2 3 4
 *   |________
 *  1| 1 2 3 A
 *  2| 4 5 6 B
 *  3| 7 8 9 C
 *  4| * 0 # D
*/
unsigned char GetKeypadKey() {
	PORTC = 0xEF; //column 1
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('1');}
	if (GetBit(PINC, 1) == 0) {return('4');}
	if (GetBit(PINC, 2) == 0) {return('7');}
	if (GetBit(PINC, 3) == 0) {return('*');}
		
	PORTC = 0xDF; //column 2
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('2');}
	if (GetBit(PINC, 1) == 0) {return('5');}
	if (GetBit(PINC, 2) == 0) {return('8');}
	if (GetBit(PINC, 3) == 0) {return('0');}
	
	PORTC = 0xBF; //column 3
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('3');}
	if (GetBit(PINC, 1) == 0) {return('6');}
	if (GetBit(PINC, 2) == 0) {return('9');}
	if (GetBit(PINC, 3) == 0) {return('#');}

	PORTC = 0x7f; //column 4
	asm("nop");
	if (GetBit(PINC, 0) == 0) {return('A');}
	if (GetBit(PINC, 1) == 0) {return('B');}
	if (GetBit(PINC, 2) == 0) {return('C');}
	if (GetBit(PINC, 3) == 0) {return('D');}
	
	return('\0');
}

/* timer code */
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A  = 125;
	TIMSK1 = 0x02;
	TCNT1  = 0x00;
	
	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
/* end timer code */

/* begin scheduler code */
typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;
/* end scheduler code */

/* begin pause task */
enum Pause_States {Pause_START, Pause_PRESS, Pause_REL};
unsigned char paused = 0;

int Tick_Pause(int state) {
	switch(state) {
		case Pause_REL:
			if (GetKeypadKey() == '#') {
				paused = (paused == 0) ? 1 : 0;
				state = Pause_PRESS;
			}
			break;
		case Pause_PRESS:
			if (GetKeypadKey() == '\0') {
				state = Pause_REL;
			}
			break;
		default:
			state = Pause_REL;
			break;
	}
	
	return state;
}
/* end pause task */

/* begin move task */
enum Move_States {Move_START, Move_IDLE};
unsigned char row = 0; //0 = top, 1 = bottom
unsigned char endgame = 0;

int Tick_Move(int state) {
	switch(state) {
		case Move_IDLE:
			if (paused == 0 && endgame == 0) {
				if ((GetKeypadKey() == 'A') && row == 1) {
					LCD_Cursor(17);
					LCD_WriteData(' ');
					LCD_Cursor(1);
					LCD_WriteData('P');
					LCD_Cursor(0);
					
					row = 0;
				}
				else if ((GetKeypadKey() == 'B') && row == 0) {
					LCD_Cursor(1);
					LCD_WriteData(' ');
					LCD_Cursor(17);
					LCD_WriteData('P');
					LCD_Cursor(0);
					
					row = 1;
				}
			}
		default:
			state = Move_IDLE;
			break;
	}
	
	return state;
}
/* end move task */

/* begin obstacle task */
enum Obs_States {Obs_START, Obs_IDLE, Obs_ENDGAME, Obs_WAIT};
unsigned char o1Pos = 25;
unsigned char o2Pos = 16;

int Tick_Obs(int state) {
	switch(state) {
		case Obs_IDLE:
			if (paused == 0) {
				o1Pos = (o1Pos > 17) ? (o1Pos - 1) : 32;
				o2Pos = (o2Pos > 1) ? (o2Pos - 1) : 16;
				
				if (o1Pos != 32) {
					LCD_Cursor(o1Pos + 1);
					LCD_WriteData(' ');
				} else {
					LCD_Cursor(17);
					LCD_WriteData(' ');
				}
				LCD_Cursor(o1Pos);
				LCD_WriteData('#');
				LCD_Cursor(0);
				
				if (o2Pos != 16) {
					LCD_Cursor(o2Pos + 1);
					LCD_WriteData(' ');
				} else {
					LCD_Cursor(1);
					LCD_WriteData(' ');
				}
				LCD_Cursor(o2Pos);
				LCD_WriteData('#');
				LCD_Cursor(0);
				
				if (((o1Pos == 17) && (row == 1)) || ((o2Pos == 1) && (row == 0))) {
					LCD_ClearScreen();
					LCD_DisplayString(1, "Game Over");
					endgame = 1;
					state = Obs_ENDGAME;
				}
				
			}
			break;
		case Obs_ENDGAME:
			if (paused == 1) {
				o1Pos = 25;
				o2Pos = 16;
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData('P');
				LCD_Cursor(0);
				row = 0;
				paused = 0;
				endgame = 0;
				state = Obs_IDLE;
			}
			break;
		default:
			state = Obs_IDLE;
			break;
	}
	
	return state;
}
/* begin obstacle task */

int main(void){
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF;
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	
	static task Pause, Move, Obs;
	task *tasks[] = {&Pause, &Move, &Obs};
	const unsigned short numTasks = 3;
	
	const char start = -1;
	
	//pause task
	Pause.state = start;
	Pause.period = 50;
	Pause.elapsedTime = Pause.period;
	Pause.TickFct = &Tick_Pause;
	
	//move task
	Move.state = start;
	Move.period = 50;
	Move.elapsedTime = Move.period;
	Move.TickFct = &Tick_Move;
	
	//obs task
	Obs.state = start;
	Obs.period = 100;
	Obs.elapsedTime = Obs.period;
	Obs.TickFct = &Tick_Obs;
	
	const unsigned short GCD = 50;
	
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData('P');
	LCD_Cursor(0);

	unsigned short i;
    while (1) {
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime >= tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		
	
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}