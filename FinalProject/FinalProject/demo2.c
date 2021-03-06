/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Final Project
 *	Exercise Description: Trench Run
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/7duukklIm7U
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "sprite.h"
#include "io.h"
#include "timer.h"
#include "pwm.h"

/* ADC Functions */
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned char RNGBoolean(unsigned char prob) {
	return ((ADC % 10) < prob) ? 1 : 0;
}

unsigned char RNGRange(unsigned char low, unsigned char high) {
	return (ADC % (high - low + 1)) + low;
}

/* begin scheduler code */
typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

/* helper functions */
void printBinary(unsigned char data, unsigned char pos) {
	for (unsigned char i = 0; i < 8; i++) {
		LCD_Cursor(pos + i);
		if (((data << i) & 0x80) == 0x80) {
			LCD_WriteData('1');
		} else {
			LCD_WriteData('0');
		}
	}
}

void printDec(unsigned char data, unsigned char pos) {
	LCD_Cursor(pos);
	LCD_WriteData((data / 100) + 48);
	LCD_Cursor(pos + 1);
	LCD_WriteData(((data % 100) / 10) + 48);
	LCD_Cursor(pos + 2);
	LCD_WriteData((data % 10) + 48);
}

void printShort(unsigned short data, unsigned char pos) {
	LCD_Cursor(pos);
	LCD_WriteData((data / 10000) + 48);
	LCD_Cursor(pos + 1);
	LCD_WriteData(((data % 10000) / 1000) + 48);
	LCD_Cursor(pos + 2);
	LCD_WriteData(((data % 1000) / 100) + 48);
	LCD_Cursor(pos + 3);
	LCD_WriteData(((data % 100) / 10) + 48);
	LCD_Cursor(pos + 4);
	LCD_WriteData((data % 10) + 48);
}

unsigned char GetBit(unsigned char var, unsigned char pos) {
	return ((var & (1 << pos)) != 0) ? 1 : 0;
}

unsigned short power(unsigned short val, unsigned short exp) {
	unsigned short ret = val;
	
	if (exp == 0) {
		return 1;
	}
	
	for (unsigned int i = 0; i < exp; i++) {
		ret *= val;
	}
	return ret;
}

/* B0 = Data  (IN)
 * B1 = Latch (OUT)
 * B2 = Clock (OUT)
 *
 * Bit order:
 * |A|B|Select|Start|Up|Down|Left|Right|
*/
unsigned char getController() {
	unsigned char data = 0x00;
	
	PORTB &= 0xFD; //pulse latch to store button states
	delay_ms(1);
	PORTB |= 0x02;
	delay_ms(1);
	PORTB &= 0xFD;
	
	for (unsigned char i = 0; i < 8; i++) {
		data = (data << 1);
		
		if ((PINB & 0x01) == 0x00) { //active low
			data |= 0x01;
		}
		
		PORTB &= 0xFB; //pulse clock
		delay_ms(1);
		PORTB |= 0x04;
		delay_ms(1);
		PORTB &= 0xFB;
	}
	
	return data;
}

/* Tasks */
/* Poll Controller */
enum PollController_States {PC_GET};
unsigned char controllerInput; 

int Tick_PollController(int state) {
	switch(state) {
		case PC_GET:
			controllerInput = getController();
			break;
		default:
			state = PC_GET;
			break;
	}
	
	return state;
}

/* Move Objects */
enum MoveObjects_States {MO_INIT, MO_MOVE};
gameObj sprites[8] = {{0}};

int Tick_MoveObjects(int state) {
	static unsigned char loop = 0;
	
	switch(state) {
		case MO_MOVE:
			for (unsigned int i = 0; i < (sizeof(sprites) / sizeof(gameObj)); i++) {
				if (sprites[i].spriteID == XWING) {
					if (GetBit(controllerInput, 3) == 1) {
						moveY(&sprites[i], 1);
					} 
					else if (GetBit(controllerInput, 2) == 1) {
						moveY(&sprites[i], -1);
					}
				}
				else if (sprites[i].fixedX == 0 && sprites[i].show == 1) {
					switch(sprites[i].spriteID) {
						case TURRET:
						case PORT:
							if ((loop % 4) == 0) {
								moveX(&sprites[i], -1);
							}
							break;
						case TIE:
						case LTURRET:
							if ((loop % 2) == 0) {
								moveX(&sprites[i], -1);
							}
							break;
						case LTIE:
							moveX(&sprites[i], -1);
							break;
						case LPLAYER:
							if ((loop % 2) == 0) {
								moveX(&sprites[i], 1);
							}
							break;
					}
				}
			}
			break;
		default:
			state = MO_MOVE;
			break;
	}
	
	loop = (loop < 7) ? loop + 1 : 0;
	
	return state;
}

/* Draw Objects */
enum DrawObjects_States {DO_DRAW};

