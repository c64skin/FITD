#ifndef _LIFE2_
#define _LIFE2_

void processLife2(int lifeNum);
void convertPaletteIfRequired(unsigned char* lpalette);

extern short int numObjInInventoryTable[2];
extern short int inHandTable[2];
extern short int currentInHand;

#endif