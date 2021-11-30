/*
 * sound.c
 *
 * Created: 11/30/2021 10:36:50 AM
 *  Author: Merrick
 */ 

#include "sound.h"

float frequencies[12] = {392.0, 440.0, 493.9, 523.3, 587.3, 659.3, 740.0, 784.0, 1500.0, 2000.0, 50.0, 100.0};

unsigned char mainThemeNotes[16] = {G4, D5, C5, B4, A4, G5, D5, C5, B4, A4, G5, D5, C5, B4, C5, A4};
/* triplet = 1, quarter = 3, half = 6, whole = 12 */
unsigned char mainThemeLens[16] = {12, 12, 2, 2, 2, 12, 6, 2, 2, 2, 12, 6, 2, 2, 2, 12};

audio mainTheme = {
	mainThemeNotes,
	mainThemeLens,
	16
};

unsigned char laserNotes[2] = {9, 8};
unsigned char laserLens[2] = {1, 1};
	
audio laserBlast = {
	laserNotes,
	laserLens,
	2
};

unsigned char explNotes[2] = {11, 10};
unsigned char explLens[2] = {1, 1};
	
audio explosion = {
	explNotes,
	explLens,
	2
};