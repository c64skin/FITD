// 22 septembre 2003 14:25

// seg002

#include "common.h"

void sysInit(void)
{
	int i;

	time_t ltime;
	FILE* fHandle;

	setupScreen();
	//setupInterrupt();
	//setupInterrupt2();
	//setupInterrupt3();

	//setupVideoMode();

	time( &ltime );

	srand(ltime);

//	if(!initAllSub1()) // recheck test
	{
		musicConfigured = 0;
		initAllVar2 = 0;
	}

	// TODO: reverse sound init code


	aux = (char*)malloc(65068);
	if(!aux)
	{
		theEnd(1,"Aux");
	}

	aux2 = (char*)malloc(64000);
	if(!aux2)
	{
		theEnd(1,"Aux2");
	}

/*	sysInitSub1(aux2,screen);
	sysInitSub2(aux2);
	sysInitSub3(aux2); */

	bufferAnim = (char*)malloc(4960);
	if(!bufferAnim)
	{
		theEnd(1,"BufferAnim");
	}

	fontData = loadPakSafe("itd_ress",5);

/*	sysInitSub4(fontData);
	sysInitSub5(2,0);*/

	aitdBoxGfx = loadPakSafe("itd_ress",4);

	priority = loadFromItd("Priority.ITD");

	fHandle = fopen("DEFINES.ITD","rb");
	if(!fHandle)
	{
		theEnd(0,"DEFINES.ITD");
	}

	fread(&defines,45,2,fHandle);
	fclose(fHandle);

/*	allocTextes();

	if(musicConfigured)
	{
		listMus = HQR_InitRessource("ListMus",11000,2);
	}

	listSamp = HQR_InitRessource("ListSamp",64000,30);

	hqrUnk = HQR_Init(10000,50); */
}

void freeAll(void)
{
/*	HQR_Free(hqrUnk);

	HQR_Free(listSamp);

	HQR_Free(listMus);

	free(languageData);

	free(tabTextes);

	free(priority);

	free(aitdBoxGfx);

	free(fontData);

	free(bufferAnim);

	if(aux != aux3)
	{
		free(aux);
	}

	free(aux2);*/

	//TODO: implement all the code that restore the interrupts & all
}

void startGame(int startupEtage, int startupRoom, int allowSystemMenu)
{
/*	loadObjects();
	initVars();

	loadEtage(startupEtage);

	currentCamera = -1;

	loadRoom(startupRoom);

	startGameVar1 = 0;
	mainVar1 = 2;

	setupCamera();

	mainLoop(allowSystemMenu);

	fadeout(8,0); */
}

void drawPartOfAITDBox(int left, int top, int index, char* gfxData)
{
	char* outPtr;
	char* inPtr;

	int width;
	int height;

	int offset;

	int i;
	int j;

	outPtr = screen + top*320 + left;
	inPtr = gfxData + *(short int*)(index * 2 + gfxData); // alignement unsafe

	inPtr +=4;

	width = *(short int*)(inPtr); // alignement unsafe
	inPtr+=2;
	height = *(short int*)(inPtr); // alignement unsafe
	inPtr+=2;

	offset = 320 - width;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			*(outPtr++) = *(inPtr++);
		}

		outPtr+=offset;
	}
}

void drawAITDBox(int x, int y, int width, int height)
{
	int top;
	int right;
	int left;
	int bottom;

	int currentLeftPosition;
	int currentTopPosition;

	int halfWidth;
	int halfHeight;

	//	setClipSize(0,0,319,199);

	halfWidth = width/2;
	currentLeftPosition = left = x - halfWidth;

	halfHeight = height/2;
	currentTopPosition = top = y - halfHeight;

	right = x + halfWidth;
	bottom = y + halfHeight;

	drawPartOfAITDBox(currentLeftPosition,currentTopPosition,0,aitdBoxGfx); // draw top left corner
    
	while(1) // draw top bar
	{
		currentLeftPosition += 20;

		if(right - 20 <= currentLeftPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition,currentTopPosition,4,aitdBoxGfx);
	}

	drawPartOfAITDBox(currentLeftPosition,currentTopPosition,1,aitdBoxGfx); // draw top right corner

	currentLeftPosition = left;

	while(1) // draw left bar
	{
		currentTopPosition += 20;

		if(bottom - 20 <= currentTopPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition,currentTopPosition,6,aitdBoxGfx);
	}

	currentLeftPosition = right - 8;
	currentTopPosition = top + 20;

	while(bottom - 20 > currentTopPosition)
	{
		drawPartOfAITDBox(currentLeftPosition,currentTopPosition,7,aitdBoxGfx);

		currentTopPosition += 20;
	}

	currentLeftPosition = left;

	drawPartOfAITDBox(currentLeftPosition,currentTopPosition,2,aitdBoxGfx); // draw bottom left corner

	while(1) // draw bottom bar
	{
		currentLeftPosition += 20;

		if(right-20 <= currentLeftPosition)
			break;

		drawPartOfAITDBox(currentLeftPosition,currentTopPosition+12,5,aitdBoxGfx);
	}

	drawPartOfAITDBox(currentLeftPosition,currentTopPosition,3,aitdBoxGfx); // draw bottom right corner

	drawPartOfAITDBox(x-20,currentTopPosition+12,8,aitdBoxGfx); // draw "in the dark"

	currentMenuLeft = left + 8;
	currentMenuTop = top + 8;
	currentMenuRight = right - 9;
	currentMenuBottom = bottom - 9;

//	fillBox(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom);
//	setClipSize(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom);

}

