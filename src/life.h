#ifndef _LIFE_
#define _LIFE_

void processLife(int lifeNum);
int createFlow( int mode, int X, int Y, int Z, int stage, int room, int alpha, int beta, int gamma, ZVStruct* zvPtr);
void animMove(int a,int b,int c,int d,int e,int f,int g);
void doRealZv(actorStruct* actorPtr);
void setStage(int newStage, int newRoomLocal, int X, int Y, int Z);
void fire(int fireAnim, int X, int Y, int Z, int hitForce, int nextAnim);
void put(int x,int y,int z,int room,int stage,int alpha,int beta,int gamma,int idx);
void getHardClip();
void throwObj(int animThrow, int frameThrow, int arg_4, int objToThrowIdx, int throwRotated, int throwForce, int animNext);

#endif
