/*	Your Name & E-mail: Merrick Slane, mslan002@ucr.edu
 *	Lab Section: 023
 *	Assignment: Final Project
 *	Exercise Description: Trench Run
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://youtu.be/t1U1krjk3h0
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "io.h"
#include "timer.h"
#include "pwm.h"

#define ADCMAX 24
#define ADCMIN 3

volatile unsigned char TimerFlag;

unsigned long _avr_timer_M;
unsigned long _avr_timer_cntcurr;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

/* begin helper functions */
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

unsigned char RNGBoolean(unsigned char prob) {
	return ((ADC % 10) < prob) ? 1 : 0;
}
/* end helper functions */

int main(void) {
	DDRA = 0x00; //ADC
	DDRB = 0x46; PORTB = 0; //controller and speaker
    DDRC = 0xFF; PORTC = 0;//LCD
	DDRD = 0xFF; PORTD = 0;//LCD
	
	ADC_init();
	
	/* disable JTAG for PORTC */
	uint8_t tmp = 1<<JTD;
	MCUCR = tmp;
	MCUCR = tmp;
	
	LCD_init();
	LCD_ClearScreen();
	
	TimerSet(50);
	TimerOn();
	
	//PWM_on();
	set_PWM(1000);
	
	
	unsigned short ADCMax = 0;
	unsigned short ADCMin = 0xFFFF;
	
    while (1) {
		printBinary(getController(), 3);
		
		LCD_Cursor(32);
		if (RNGBoolean(2) == 1) {
			LCD_WriteData('R');
		} else {
			LCD_WriteData(' ');
		}
		
		if (ADC > ADCMax) {
			ADCMax = ADC;
		}
		if (ADC < ADCMin) {
			ADCMin = ADC;
		}
		
		printShort(ADCMax, 20);
		printShort(ADCMin, 26);
		
		printShort(ADC, 12);
		
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}