void drawStartupMenu(int selectedEntry)
{
	int currentY = 76;
	int currentTextNum = 0;

	drawAITDBox(160,100,320,80);

	while(currentTextNum<3)
	{
		if(currentTextNum == selectedEntry) // hilight selected entry
		{
//			fillBox(10,currentY,309,currentY+16,100);
//			drawSlectedText(160,currentY,currentTextNum+11,15,4);
		}
		else
		{
//			drawText(160,currentY,currentTextNum+11,4);
		}

		currentY+=16; // next line
		currentTextNum++; // next text
	}
}

void flipScreen()
{
	int i;
	char paletteRGBA[256*4];

	memcpy(unkScreenVar,screen,320*200);

	for(i=0;i<256;i++)
	{
		paletteRGBA[i*4] = palette[i*3];
		paletteRGBA[i*4+1] = palette[i*3+1];
		paletteRGBA[i*4+2] = palette[i*3+2];
		paletteRGBA[i*4+3] = 0xFF;
	}
	osystem.setPalette(paletteRGBA);
	osystem.Flip((unsigned char*)unkScreenVar);

	FILE* fileHandle;
	fileHandle = fopen("dump.raw","wb+");
	fwrite(screen,320,200,fileHandle);
	fclose(fileHandle);
}


int processStartupMenu(void)
{
	int currentSelectedEntry = 0;
	unsigned int chrono;
	int selectedEntry = -1;

//	flushScreen();

	drawStartupMenu(0);
	flipScreen();
//	make3dTatouUnk1(16,0);
	startChrono(&chrono);

	while(evalChrono(&chrono) <= 0x10000 || ( evalChrono(&chrono) > 0x10000 && selectedEntry==-1)) // exit loop only if time out or if choice made
	{
		process_events();

	/*	if(inputKey&1) // up key
		{
			currentSelectedEntry--;

			if(currentSelectedEntry<0)
			{
				currentSelectedEntry = 2;
			}

			drawStartupMenu(currentSelectedEntry);

			menuWaitVSync();

			startChrono(&chrono);

			while(!inputKey)
			{
			}
		} */

		/*
		if(inputKey&2) // down key
		{
			currentSelectedEntry++;

			if(currentSelectedEntry>2)
			{
				currentSelectedEntry = 0;
			}

			drawStartupMenu(currentSelectedEntry);

			menuWaitVSync();

			startChrono(&chrono);

			while(!inputKey)
			{
			}
		} */

/*		if(input2 == 28 || (input1 != 28 && input1!=0)) // select current entry
		{
			selectedEntry = currentSelectedEntry;
		}*/
	}

	if(selectedEntry==2) // if exit game, do not fade
	{
//		fadeOut(16,0);
	}

	return(selectedEntry);
}

int main(int argc, char** argv)
{
	int startupMenuResult;
	int protectionToBeDone = 1;

	osystem.initBuffer(unkScreenVar,320,200);
	startThreadTimer();

	sysInit();

	paletteFill(palette,0,0,0);
	fadeIn(palette);

	//preloadResource();

	if(!make3dTatou())
	{
	//	makeIntroScreens();
	}

/*	while(1)
	{*/
		startupMenuResult = processStartupMenu();

/*		switch(startupMenuResult)
		{
		case -1: // timeout
			{
				defines.hero = rand()&1;
				startGame(7,1,0);

				if(!make3dTatou())
				{
					if(!makeIntroScreens())
					{
						makeSlideshow();
					}
				}

				break;
			}
		case 0: // new game
			{
				if(protectionToBeDone)
				{
					makeProtection();
					protectionToBeDone = 0;
				}

				if(selectHero()!=-1)
				{
					startGame(7,1,0);

					if(!protectionState)
					{
						freeAll();
						exit(-1);
					}

					startGame(0,0,1);

					if(giveUp == 0)
					{
						freeAll();
						exit(-1);
					}
				}

				break;
			}
		case 1: // continue
			{
				if(protectionToBeDone)
				{
					makeProtection();
					protectionToBeDone = 0;
				}

				if(restoreSave(12,0))
				{
					if(!protectionState)
					{
						freeAll();
						exit(-1);
					}

					mainSub1();

					mainVar1 = 2;

					setupCamera();

					mainLoop(1);

					freeScene();

					fadeOut();

					if(giveUp == 0)
					{
						freeAll();
						exit(-1);
					}
				}

				break;
			}
		case 2: // exit
			{
				freeAll();
				exit(-1);

				break;
			}
		}
	}*/

	return(0);
}