#include "common.h"

hqrEntryStruct* hqrUnk;

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
char input1 = 0;
char input2 = 0;
char input3;
char input4;
char joy;

char languageNameString[] = "FRANCAIS";

regularTextEntryStruct textTable[40];

int readVar;

int hqrKeyGen = 0;

char* screenSm1;
char* screenSm2;
char* screenSm3;
char* screenSm4;
char* screenSm5;

actorStruct actorTable[50];

int currentCameraTarget;

int fileSize;

hqrEntryStruct* listBody;
hqrEntryStruct* listAnim;
hqrEntryStruct* listLife;
hqrEntryStruct* listTrack;

short int maxObjects;

objectStruct objectTable[300]; // may be less

short int* vars;

int varSize;

char* messageVar1[5]; // may be bigger

int currentMusic;
int found;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct *genVar1;
boxStruct *genVar3;

int genVar5;
int genVar6;
int genVar7;
int genVar8;
int genVar9;
int giveUp;
int inHand;
int lightVar1;
int lightVar2;
int numObjInInventory;
int soundVar1;
int soundVar2;
int statusScreenAllowed;

char* etageVar0 = NULL;
char* etageVar1 = NULL;

int changeFloor;
int currentCamera;
int currentEtage;
int needChangeRoom;

char* cameraPtr;
int currentDisplayedRoom;
int mainVar1;
int roomVar0;
int roomVar1;
char* roomVar2;
int roomVar3;
char* roomVar4;
char* roomVar5[15];
short int roomVar6[15];
int startGameVar1;

int transformX;
int transformY;
int transformZ;
int transformXCos;
int transformXSin;
int transformYCos;
int transformYSin;
int transformZCos;
int transformZSin;
bool transfromUseX;
bool transfromUseY;
bool transfromUseZ;

int translateX;
int translateY;
int translateZ;

int cameraCenterX;
int cameraCenterY;
int cameraX;
int cameraY;
int cameraZ;

char cameraDataTab[30];

