#ifndef _MAIN_
#define _MAIN_

void sysInit(void);
void freeAll(void);
void startGame(int startupEtage, int startupRoom, int allowSystemMenu);
void fillBox(int x1, int y1, int x2, int y2, char color); // fast recode. No RE


///
int printText(int index, int left, int top, int right, int bottom, int mode, int color);

#endif
