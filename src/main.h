#ifndef _MAIN_
#define _MAIN_

extern char scaledScreen[640*400];

void sysInit(void);
void freeAll(void);
void startGame(int startupEtage, int startupRoom, int allowSystemMenu);
void fillBox(int x1, int y1, int x2, int y2, char color); // fast recode. No RE



///
int printText(int index, int left, int top, int right, int bottom, int mode, int color);

void setupSMCode(int centerX, int centerY, int x, int y, int z);
void setupPointTransformSM(int x, int y, int z);
void setupSelfModifyingCode(int x, int y, int z);
short int computeDistanceToPoint(int x1, int z1, int x2, int z2);
void setMoveMode(int trackMode, int trackNumber);
void startActorRotation(short int beta, short int newBeta, short int param, rotateStruct* rotatePtr);
short int updateActorRotation(rotateStruct* rotatePtr);
int anim(int animNum,int arg_2, int arg_4);
short int getAnimParam(char* animPtr);
int initAnimInBody(int frame, char* anim, char* body);
void deleteObject(int objIdx);
void deleteSub(int actorIdx);
void stopAnim(int actorIdx);
void makeMessage(int messageIdx);
int drawTextOverlay(void);

#endif
