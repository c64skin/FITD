#include "common.h"

int videoMode;
int musicConfigured;
int initAllVar2;

char* aux;
char* aux2;
char* bufferAnim;

char* screen;

int screenBufferSize;
int unkScreenVar2;

definesStruct defines;

char* priority;

char* fontData;

char* aitdBoxGfx;

char palette[256*3];

OSystem osystem;

char unkScreenVar[320*200];

char rgbaBuffer[320*200*4];

volatile int timer;

int currentMenuLeft;
int currentMenuTop;
int currentMenuRight;
int currentMenuBottom;

char* fontVar1 = NULL;
short int currentFontColor = 0;
char* fontVar4 = NULL;
char* fontVar5 = NULL;
short int fontVar6 = 0;

short int fontSm1 = 0;
short int fontSm2 = 0x1234;
short int fontSm3 = 18;
short int fontSm4 = 2;
short int fontSm5 = 1;
short int fontSm7 = 0x1234;
short int fontSm8 = 0x1234;
short int fontSm9 = 0x80;

textEntryStruct* tabTextes;
char* systemTextes;

char inputKey = 0;