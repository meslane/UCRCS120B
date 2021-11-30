/*
 * sound.h
 *
 * Created: 11/30/2021 10:36:50 AM
 *  Author: Merrick
 */ 

#ifndef __sound_h__
#define __sound_h__

enum noteNames{G4, A4, B4, C5, D5, E5, FS5, G5};

extern float frequencies[12];

typedef struct audioSample {
	unsigned char* notes;
	unsigned char* noteLen;	
	unsigned short length;
}audio;

extern audio mainTheme;
extern audio laserBlast;
extern audio explosion;

#endif