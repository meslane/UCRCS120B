/*
 * timer.h
 *
 * Created: 11/19/2021 6:04:07 PM
 *  Author: Merrick
 */ 

#ifndef __timer_h__
#define __timer_h__

extern volatile unsigned char TimerFlag;

extern unsigned long _avr_timer_M;
extern unsigned long _avr_timer_cntcurr;

void TimerOn();
void TimerOff();
void TimerISR();
//ISR(TIMER1_COMPA_vect);
void TimerSet(unsigned long M);

#endif