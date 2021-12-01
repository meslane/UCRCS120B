/*
 * sound.h
 *
 * Created: 11/30/2021 10:36:50 AM
 *  Author: Merrick
 */ 

#ifndef __sound_h__
#define __sound_h__

enum noteNames{C4, D4, E4, FS4, G4, A4, B4, C5, D5, E5, FS5, G5, L0, L1, E0, E1, OFF};

extern float frequencies[16];

typedef struct audioSample {
	unsigned char* notes;
	unsigned char* noteLen;	
	unsigned short length;
}audio;

extern audio mainTheme;
extern audio impMarch;
extern audio laserBlast;
extern audio explosion;

#endif