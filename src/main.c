// 22 septembre 2003 14:25

// seg002

#include "common.h"

void initFont(char* fontData, int color)
{
	short int tempDx;
	short int tempAxFlip;

	fontVar1 = fontData;

	tempDx = *(short int*)fontData; // alignement
	fontData+=2;

	fontSm1 = *(fontData++);
	fontSm2 = *(fontData++);

	if(!fontSm2)
	{
		fontSm2 = *(short int*)(fontData);
	}
	
	fontData+=2;

	tempAxFlip = *(short int*)fontData;
	fontData+=2;

	tempAxFlip = ((tempAxFlip & 0xFF) << 8 ) | ((tempAxFlip &0xFF00) >> 8);

	fontVar4 = fontData;

	fontVar5 = fontVar1 + tempAxFlip - tempDx*2;

	currentFontColor = color;

	fontSm3 = color;
}

void initFont2(int var1, int var2)
{
	fontSm4 = var1;
	fontSm5 = var2;
}

int computeStringWidth(char* string)
{
	int width = 0;
	char character;

	while(character = *(string++))
	{
		char* dataPtr;
		unsigned short int data;

		dataPtr = fontVar5 + character*2;
		data = *(short int*)dataPtr;
	
		data = ((data & 0xFF)<<8)| ((data&0xFF00)>>8);

		int i;
		for(i=0;i<4;i++)
		{
			data = ((data<<1)&0xFFFE) | ((data & 0x7FFF)>>14);
		}

		data &= 0xF;

		if(!data)
		{
			width += fontSm4;
		}

		width += fontSm5;
		width += data;
	}

	return(width);
}

void allocTextes(void)
{
	int currentIndex;
	char* currentPosInTextes;
	int textCounter;
	int stringIndex;
	char* stringPtr;

	tabTextes = (textEntryStruct*)malloc(250 * sizeof(textEntryStruct)); // 2000 = 250 * 8

	if(!tabTextes)
	{
		theEnd(1,"TabTextes");
	}

	systemTextes = loadPakSafe("FRANCAIS", 0); // todo: use real language name

	for(currentIndex=0;currentIndex<250;currentIndex++)
	{
		tabTextes[currentIndex].index = -1;
		tabTextes[currentIndex].textPtr = NULL;
		tabTextes[currentIndex].width = 0;
	}

	currentPosInTextes = systemTextes;

	textCounter = 0;

	while((currentIndex = *(currentPosInTextes++)) != 26)
	{
		if(currentIndex == '@') // start of string marker
		{
			stringIndex = 0;

			while((currentIndex = *(currentPosInTextes++)) >= '0' && currentIndex <= '9') // parse string number
			{
				stringIndex = stringIndex * 10 + currentIndex - 48;
			}

			if(currentIndex == ':') // start of string
			{
				stringPtr = currentPosInTextes;

				do
				{
					currentPosInTextes ++;
				}while((unsigned char)*(currentPosInTextes-1) >= ' '); // detect the end of the string

				*(currentPosInTextes-1) = 0; // add the end of string

				tabTextes[textCounter].index = stringIndex;
				tabTextes[textCounter].textPtr = stringPtr;
				tabTextes[textCounter].width = computeStringWidth(stringPtr);

				textCounter++;
			}

			if(currentIndex == 26)
			{
				return;
			}
		}
	}
}

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

	initFont(fontData, 14);
	initFont2(2,0);

	aitdBoxGfx = loadPakSafe("itd_ress",4);

	priority = loadFromItd("Priority.ITD");

	fHandle = fopen("DEFINES.ITD","rb");
	if(!fHandle)
	{
		theEnd(0,"DEFINES.ITD");
	}

	fread(&defines,45,2,fHandle);
	fclose(fHandle);

	allocTextes();

/*	if(musicConfigured)
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

char flagTable[]= {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void renderText(int x, int y, char* surface, char* string)
{
	char character;

	fontVar6 = x;
	fontSm7 = y;

	while(character = *(string++))
	{
		char* dataPtr;
		unsigned short int data;
		unsigned short int dx;

		dataPtr = fontVar5 + character*2;
		data = *(short int*)dataPtr;
	
		data = ((data & 0xFF)<<8)| ((data&0xFF00)>>8);

		dx = data;

		data >>= 4;

		if(data&0xF) // real character (width != 0)
		{
			char* characterPtr;

			dx &= 0xFFF;

			characterPtr = (dx >>3) + fontVar4;

			fontSm9 = flagTable[dx & 7];

			int bp = fontSm7;

			fontSm8 = fontVar6;

			int ch = fontSm1;

			for(ch = fontSm1; ch>0; ch--)
			{
				char* outPtr = screen + bp*320 + fontSm8;
				bp++;

				int dh = fontSm9;
				int cl = data&0xF;

				int al = *characterPtr;

				int bx;

				for(bx = 0; cl>0; cl--)
				{
					if(dh & al)
					{
						*(outPtr) = fontSm3;
					}

					outPtr++;

					dh = ((dh<<1) & 0xFE) | ((dh&0x7F)>>7);

					if(dh&0x80)
					{
						bx++;
						al = *(characterPtr + bx);
					}
				}

				characterPtr += fontSm2;
			}

			fontVar6 += data&0xF;
		}
		else // space character
		{
			fontVar6 += fontSm4;
		}

		fontVar6 += fontSm5;
	}
}

textEntryStruct* getTextFromIdx(int index)
{
	int currentIndex;

	for(currentIndex = 0; currentIndex < 250; currentIndex++)
	{
		if(tabTextes[currentIndex].index == index)
		{
			return(&tabTextes[currentIndex]);
		}
	}

	return(NULL);
}

void drawText(int x, int y, int index, int color)
{
	textEntryStruct* entryPtr;
	char* textPtr;

	entryPtr = getTextFromIdx(index);

	if(!entryPtr)
		return;

	x -= (entryPtr->width/2); // center

	textPtr = entryPtr->textPtr;

	initFont(fontData,color);

	renderText(x,y+1,screen,textPtr);
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
			drawText(160,currentY,currentTextNum+11,4);
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

void flushScreen(void)
{
	int i;
	int j;

	for(i=0;i<200;i++)
	{
		for(j=0;j<320;j++)
		{
			*(screen+i*320+j) = 0;
		}
	}
}

int processStartupMenu(void)
{
	int currentSelectedEntry = 0;
	unsigned int chrono;
	int selectedEntry = -1;

	flushScreen();

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

void preloadResource(void)
{
	loadPakToPtr("itd_ress",3,aux);
	copyPalette(aux,palette);
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

	preloadResource();

//	if(!make3dTatou())
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