#ifndef _VARS_
#define _VARS_

#include "osystem.h"

#pragma pack(1)

struct textEntryStruct
{
	short int index;
	char* textPtr;
	short int width;
};

struct messageStruct
{
	textEntryStruct* string;
	short int time;
};

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
	short int lightVar;
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

struct saveEntry
{
	void* ptr;
	unsigned int size;
};

struct regularTextEntryStruct
{
	char* textPtr;
	short int width;
};

struct hqrEntryStruct
{
	char string[10];
	unsigned short int maxFreeData;
	unsigned short int sizeFreeData;
	unsigned short int numMaxEntry;
	unsigned short int numUsedEntry;
	char* dataPtr;
};

struct hqrSubEntryStruct
{
	short int key;
	short int offset;
	short int size;
	unsigned int lastTimeUsed;
};

struct ZVStruct
{
	 short int ZVX1;
	 short int ZVX2;
	 short int ZVY1;
	 short int ZVY2;
	 short int ZVZ1;
	 short int ZVZ2;
};

struct rotateStruct
{
	short int oldAngle;
	short int newAngle;
	short int param;
	short int timeOfRotate;
};

struct actorStruct // used to read data from file too
{
	short int field_0;
	short int bodyNum;
	short int flags;
	short int dynFlags;
	ZVStruct zv;
	short int field_14;
	short int field_16;
	short int field_18;
	short int field_1A;
	short int roomX;
	short int roomY;
	short int roomZ;
	short int worldX;
	short int worldY;
	short int worldZ;
	short int alpha;
	short int beta;
	short int gamma;
	short int stage;
	short int room;
	short int lifeMode;
	short int life;
	unsigned int CHRONO;
	unsigned int ROOM_CHRONO;
	short int ANIM;
	short int field_40;
	short int field_42;
	short int field_44;
	short int field_46;
	short int field_48;
	short int FRAME;
	short int field_4C;
	short int END_FRAME;
	short int END_ANIM;
	short int trackMode;
	short int trackNumber;
	short int MARK;
	short int positionInTrack;

	short int modX;
	short int modY;
	short int modZ;

	rotateStruct field_60;
	short int falling;
	rotateStruct rotate;
	short int field_72;
	short int speed;
	rotateStruct speedChange;
	short int COL[3];
	short int COL_BY;
	short int HARD_DEC;
	short int HARD_COL;
	short int HIT;
	short int HIT_BY;
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
	short int ownerIdx;
	short int field_2;
	short int flags;
	short int field_6;
	short int foundBody;
	short int foundName;
	short int flags2;
	short int foundLife;
	short int x;
	short int y;
	short int z;
	short int alpha;
	short int beta;
	short int gamma;
	short int stage;
	short int room;
	short int lifeMode;
	short int life;
	short int field_24;
	short int field_26;
	short int field_28;
	short int field_2A;
	short int field_2C;
	short int trackMode;
	short int trackNumber;
	short int positionInTrack;
};

struct boxStruct
{
	short int var0;
	short int var1;
	short int var2;
	short int var3;
};

struct roomDefStruct
{
	short int offsetToCameraDef; // 0
	short int offsetToPosDef; // 2
	short int worldX;//4
	short int worldY;//6
	short int worldZ;//8
	short int numCameraInRoom;//0xA
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
extern char button;

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

extern short int currentCameraTarget;

extern int fileSize;

extern hqrEntryStruct* listBody;
extern hqrEntryStruct* listAnim;
extern hqrEntryStruct* listLife;
extern hqrEntryStruct* listTrack;

extern short int maxObjects;

extern objectStruct objectTable[300]; // may be less

extern short int* vars;

extern int varSize;

extern messageStruct messageTable[5];

extern short int currentMusic;
extern int action;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct *genVar1;
extern boxStruct *genVar3;

extern int genVar5;
extern int genVar6;
extern int genVar7;
extern int genVar8;
extern short int genVar9;
extern short int giveUp;
extern short int inHand;
extern short int lightVar1;
extern int lightVar2;
extern short int numObjInInventory;
extern int soundVar1;
extern int soundVar2;
extern short int statusScreenAllowed;

extern char* etageVar0;
extern char* etageVar1;

extern int changeFloor;
extern short int currentCamera;
extern short int currentEtage;
extern int needChangeRoom;

extern char* cameraPtr;
extern short int currentDisplayedRoom;
extern int mainVar1;
extern int numCameraInRoom;
extern int numCameraZone;
extern char* cameraZoneData;
extern int numRoomZone;
extern char* roomZoneData;
extern char* roomVar5[15];
extern short int roomVar6[15];
extern int startGameVar1;

extern int transformX;
extern int transformY;
extern int transformZ;
extern int transformXCos;
extern int transformXSin;
extern int transformYCos;
extern int transformYSin;
extern int transformZCos;
extern int transformZSin;
extern bool transformUseX;
extern bool transformUseY;
extern bool transformUseZ;

extern int translateX;
extern int translateY;
extern int translateZ;

extern int cameraCenterX;
extern int cameraCenterY;
extern int cameraX;
extern int cameraY;
extern int cameraZ;

extern char cameraDataTab[30];

extern int actorTurnedToObj;

extern int currentProcessedActorIdx;
extern actorStruct* currentProcessedActorPtr;

extern int currentLifeActorIdx;
extern actorStruct* currentLifeActorPtr;
extern int currentLifeNum;

extern char* currentLifePtr;

extern int setupCameraVar1;

#ifdef USE_GL
extern float renderPointList[6400];
#else
extern short int renderPointList[6400];
#endif

extern int numActorInList;
extern int sortedActorTable[50];

extern int angleCompX;
extern int angleCompZ;
extern int angleCompBeta;

extern int bufferAnimCounter;

extern int animCurrentTime;
extern int animKeyframeLength;
extern int animMoveX;
extern int animMoveY;
extern int animMoveZ;
extern int animRot1;
extern int animRot2;
extern int animRot3;
extern char* animVar1;
extern char* animVar3;
extern char* animVar4;

extern short int newFloor;

extern int paletteVar;

extern char cameraBuffer[512];

extern short int newRoom;

extern char* listBodySelect[];
extern char* listAnimSelect[];

extern short int inventory[30];

extern short int shakeVar1;
extern short int shakingAmplitude;
extern unsigned int timerFreeze1;

extern char* hardColTable[30];

extern short int hardColVar1;
extern short int hardColVar2;

extern short int hardClipX1;
extern short int hardClipX2;
extern short int hardClipY1;
extern short int hardClipY2;
extern short int hardClipZ1;
extern short int hardClipZ2;

extern saveEntry saveTable[];

#endif