int Tick_DrawObjects(int state) {
	switch(state) {
		case DO_DRAW:
			for (unsigned int i = 0; i < (sizeof(sprites) / sizeof(gameObj)); i++) {
				if (sprites[i].show == 1) {
					draw(sprites[i]);
				}	
			}
			LCD_Cursor(0); //hide cursor
			break;
		default:
			state = DO_DRAW;
			break;
	}
	
	return state;
}

/* Spawn Enemies */
enum Spawn_States {S_SPAWN};
unsigned char ammo = 10;
	
int Tick_Spawn(int state) {
	PORTA &= 0x01;
	PORTA |= ((power(2, ammo) - 1) & 0xFE);
	
	PORTD &= 0xF8;
	PORTD |= (((power(2, (unsigned short)ammo) - 1) >> 8) & 0x07);
	
	switch(state) {
		case S_SPAWN:
			if ((ADC % 5) == 1 && sprites[1].show == 0) { //TIE Fighter
				sprites[1].show = 1;
				sprites[1].x = 16;
				sprites[1].y = 0;
				sprites[1].subY = RNGRange(0, sprites[1].subYMax);
				loadCustomChar(sprites[1].table[sprites[1].subY], sprites[1].spriteID);
			}
			if (GetBit(controllerInput, 7) == 1 && sprites[2].show == 0 && ammo > 0) { //Player Laser
				sprites[2].show = 1;
				sprites[2].x = 4;
				sprites[2].y = sprites[0].y;
				sprites[2].subY = sprites[0].subY;
				loadCustomChar(sprites[2].table[sprites[2].subY], sprites[2].spriteID);
				ammo--;
			}
			if ((ADC % 5) == 2 && sprites[6].show == 0) { //Turret
				sprites[6].show = 1;
				sprites[6].x = 16;
				sprites[6].y = 1;
			}
			break;
		default:
			state = S_SPAWN;
			break;
	}
	
	return state;
}

/* Do Collisions */
enum Coll_States {DC_COLL};
	
unsigned char areColliding(gameObj a, gameObj b) {
	if (a.spriteID == LPLAYER) {
		if ((a.x >= b.x) && (b.x != 0) && (a.y == b.y) && (b.spriteID != XWING)) {
			return 1;
		}
	}

	return ((a.x == b.x) && (a.y == b.y) && (a.show == 1) && (b.show == 1) && (a.x != 0)) ? 1 : 0;
}

void killSprite(gameObj *a) {
	LCD_Cursor(a->x + (a->y * 16));
	LCD_WriteData(' ');
	LCD_Cursor(0);
	
	a->x = 0;
	a->show = 0;	
}
	
int Tick_Collisions(int state) {
	switch(state) { /* actions */
		case DC_COLL:
			for (unsigned char i = 0; i < 8; i++) {
				for (unsigned char j = 0; j < 8; j++) {
					if ((i != j) && areColliding(sprites[i], sprites[j]) == 1) {
						killSprite(&sprites[i]);
						killSprite(&sprites[j]);
					}
				}
			}
			break;
		default:
			break;
	}
	
	switch(state) { /* transitions */
		case DC_COLL:
			break;
		default:
			state = DC_COLL;
			break;
	}
	
	return state;
}

int main(void) {
	DDRA = 0xFE; //ADC and LED bar
	DDRB = 0x46; PORTB = 0; //controller and speaker
    DDRC = 0xFF; PORTC = 0;//LCD
	DDRD = 0xFF; PORTD = 0;//LCD
	
	ADC_init();
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	static task PollController, MoveObjects, DrawObjects, Spawn, DoCollisions;
	task *tasks[] = {&PollController, &MoveObjects, &Spawn, &DrawObjects, &DoCollisions};
	const unsigned short numTasks = 5;
	
	const char start = -1;
	
	PollController.state = start;
	PollController.period = 50;
	PollController.elapsedTime = PollController.period;
	PollController.TickFct = &Tick_PollController;
	
	MoveObjects.state = start;
	MoveObjects.period = 50;
	MoveObjects.elapsedTime = MoveObjects.period;
	MoveObjects.TickFct = &Tick_MoveObjects;
	
	DrawObjects.state = start;
	DrawObjects.period = 100;
	DrawObjects.elapsedTime = DrawObjects.period;
	DrawObjects.TickFct = &Tick_DrawObjects;
	
	Spawn.state = start;
	Spawn.period = 100;
	Spawn.elapsedTime = Spawn.period;
	Spawn.TickFct = &Tick_Spawn;
	
	DoCollisions.state = start;
	DoCollisions.period = 50;
	DoCollisions.elapsedTime = DoCollisions.period;
	DoCollisions.TickFct = &Tick_Collisions;
	
	const unsigned short GCD = 50;
	
	TimerSet(GCD);
	TimerOn();

	LCD_init();
	LCD_ClearScreen();
	
	loadSpriteTable(spriteTable, 8);
	
	initSprites(sprites);
	
    while (1) {
		for (unsigned short i = 0; i < numTasks; i++) {
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

