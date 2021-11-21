/*
 * sprite.h
 *
 * Created: 11/20/2021 12:35:33 PM
 *  Author: Merrick
 */ 


#ifndef __sprite_h__
#define __sprite_h__

enum spriteIDs{XWING, TIE, LPLAYER, LTIE, TORP, LTURRET, TURRET, PORT};

typedef struct GameObj {
	unsigned char x;
	unsigned char y;
	unsigned char subY; //for motion within a cell
	unsigned char subYMax;
	unsigned char show; //show or not (boolean)
	unsigned char fixedX; //move x every tick or not (boolean)
	
	unsigned char spriteID; //index in sprite table
	unsigned char (*table)[8]; //reference to own table
}gameObj;

extern unsigned char spriteTable[8][8];

extern unsigned char xWingSprites[8][8];
extern unsigned char tieSprites[8][8];
extern unsigned char laserSprites[8][8]; 
extern unsigned char tlSprites[8][8];
extern unsigned char torpedoSprites[8][8];

void loadCustomChar(unsigned char* data, unsigned char index);
void loadSpriteTable(unsigned char table[][8], unsigned char tableSize);
void printAllSprites(unsigned char pos);

void initSprites(gameObj sprites[8]);

void moveY(gameObj *o, int dir);
void moveX(gameObj *o, int dir);
void draw(gameObj o);

#endif