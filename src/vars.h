#ifndef _VARS_
#define _VARS_

#include "osystem.h"

struct definesStruct // warning ! Used to read data from a file. Alignement check required
{
	short int field_0;
	short int field_2;
	short int field_4;
	short int field_6;
	short int field_8;
	short int field_A;
	short int field_C;
	short int field_E;
	short int hero;
	short int field_12;
	short int field_14;
	short int field_16;
	short int field_18;
	short int field_1A;
	short int field_1C;
	short int field_1E;
	short int field_20;
	short int field_22;
	short int field_24;
	short int field_26;
	short int field_28;
	short int field_2A;
	short int field_2C;
	short int field_2E;
	short int field_30;
	short int field_32;
	short int field_34;
	short int field_36;
	short int field_38;
	short int field_3A;
	short int field_3C;
	short int field_3E;
	short int field_40;
	short int field_42;
	short int field_44;
	short int field_46;
	short int field_48;
	short int field_4A;
	short int field_4C;
	short int field_4E;
	short int field_50;
	short int field_52;
	short int field_54;
	short int field_56;
	short int field_58;
};

struct textEntryStruct
{
	short int index;
	char* textPtr;
	short int width;
};

struct regularTextEntryStruct
{
	char* textPtr;
	short int width;
};

struct hqrEntryStruct
{
	char string[10];
	short int maxFreeData;
	short int sizeFreeData;
	short int numMaxEntry;
	short int numUsedEntry;
	char* dataPtr;
};

struct hqrSubEntryStruct
{
	short int key;
	short int offset;
	short int size;
	short int field_6;
	short int field_8;
};

struct actorStruct
{
	short int field_0;
	short int bodyNum;
	short int flags;
	short int dynFlags;
	short int ZVX1;
	short int ZVX2;
	short int ZVY1;
	short int ZVY2;
	short int ZVZ1;
	short int ZVZ2;
	short int field_14;
	short int field_16;
	short int field_18;
	short int field_1A;
	short int x;
	short int y;
	short int z;
	short int field_22;
	short int field_24;
	short int field_26;
	short int alpha;
	short int beta;
	short int gamma;
	short int stage;
	short int room;
	short int lifeMode;
	short int life;
	short int field_36;
	short int field_38;
	unsigned int chronoStructure;
	short int hitBy;
	short int field_40;
	short int field_42;
	short int field_44;
	short int field_46;
	short int field_48;
	short int endAnim;
	short int field_4C;
	short int frame;
	short int anim;
	short int field_52;
	short int trackNumber;
	short int body;
	short int positionInTrack;
	short int field_5A;
	short int field_5C;
	short int field_5E;
	short int field_60;
	short int field_62;
	short int field_64;
	short int field_66;
	short int falling;
	short int field_6A;
	short int field_6C;
	short int field_6E;
	short int field_70;
	short int field_72;
	short int speed;
	short int field_76;
	short int field_78;
	short int field_7A;
	short int field_7C;
	short int field_7E;
	short int field_80;
	short int field_82;
	short int field_84;
	short int col;
	short int hardDec;
	short int hardCol;
	short int hit;
	short int field_8E;
	short int field_90;
	short int field_92;
	short int field_94;
	short int hitForce;
	short int field_98;
	short int field_9A;
	short int field_9C;
	short int field_9E;
};

struct objectStruct
{
	int ownerIdx;
	short int flags;
	short int field_6;
	short int foundBody;
	short int foundName;
	short int flags2;
	short int foundLife;
	short int pos1;
	short int pos2;
	short int pos3;
	short int pos4;
	short int pos5;
	short int pos6;
	short int pos7;
	short int pos8;
	short int field_20;
	short int field_22;
	short int field_24;
	short int field_26;
	short int field_28;
	short int field_2A;
	short int field_2C;
	short int field_2E;
	short int field_30;
	short int foundWeight;
};

extern hqrEntryStruct* hqrUnk;

extern int videoMode;
extern int musicConfigured;
extern int initAllVar2;

extern char* aux;
extern char* aux2;
extern char* bufferAnim;

extern char* screen;

extern int screenBufferSize;
extern int unkScreenVar2;

extern definesStruct defines;

extern char* priority;

extern char* fontData;

extern char* aitdBoxGfx;

extern char palette[0x300];

extern OSystem osystem;

extern char unkScreenVar[320*200];

extern char rgbaBuffer[320*200*4];

extern volatile int timer;

extern int currentMenuLeft;
extern int currentMenuTop;
extern int currentMenuRight;
extern int currentMenuBottom;

extern char* fontVar1;
extern short int currentFontColor;
extern char* fontVar4;
extern char* fontVar5;
extern short int fontVar6;

extern short int fontSm1;
extern short int fontSm2;
extern short int fontSm3;
extern short int fontSm4;
extern short int fontSm5;
extern short int fontSm6;
extern short int fontSm7;
extern short int fontSm8;
extern short int fontSm9;

extern textEntryStruct* tabTextes;
extern char* systemTextes;

extern char inputKey;
extern char input1;
extern char input2;
extern char input3;
extern char input4;
extern char joy;

extern char languageNameString[];

extern regularTextEntryStruct textTable[40];

extern int readVar;

extern int hqrKeyGen;

extern char* screenSm1;
extern char* screenSm2;
extern char* screenSm3;
extern char* screenSm4;
extern char* screenSm5;

extern actorStruct actorTable[50];

extern int currentCameraTarget;

extern int fileSize;

extern hqrEntryStruct* listBody;
extern hqrEntryStruct* listAnim;
extern hqrEntryStruct* listLife;
extern hqrEntryStruct* listTrack;

extern short int maxObjects;

extern objectStruct objectTable[300]; // may be less

extern short int* vars;

extern int varSize;

#endif