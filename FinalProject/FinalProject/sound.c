/*
 * sound.c
 *
 * Created: 11/30/2021 10:36:50 AM
 *  Author: Merrick
 */ 

#include "sound.h"

float frequencies[16] = {261.6, 293.7, 329.6, 370.0, 392.0, 440.0, 493.9, 523.3, 587.3, 659.3, 740.0, 784.0, 1500.0, 2000.0, 50.0, 100.0, 30000.0};

unsigned char mainThemeNotes[16] = {G4, D5, C5, B4, A4, G5, D5, C5, B4, A4, G5, D5, C5, B4, C5, A4};
/* triplet = 2, eighth = 3, quarter = 6, half = 12, whole = 24 */
unsigned char mainThemeLens[16] = {12, 12, 2, 2, 2, 12, 6, 2, 2, 2, 12, 6, 2, 2, 2, 12};

audio mainTheme = {
	mainThemeNotes,
	mainThemeLens,
	16
};

unsigned char marchNotes[25] = {E4, OFF, E4, OFF, E4, OFF, C4, G4, E4, C4, G4, E4, B4, OFF, B4, OFF, B4, OFF, C5, G4, E4, C4, G4, E4, OFF};
unsigned char marchLens[25] = {5, 1, 5, 1, 5, 1, 3, 3, 6, 3, 3, 12, 5, 1, 5, 1, 5, 1, 3, 3, 6, 3, 3, 12, 1};

audio impMarch = {
	marchNotes,
	marchLens,
	25
};

unsigned char laserNotes[2] = {L1, L0};
unsigned char laserLens[2] = {1, 1};
	
audio laserBlast = {
	laserNotes,
	laserLens,
	2
};

unsigned char explNotes[2] = {E1, E0};
unsigned char explLens[2] = {1, 1};
	
audio explosion = {
	explNotes,
	explLens,
	2
};