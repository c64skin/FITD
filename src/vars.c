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
char button;

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

short int currentCameraTarget;

int fileSize;

hqrEntryStruct* listBody;
hqrEntryStruct* listAnim;
hqrEntryStruct* listLife;
hqrEntryStruct* listTrack;

short int maxObjects;

objectStruct objectTable[300]; // may be less

short int* vars;

int varSize;

messageStruct messageTable[5];

short int currentMusic;
int action;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct *genVar1;
boxStruct *genVar3;

int genVar5;
int genVar6;
int genVar7;
int genVar8;
short int genVar9;
short int giveUp;
short int inHand;
short int lightVar1;
int lightVar2;
short int numObjInInventory;
int soundVar1;
int soundVar2;
short int statusScreenAllowed;

char* etageVar0 = NULL;
char* etageVar1 = NULL;

int changeFloor;
short int currentCamera;
short int currentEtage;
int needChangeRoom;

char* cameraPtr;
short int currentDisplayedRoom;
int mainVar1;
int numCameraInRoom;
int numCameraZone;
char* cameraZoneData;
int numRoomZone;
char* roomZoneData;
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
bool transformUseX;
bool transformUseY;
bool transformUseZ;

int translateX;
int translateY;
int translateZ;

int cameraCenterX;
int cameraCenterY;
int cameraX;
int cameraY;
int cameraZ;

char cameraDataTab[30];

int actorTurnedToObj = 0;

int currentProcessedActorIdx;
actorStruct* currentProcessedActorPtr;

int currentLifeActorIdx;
actorStruct* currentLifeActorPtr;
int currentLifeNum;

char* currentLifePtr;

int setupCameraVar1;

#ifdef USE_GL
float renderPointList[6400];
#else
short int renderPointList[6400];
#endif

int numActorInList;
int sortedActorTable[50];

int angleCompX;
int angleCompZ;
int angleCompBeta;

int bufferAnimCounter = 0;

int animCurrentTime;
int animKeyframeLength;
int animMoveX;
int animMoveY;
int animMoveZ;
int animRot1;
int animRot2;
int animRot3;
char* animVar1;
char* animVar3;
char* animVar4;

short int newFloor;

int paletteVar;

char cameraBuffer[512];

short int newRoom;

short int inventory[30];

short int shakeVar1;
short int shakingAmplitude;
unsigned int timerFreeze1;

char* hardColTable[30];

short int hardColVar1;
short int hardColVar2;

short int hardClipX1;
short int hardClipX2;
short int hardClipY1;
short int hardClipY2;
short int hardClipZ1;
short int hardClipZ2;

char* listBodySelect[] = {
	"LISTBODY",
	"LISTBOD2",
};

char* listAnimSelect[] = {
	"LISTANIM",
	"LISTANI2",
};

saveEntry saveTable[] = {
	{ &currentDisplayedRoom, 2 },
	{ &currentEtage, 2 },
	{ &currentCamera, 2 },
	{ &currentCameraTarget, 2 },
	{ &genVar9, 2 },
	{ &maxObjects, 2 },
	{ objectTable, 15600 },
	{ &defines, 90 },
	{ &inHand, 2 },
	{ &numObjInInventory, 2 },
	{ inventory, 60 },
	{ &statusScreenAllowed, 2 },
	{ &giveUp, 2 },
	{ &lightVar1, 2 },
	{ &shakingAmplitude, 2 },
	{ &shakeVar1, 2 },
	{ (void*)timer, 4 },
	{ &timerFreeze1, 4},
	{ &currentMusic, 2},
	{ NULL, 0 },
};
