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

#endif