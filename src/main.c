// 22 septembre 2003 14:25

// seg002

#include "common.h"

char scaledScreen[640*400];

int currentFoundBodyIdx;
char* currentFoundBody;
int numInventoryActions;
short int inventoryActionTable[5];
//short int inventory[30];

int statusLeft;
int statusTop;
int statusRight;
int statusBottom;

int statusVar1;

void updateInHand(int objIdx)
{
	int var_2;
	int actorIdx;
	int lifeOffset;
	int currentActorIdx;
	int currentActorLifeIdx;
	int currentActorLifeNum;
	int foundLife;
	actorStruct* currentActorPtr;
	actorStruct* currentActorLifePtr;

	if(objIdx == -1)
		return;

	foundLife = objectTable[objIdx].foundLife;

	if(objectTable[objIdx].foundLife == -1)
		return;

	currentActorPtr = currentProcessedActorPtr;
	currentActorIdx = currentProcessedActorIdx;
	currentActorLifeIdx = currentLifeActorIdx;
	currentActorLifePtr = currentLifeActorPtr;
	currentActorLifeNum = currentLifeNum;

	if(currentLifeNum != -1)
	{
		lifeOffset = (currentLifePtr - HQR_Get(listLife,currentActorLifeNum))/2;
	}

	var_2 = 0;

	actorIdx = objectTable[objIdx].ownerIdx;

	if(actorIdx==-1)
	{
		actorStruct* currentActorEntryPtr = &actorTable[49];
		int currentActorEntry = 49;

		while(currentActorEntry>=0)
		{
			if(currentActorEntryPtr->field_0 == -1)
				break;

			currentActorEntryPtr--;
			currentActorEntry--;
		}

		if(currentActorEntry==-1) // no space, we will have to overwrite actor 49 !
		{
			currentActorEntry = 49;
			currentActorEntryPtr = &actorTable[49];
		}

		actorIdx = currentActorEntry;
		var_2 = 1;

		currentProcessedActorPtr = &actorTable[actorIdx];
		currentLifeActorPtr = &actorTable[actorIdx];
		currentProcessedActorIdx = actorIdx;
		currentLifeActorIdx = actorIdx;

		currentProcessedActorPtr->field_0 = objIdx;
		currentProcessedActorPtr->life = -1;
		currentProcessedActorPtr->bodyNum = -1;
		currentProcessedActorPtr->flags = 0;
		currentProcessedActorPtr->trackMode = -1;
		currentProcessedActorPtr->room = -1;
		currentProcessedActorPtr->lifeMode = -1;
		currentProcessedActorPtr->ANIM = -1;

	}

	processLife(foundLife);

	if(var_2)
	{
		currentProcessedActorPtr->field_0 = -1;
	}

	currentProcessedActorPtr = currentActorPtr;
	currentProcessedActorIdx = currentActorIdx;
	currentLifeActorIdx = currentActorLifeIdx;
	currentLifeActorPtr = currentActorLifePtr;

	if(currentActorLifeNum != -1)
	{
		currentLifeNum = currentActorLifeNum;
		currentLifePtr = HQR_Get(listLife,currentLifeNum) + lifeOffset * 2;
	}
}

void sysInitSub1(char* var0, char* var1)
{
	screenSm1 = var0;
	screenSm2 = var0;

	screenSm3 = var1;
	screenSm4 = var1;
	screenSm5 = var1;
}

void initFont(char* fontData, int color)
{
	short int tempDx;
	short int tempAxFlip;

	fontVar1 = fontData; // fontPtr

	tempDx = *(short int*)fontData; // alignement
	fontData+=2;

	fontSm1 = *(fontData++); // character height
	fontSm2 = *(fontData++); // character size

	if(!fontSm2)
	{
		fontSm2 = *(short int*)(fontData);
	}
	
	fontData+=2;

	tempAxFlip = *(short int*)fontData;
	fontData+=2;

	tempAxFlip = ((tempAxFlip & 0xFF) << 8 ) | ((tempAxFlip &0xFF00) >> 8);

	fontVar4 = fontData;

	fontVar5 = fontVar1 + tempAxFlip - (tempDx&0xFF)*2;

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
	unsigned char character;

	while((character = ((unsigned char)(*(string++)))))
	{
		char* dataPtr;
		unsigned short int data;

		dataPtr = fontVar5 + character*2;
		data = *(short int*)dataPtr;
	
		data>>=4;

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

	systemTextes = loadPakSafe(languageNameString, 0); // todo: use real language name

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

hqrEntryStruct* HQR_Init(int size,int numEntry)
{
	hqrEntryStruct* dest;
	char* dest2;

	dest = (hqrEntryStruct*)malloc(numEntry*sizeof(hqrSubEntryStruct)+sizeof(hqrEntryStruct));

	if(!dest)
		return NULL;

	dest2 = (char*)malloc(size);

	if(!dest2)
		return NULL;

	strcpy(dest->string,"_MEMORY_");

	dest->sizeFreeData = size;
	dest->maxFreeData = size;
	dest->numMaxEntry = numEntry;
	dest->numUsedEntry = 0;
	dest->dataPtr = dest2;

	return(dest);
}

void sysInit(void)
{
	int i;

#ifndef PCLIKE
	unsigned long int ltime;
#else
	time_t ltime;
#endif
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

	sysInitSub1(aux2,screen);
/*	sysInitSub2(aux2);
	sysInitSub3(aux2); */

	bufferAnim = (char*)malloc(4960);
	if(!bufferAnim)
	{
		theEnd(1,"BufferAnim");
	}

	fontData = loadPakSafe("ITD_RESS",5);

	initFont(fontData, 14);
	initFont2(2,0);

	aitdBoxGfx = loadPakSafe("ITD_RESS",4);

	priority = loadFromItd("PRIORITY.ITD");

	fHandle = fopen("DEFINES.ITD","rb");
	if(!fHandle)
	{
		theEnd(0,"DEFINES.ITD");
	}

///////////////////////////////////////////////
	{
		short int table[45];

		fread(table,45,2,fHandle);
		fclose(fHandle);
		
		for(i=0;i<45;i++)
		{
			table[i] = ((table[i]&0xFF)<<8) | ((table[i]&0xFF00)>>8);
		}

		memcpy(&defines,table,45*2);
	}
//////////////////////////////////////////////

	allocTextes();

/*	if(musicConfigured)
	{
		listMus = HQR_InitRessource("ListMus",11000,2);
	}

	listSamp = HQR_InitRessource("ListSamp",64000,30); */

	hqrUnk = HQR_Init(10000,50);
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

	setClipSize(0,0,319,199);

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

	fillBox(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom,0);
	setClipSize(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom);

}

unsigned char flagTable[]= {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void renderText(int x, int y, char* surface, char* string)
{
	unsigned char character;

	fontVar6 = x;
	fontSm7 = y;

	while((character = *((unsigned char*)(string++))))
	{
		char* dataPtr;
		unsigned short int data;
		unsigned short int dx;

		dataPtr = fontVar5 + character*2;
		data = *(unsigned short int*)dataPtr;
	
		data = ((data & 0xFF)<<8)| ((data&0xFF00)>>8);

		dx = data;

		data >>= 12;

		if(data&0xF) // real character (width != 0)
		{
			char* characterPtr;
			int bp;
			int ch;

			dx &= 0xFFF;

			characterPtr = (dx>>3) + fontVar4;

			fontSm9 = flagTable[dx & 7];

			bp = fontSm7;

			fontSm8 = fontVar6;

			ch;

			for(ch = fontSm1; ch>0; ch--)
			{
				char* outPtr = screen + bp*320 + fontSm8;
				

				int dh = fontSm9;
				int cl = data&0xF;

				int al = *characterPtr;

				int bx;

				bp++;

				for(bx = 0; cl>0; cl--)
				{
					if(dh & al)
					{
						*(outPtr) = (char)fontSm3;
					}

					outPtr++;

					dh = ((dh>>1) & 0x7F) | ((dh<<7)&0x80);

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

void fillBox(int x1, int y1, int x2, int y2, char color) // fast recode. No RE
{
	int width = x2 - x1 + 1;
	int height = y2 - y1 + 1;

	char* dest = screen + y1*320 + x1;

	int i;
	int j;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			*(dest++)= color;
		}

		dest += 320-width;
	}
}

void drawSlectedText(int x, int y, int index, int color1, int color2)
{
	textEntryStruct* entryPtr;
	char* textPtr;

	entryPtr = getTextFromIdx(index);

	if(!entryPtr)
		return;

	x -= (entryPtr->width/2); // center

	textPtr = entryPtr->textPtr;

	initFont(fontData,color2);
	renderText(x,y+1,screen,textPtr);

	initFont(fontData,color1);
	renderText(x,y,screen,textPtr);

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
			fillBox(10,currentY,309,currentY+16,100);
			drawSlectedText(160,currentY,currentTextNum+11,15,4);
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
	char* outPtr = scaledScreen;
	char* inPtr = unkScreenVar;
#ifdef USE_GL
	osystem_flip(NULL);
	return;
#endif

	memcpy(unkScreenVar,screen,320*200);

	for(i=0;i<256;i++)
	{
		paletteRGBA[i*4] = palette[i*3];
		paletteRGBA[i*4+1] = palette[i*3+1];
		paletteRGBA[i*4+2] = palette[i*3+2];
		paletteRGBA[i*4+3] = -1;
	}

	outPtr = scaledScreen;
	inPtr = unkScreenVar;

	for(i=0;i<200;i++)
	{
		int j;
		char* copySource = outPtr;

		for(j=0;j<320;j++)
		{
			*(outPtr++) = *(inPtr);
			*(outPtr++) = *(inPtr++);
		}

		// copy line
		for(j=0;j<640;j++)
		{
			*(outPtr++) = *(copySource++);
		}
		
	}

	//osystem_setPalette(paletteRGBA);
	osystem_flip((unsigned char*)scaledScreen);
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
#ifdef USE_GL
	osystem_startFrame();
	osystem_stopFrame();
	osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
#endif
	flipScreen();
	make3dTatouUnk1(16,0);
	startChrono(&chrono);

	while(evalChrono(&chrono) <= 0x10000) // exit loop only if time out or if choice made
	{
#ifdef USE_GL
	osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
	osystem_startFrame();
#endif

		if(selectedEntry!=-1 || evalChrono(&chrono) > 0x10000)
		{
			break;
		}

		process_events();
		readKeyboard();

		if(inputKey&1) // up key
		{
			currentSelectedEntry--;

			if(currentSelectedEntry<0)
			{
				currentSelectedEntry = 2;
			}

			drawStartupMenu(currentSelectedEntry);
			flipScreen();
//			menuWaitVSync();

			startChrono(&chrono);

			while(inputKey)
			{
				readKeyboard();
			}
		}

		
		if(inputKey&2) // down key
		{
			currentSelectedEntry++;

			if(currentSelectedEntry>2)
			{
				currentSelectedEntry = 0;
			}

			drawStartupMenu(currentSelectedEntry);
			//menuWaitVSync();
			flipScreen();

			startChrono(&chrono);

			while(inputKey)
			{
				readKeyboard();
			}
		} 

		if(input2 == 28 || (input2 != 28 && input1!=0)) // select current entry
		{
			selectedEntry = currentSelectedEntry;
		}
#ifdef USE_GL
		osystem_stopFrame();
		flipScreen();
#endif
	}

	if(selectedEntry==2) // if exit game, do not fade
	{
		fadeOut(16,0);
	}

	readKeyboard();
	while(inputKey)
	{
		readKeyboard();
	}

	return(selectedEntry);
}

void preloadResource(void)
{
	char localPalette[768];

	loadPakToPtr("ITD_RESS",3,aux);
	copyPalette(aux,palette);

	copyPalette(palette,localPalette);
//	fadeInSub1(localPalette);

	// to finish
}

void selectHeroSub1(int x1,int y1, int x2, int y2)
{
	int i;
	int j;

	for(i=y1;i<y2;i++)
	{
		for(j=x1;j<x2;j++)
		{
			*(screenSm3+i*320+j) = *(screenSm1+i*320+j);
		}
	}
}

int selectHero(void)
{
	int choice = 0;
	int var_4 = 1;
	int choiceMade = 0;

	sysInitSub1(aux,screen);

	while(choiceMade == 0)
	{
		process_events();
		readKeyboard();
	
		loadPakToPtr("ITD_RESS",10,aux);
		copyToScreen(aux,screen);
		copyToScreen(screen,aux2);

		if(choice == 0)
		{
			drawAITDBox(80,100,160,200);
			selectHeroSub1(10,10,149,190);
		}
		else
		{
			drawAITDBox(240,100,160,200);
			selectHeroSub1(170,10,309,190);
		}

		flipScreen();

		if(var_4 != 0)
		{
			make3dTatouUnk1(0x40,0);

			do
			{
				readKeyboard();
			}while(input1 || input2);

			var_4 = 0;
		}

		while((input3 = input2)!=28 && input1 ==0) // process input
		{
			readKeyboard();
			process_events();

			if(inputKey & 4) // left
			{
				choice = 0;
				copyToScreen(aux2,screen);
				drawAITDBox(80,100,160,200);
				selectHeroSub1(10,10,149,190);
				flipScreen();

				while(inputKey!=0)
				{
					readKeyboard();
				}
			}

			if(inputKey & 8) // right
			{
				choice = 1;
				copyToScreen(aux2,screen);
				drawAITDBox(240,100,160,200);
				selectHeroSub1(170,10,309,190);
				flipScreen();

				while(inputKey!=0)
				{
					readKeyboard();
				}
			}

			if(input3 == 1)
			{
				sysInitSub1(aux2,screen);
				fadeOut(0x40,0);
				return(-1);
			}
		}

		fadeOut(0x40,0);
		readVar = 0;

		switch(choice)
		{
		case 0:
			{
				copyToScreen(unkScreenVar,screen);
				setClipSize(0,0,319,199);
				loadPakToPtr("ITD_RESS",14,aux);
				selectHeroSub1(160,0,319,199);
				copyToScreen(screen,aux);
				printText(defines.field_C+1,165,5,314,194,2,15);
				defines.hero = 1;
				break;
			}
		case 1:
			{
				copyToScreen(unkScreenVar,screen);
				setClipSize(0,0,319,199);
				loadPakToPtr("ITD_RESS",14,aux);
				selectHeroSub1(0,0,159,199);
				copyToScreen(screen,aux);
				printText(defines.field_A+1,5,5,154,194,2,15);
				defines.hero = 0;
				break;
			}
		}

		if(input3 && 0x1C)
		{
			choiceMade = 1;
		}

	}

	fadeOut(0x40,0);

	sysInitSub1(aux2,screen);

	return(choice);
}

void printTextSub5(int x, int y, int param, char* gfx)
{
}

void printTextSub6(hqrEntryStruct* hqrPtr, int index)
{
}

void printTextSub7()
{
}

void printTextSub8()
{
}

int printText(int index, int left, int top, int right, int bottom, int mode, int color)
{
	bool lastPageReached = false;
	char tabString[] = "    ";
	int var_12 = 1;
	int currentPage = 0;
	int quit = 0;
	int var_2 = -1;
	int var_1C3;
	char* localTextTable[100];
	int currentTextIdx;
	int maxStringWidth;
	int var_8;
	char* textPtr;

	initFont(fontData, color);

	maxStringWidth = right - left + 4;

	var_8 = printTextSub1(hqrUnk,getPakSize(languageNameString,index)+300);

	textPtr = printTextSub2(hqrUnk, var_8);

	if(!loadPakToPtr( languageNameString, index, textPtr))
	{
		theEnd(1, languageNameString );
	}

	localTextTable[0] = textPtr;

//	soundVar2 = -1;
//	soundVar1 = -1;

	while(!quit)
	{
		char* var_1C2;
		int currentTextY;
		copyToScreen(aux,screen);
		process_events();
		setClipSize(left,top,right,bottom);

		var_1C2 = localTextTable[currentPage];

		currentTextY = top;
		lastPageReached = false;

		while(currentTextY <= bottom - 16)
		{
			int var_1AA = 1;
			int var_1BA = 0;
			int currentStringWidth;
			int currentTextX;

			regularTextEntryStruct* currentText = textTable;

			int numWordInLine = 0;

			int interWordSpace = 0;

parseSpe:	while(*var_1C2 == '#')
			{
				//char* var_1BE = var_1C2;
				var_1C2++;

				switch(*(var_1C2++))
				{
				case 'P': // page change
					{
						if(currentTextY>top) // Hu ?
							goto pageChange;
						break;
					}
				case 'T': // tab
					{
						currentText->textPtr = tabString;
						currentText->width = computeStringWidth(currentText->textPtr)+3;
						var_1BA+=currentText->width;
						numWordInLine++;
						currentText++;
						break;
					}
				case 'C': // center
					{
						var_1AA &= 0xFFFE;
						var_1AA |= 8;
						break;
					}
				case 'G': // print number
					{
						currentTextIdx = 0;

						while(*var_1C2>='0' && *var_1C2<='9')
						{
							currentTextIdx = (currentTextIdx * 10 + *var_1C2 - 48);
							var_1C2 ++;
						}

						if(loadPakToPtr("ITD_RESS",9,aux2))
						{
						/*	var_C = printTextSub3(currentTextIdx,aux2);
							var_A = printTextSub4(currentTextIdx,aux2);

							if(currentTextY + var_A > bottom)
							{
								var_1C2 = var_1BE;

								goto pageChange;
							}
							else
							{
								printTextSub5((((right-left)/2)+left)-var_C, currentTextY, currentTextIdx, aux2);
								currentTextY = var_A;
							}*/
						}

						break;
					}
				}
			}

			currentText->textPtr = var_1C2;

			do
			{
				var_1C3 = *((unsigned char*)var_1C2++);
			}while(var_1C3>' '); // go to the end of the string

			*(var_1C2-1) = 0; // add end of string marker to cut the word

			currentStringWidth = computeStringWidth(currentText->textPtr) + 3;

			if(currentStringWidth <= maxStringWidth)
			{
				if( var_1BA + currentStringWidth > maxStringWidth )
				{
					var_1C2 = currentText->textPtr;
				}
				else
				{
					currentText->width = currentStringWidth;
					var_1BA += currentStringWidth;

					numWordInLine++;
					currentText++;

					switch(var_1C3) // eval the character that caused the 'end of word' state
					{
					case 0x1A: // end of text
						{
							var_1AA &= 0xFFFE;
							var_1AA |= 4;
							lastPageReached = true;

							break;
						}
					case 0x0D:
					case 0x00:
						{
							if(*var_1C2 < ' ')
							{
								if(*(++var_1C2) == 0xD)
								{
									var_1C2+=2;
									var_1AA&=0xFFFE;
									var_1AA|=2;
								}
								else
								{
									if(*var_1C2=='#')
									{
										var_1AA &=0xFFFE;
									}
								}
							}
							else
							{
								goto parseSpe;
							}
							break;
						}
					default:
						{
							goto parseSpe;
							break;
						}
					}
				}
			}
			else
			{
				quit = 1;
			}

			if(var_1AA & 1) // stretch words on line
			{
				//interWordSpace = (maxStringWidth - var_1BA) / (numWordInLine-1);
			}

			currentText = textTable;
			currentTextX;

			if(var_1AA & 8) // center
			{
				currentTextX = left + ((maxStringWidth - var_1BA) / 2);
			}
			else
			{
				currentTextX = left;
			}

			currentTextIdx = 0;

			while(currentTextIdx < numWordInLine)
			{
				renderText(currentTextX,currentTextY,screen,currentText->textPtr);

				currentTextX += currentText->width + interWordSpace; // add inter word space

				currentText++;
				currentTextIdx++;
			}

			if(var_1AA&2) // font size
			{
				currentTextY += 8;
			}

			currentTextY += 16;
		}

		if(!lastPageReached || bottom+16<currentTextY)
		{
pageChange:	if(lastPageReached)
			{
				*(var_1C2-1) = 0x1A;
			}
			else
			{
				localTextTable[currentPage+1] = var_1C2;
			}

			if(mode==0)
			{
				if(currentPage>0)
				{
					printTextSub5(left-19,185,12,aitdBoxGfx);
				}

				if(!lastPageReached)
				{
					printTextSub5(right+4,185,11,aitdBoxGfx);
				}
			}

			if(mode==2)
			{
				if(currentPage>0)
				{
					printTextSub5(left-3,191,13,aitdBoxGfx);
				}

				if(!lastPageReached)
				{
					printTextSub5(right-10,191,14,aitdBoxGfx);
				}
			} 

			if(var_12)
			{
				if(mode!=1)
				{
					flipScreen();
					make3dTatouUnk1(16,0);
				}
				else
				{
					if(readVar)
					{
						printTextSub7();
					}
					else
					{
						flipScreen();
					}
				}

				var_12 = 0;
			}
			else
			{
				if(readVar)
				{
					if(var_2<currentPage)
					{
						printTextSub7();
					}
					else
					{
						printTextSub8();
					}
				}
				else
				{
					flipScreen();
				}
			}

			if(mode!=1) // mode != 1: normal behavior (user can flip pages)
			{
				do
				{
					readKeyboard();
				}while(input2 || inputKey || input1);

				input3 = input2 & 0xFF;
				input4 = inputKey & 0xFF;
				button = input1;

				if(input3==1 || button)
				{
					quit = 1;
				}
				else
				{
					if(mode!=2 || input3 != 0x1C)
					{
						if(inputKey&0xA0 || input3 == 0x1C)
						{
							if(!lastPageReached) // flip to next page
							{
								var_2 = currentPage++;

								if(mode==2)
								{
									playSound(defines.field_0);
//												soundVar2 = -1;
//												soundVar1 = -1;
								}
							}
						}
						else
						{
							// flip to previous page

							// TODO: implement...
						}
					}
					else
					{
						quit = 1;
					}
				}
			}
			else // auto page fip
			{
				unsigned int var_6;
				startChrono(&var_6);

				do
				{
					process_events();
					readKeyboard();
					if(evalChrono(&var_6) > 300)
					{
						break;
					}
				}while(!input2 && !input1);

				if(input2 || input1)
				{
					quit = 1;
				}

				if(!lastPageReached)
				{
					currentPage++;
					playSound(defines.field_0);
//								soundVar2 = -1;
				}
				else
				{
					quit = 1;
					mode = 0;
				}
			}
		}
	}

	printTextSub6(hqrUnk, var_8);

	return(mode);
}

int makeIntroScreens(void)
{
	char* data;
	unsigned int chrono;

	data = loadPak("ITD_RESS",13);
	copyToScreen(data+770,unkScreenVar);
	make3dTatouUnk1(8,0);
	memcpy(screen,unkScreenVar,320*200);
	flipScreen();
	free(data);
	loadPakToPtr("ITD_RESS",7,aux);
	startChrono(&chrono);

	do
	{
		int time;

		process_events();
		readKeyboard();

		time = evalChrono(&chrono);

		if(time>=0x30)
			break;

	}while(input2 == 0 && input1 == 0);

	playSound(defines.field_0);
/*	soundVar2 = -1;
	soundVar1 = -1;
	soundVar2 = -1;
	soundVar1 = 0; */
//	readVar = 1;
	printText(defines.field_6+1,48, 2,260,197,1,26);

	return(0);
}

hqrEntryStruct* HQR_InitRessource(char* name, int size, int numEntries)
{
	hqrEntryStruct* dest;
	char* dest2;

	dest = (hqrEntryStruct*)malloc(numEntries*sizeof(hqrSubEntryStruct)+sizeof(hqrEntryStruct));

	if(!dest)
		return NULL;

	dest2 = (char*)malloc(size + 300);

	if(!dest2)
		return NULL;

	strcpy(dest->string,"        ");
	strncpy(dest->string,name,8);

	dest->sizeFreeData = size;
	dest->maxFreeData = size;
	dest->numMaxEntry = numEntries;
	dest->numUsedEntry = 0;
	dest->dataPtr = dest2;

	return(dest);
}

void initEngine(void)
{
	FILE* fHandle;
	int i;

	fHandle = fopen("OBJETS.ITD","rb");
	if(!fHandle)
		theEnd(0,"OBJETS.ITD");

	fread(&maxObjects,2,1,fHandle);

	for(i=0;i<maxObjects;i++)
	{
		fread(&objectTable[i],0x34,1,fHandle);

		objectTable[i].flags |= 0x20;
	}

	fclose(fHandle);

//
	fHandle = fopen("objDump.txt","w+");
	for(i=0;i<maxObjects;i++)
	{
		fprintf(fHandle,"Object %d:", i);

		fprintf(fHandle,"\t body:%03d",objectTable[i].field_2);
		fprintf(fHandle,"\t anim:%03d",objectTable[i].field_26);
		fprintf(fHandle,"\t stage:%01d",objectTable[i].stage);
		fprintf(fHandle,"\t room:%02d",objectTable[i].room);
		fprintf(fHandle,"\t trackMode: %01d",objectTable[i].trackMode);
		fprintf(fHandle,"\t life: %02d",objectTable[i].life);
		fprintf(fHandle,"\t beta: %03X",objectTable[i].beta);

		fprintf(fHandle,"\n");
	}
	fclose(fHandle);

/*	fHandle = fopen("objNames.txt","w+");
	for(i=0;i<maxObjects;i++)
	{
		fprintf(fHandle,"obj%03d ",i);
		if(objectTable[i].foundName == -1)
		{
			fprintf(fHandle,"-1\n");
		}
		else
		{
			textEntryStruct* name = getTextFromIdx(objectTable[i].foundName);
			if(name)
				fprintf(fHandle,"%s\n",name->textPtr);
			else
				fprintf(fHandle,"?\n");
		}
	}
	fclose(fHandle);*/
//

	vars = (short int*)loadFromItd("VARS.ITD");

	varSize = fileSize;

	i = defines.hero; // backup hero selection

	fHandle = fopen("DEFINES.ITD","rb");
	if(!fHandle)
	{
		theEnd(0,"DEFINES.ITD");
	}

///////////////////////////////////////////////
	{
		short int table[45];
		int i;

		fread(table,45,2,fHandle);
		fclose(fHandle);
		
		for(i=0;i<45;i++)
		{
			table[i] = ((table[i]&0xFF)<<8) | ((table[i]&0xFF00)>>8);
		}

		memcpy(&defines,table,45*2);
	}

	defines.hero = i;

	/*listLife = HQR_InitRessource("LISTLIFE", 10000, 100);
	listTrack = HQR_InitRessource("LISTTRAK", 1000, 10);*/

	listLife = HQR_InitRessource("LISTLIFE", 1000000, 1000);
	listTrack = HQR_InitRessource("LISTTRAK", 100000, 1000);

	// TODO: missing dos memory check here

	/*listBody = HQR_InitRessource(listBodySelect[defines.hero],100000, 50); // was calculated from free mem size
	listAnim = HQR_InitRessource(listAnimSelect[defines.hero],100000, 50); // was calculated from free mem size*/

	listBody = HQR_InitRessource(listBodySelect[defines.hero],10000000, 500); // was calculated from free mem size
	listAnim = HQR_InitRessource(listAnimSelect[defines.hero],10000000, 500); // was calculated from free mem size

	for(i=0;i<50;i++)
	{
		actorTable[i].field_0 = -1;
	}

	currentCameraTarget = defines.field_E;
}

void initVarsSub1(void)
{
	int i;

	for(i=0;i<5;i++)
	{
		messageTable[i].string = NULL;
	}
}

void initVars()
{
	giveUp = 0;
	inHand = -1;
	numObjInInventory = 0;
	action = 0;

	genVar1 = genVar2;
	genVar3 = genVar4;

	genVar5 = 0;
	genVar6 = 0;

	soundVar2 = -1;
	genVar7 = -1;
	soundVar1 = -1;
	currentMusic = -1;
	genVar8 = -1;

	lightVar1 = 0;
	lightVar2 = 0;

	genVar9 = -1;
	currentCameraTarget = -1;

	statusScreenAllowed = 1;

	initVarsSub1();
}

void HQR_Reset(hqrEntryStruct* hqrPtr)
{
	hqrPtr->sizeFreeData = hqrPtr->maxFreeData;
	hqrPtr->numUsedEntry = 0;
}

void loadFloor(int floorNumber)
{
	char buffer[256];

	if(etageVar1)
	{
		free(etageVar1);
		free(etageVar0);
	}

	//stopSounds();

	HQR_Reset(listBody);
	HQR_Reset(listAnim);

	currentEtage = floorNumber;

	sprintf(buffer,"ETAGE0%d",floorNumber);

	etageVar0 = loadPakSafe(buffer,0);
	etageVar1 = loadPakSafe(buffer,1);

	currentCamera = -1;
	needChangeRoom = 1;
	changeFloor = 0;
}

void loadRoom(int roomNumber)
{
	int i;
	int cameraVar0;
	int cameraVar1;
	int cameraVar2;
	int currentCameraIdx;
	roomDefStruct* roomDataPtr;
	char* var_20;
	int var_1A = 0;
	int var_10 = -1;
	int var_1C;

	freezeTime();
	printf("Load room %d\n",roomNumber);

	if(currentCamera == -1)
	{
		currentCameraIdx = -1;
	}
	else
	{
		cameraVar0 = *(short int*)(cameraPtr+4);
		cameraVar1 = *(short int*)(cameraPtr+6);
		cameraVar2 = *(short int*)(cameraPtr+8);

		currentCameraIdx = *(short int*)(cameraPtr + (currentCamera+6)*2);
	}

	cameraPtr = etageVar0+*(unsigned int*)(etageVar0 + (roomNumber * 4));
	roomDataPtr = (roomDefStruct*)(etageVar0+*(unsigned int*)(etageVar0 + (roomNumber * 4)));

	currentDisplayedRoom = roomNumber;

	numCameraInRoom = roomDataPtr->numCameraInRoom;

	var_20 = cameraPtr + roomDataPtr->offsetToPosDef;
	numCameraZone = *(short int*)var_20;
	var_20 += 2;
	cameraZoneData = var_20;

	var_20 = cameraPtr + roomDataPtr->offsetToCameraDef;
	numRoomZone = *(short int*)var_20;
	var_20 += 2;
	roomZoneData = var_20;
	
	for(i=0;i<numCameraInRoom;i++) // build all the camera list
	{
		int cameraIdx = *(short int*)(cameraPtr + (i+6)*2); // indexes are between the roomDefStruct and the first zone data
		int j;
		char* var_8;

		if(currentCameraIdx == cameraIdx)
		{
			var_1A = i;
			var_10 = cameraIdx;
		}

		roomVar5[i] = etageVar1 + *(unsigned int*)(etageVar1 + cameraIdx * 4);
		var_20 = roomVar5[i] + 0x12;
		cameraIdx = *(short int*)var_20;
		var_20 +=2;

	/*	for(j=0;*(short int*)(var_20+=2)!=currentDisplayedRoom;(j++) && (var_20+=0xA))
		{
			if(j>= cameraIdx)
			{
				break;
			}
		} */

		for(j=0;j<cameraIdx;j++)
		{
			if(*(short int*)var_20 == currentDisplayedRoom)
				break;

			var_20+=2;
			var_20+=0xA;
		}

		var_1C = j;

		var_8 = roomVar5[i] + (var_1C*12) + 0x18;

		roomVar6[i] = (*(short int*)var_8)/2;
	}

	if(currentCameraIdx != -1) // if the two room have the current camera in common, update the actor transform
	{
		int var_E = (*(short int*)(cameraPtr + 4) - cameraVar0) * 10;
		int var_C = (*(short int*)(cameraPtr + 6) - cameraVar1) * 10;
		int var_A = (*(short int*)(cameraPtr + 8) - cameraVar2) * 10;

		for(i=0;i<50;i++)
		{
			if(actorTable[i].field_0 != -1)
			{
				actorTable[i].worldX -= var_E;
				actorTable[i].worldY += var_C;
				actorTable[i].worldZ += var_A;
			}
		}
	}

	startGameVar1 = var_1A;
	mainVar1 = 1;
	needChangeRoom = 0;
	unfreezeTime();
}

void loadCamera(int cameraIdx)
{
	char name[16];
	int useSpecial = -1;

	sprintf(name,"CAMERA0%d",currentEtage);
	//strcat(name,".PAK");

	if(defines.lightVar==1)
	{
		switch(currentEtage)
		{
		case 6:
			{
				if(cameraIdx == 0)
				{
					useSpecial = 17;
				}
				if(cameraIdx == 5)
				{
					useSpecial = 18;
				}
				break;
			}
		case 7:
			{
				if(cameraIdx == 1)
				{
					useSpecial = 16;
				}
				break;
			}
		}
	}

	if(useSpecial != -1)
	{
		strcpy(name,"ITD_RESS");
		cameraIdx = useSpecial;
	}

	if(!loadPakToPtr(name,cameraIdx,aux))
	{
		theEnd(0,name);
	}
}

void setupPointTransformSM(int x, int y, int z)
{
	transformX = x&0x3FF;
	if(transformX)
	{
		transformXCos = cosTable[transformX];
		transformXSin = cosTable[(transformX+0x100)&0x3FF];
		transformUseX = true;
	}
	else
	{
		transformUseX = false;
	}

	transformY = y&0x3FF;
	if(transformY)
	{
		transformYCos = cosTable[transformY];
		transformYSin = cosTable[(transformY+0x100)&0x3FF];
		transformUseY = true;
	}
	else
	{
		transformUseY = false;
	}

	transformZ = z&0x3FF;
	if(transformZ)
	{
		transformZCos = cosTable[transformZ];
		transformZSin = cosTable[(transformZ+0x100)&0x3FF];
		transformUseZ = true;
	}
	else
	{
		transformUseZ = false;
	}
}

void setupSelfModifyingCode(int x, int y, int z)
{
	translateX = x;
	translateY = y;
	translateZ = z;
}

void setupSMCode(int centerX, int centerY, int x, int y, int z)
{
	cameraCenterX = centerX;
	cameraCenterY = centerY;
	
	cameraX = x;
	cameraY = y;
	cameraZ = z;
}

int setupCameraSub1Sub1(int value)
{
	char* ptr = cameraDataTab;
	int var;

	while((var=*(ptr++))!=-1)
	{
		if(value == var)
		{
			return(1);
		}
	}

	return(0);
}

void setupCameraSub1()
{
	int i;
	int j;
	int var_10;
	int var_6;
	short int* ptr2;
	int var_12;

	char* dataTabPos = cameraDataTab;
	short int* ptr = (short int*)(etageVar0 + currentDisplayedRoom*4);

	ptr+= (unsigned short int)*(ptr+1);
	*dataTabPos = -1;

	var_6 = *(ptr++);

	for(i=0;i<var_6;i++)
	{
		if(*(ptr+7) == 0)
		{
			var_10 = *(ptr+6);
			if(!setupCameraSub1Sub1(var_10))
			{
				*(dataTabPos++) = var_10;
				*(dataTabPos) = -1;
			}
		}
	}

	ptr2 = (short int*)(roomVar5[currentCamera] + 18);

	var_12 = *(ptr2++);

	for(j=0;j<var_12;j++)
	{
		var_10 = *(ptr2);

		if(!setupCameraSub1Sub1(var_10))
		{
			*(dataTabPos++) = var_10;
			*(dataTabPos) = -1;
		}

		ptr2 += 6;
	}
}

void updateAllActorAndObjectsSub1(int index) // remove actor
{
	actorStruct *actorPtr = &actorTable[index];

	if(actorPtr->field_0 == -2) // flow
	{
		actorPtr->field_0 = -1;

		if(actorPtr->ANIM == 4 )
		{
			defines.field_1C = 0;
		}

		printTextSub6(hqrUnk,actorPtr->FRAME);
	}
	else
	{
		if(actorPtr->field_0 >= 0)
		{
			objectStruct* objectPtr = &objectTable[actorPtr->field_0];

			objectPtr->ownerIdx = -1;
			actorPtr->field_0 = -1;

			objectPtr->field_2 = actorPtr->bodyNum;
			objectPtr->field_26 = actorPtr->ANIM;
			objectPtr->field_28 = actorPtr->FRAME;
			objectPtr->field_2A = actorPtr->field_40;
			objectPtr->field_2C = actorPtr->field_42;
			objectPtr->flags = actorPtr->flags &0xFFF7;
			objectPtr->flags = actorPtr->dynFlags << 5; // ???!!!?
			objectPtr->life = actorPtr->life;
			objectPtr->lifeMode = actorPtr->lifeMode;
			objectPtr->trackMode = actorPtr->trackMode;

			if(objectPtr->trackMode)
			{
				objectPtr->trackNumber = actorPtr->trackNumber;
				objectPtr->positionInTrack = actorPtr->positionInTrack;
			}

			objectPtr->x = actorPtr->roomX + actorPtr->modX;
			objectPtr->y = actorPtr->roomY + actorPtr->modY;
			objectPtr->z = actorPtr->roomZ + actorPtr->modZ;

			objectPtr->alpha = actorPtr->alpha;
			objectPtr->beta = actorPtr->beta;
			objectPtr->gamma = actorPtr->gamma;

			objectPtr->stage = actorPtr->stage;
			objectPtr->room = actorPtr->room;

			actorTurnedToObj = 1;
		}
	}
}

void getZvCube(char* bodyPtr, ZVStruct* zvPtr)
{
	short int* ptr;

	ptr = (short int*)(bodyPtr+2);

	zvPtr->ZVX1 = *(ptr++);
	zvPtr->ZVX2 = *(ptr++);
	zvPtr->ZVY1 = *(ptr++);
	zvPtr->ZVY2 = *(ptr++);
	zvPtr->ZVZ1 = *(ptr++);
	zvPtr->ZVZ2 = *(ptr++);

	zvPtr->ZVZ2 = zvPtr->ZVX2 = (zvPtr->ZVX2 + zvPtr->ZVZ2)/2;
	zvPtr->ZVX1 = zvPtr->ZVZ1 = -zvPtr->ZVZ2;
}

void getZvNormal(char* bodyPtr, ZVStruct* zvPtr)
{
	short int* ptr;

	ptr = (short int*)(bodyPtr+2);

	zvPtr->ZVX1 = *(ptr++);
	zvPtr->ZVX2 = *(ptr++);
	zvPtr->ZVY1 = *(ptr++);
	zvPtr->ZVY2 = *(ptr++);
	zvPtr->ZVZ1 = *(ptr++);
	zvPtr->ZVZ2 = *(ptr++);
}

void makeDefaultZV(ZVStruct* zvPtr)
{
	zvPtr->ZVX1 = -100;
	zvPtr->ZVX2 = 100;

	zvPtr->ZVY1 = -2000;
	zvPtr->ZVY2 = 0;

	zvPtr->ZVZ1 = -100;
	zvPtr->ZVZ2 = 100;
}

void getZvMax(char* bodyPtr, ZVStruct* zvPtr)
{
	int x1;
	int x2;
	int z1;
	int z2;
	
	getZvNormal(bodyPtr,zvPtr);

	x1 = zvPtr->ZVX1;
	x2 = zvPtr->ZVX2;

	z1 = zvPtr->ZVZ1;
	z2 = zvPtr->ZVZ2;

	x2 = - x1 + x2;
	z2 = - z1 + z2;

	if(x2 < z2)
	{
		x2 = z2;
	}

	x2 /= 2;

	zvPtr->ZVX1 = -x2;
	zvPtr->ZVX2 = x2;

	zvPtr->ZVZ1 = -x2;
	zvPtr->ZVZ2 = x2;
}

bool pointRotateEnable = true;

int pointRotateCosX;
int pointRotateSinX;
int pointRotateCosY;
int pointRotateSinY;
int pointRotateCosZ;
int pointRotateSinZ;

void setupPointRotate(int alpha, int beta, int gamma)
{
	pointRotateEnable = true;

	pointRotateCosX = cosTable[alpha&0x3FF];
	pointRotateSinX = cosTable[((alpha&0x3FF) + 0x100) & 0x3FF];

	pointRotateCosY = cosTable[beta&0x3FF];
	pointRotateSinY = cosTable[((beta&0x3FF) + 0x100) & 0x3FF];

	pointRotateCosZ = cosTable[gamma&0x3FF];
	pointRotateSinZ = cosTable[((gamma&0x3FF) + 0x100) & 0x3FF];
}

void pointRotate(int x, int y, int z, int* destX, int* destY, int* destZ)
{
	if(pointRotateEnable)
	{
		{
			int tempX = x;
			int tempY = y;
			x = ((((tempX * pointRotateSinZ) - ( tempY * pointRotateCosZ)))>>16)<<1;
			y = ((((tempX * pointRotateCosZ) + ( tempY * pointRotateSinZ)))>>16)<<1;
		}

		{
			int tempX = x;
			int tempZ = z;

			x = ((((tempX * pointRotateSinY) - (tempZ * pointRotateCosY)))>>16)<<1;
			z = ((((tempX * pointRotateCosY) + (tempZ * pointRotateSinY)))>>16)<<1;
		}

		{
			int tempY = y;
			int tempZ = z;
			y = ((((tempY * pointRotateSinX ) - (tempZ * pointRotateCosX)))>>16)<<1;
			z = ((((tempY * pointRotateCosX ) + (tempZ * pointRotateSinX)))>>16)<<1;
		}

		*destX = x;
		*destY = y;
		*destZ = z;
	}
}

void zvRotSub(int X, int Y, int Z, int alpha, int beta, int gamma)
{
	if( alpha || beta || gamma )
	{
		setupPointRotate(alpha,beta,gamma);
		pointRotate(X, Y, Z, &animMoveX,&animMoveY,&animMoveZ);
	}
	else
	{
		animMoveX = X;
		animMoveY = Y;
		animMoveZ = Z;
	}
}

void getZvRot(char* bodyPtr, ZVStruct* zvPtr, int alpha, int beta, int gamma)
{
	int X1 = 32000;
	int Y1 = 32000;
	int Z1 = 32000;

	int X2 = -32000;
	int Y2 = -32000;
	int Z2 = -32000;

	int i;
	int tempX;
	int tempY;
	int tempZ;

	getZvNormal(bodyPtr, zvPtr);

	for(i=0;i<8;i++)
	{
		switch(i)
		{
		case 0:
			{
				tempX = zvPtr->ZVX1;
				tempY = zvPtr->ZVY1;
				tempZ = zvPtr->ZVZ1;
				break;
			}
		case 1:
			{
				tempZ = zvPtr->ZVZ2;
				break;
			}
		case 2:
			{
				tempX = zvPtr->ZVX2;
				break;
			}
		case 3:
			{
				tempZ = zvPtr->ZVZ1;
				break;
			}
		case 4:
			{
				tempY = zvPtr->ZVY2;
				break;
			}
		case 5:
			{
				tempX = zvPtr->ZVX1;
				break;
			}
		case 6:
			{
				tempZ = zvPtr->ZVZ2;
				break;
			}
		case 7:
			{
				tempX = zvPtr->ZVX2;
				break;
			}
		}

		zvRotSub(tempX, tempY, tempZ, alpha, beta, gamma);

		if(animMoveX<X1)
			X1 = animMoveX;

		if(animMoveX>X2)
			X2 = animMoveX;

		if(animMoveY<Y1)
			Y1 = animMoveY;

		if(animMoveY>Y2)
			Y2 = animMoveY;

		if(animMoveZ<Z1)
			Z1 = animMoveZ;

		if(animMoveZ>Z2)
			Z2 = animMoveZ;

	}

	zvPtr->ZVX1 = X1;
	zvPtr->ZVX2 = X2;
	zvPtr->ZVY1 = Y1;
	zvPtr->ZVY2 = Y2;
	zvPtr->ZVZ1 = Z1;
	zvPtr->ZVZ2 = Z2;
}

void copyZv(ZVStruct* source, ZVStruct* dest)
{
	memcpy(dest,source,sizeof(ZVStruct));
}

int copyObjectToActor(int flag2, int var1, int foundName, int flag, int x, int y, int z, int stage, int room, int alpha, int beta, int gamma, int var2, int var3, int var4, int var5)
{
	int i;
	int j;
	actorStruct* actorPtr = actorTable;
	char* bodyPtr;
	ZVStruct* zvPtr;

	for(i=0;i<50;i++)
	{
		if(actorPtr->field_0 == -1)
			break;

		actorPtr++;
	}

	if(i==50)
		return -1;

	currentProcessedActorPtr = actorPtr;
	currentProcessedActorIdx = i;

	actorPtr->bodyNum = flag2;
	actorPtr->flags = flag;
	actorPtr->stage = stage;
	actorPtr->room = room;
	actorPtr->worldX = actorPtr->roomX = x;
	actorPtr->worldY = actorPtr->roomY = y;
	actorPtr->worldZ = actorPtr->roomZ = z;

	if(room != currentDisplayedRoom)
	{
		char* roomPtr = etageVar0 + *(unsigned int*)(etageVar0+actorPtr->room*4);

		actorPtr->worldX -= ((*(short int*)(cameraPtr+4)) - (*(short int*)(roomPtr+4))) * 10;
		actorPtr->worldY += ((*(short int*)(cameraPtr+6)) - (*(short int*)(roomPtr+6))) * 10;
		actorPtr->worldZ += ((*(short int*)(cameraPtr+8)) - (*(short int*)(roomPtr+8))) * 10;
	}

	actorPtr->alpha = alpha;
	actorPtr->beta = beta;
	actorPtr->gamma = gamma;

	actorPtr->dynFlags = 1;
	
	actorPtr->ANIM = var2;
	actorPtr->FRAME = var3;

	actorPtr->field_40 = var4;
	actorPtr->field_42 = var5;

	actorPtr->END_FRAME = 1;
	actorPtr->END_ANIM = 1;
	actorPtr->field_44 = -1;
	actorPtr->field_46 = 0;
	actorPtr->field_48 = -1;

	actorPtr->modX = 0;
	actorPtr->modY = 0;
	actorPtr->modZ = 0;

	for(j=0;j<3;j++)
	{
		actorPtr->COL[j] = -1;
	}

	actorPtr->COL_BY = -1;
	actorPtr->HARD_DEC = -1;
	actorPtr->HARD_COL = -1;

	actorPtr->rotate.oldAngle = 0;
	actorPtr->rotate.newAngle = 0;
	actorPtr->rotate.param = 0;

	actorPtr->field_60.oldAngle = 0;
	actorPtr->field_60.newAngle = 0;
	actorPtr->field_60.param = 0;

	actorPtr->falling = 0;

	actorPtr->field_72 = 0;

	actorPtr->speed = 0;

	actorPtr->trackMode = 0;
	actorPtr->trackNumber = -1;

	actorPtr->animActionType = 0;
	actorPtr->HIT = -1;
	actorPtr->HIT_BY = -1;

	if(flag2 != -1)
	{
		bodyPtr = HQR_Get(listBody,actorPtr->bodyNum);

		if(var2 != -1)
		{
			char* animPtr = HQR_Get(listAnim,var2);

			initAnimInBody(var3,animPtr,bodyPtr);

			actorPtr->field_4C = getAnimParam(animPtr);
			actorPtr->END_ANIM = 0;
			actorPtr->flags |= 1;

//			computeScreenBox(actorPtr->field_22 + actorPtr->field_5A, actorPtr->field_24 + actorPtr->field_5C, actorPtr->field_26 + actorPtr->field_5E, actorPtr->alpha, actorPtr->beta, actorPtr->gamma, bodyPtr);

			if(BBox3D1<0)
				BBox3D1 = 0;

			if(BBox3D3>319)
				BBox3D3 = 319;

			if(BBox3D2<0)
				BBox3D2 = 0;

			if(BBox3D4>199)
				BBox3D4 = 199;

			actorPtr->field_14 = BBox3D1;
			actorPtr->field_16 = BBox3D2;
			actorPtr->field_18 = BBox3D3;
			actorPtr->field_1A = BBox3D4;
		}
		else
		{
			if(!(actorPtr->flags & 4))
			{
				actorPtr->flags &= 0xFFFE;
			}
		}
	}

	startChrono(&actorPtr->ROOM_CHRONO);
	startChrono(&actorPtr->CHRONO);

	zvPtr = &actorPtr->zv;

	switch(var1)
	{
	case 0:
		{
			if(flag2!=-1)
			{
				getZvMax(bodyPtr,zvPtr);
			}
			else
			{
				makeDefaultZV(zvPtr);
			}
			break;
		}
	case 1:
		{
			if(flag2!=-1)
			{
				getZvNormal(bodyPtr,zvPtr);
			}
			else
			{
				makeDefaultZV(zvPtr);
			}
			break;
		}
	case 2:
		{
			if(flag2!=-1)
			{
				getZvCube(bodyPtr,zvPtr);
			}
			else
			{
				makeDefaultZV(zvPtr);
			}
			break;
		}
	case 3:
		{
			if(flag2!=-1)
			{
				getZvRot(bodyPtr,zvPtr,alpha,beta,gamma);
			}
			else
			{
				makeDefaultZV(zvPtr);
			}
			break;
		}
	case 4:
		{
			char* roomDataPtr = etageVar0 + *(unsigned int*)(etageVar0 + room*4);
			int numElements;
			int j;

			roomDataPtr += *(short int*)roomDataPtr;

			numElements = *(short int*)roomDataPtr;
			roomDataPtr+=2;

			for(j=0;j<numElements;j++)
			{
				if(*(short int*)(roomDataPtr+0xE) == 9)
				{
					if(*(short int*)(roomDataPtr+0xC) == foundName)
					{
						int tempX;
						int tempY;
						int tempZ;
						
						copyZv((ZVStruct*)roomDataPtr,zvPtr);

						x = 0;
						y = 0;
						z = 0;

						tempX = ((*(short int*)roomDataPtr) + (*(short int*)(roomDataPtr+2)))/2;
						actorPtr->worldX = tempX;
						actorPtr->roomX = tempX;

						tempY = ((*(short int*)(roomDataPtr+4)) + (*(short int*)(roomDataPtr+6)))/2;
						actorPtr->worldY = tempY;
						actorPtr->roomY = tempY;

						tempZ = ((*(short int*)(roomDataPtr+8)) + (*(short int*)(roomDataPtr+0xA)))/2;
						actorPtr->worldZ = tempZ;
						actorPtr->roomZ = tempZ;

						if(room != currentDisplayedRoom)
						{
							char* roomPtr = etageVar0+room*4;

							actorPtr->worldX = ((*(short int*)(cameraPtr+4)) - (*(short int*)(roomPtr+4))) * 10;
							actorPtr->worldY = ((*(short int*)(cameraPtr+6)) - (*(short int*)(roomPtr+6))) * 10;
							actorPtr->worldZ = ((*(short int*)(cameraPtr+8)) - (*(short int*)(roomPtr+8))) * 10;
						}

						break;
					}
				}

				roomDataPtr+=0x10;
			}

			if(j==numElements)
			{
				makeDefaultZV(zvPtr);
			}

			break;
		}
	default:
		{
			printf("Unsupported ZV type in copyObjectToActor\n");
			exit(1);
			break;
		}
	}

	zvPtr->ZVX1 += x;
	zvPtr->ZVX2 += x;

	zvPtr->ZVY1 += y;
	zvPtr->ZVY2 += y;

	zvPtr->ZVZ1 += z;
	zvPtr->ZVZ2 += z;

	return(i);
}

void setupCameraSub4(void)
{
	copyToScreen(aux,aux2);

	//TODO: implementer la suite
}

void setMoveMode(int trackMode, int trackNumber)
{
	currentProcessedActorPtr->trackMode = trackMode;

	switch(trackMode)
	{
	case 2:
		{
			currentProcessedActorPtr->trackNumber = trackNumber;
			currentProcessedActorPtr->MARK = -1;
			break;
		}
	case 3:
		{
			currentProcessedActorPtr->trackNumber = trackNumber;
			currentProcessedActorPtr->positionInTrack = 0;
			currentProcessedActorPtr->MARK = -1;
			break;
		}
	}
}

void updateAllActorAndObjects()
{
	int i;
	actorStruct *currentActor = actorTable;
	objectStruct* currentObject;

	for(i=0;i<50;i++)
	{
		if(currentActor->field_0 != -1)
		{
			if(currentActor->stage == currentEtage)
			{
				if(currentActor->life != -1)
				{
					switch(currentActor->lifeMode)
					{
					case 0:
						{
							break;
						}
					case 1:
						{
							if(currentActor->room != currentDisplayedRoom)
							{
								updateAllActorAndObjectsSub1(i);
							}
							break;
						}
					case 2:
						{
							if(!setupCameraSub1Sub1(currentActor->room))
							{
								updateAllActorAndObjectsSub1(i);
							}
							break;
						}
					default:
						{
							updateAllActorAndObjectsSub1(i);
							break;
						}
					}
				}
				else
				{
					if(!setupCameraSub1Sub1(currentActor->room))
					{
						updateAllActorAndObjectsSub1(i);
					}
				}
			}
			else
			{
				updateAllActorAndObjectsSub1(i);
			}
		}

		currentActor++;
	}

	currentObject = objectTable;

	for(i=0;i<maxObjects;i++)
	{
		if(currentObject->ownerIdx != -1)
		{
			if(currentCameraTarget == i)
			{
				genVar9 = currentObject->ownerIdx;
			}
		}
		else
		{
			if(currentObject->stage == currentEtage)
			{
				if(currentObject->life != -1)
				{
					if(currentObject->lifeMode != -1)
					{
						int actorIdx;
						
						switch(currentObject->lifeMode)
						{
						case 1:
							{
								if(currentObject->room != currentDisplayedRoom)
								{
									currentObject++;
									continue;
								}
								break;
							}
						case 2:
							{
								if(!setupCameraSub1Sub1(currentObject->room))
								{
									currentObject++;
									continue;
								}
								break;
							}
						}

						//int var_C = currentObject->flags & 0xFFDF;
						//int var_E = currentObject->field_2;
						//int var_A = currentObject->field_26;

addObject:				actorIdx = copyObjectToActor(	currentObject->field_2, currentObject->field_6, currentObject->foundName,
															currentObject->flags & 0xFFDF,
															currentObject->x, currentObject->y, currentObject->z,
															currentObject->stage, currentObject->room,
															currentObject->alpha, currentObject->beta, currentObject->gamma,
															currentObject->field_26,
															currentObject->field_28, currentObject->field_2A, currentObject->field_2C);

						currentObject->ownerIdx = actorIdx;

						if(actorIdx != -1)
						{
							currentProcessedActorPtr = &actorTable[actorIdx];
							currentProcessedActorIdx = actorIdx;

							if(currentCameraTarget == i)
							{
								genVar9 = currentProcessedActorIdx;
							}

							currentProcessedActorPtr->dynFlags = (currentObject->flags & 0x20) / 0x20; // recheck
							currentProcessedActorPtr->life = currentObject->life;
							currentProcessedActorPtr->lifeMode = currentObject->lifeMode;

							currentProcessedActorPtr->field_0 = i;

							setMoveMode(currentObject->trackMode, currentObject->trackNumber);

							currentProcessedActorPtr->positionInTrack = currentObject->positionInTrack;

							actorTurnedToObj = 1;
						}
					}
				}
				else
				{
					if(setupCameraSub1Sub1(currentObject->room))
						goto addObject;
				}
			}
		}

		currentObject++;
	}

//	objModifFlag1 = 0;

	//TODO: object update
}

int checkActorInRoom(int room)
{
	int i;

	char* ptr = roomVar5[currentCamera] + 18;

	int var2 = *(short int*)ptr;
	ptr+=2;

	for(i=0;i<var2;i++)
	{
		if(*(short int*)ptr == room)
		{
			return(1);
		}

		ptr+=0xC;
	}

	return(0);

}

void createActorList()
{
	int i;
	actorStruct* actorPtr;
	
	numActorInList = 0;

	actorPtr = actorTable;

	for(i=0;i<50;i++)
	{
		if(actorPtr->field_0 != -1 && actorPtr->bodyNum != -1)
		{
			if(checkActorInRoom(actorPtr->room))
			{
				sortedActorTable[numActorInList] = i;
				if(!(actorPtr->flags & 0x21))
				{
					actorPtr->flags |= 8;
				//	objModifFlag2 = 1;
				}
				numActorInList++;
			}
		}

		actorPtr++;
	}
}

void setupCamera()
{
	char* ptr;
	int x;
	int y;
	int z;
	
	freezeTime();

	currentCamera = startGameVar1;

	loadCamera(*(cameraPtr+(startGameVar1+6)*2));

	ptr = roomVar5[currentCamera];

	setupPointTransformSM(*(short int*)(ptr),*(short int*)(ptr+2),*(short int*)(ptr+4));
	ptr+= 6;

	//TODO: recheck
	x = (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+4))) << 3) + (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+4))) << 1);
	ptr+=2;
	y = (((*(short int*)(cameraPtr+6)) - (*(short int*)(ptr))) << 3) + (((*(short int*)(cameraPtr+6)) - (*(short int*)(ptr))) << 1);
	ptr+=2;
	z = (((*(short int*)(cameraPtr+8)) - (*(short int*)(ptr))) << 3) + (((*(short int*)(cameraPtr+8)) - (*(short int*)(ptr))) << 1);
	ptr+=2;
//
	//setupSelfModifyingCode(x-5000,y,z-5000); // debug intro
//
	setupSelfModifyingCode(x,y,z);

	setupSMCode(160,100,*(short int*)(ptr),*(short int*)(ptr+2),*(short int*)(ptr+4));
	ptr+=6;

	setupCameraSub1();
	updateAllActorAndObjects();
	createActorList();
//	setupCameraSub3();
	setupCameraSub4();
/*	setupCameraSub5();
*/
	if(mainVar1==2)
	{
		setupCameraVar1 = 2;
	}
	else
	{
		if(setupCameraVar1!=2)
		{
			setupCameraVar1 = 1;
		}
	}

	mainVar1 = 0;
	unfreezeTime();
}

short int computeDistanceToPoint(int x1, int z1, int x2, int z2)
{
	int axBackup = x1;
	x1 -= x2;
	if((short int)x1 < 0)
	{
		x1 = -(short int)x1;
	}

	z1 -= z2;
	if((short int)z1 < 0)
	{
		z1 = -(short int)z1;
	}

	if((x1+z1)> 0xFFFF)
	{
		return(0x7D00);
	}
	else
	{
		return(x1+z1);
	}
}

void startActorRotation(short int beta, short int newBeta, short int param, rotateStruct* rotatePtr)
{
	rotatePtr->oldAngle = beta;
	rotatePtr->newAngle = newBeta;
	rotatePtr->param = param;
	rotatePtr->timeOfRotate = timer;
}

short int updateActorRotation(rotateStruct* rotatePtr)
{
	int timeDif;
	int angleDif;
	
	if(!rotatePtr->param)
		return(rotatePtr->newAngle);

	timeDif = timer - rotatePtr->timeOfRotate;

	if(timeDif>rotatePtr->param)
	{
		rotatePtr->param = 0;
		return(rotatePtr->newAngle);
	}

	angleDif = (rotatePtr->newAngle&0x3FF) - (rotatePtr->oldAngle&0x3FF);

	if(angleDif<=0x200)
	{
		if(angleDif>=-0x200)
		{
			int angle = (rotatePtr->newAngle&0x3FF) - (rotatePtr->oldAngle&0x3FF);
			return (rotatePtr->oldAngle&0x3FF) + (angle*timeDif)/rotatePtr->param;
		}
		else
		{
			short int angle = ((rotatePtr->newAngle&0x3FF)+0x400) - ((rotatePtr->oldAngle&0x3FF));
			return (((rotatePtr->oldAngle&0x3FF)) + ((angle*timeDif)/rotatePtr->param));
		}
	}
	else
	{
		int angle = (rotatePtr->newAngle&0x3FF) - ((rotatePtr->oldAngle&0x3FF)+0x400);
		return ((angle*timeDif)/rotatePtr->param) + ((rotatePtr->oldAngle&0x3FF));
	}
}

void deleteSub(int actorIdx)
{
	actorStruct* actorPtr = &actorTable[actorIdx];

	actorPtr->flags &= 0xFFF7;

//	objModifFlag2 = 1;

	BBox3D1 = actorPtr->field_14;

	if(BBox3D1 > -1)
	{
		BBox3D2 = actorPtr->field_16;
		BBox3D3 = actorPtr->field_18;
		BBox3D4 = actorPtr->field_1A;

		//deleteSubSub();
	}
}

int findObjectInInventory(int objIdx)
{
	int i;

	for(i=0;i<numObjInInventory;i++)
	{
		if(inventory[i] == objIdx)
		{
			return(i);
		}
	}

	return(-1);
}

void removeObjFromInventory(int objIdx)
{
	int inventoryIdx;

	inventoryIdx = findObjectInInventory(objIdx);

	if(inventoryIdx != -1)
	{
		memmove(&inventory[inventoryIdx],&inventory[inventoryIdx+1],(30-inventoryIdx-1)*2);

		numObjInInventory--;
	}
}

void deleteObject(int objIdx)
{
	objectStruct* objPtr;
	int actorIdx;
	actorStruct* actorPtr;

	objPtr = &objectTable[objIdx];
	actorIdx = objPtr->ownerIdx;

	if(actorIdx != -1)
	{
		actorPtr = &actorTable[actorIdx];

		actorPtr->room = -1;
		actorPtr->stage = -1;

//		objModifFlag1 = 1;

		if(actorPtr->flags & 8)
		{
			deleteSub(actorIdx);
		}
	}

	objPtr->room = -1;
	objPtr->stage = -1;

	removeObjFromInventory(objIdx);
}

int anim(int animNum,int arg_2, int arg_4)
{
	if(animNum == currentProcessedActorPtr->ANIM)
	{
		if(!(currentProcessedActorPtr->flags & 1))
		{
			if(currentProcessedActorPtr->flags & 8)
			{
				deleteSub(currentProcessedActorIdx);
			}

			currentProcessedActorPtr->flags |= 1;

			initAnimInBody(currentProcessedActorPtr->FRAME, HQR_Get(listAnim,animNum), HQR_Get(listBody, currentProcessedActorPtr->bodyNum));

			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;
		}
		else
		{
			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;

			return(0);
		}
	}
	else
	{
		if(animNum == -1)
		{
			currentProcessedActorPtr->field_44 = -2;
			return(1);
		}

		if(!(currentProcessedActorPtr->flags & 1))
		{
			if(currentProcessedActorPtr->flags & 8)
			{
				deleteSub(currentProcessedActorIdx);
			}

			currentProcessedActorPtr->flags |= 1;

			initAnimInBody(currentProcessedActorPtr->FRAME, HQR_Get(listAnim,animNum), HQR_Get(listBody, currentProcessedActorPtr->bodyNum));

			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;
		}
		else
		{
			if((currentProcessedActorPtr->field_40 & 2) || (currentProcessedActorPtr->field_46 & 2))
				return(0);

		}

		currentProcessedActorPtr->field_44 = animNum;
		currentProcessedActorPtr->field_46 = arg_2;
		currentProcessedActorPtr->field_48 = arg_4;
	}

	return(1);
}


void line(int x1, int y1, int x2, int y2, char c);

#ifdef USE_GL
void drawProjectedLine(float x1, float y1, float z1, float x2, float y2, float z2,int c)
#else
void drawProjectedLine(int x1, int y1, int z1, int x2, int y2, int z2,int c)
#endif
{
#ifdef USE_GL
	float transformedX1;
	float transformedX2;

	float transformedY1;
	float transformedY2;
#else
	int transformedX1;
	int transformedX2;

	int transformedY1;
	int transformedY2;
#endif


	x1 -= translateX;
	x2 -= translateX;

	y1 -= translateY;
	y2 -= translateY;

	z1 -= translateZ;
	z2 -= translateZ;

	transformPoint(&x1,&y1,&z1);
	transformPoint(&x2,&y2,&z2);

	z1 += cameraX;
	z2 += cameraX;

#ifdef USE_GL
	transformedX1 = ((x1 * cameraY) / (float)z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / (float)z2) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / (float)z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / (float)z2) + cameraCenterY;
#else
	transformedX1 = ((x1 * cameraY) / z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / z2) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / z2) + cameraCenterY;
#endif

#ifdef USE_GL
	if(z1>0 && z2>0)
		osystem_draw3dLine(transformedX1,transformedY1,z1,transformedX2,transformedY2,z2,c);
#else
	if(z1>0 && z2>0)
		line(transformedX1,transformedY1,transformedX2,transformedY2,c);
#endif
}

void drawZv(actorStruct* actorPtr)
{
	ZVStruct localZv;

	if( actorPtr->room != actorTable[genVar9].room )
	{
		getZvRelativePosition(&localZv, actorPtr->room, actorTable[genVar9].room);
	}
	else
	{
		copyZv(&actorPtr->zv,&localZv);
	}


	// bottom
	drawProjectedLine(	localZv.ZVX1,
						localZv.ZVY2,
						localZv.ZVZ1,
						localZv.ZVX1,
						localZv.ZVY2,
						localZv.ZVZ2,
						10);

	drawProjectedLine(localZv.ZVX1,localZv.ZVY2,localZv.ZVZ2,localZv.ZVX2,localZv.ZVY2,localZv.ZVZ2,10);
	drawProjectedLine(localZv.ZVX2,localZv.ZVY2,localZv.ZVZ2,localZv.ZVX2,localZv.ZVY2,localZv.ZVZ1,10);
	drawProjectedLine(localZv.ZVX2,localZv.ZVY2,localZv.ZVZ1,localZv.ZVX1,localZv.ZVY2,localZv.ZVZ1,10);

	// top
	drawProjectedLine(localZv.ZVX1,localZv.ZVY1,localZv.ZVZ1,localZv.ZVX1,localZv.ZVY1,localZv.ZVZ2,10);
	drawProjectedLine(localZv.ZVX1,localZv.ZVY1,localZv.ZVZ2,localZv.ZVX2,localZv.ZVY1,localZv.ZVZ2,10);
	drawProjectedLine(localZv.ZVX2,localZv.ZVY1,localZv.ZVZ2,localZv.ZVX2,localZv.ZVY1,localZv.ZVZ1,10);
	drawProjectedLine(localZv.ZVX2,localZv.ZVY1,localZv.ZVZ1,localZv.ZVX1,localZv.ZVY1,localZv.ZVZ1,10);

	drawProjectedLine(	localZv.ZVX1,localZv.ZVY2,localZv.ZVZ1,
						localZv.ZVX1,localZv.ZVY1,localZv.ZVZ1,10);
	drawProjectedLine(	localZv.ZVX1,localZv.ZVY2,localZv.ZVZ2,
						localZv.ZVX1,localZv.ZVY1,localZv.ZVZ2,10);
	drawProjectedLine(	localZv.ZVX2,localZv.ZVY2,localZv.ZVZ2,
						localZv.ZVX2,localZv.ZVY1,localZv.ZVZ2,10);
	drawProjectedLine(	localZv.ZVX2,localZv.ZVY2,localZv.ZVZ1,
						localZv.ZVX2,localZv.ZVY1,localZv.ZVZ1,10);


}

void drawConverZone(char* src, int param)
{
	int i;

	for(i=0;i<param;i++)
	{
		int j;
		char* dest = cameraBuffer;

		int var1 = *(short int*)src;
		src+=2;

		memcpy(dest,src,var1<<2);
		dest+= var1<<2;

		*(short int*)dest = *(short int*)src;
		dest+=2;
		*(short int*)dest = *(short int*)(src+2);
		dest+=2;

		src += (var1)<<2;

		dest = cameraBuffer;

		for(j=0;j<var1;j++)
		{
			int zoneX1;
			int zoneZ1;
			int zoneX2;
			int zoneZ2;
			
			zoneX1= *(short int*)dest;
			dest+=2;
			zoneZ1= *(short int*)dest;
			dest+=2;
			zoneX2= *(short int*)dest;
			zoneZ2= *(short int*)(dest+2);

			drawProjectedLine(zoneX1*10,0,zoneZ1*10,zoneX2*10,0,zoneZ2*10,20);
		}
	}
}

void drawConverZones()
{
	int i;
	for(i=0;i<numCameraInRoom;i++)
	{
		char* cameraDataPtr = roomVar5[i] + 2*roomVar6[i];

		drawConverZone(cameraDataPtr+2,*(short int*)cameraDataPtr);
	}
}

#ifdef USE_GL
void drawProjectedQuad(float x1,float x2, float x3, float x4, float y1,float y2, float y3, float y4, float z1,float z2, float z3, float z4, int color)
{
	float transformedX1;
	float transformedX2;
	float transformedX3;
	float transformedX4;

	float transformedY1;
	float transformedY2;
	float transformedY3;
	float transformedY4;
	
	x1 -= translateX;
	x2 -= translateX;
	x3 -= translateX;
	x4 -= translateX;

	y1 -= translateY;
	y2 -= translateY;
	y3 -= translateY;
	y4 -= translateY;

	z1 -= translateZ;
	z2 -= translateZ;
	z3 -= translateZ;
	z4 -= translateZ;

	transformPoint(&x1,&y1,&z1);
	transformPoint(&x2,&y2,&z2);
	transformPoint(&x3,&y3,&z3);
	transformPoint(&x4,&y4,&z4);

	z1 += cameraX;
	z2 += cameraX;
	z3 += cameraX;
	z4 += cameraX;

	transformedX1 = ((x1 * cameraY) / (float)z1) + cameraCenterX;
	transformedX2 = ((x2 * cameraY) / (float)z2) + cameraCenterX;
	transformedX3 = ((x3 * cameraY) / (float)z3) + cameraCenterX;
	transformedX4 = ((x4 * cameraY) / (float)z4) + cameraCenterX;

	transformedY1 = ((y1 * cameraZ) / (float)z1) + cameraCenterY;
	transformedY2 = ((y2 * cameraZ) / (float)z2) + cameraCenterY;
	transformedY3 = ((y3 * cameraZ) / (float)z3) + cameraCenterY;
	transformedY4 = ((y4 * cameraZ) / (float)z4) + cameraCenterY;

	if(z1>500 && z2>500 && z3>500 && z4>500)
		osystem_draw3dQuad(transformedX1,transformedY1,z1, transformedX2,transformedY2,z2, transformedX3,transformedY3,z3, transformedX4,transformedY4,z4, color);
}

void drawProjectedBox(int x1,int x2,int y1,int y2,int z1,int z2, int color)
{
	//bottom
	drawProjectedQuad(x1,x1,x2,x2,y1,y1,y1,y1,z1,z2,z2,z1,color);
	//top
	drawProjectedQuad(x1,x1,x2,x2,y2,y2,y2,y2,z1,z2,z2,z1,color);
	//left
	drawProjectedQuad(x1,x1,x1,x1,y1,y2,y2,y1,z1,z1,z2,z2,color);
	//right
	drawProjectedQuad(x2,x2,x2,x2,y1,y2,y2,y1,z1,z1,z2,z2,color);
	//front
	drawProjectedQuad(x1,x2,x2,x1,y1,y1,y2,y2,z1,z1,z1,z1,color);
	//back
	drawProjectedQuad(x1,x2,x2,x1,y1,y1,y2,y2,z2,z2,z2,z2,color);
}
#endif

void drawZone(char* zoneData,int color)
{
	int x1;
	int x2;

	int y1;
	int y2;

	int z1;
	int z2;

	int type;

	type = *(short int*)(zoneData+0xE);

	x1 = *( short int*)(zoneData+0x0);
	x2 = *( short int*)(zoneData+0x2);
	y1 = *( short int*)(zoneData+0x4);
	y2 = *( short int*)(zoneData+0x6);
	z1 = *( short int*)(zoneData+0x8);
	z2 = *( short int*)(zoneData+0xA);

#ifdef USE_GL
	drawProjectedBox(x1,x2,y1,y2,z1,z2,type);
#else
	drawProjectedLine(x1,y1,z1,x1,y1,z2,type);
	drawProjectedLine(x1,y1,z2,x2,y1,z2,type);
	drawProjectedLine(x2,y1,z2,x2,y1,z1,type);
	drawProjectedLine(x2,y1,z1,x1,y1,z1,type);

	drawProjectedLine(x1,y2,z1,x1,y2,z2,type);
	drawProjectedLine(x1,y2,z2,x2,y2,z2,type);
	drawProjectedLine(x2,y2,z2,x2,y2,z1,type);
	drawProjectedLine(x2,y2,z1,x1,y2,z1,type);

	drawProjectedLine(x1,y1,z1,x1,y2,z1,type);
	drawProjectedLine(x1,y1,z2,x1,y2,z2,type);
	drawProjectedLine(x2,y1,z2,x2,y2,z2,type);
	drawProjectedLine(x2,y1,z1,x2,y2,z1,type);
#endif
}

void drawOverlayZone(char* zoneData,int color)
{
	int x1;
	int x2;

	int y1;
	int y2;

	int z1;
	int z2;

	x1 = *( short int*)(zoneData+0x0) * 10;
	z1 = *( short int*)(zoneData+0x2) * 10;
	x2 = *( short int*)(zoneData+0x4) * 10;
	z2 = *( short int*)(zoneData+0x6) * 10;

	y1=0;
	y2=0;

#ifdef USE_GL
	drawProjectedBox(x1,x2,y1,y2,z1,z2,color);
#else
	drawProjectedLine(x1,y1,z1,x1,y1,z2,color);
	drawProjectedLine(x1,y1,z2,x2,y1,z2,color);
	drawProjectedLine(x2,y1,z2,x2,y1,z1,color);
	drawProjectedLine(x2,y1,z1,x1,y1,z1,color);

	drawProjectedLine(x1,y2,z1,x1,y2,z2,color);
	drawProjectedLine(x1,y2,z2,x2,y2,z2,color);
	drawProjectedLine(x2,y2,z2,x2,y2,z1,color);
	drawProjectedLine(x2,y2,z1,x1,y2,z1,color);

	drawProjectedLine(x1,y1,z1,x1,y2,z1,color);
	drawProjectedLine(x1,y1,z2,x1,y2,z2,color);
	drawProjectedLine(x2,y1,z2,x2,y2,z2,color);
	drawProjectedLine(x2,y1,z1,x2,y2,z1,color);
#endif
}

void drawHardCol()
{
	char* data = etageVar0 + *(unsigned int*)(etageVar0 + actorTable[genVar9].room * 4);
	short int numHardCol;
	int i;
	
	data += *(short int*)(data);

	numHardCol = *(short int*)data;
	data+=2;

	for(i=0;i<numHardCol;i++)
	{
		drawZone(data,100);
		data+=0x10;
	}
}

void drawZones()
{
	int i;
	int numZones;
	
	char* zoneData = etageVar0 + *(unsigned int*)(etageVar0 + actorTable[genVar9].room *4);
	zoneData += *(short int*)(zoneData+2);

	numZones = *(short int*)zoneData;
	zoneData+=2;

	for(i=0;i<numZones;i++)
	{
		drawZone(zoneData,40);
		zoneData+=16;
	}
}

void drawOverlayZones()
{
	char* etageData = etageVar0 + *(unsigned int*)(etageVar0 + actorTable[genVar9].room * 4);
	short int numEntry;
	int i;
	
	etageData += *(short int*)etageData;

	numEntry = *(short int*)(etageData);
	etageData += 2;

	for(i=0;i<numEntry;i++)
	{
		drawZone(etageData,60);
		etageData+=16;
	}

}

int isBgOverlayRequired( int X1, int X2, int Z1, int Z2, char* data, int param )
{
	int i;
	for(i=0;i<param;i++)
	{
////////////////////////////////////// DEBUG
	//	drawOverlayZone(data, 80);
/////////////////////////////////////

		int zoneX1 = *(short int*)(data);
		int zoneZ1 = *(short int*)(data+2);
		int zoneX2 = *(short int*)(data+4);
		int zoneZ2 = *(short int*)(data+6);

		if(X1 >= zoneX1 && Z1 >= zoneZ1 && X2 <= zoneX2 && Z2 <= zoneZ2)
		{
			return(1);
		}

		data+=0x8;
	}

	return(0);
}

void drawBgOverlaySub2(int size)
{
	int bx = 32767;
	int bp = 32767;
	int cx = -32768;
	int dx = -32768;
	short int* out;
	int tempBxPtr;
	int tempCxPtr;

	char* si;

	int i;
	int saveDx;
	int saveAx;

	char* tempBufferSE;

	int direction = 1;

	short int* data = (short int*)cameraBuffer;
	
	overlaySize1 = size;
	overlaySize2 = size;

	bgOverlayVar1 = 0;

	for(i=0;i<size;i++)
	{
		int temp = data[0];

		if(temp<bx)
			bx = temp;
		if(temp>dx)
			dx = temp;
	
		temp = data[1];

		if(temp<bp)
			bp = temp;
		if(temp>cx)
			cx = temp;

		data += 2;
	}

	out = data;
	data = (short int*)cameraBuffer;

	out[0] = data[0];
	out[1] = data[1];

	out+=4;
	data+=4;

	if(cx == bp)
	{
		return;
	}

	cameraBufferPtr = cameraBuffer;
	cameraBuffer2Ptr = cameraBuffer2;
	cameraBuffer3Ptr = cameraBuffer3;

	si = cameraBufferPtr;
	tempBxPtr = *(short int*)si;
	si += 2;
	tempCxPtr = *(short int*)si;
	si += 2;

	tempBufferSE = cameraBuffer4;

	direction = 1;

	osystem_startBgPoly();

	do
	{
		int dx;
		int ax;
		
		saveDx = *(short int*)si;
		si += 2;
		saveAx = *(short int*)si;
		si += 2;

		//osystem_draw3dLine(tempBxPtr, tempCxPtr, 0, saveDx,saveAx, 0, 130);
		osystem_addBgPolyPoint(tempBxPtr, tempCxPtr);

		dx = saveDx;
		ax = saveAx;

		if(ax != tempCxPtr)
		{
			char* ptr1;
			if(tempCxPtr == bp || tempCxPtr == cx)
			{
				char* temp = cameraBuffer3Ptr;
				cameraBuffer3Ptr = tempBufferSE;
				tempBufferSE = temp;
			}

			if(tempBxPtr>=dx)
			{
				int temp;
				temp = tempBxPtr;
				tempBxPtr = dx;
				dx = temp;

				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			ptr1 = tempBufferSE+tempCxPtr*2;

			if(tempCxPtr > ax)
			{
				int temp;
				direction = -1;
				

				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			{ // stupid, need optimisation
				int temp;

				temp = tempCxPtr;
				tempCxPtr = ax;
				ax = temp;
			}

			tempCxPtr -= ax;

			*(short int*)ptr1 = tempBxPtr;
			ptr1+=2*direction;

			ax = tempBxPtr;

			dx -= tempBxPtr;
			tempBxPtr = dx;
			dx = tempCxPtr;
			bp = tempCxPtr / 2;

			do
			{
				bp += tempBxPtr;

				do
				{
					if(bp<dx)
						break;

					bp -= dx;
					ax ++;
				}while(1);

				*(short int*)ptr1 = ax;
				ptr1+=2*direction;
			}while(--tempCxPtr);

			direction = 1;
		}

		tempCxPtr = saveAx;
		tempBxPtr = saveDx;
	}while(--overlaySize1);

	osystem_endBgPoly();
}

void drawBgOverlay(actorStruct* actorPtr)
{
	char* data;
	char* data2;
	int numEntry;
	int i;
	int numOverlayZone;
	
	actorPtr->field_14 = BBox3D1;
	actorPtr->field_16 = BBox3D2;
	actorPtr->field_18 = BBox3D3;
	actorPtr->field_1A = BBox3D4;

	setClipSize(BBox3D1, BBox3D2, BBox3D3, BBox3D4);

	data = roomVar5[currentCamera] + 0x12;
	numEntry = *(short int*)(data);
	data+=2;

	while(numEntry>0)
	{
		if(actorPtr->room == *(short int*)(data))
		{
			break;
		}
		data+=12;
		numEntry--;
	}

	if(numEntry == 0)
		return;

	data+=2;

	data2 = roomVar5[currentCamera] + *(unsigned short int*)(data);
	data = data2;
	data+=2;

	numOverlayZone = *(short int*)(data2);

	for(i=0;i<numOverlayZone;i++)
	{
		int numOverlay;
		char* src = data2 + *(unsigned short int*)(data+2);

		if(isBgOverlayRequired(	actorPtr->zv.ZVX1 / 10, actorPtr->zv.ZVX2 / 10,
								actorPtr->zv.ZVZ1 / 10, actorPtr->zv.ZVZ2 / 10,
								data+4,
								*(short int*)(data) ))
		{
			int j;
			numOverlay = *(short int*)src;
			src += 2;

			for(j=0;j<numOverlay;j++)
			{
				int param = *(short int*)(src);
				src+=2;

				memcpy(cameraBuffer, src, param*4);

				src+=param*4;

				drawBgOverlaySub2(param);
			}

//			blitOverlay(src);
		}

		numOverlay = *(short int*)(data);
		data+=2;
		data+=((numOverlay*4)+1)*2;
	}

	setClipSize(0,0,319,199);
}

void mainDrawSub2(int actorIdx) // draw flow
{
	actorStruct* actorPtr = &actorTable[actorIdx];

	char* data = printTextSub2(hqrUnk, actorPtr->FRAME);

	// TODO: finish
}

void mainDraw(int mode)
{
	int i;
#ifdef USE_GL
	if(mode == 2)
		osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
#endif


	if(mode== 0)
	{
		//restoreDirtyRects();
	}
	else
	{
		genVar5 = 0;
		copyToScreen(aux2,screen);
	}

#ifdef USE_GL
	osystem_startFrame();
#endif

	setClipSize(0,0,319,199);
	genVar6 = 0;

	//drawHardCol();
#ifdef USE_GL
	osystem_cleanScreenKeepZBuffer();
#endif

	//drawConverZones();
	//drawZones();
	//drawOverlayZones();
	

#ifdef USE_GL
	osystem_startModelRender();
#endif

	for(i=0;i<numActorInList;i++)
	{
		int currentDrawActor = sortedActorTable[i];
		actorStruct* actorPtr;

		actorPtr = &actorTable[currentDrawActor];

		//if(actorPtr->flags & 0x25)
		{
			actorPtr->flags &= 0xFFFB;
			
			if(actorPtr->flags & 0x20)
			{
				mainDrawSub2(currentDrawActor);
			}
			else 
			{
				char* bodyPtr = HQR_Get(listBody,actorPtr->bodyNum);

				if(hqrVar1)
				{
					initAnimInBody(actorPtr->FRAME, HQR_Get(listAnim, actorPtr->ANIM), bodyPtr);
				}

				renderModel(actorPtr->worldX + actorPtr->modX, actorPtr->worldY + actorPtr->modY, actorPtr->worldZ + actorPtr->modZ,
							actorPtr->alpha, actorPtr->beta, actorPtr->gamma, bodyPtr);

				if(actorPtr->animActionType && actorPtr->field_98 != -1)
				{
					//TODO: implement special
//					mainDrawSub3(actorPtr->field_98, bodyPtr, &actorPtr->field_9A);
				}

///////////////////////////////////// DEBUG
		//		drawZv(actorPtr);
/////////////////////////////////////
			}

			if(BBox3D1 < 0)
				BBox3D1 = 0;
			if(BBox3D3 > 319)
				BBox3D3 = 319;
			if(BBox3D2 < 0)
				BBox3D2 = 0;
			if(BBox3D4 > 199)
				BBox3D4 = 199;

			if(BBox3D1<=319 && BBox3D2<=199 && BBox3D3>=0 && BBox3D4>=0) // is the character on screen ?
			{
				if(actorPtr->field_0 == defines.field_1A)
				{
					mainVar3 = (BBox3D3 + BBox3D1) / 2;
					mainVar2 = (BBox3D4 + BBox3D2) / 2;
				}

				drawBgOverlay(actorPtr);
				//addToRedrawBox();
			}
			else
			{
				actorPtr->field_1A = -1;
				actorPtr->field_18 = -1;
				actorPtr->field_16 = -1;
				actorPtr->field_14 = -1;
			}
		}
	}

#ifdef USE_GL
	osystem_stopModelRender();
#endif

	if(drawTextOverlay())
	{
		//addToRedrawBox();
	}

	if(!lightVar1)
	{
		if(mode)
		{
			if(mode!=2 || lightVar2)
			{
				//makeBlackPalette();
				flipScreen();
				make3dTatouUnk1(0x10,0);
				lightVar2 = 0;
			}
			else
			{
				//flipScreen();
			}
		}
		else
		{
			//mainDrawSub1();
		}
	}
	else
	{
	}

#ifdef USE_GL
	osystem_stopFrame();
#endif

	flipScreen();
}

void initBufferAnim(char* buffer, char* bodyPtr)
{
	if((*(short int*)bodyPtr) & 2)
	{
		char* source = bodyPtr+0x10;
		short int ax;
		int cx;
		int i;

		*(unsigned short int*)(source+4) = (unsigned short int)timer;
		*(char**)(source) = buffer;

		source += *(short int*)(source-2);

		ax = *(short int*)(source);

		ax = (((ax * 2) + ax)*2)+2;

		source += ax;

		cx = *(short int*)source;
		
		source += cx*2;

		buffer+= 8;
		source+= 10;

		for(i=0;i<cx;i++)
		{
			*(short int*)(buffer) = *(short int*)(source);
			*(short int*)(buffer+2) = *(short int*)(source+2);
			*(short int*)(buffer+4) = *(short int*)(source+4);
			*(short int*)(buffer+6) = *(short int*)(source+6);

			buffer+=8;
			source+=8;

			source+=8;
		}

	}
}

short int getAnimParam(char* animPtr)
{
	return(*(short int*)animPtr);
}

short int getAnimType(char** bodyPtr)
{
	short int temp = *(short int*)animVar1;

	animVar1+=2;

	animVar4+=2;

	*(short int*)(*bodyPtr) = temp;
	(*bodyPtr)+=2;

	return(temp);
}

void processAnimRotation(char** bodyPtr, int bp, int bx)
{
	short int oldRotation = *(short int*)animVar4;
	short int newRotation;
	short int diff;
	
	animVar4+=2;

	newRotation = *(short int*)animVar1;
	animVar1+=2;

	diff = newRotation - oldRotation;

	if(diff == 0)
	{
		*(short int*)(*bodyPtr) = newRotation;
	}
	else
	{
		if(diff <= 0x200)
		{
			if(diff >= -0x200)
			{
				*(short int*)(*bodyPtr) = ((diff*bp)/bx) + oldRotation;
			}
			else
			{
				newRotation += 0x400;
				newRotation -= oldRotation;

				*(short int*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
			}
		}
		else
		{
			oldRotation += 0x400;
			newRotation -= oldRotation;

			*(short int*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
		}
	}

	(*bodyPtr)+=2;
}

void processAnimTranslation(char** bodyPtr, int bp, int bx)
{
	short int cx = *(short int*)animVar4;
	short int ax;
	animVar4+=2;

	ax = *(short int*)animVar1;
	animVar1+=2;

	if(ax == cx)
	{
		*(short int*)(*bodyPtr) = ax;
	}
	else
	{
		*(short int*)(*bodyPtr) = (((ax - cx)*bp)/bx) + cx;
	}

	(*bodyPtr)+=2;
}

short int processAnim(int frame, char* animPtr, char* bodyPtr)
{
	int numOfBonesInAnim = *(short int*)(animPtr+2);
	unsigned short int keyframeLength;
	unsigned short int timeOfKeyframeStart;
	char* animBufferPtr;
	int ax;
	unsigned short int bx;
	unsigned short int time;
	int bp;
	
	animPtr+=4;

	animPtr += ((numOfBonesInAnim+1)*8)*frame; // seek to keyframe

	// animVar1 = ptr to the current keyFrame
	animVar1 = animPtr;

	keyframeLength = *(unsigned short int*)animPtr; // keyframe length

	if(!((*(short int*)bodyPtr) & 2)) // do not anim if the model can't be animated
	{
		return(0);
	}

	bodyPtr+=16;

	animVar3 = bodyPtr;

	timeOfKeyframeStart = *(unsigned short int*)(bodyPtr+4); // time of start of keyframe
	
	animBufferPtr = *(char**)(bodyPtr);

	if(!animBufferPtr)
	{
		animBufferPtr = animVar1;
	}

	// animVar4 = ptr to previous key frame
	animVar4 = animBufferPtr;

	bodyPtr+= *(short int*)(bodyPtr-2);

	ax = *(short int*)bodyPtr;
	ax = (ax*6)+2;
	bodyPtr+=ax; // skip the points data

	ax = *(short int*)bodyPtr; // num of bones
	bx = ax;
	bodyPtr+=bx*2; // skip bones idx table

	if(numOfBonesInAnim > ax)
	{
		numOfBonesInAnim = ax;
	}

	bodyPtr+=10; // skip bone 0

	time = (unsigned short int)timer - timeOfKeyframeStart;

	bx = keyframeLength;
	bp = time;

	if(time<keyframeLength) // interpole keyframe
	{
		char* animVar1Backup = animVar1;
		// skip bone 0 anim
		animVar4 += 8; // anim buffer
		animVar1 += 8; // current keyframe ptr

		do
		{
			switch(getAnimType(&bodyPtr))
			{
			case 0:
				{
					processAnimRotation(&bodyPtr,bp,bx);
					processAnimRotation(&bodyPtr,bp,bx);
					processAnimRotation(&bodyPtr,bp,bx);
					break;
				}
			case 1:
			case 2:
				{
					processAnimTranslation(&bodyPtr,bp,bx);
					processAnimTranslation(&bodyPtr,bp,bx);
					processAnimTranslation(&bodyPtr,bp,bx);
					break;
				}
			}

			bodyPtr+=8;
		}
		while(--numOfBonesInAnim);

		animVar1 = animVar1Backup;

		animVar1+=2;

		animRot2 = ((*(short int*)(animVar1))*bp)/bx; // X
		animRot3 = ((*(short int*)(animVar1+2))*bp)/bx; // Y
		animRot1 = ((*(short int*)(animVar1+4))*bp)/bx; // Z

		animVar1+=6;

		animCurrentTime = bx;
		animKeyframeLength = bp;

		return(0);
	}
	else // change keyframe
	{
		char* tempBx = animVar1;
		char* si = animVar1;


		si+=8;

		do
		{
			*(short int*)(bodyPtr) = *(short int*)(si);
			*(short int*)(bodyPtr+2) = *(short int*)(si+2);
			*(short int*)(bodyPtr+4) = *(short int*)(si+4);
			*(short int*)(bodyPtr+6) = *(short int*)(si+6);

			bodyPtr+=8;
			si+=8;

			bodyPtr+=8;

		}while(--numOfBonesInAnim);

		*(char**)animVar3 = animVar1;

		*(unsigned short int*)(animVar3+4) = (unsigned short int)timer;

		tempBx+=2;

		animCurrentTime = bx;
		animKeyframeLength = bx;

		animRot2 = *(short int*)(tempBx);
		animRot3 = *(short int*)(tempBx+2);
		animRot1 = *(short int*)(tempBx+4);

		tempBx += 6;

		return(1);

	}

}

void walkStep(int angle1, int angle2, int angle3)
{
	makeRotationMtx(angle3,angle1,angle2,&animMoveY,&animMoveX);
}

void stopAnim(int actorIdx)
{
	actorTable[actorIdx].flags |= 0xC;
	actorTable[actorIdx].flags &= 0xFFFE;

	//objModifFlag2 = 1;
}

int checkZvCollision(ZVStruct* zvPtr1,ZVStruct* zvPtr2)
{
	if(zvPtr1->ZVX1 >= zvPtr2->ZVX2)
		return 0;

	if(zvPtr2->ZVX1 >= zvPtr1->ZVX2)
		return 0;

	if(zvPtr1->ZVY1 >= zvPtr2->ZVY2)
		return 0;

	if(zvPtr2->ZVY1 >= zvPtr1->ZVY2)
		return 0;

	if(zvPtr1->ZVZ1 >= zvPtr2->ZVZ2)
		return 0;

	if(zvPtr2->ZVZ1 >= zvPtr1->ZVZ2)
		return 0;

	return 1;
}

void getZvRelativePosition(ZVStruct* zvPtr, int startRoom, int destRoom)
{
	char* startRoomData = etageVar0 + *(unsigned int*)(etageVar0+startRoom*4);
	char* destRoomData = etageVar0 + *(unsigned int*)(etageVar0+destRoom*4);

	int Xdif = 10*(*(short int*)(destRoomData+4) - *(short int*)(startRoomData+4));
	int Ydif = 10*(*(short int*)(destRoomData+6) - *(short int*)(startRoomData+6));
	int Zdif = 10*(*(short int*)(destRoomData+8) - *(short int*)(startRoomData+8));

	zvPtr->ZVX1 -= Xdif;
	zvPtr->ZVX2 -= Xdif;
	zvPtr->ZVY1 += Ydif;
	zvPtr->ZVY2 += Ydif;
	zvPtr->ZVZ1 += Zdif;
	zvPtr->ZVZ2 += Zdif;
}

int processActor1Sub1(int actorIdx, ZVStruct* zvPtr)
{
	int currentCollisionSlot = 0;
	actorStruct* currentActor = actorTable;
	int actorRoom = actorTable[actorIdx].room;

	int i;

	for(i=0;i<50;i++)
	{
		if(currentActor->field_0 != -1 && i!=actorIdx)
		{
			ZVStruct* currentActorZv = &currentActor->zv;

			if(currentActor->room != actorRoom)
			{
				ZVStruct localZv;

				copyZv(zvPtr,&localZv);

				getZvRelativePosition(&localZv,actorRoom,currentActor->room);

				if(checkZvCollision(&localZv,currentActorZv))
				{
					currentProcessedActorPtr->COL[currentCollisionSlot++] = i;

					if(currentCollisionSlot == 3)
						return(3);
				}
			}
			else
			{
				if(checkZvCollision(zvPtr,currentActorZv))
				{
					currentProcessedActorPtr->COL[currentCollisionSlot++] = i;

					if(currentCollisionSlot == 3)
						return(3);
				}
			}
		}
		currentActor++;
	}

	return(currentCollisionSlot);
}

void cleanFoundObjectScreen()
{
	// 160 120 whidth = ,240,120
	memset(screen,0,320*200);
}

void drawFoundObect(int menuState,int objectName,int zoomFactor)
{
	cleanFoundObjectScreen();

	rotateModel(0,0,0,60,statusVar1,0,zoomFactor);

	renderModel(0,0,0,0,0,0, HQR_Get(listBody, currentFoundBodyIdx));

	drawText(160,currentMenuTop,20,1);
	drawText(160,currentMenuTop+16,objectName,1);
	drawText(160,currentMenuTop+16,objectName,1);

	switch(menuState)
	{
	case 0:
		{
			drawSlectedText(130,currentMenuBottom-16,21,1,4);
			drawText(190,currentMenuBottom-16,22,4);
			break;
		}
	case 1:
		{
			drawText(130,currentMenuBottom-16,21,4);
			drawSlectedText(190,currentMenuBottom-16,22,1,4);
			break;
		}
	case 2:
		{
			drawSlectedText(160,currentMenuBottom-16,10,1,4);
			break;
		}
	}
}

void take(int objIdx)
{
	objectStruct* objPtr = &objectTable[objIdx];

	if(numObjInInventory == 0)
	{
		inventory[0] = objIdx;
	}
	else
	{
		int i;

		for(i=numObjInInventory;i>0;i--)
		{
			inventory[i+1] = inventory[i];
		}

		inventory[1] = objIdx;
	}

	numObjInInventory++;

	action = 0x800;

	updateInHand(objIdx);

	if(objPtr->ownerIdx != -1)
	{
		updateAllActorAndObjectsSub1(objPtr->ownerIdx);
	}

	objPtr->flags2 &= 0xBFFF;
	objPtr->flags2 |= 0x8000;

	objPtr->room = -1;
	objPtr->stage = -1;
}

int input5;

void foundObject(int objIdx, int param)
{
	objectStruct* objPtr;
	int var_C = 0;
	int var_6 = 1;
	int var_2 = 0;
	int i;
	int var_A = 15000;
	int var_8 = -200;

	if(objIdx < 0)
		return;

	objPtr = &objectTable[objIdx];

	if( param != 0 && (objPtr->flags2 & 0xC000))
	{
		return;
	}

	if(objPtr->trackNumber)
	{
		if(timer - objPtr->trackNumber < 300) // prevent from reopening the window every frame
			return;
	}

	objPtr->trackNumber = 0;

	freezeTime();
//	setupShaking(1000); // probably to remove the shaking when in foundObject screen

	for(i=0;i<numObjInInventory;i++)
	{
		var_2 += objectTable[inventory[i]].positionInTrack;
	}

	if(objPtr->positionInTrack + var_2 > defines.field_4 || numObjInInventory +1 == 30)
	{
		var_6 = 3;
	}

	currentFoundBodyIdx = objPtr->foundBody;
	currentFoundBody = HQR_Get(listBody,currentFoundBodyIdx);

	setupSMCode(160,100,128,300,298);

	statusVar1 = 0;

	copyToScreen(unkScreenVar,screen);

	drawAITDBox(160,100,240,120);

	drawFoundObect(var_6, objPtr->foundName, var_A);
	flipScreen();

	input5 = 1;

	while(!var_C)
	{
#ifdef USE_GL
		osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
		osystem_startFrame();
		osystem_cleanScreenKeepZBuffer();
#endif

		process_events();
		readKeyboard();

		input3 = input2;
		input4 = inputKey;
		button = input1;

		if(!input5)
		{
			if(input3 == 1)
			{
				if(var_6 != 2)
				{
					var_6 = 0;
				}

				var_C = 1;
			}
			if(var_6 != 2)
			{
				if(input4&4)
				{
					var_6 = 0;
				}

				if(input4&8)
				{
					var_6 = 1;
				}
			}

			if(input3 == 28 || button !=0)
			{
				var_C = 1;
			}
		}
		else
		{
			if(!input3 && !input4 && !button)
				input5 = 0;
		}

		statusVar1 -= 8;

		var_A += var_8; // zoom / dezoom

		if(var_A> 8000) // zoom management
			var_8 = -var_8;

		if(var_A< 25000)
			var_8 = -var_8;

		drawFoundObect(var_6,objPtr->foundName,var_A);
		flipScreen();

//		menuWaitVSync();
	}

	unfreezeTime();

	if(var_6 == 1)
	{
		take(objIdx);
	}
	else
	{
		objPtr->trackNumber = timer;
	}

	while(input2 && input1)
	{
		readKeyboard();
	}

	input4 = 0;
	input3 = 0;
	button = 0;

//	if(mainLoopVar1 != 0)
	{
		//setupShaking(-600);
	}

	mainVar1 = 1;
}

void hardColSuB1Sub1(int flag)
{
	switch(flag)
	{
	case 1:
	case 2:
		{
			hardColVar2 = 0;
			break;
		}
	case 4:
	case 8:
		{
			hardColVar1 = 0;
			break;
		}
	default:
		{
			break;
		}
	}
}

void hardColSuB1(ZVStruct* startZv, ZVStruct* zvPtr2, ZVStruct* zvPtr3)
{
	int flag = 0;
	int var_8;
	int halfX;
	int halfZ;
	int var_A;
	int var_6;
	
	if(startZv->ZVX2 > zvPtr3->ZVX1)
	{
		if(zvPtr3->ZVX2 <= startZv->ZVX1)
		{
			flag = 8;
		}
	}
	else
	{
		flag = 4;
	}

	if(startZv->ZVZ2 > zvPtr3->ZVZ1)
	{
		if(startZv->ZVZ1 >= zvPtr3->ZVZ2)
		{
			flag |= 2;
		}
	}
	else
	{
		flag |= 1;
	}

	if( flag == 5 || flag == 9 || flag == 6 || flag == 10 )
	{
		var_8 = 2;
	}
	else
	{
		if(!flag)
		{
			var_8 = 0;

			hardColVar2 = 0;
			hardColVar1 = 0;

			return;
		}
		else
		{
			var_8 = 1;
		}
	}

	halfX = (zvPtr2->ZVX1 + zvPtr2->ZVX2) / 2;
	halfZ = (zvPtr2->ZVZ1 + zvPtr2->ZVZ2) / 2;

	if(zvPtr3->ZVX1 > halfX)
	{
		var_A = 4;
	}
	else
	{
		if(zvPtr3->ZVX2 < halfX)
		{
			var_A = 0;
		}
		else
		{
			var_A = 8;
		}
	}

	if(zvPtr3->ZVZ1 > halfZ)
	{
		var_A |= 1;
	}
	else
	{
		if(zvPtr3->ZVZ2 < halfZ)
		{
			var_A |= 0; // once again, not that much usefull
		}
		else
		{
			var_A |= 2;
		}
	}

	if( var_A == 5 || var_A == 9 || var_A == 6 || var_A == 10 )
	{
		var_6 = 2;
	}
	else
	{
		if(!var_A)
		{
			var_6 = 0;
		}
		else
		{
			var_6 = 1;
		}
	}

	if(var_8 == 1)
	{
		hardColSuB1Sub1(flag);
		return;
	}

	if(var_6 == 1 && (var_A & flag))
	{
		hardColSuB1Sub1(var_A);
		return;
	}

	if(var_A == flag || flag == 15)
	{
		int Xmod = abs(zvPtr2->ZVX1 - startZv->ZVX1); // recheck
		int Zmod = abs(zvPtr2->ZVZ1 - startZv->ZVZ1);

		if(Xmod > Zmod)
		{
			hardColVar2 = 0;
		}
		else
		{
			hardColVar1 = 0;
		}
	}
	else
	{
		if(!var_6 || (var_6 == 1 && !(var_A & flag)))
		{
			hardColVar2 = 0;
			hardColVar1 = 0;
		}
		else
		{
			hardColSuB1Sub1(flag&var_A);
		}
	}
}

int checkForHardCol(ZVStruct* zvPtr, char* dataPtr)
{
	int hardColVar = 0;
	short int counter;
	
	dataPtr += *(short int*)(dataPtr); // skip to data

	counter = *(short int*)dataPtr;
	dataPtr+=2;

	do
	{

		short int X1 = *(short int*)dataPtr;
		short int X2 = *(short int*)(dataPtr+2);

		if(X1 < zvPtr->ZVX2 && zvPtr->ZVX1 < X2)
		{
			short int Y1 = *(short int*)(dataPtr+4);
			short int Y2 = *(short int*)(dataPtr+6);

			if(Y1 < zvPtr->ZVY2 && zvPtr->ZVY1 < Y2)
			{
				short int Z1 = *(short int*)(dataPtr+8);
				short int Z2 = *(short int*)(dataPtr+10);

				if(Z1 < zvPtr->ZVZ2 && zvPtr->ZVZ1 < Z2)
					hardColTable[hardColVar++] = dataPtr;
			}
		}

		dataPtr += 0x10;
	}while(--counter);

	return hardColVar;
}

int manageFall(int actorIdx, ZVStruct* zvPtr)
{
	int fallResult = 0;
	int i;
	int room = actorTable[actorIdx].room;

	for(i=0;i<50;i++)
	{
		actorStruct* currentTestedActorPtr = &actorTable[i];

		if(currentTestedActorPtr->field_0 != -1 && i != actorIdx)
		{
			ZVStruct* testedZv = &currentTestedActorPtr->zv;

			if(currentTestedActorPtr->room != room)
			{
				ZVStruct localZv;
				copyZv(zvPtr, &localZv);
				getZvRelativePosition(&localZv, room, currentTestedActorPtr->room);

				if(checkZvCollision(&localZv, testedZv))
				{
					actorTable[i].COL_BY = actorIdx;
					fallResult++;
				}
			}
			else
			{
				if(checkZvCollision(zvPtr, testedZv))
				{
					actorTable[i].COL_BY = actorIdx;
					fallResult++;
				}
			}
		}
	}

	return(fallResult);
}

int processActor1Sub2(rotateStruct* data)
{
	return(0);
}

void processActor1(void)
{
	int var_42 = 0;
	int var_6 = currentProcessedActorPtr->field_44;
	int var_40;
	int var_48;
	int var_4A;
	int var_4C;
	int var_4E;
	int var_50=0;
	int var_52=0;
	int var_56;
	short int localTable[3];
	ZVStruct zvLocal;
	ZVStruct* zvPtr;
	int j;

	if(var_6 != -1) // next anim ?
	{
		if(var_6 == -2) // completly stop anim
		{
			stopAnim(currentProcessedActorIdx);
			currentProcessedActorPtr->field_44 = -1;
			currentProcessedActorPtr->field_46 = 0;
			currentProcessedActorPtr->field_48 = -1;
			currentProcessedActorPtr->END_ANIM = 1;

			return;
		}

		if(currentProcessedActorPtr->END_FRAME == 0)
		{
			currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
			currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

			currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
			currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

			currentProcessedActorPtr->modX = 0;
			currentProcessedActorPtr->modZ = 0;
		}

		initBufferAnim(bufferAnim + (bufferAnimCounter++) * 248, HQR_Get(listBody,currentProcessedActorPtr->bodyNum)); 

		if(bufferAnimCounter == 20)
			bufferAnimCounter = 0;

		currentProcessedActorPtr->ANIM = var_6;
		currentProcessedActorPtr->field_40 = currentProcessedActorPtr->field_46;
		currentProcessedActorPtr->field_42 = currentProcessedActorPtr->field_48;
		currentProcessedActorPtr->field_44 = -1;
		currentProcessedActorPtr->field_46 = 0;
		currentProcessedActorPtr->field_48 = -1;
		currentProcessedActorPtr->END_ANIM = 0;
		currentProcessedActorPtr->FRAME = 0;

		currentProcessedActorPtr->field_4C = getAnimParam(HQR_Get(listAnim,var_6));
	}

	if(currentProcessedActorPtr->ANIM == -1) // no animation
	{
		currentProcessedActorPtr->END_FRAME = 0;
		if(currentProcessedActorPtr->speed == 0)
		{
			var_42 = processActor1Sub1(currentProcessedActorIdx, &currentProcessedActorPtr->zv);

			if(var_42)
			{
				for(var_40 = 0; var_40<var_42; var_40 ++)
				{
					actorTable[currentProcessedActorPtr->COL[var_40]].COL_BY = currentProcessedActorIdx; // collision with current actor
				}
			}

			var_40 = var_42;

			while(var_40>0)
			{
				currentProcessedActorPtr->COL[var_40] = -1;

				var_40 --;
			}

			var_4A = 0;
			var_48 = 0;
			var_52 = 0;
			var_50 = 0;
		}
		else
		{
			var_4C = currentProcessedActorPtr->modX;
			var_4A = currentProcessedActorPtr->modY;
			var_48 = currentProcessedActorPtr->modZ;

			animRot3 = 0;
			animRot2 = 0;

			animRot1 = processActor1Sub2(&currentProcessedActorPtr->speedChange);

			walkStep(0,animRot1,currentProcessedActorPtr->beta);

			var_52 = animMoveX - var_4C;
			var_50 = animMoveY - var_48;
		}

		var_4E = 0;
	}
	else // animation
	{
		var_4C = currentProcessedActorPtr->modX;
		var_4A = currentProcessedActorPtr->modY;
		var_48 = currentProcessedActorPtr->modZ;

		currentProcessedActorPtr->END_FRAME = processAnim(currentProcessedActorPtr->FRAME, HQR_Get(listAnim, currentProcessedActorPtr->ANIM), HQR_Get(listBody, currentProcessedActorPtr->bodyNum));

		walkStep(animRot2,animRot1,currentProcessedActorPtr->beta);

		var_52 = animMoveX - var_4C;
		var_50 = animMoveY - var_48;

	}

	if(currentProcessedActorPtr->field_60.param) // currently falling ?
	{
		if(currentProcessedActorPtr->field_60.param != -1)
		{
			var_4E = processActor1Sub2(&currentProcessedActorPtr->speedChange) - var_4A;
		}
		else // stop falling
		{
			var_4E = currentProcessedActorPtr->field_60.newAngle - var_4A;

			currentProcessedActorPtr->field_60.param = 0;
			currentProcessedActorPtr->field_60.newAngle = 0;
			currentProcessedActorPtr->field_60.oldAngle = 0;
		}
	}
	else
	{
		var_4E = 0;
	}

	memcpy(localTable,currentProcessedActorPtr->COL,6);
	var_56 = -1;

	if(var_52 || var_50 || var_4E) // start of movement management
	{
		zvPtr = &currentProcessedActorPtr->zv;
		copyZv(&currentProcessedActorPtr->zv,&zvLocal);

		zvLocal.ZVX1 += var_52;
		zvLocal.ZVX2 += var_52;

		zvLocal.ZVY1 += var_4E;
		zvLocal.ZVY2 += var_4E;

		zvLocal.ZVZ1 += var_50;
		zvLocal.ZVZ2 += var_50;

		if(currentProcessedActorPtr->dynFlags & 1) // hard collision enabled for actor ?
		{
			int i;

			var_42 = checkForHardCol(&zvLocal, etageVar0 + *(unsigned int*)(etageVar0 + currentProcessedActorPtr->room * 4));

			for(i=0;i<var_42;i++)
			{
				char* var_3E = hardColTable[i];

				if(*(short int*)(var_3E + 0xE) == 9)
				{
					currentProcessedActorPtr->HARD_COL = *(short int*)(var_3E + 0xC);
				}

				if(*(short int*)(var_3E + 0xE) == 3)
				{
					currentProcessedActorPtr->HARD_COL = 255;
				}

				if(var_52 || var_50) // move on the X or Y axis ? update to avoid entering the hard col
				{
					hardColVar1 = var_52;
					hardColVar2 = var_50;

					hardColSuB1(zvPtr, &zvLocal, (ZVStruct*)var_3E);

					zvLocal.ZVX1 +=  hardColVar1 - var_52;
					zvLocal.ZVX2 +=  hardColVar1 - var_52;
					zvLocal.ZVZ1 +=  hardColVar2 - var_50;
					zvLocal.ZVZ2 +=  hardColVar2 - var_50;

					var_52 = hardColVar1;
					var_50 = hardColVar2;
				}
			}
		}
		else // no hard collision -> just update the flag without performing the position update
		{
			if(checkForHardCol(&zvLocal,etageVar0+*(unsigned int*)(etageVar0+currentProcessedActorPtr->room*4)))
			{
				currentProcessedActorPtr->HARD_COL = 1;
			}
			else
			{
				currentProcessedActorPtr->HARD_COL = 0;
			}
		}

		var_42 = processActor1Sub1(currentProcessedActorIdx,&zvLocal); // get the number of actor/actor collision

		for(j=0;j<var_42;j++) // process the actor/actor collision
		{
			ZVStruct* touchedZv;
			actorStruct* actorTouchedPtr;

			var_56 = currentProcessedActorPtr->COL[j];

			actorTouchedPtr = &actorTable[var_56];

			actorTouchedPtr->COL_BY = currentProcessedActorIdx;

			touchedZv = &actorTouchedPtr->zv;

			if(actorTouchedPtr->flags & 0x80) // takable
			{
				if(currentProcessedActorPtr->trackMode == 1 && defines.field_1E == 0)
				{
					foundObject(actorTouchedPtr->field_0, 0);
				}
			}
			else
			{
				if(actorTouchedPtr->flags & 0x10) // can be pushed ?
				{
					ZVStruct localZv2;

					bool isPushPossible = true;

					copyZv(touchedZv, &localZv2);

					localZv2.ZVX1 += var_52;
					localZv2.ZVX2 += var_52;

					localZv2.ZVZ1 += var_50;
					localZv2.ZVZ2 += var_50;

					if(!checkForHardCol(&localZv2, etageVar0 + *(unsigned int*)(etageVar0 + actorTouchedPtr->room * 4)))
					{
						if(processActor1Sub1(var_56, &localZv2))
						{
							isPushPossible = false;
						}
					}
					else
					{
						isPushPossible = false;
					}

					if(!isPushPossible)
					{
						if(var_52 || var_50) //if we're trying to move
						{
							if(actorTouchedPtr->room != currentProcessedActorPtr->room)
							{
								ZVStruct localZv3;

								copyZv(touchedZv, &localZv3);

								getZvRelativePosition(&localZv3, actorTouchedPtr->room, currentProcessedActorPtr->room);

								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, &localZv3);

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
							else
							{
								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, touchedZv); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
						}
					}
					else // push succeed
					{
						if(actorTouchedPtr->flags & 8)
						{
							deleteSub(var_56);
						}
						
						actorTouchedPtr->flags |= 1;

						actorTouchedPtr->worldX += var_52; // apply push to object
						actorTouchedPtr->worldZ += var_50;

						actorTouchedPtr->roomX += var_52;
						actorTouchedPtr->roomZ += var_50;

						copyZv(&localZv2,touchedZv);
					}
				}
				else // can be pushed
				{
					if(currentProcessedActorPtr->dynFlags & 1)
					{
						if(var_52 || var_50) // if moving
						{
							if(actorTouchedPtr->room == currentProcessedActorPtr->room) // same room -> easy case
							{
								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, touchedZv); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
							else // different room
							{
								ZVStruct localZv3;

								copyZv(touchedZv, &localZv3);

								getZvRelativePosition(&localZv3, actorTouchedPtr->room, currentProcessedActorPtr->room);

								hardColVar1 = var_52;
								hardColVar2 = var_50;

								hardColSuB1(zvPtr, &zvLocal, &localZv3); // manage as hard collision

								var_52 = hardColVar1;
								var_50 = hardColVar2;
							}
						}
					}
				}
			}
		} // end of actor/actor collision

		currentProcessedActorPtr->modX = var_52 + var_4C;
		currentProcessedActorPtr->modY = var_4E + var_4A;
		currentProcessedActorPtr->modZ = var_50 + var_48;

		currentProcessedActorPtr->zv.ZVX1 += var_52;
		currentProcessedActorPtr->zv.ZVX2 += var_52;

		currentProcessedActorPtr->zv.ZVY1 += var_4E;
		currentProcessedActorPtr->zv.ZVY2 += var_4E;

		currentProcessedActorPtr->zv.ZVZ1 += var_50;
		currentProcessedActorPtr->zv.ZVZ2 += var_50;
	} // end of movement management

	if(!currentProcessedActorPtr->field_60.param)
	{
		// fall management ?
		currentProcessedActorPtr->worldY += currentProcessedActorPtr->modY;
		currentProcessedActorPtr->roomY += currentProcessedActorPtr->modY;

		currentProcessedActorPtr->modY = 0;

		if(currentProcessedActorPtr->flags & 0x100)
		{
			zvPtr = &currentProcessedActorPtr->zv;

			copyZv(zvPtr, &zvLocal);

			zvLocal.ZVY2 += 100;

			if(currentProcessedActorPtr->roomY < -10 && !checkForHardCol(&zvLocal,etageVar0+*(unsigned int*)(etageVar0 + currentProcessedActorPtr->room*4)) && !manageFall(currentProcessedActorIdx,&zvLocal))
			{
				startActorRotation(0, 2000, 40, &currentProcessedActorPtr->field_60);
			}
			else
			{
				currentProcessedActorPtr->falling = 0;
			}
		}
	}
	else
	{
		if((currentProcessedActorPtr->field_60.param != -1) && (currentProcessedActorPtr->flags & 0x100))
		{
			currentProcessedActorPtr->falling = 1;
		}
	}

	var_40 = var_42;

	while(var_40>0)
	{
		currentProcessedActorPtr->COL[var_40] = -1;

		var_40 --;
	}

	for(var_40=0; var_40<3; var_40++)
	{
		var_56 = localTable[var_40];

		if(var_56 != -1)
		{
			actorStruct* actorTouchedPtr = &actorTable[var_56];

			if(actorTouchedPtr->flags & 0x10)
			{
				int i;

				for(i=0;i<3;i++)
				{
					if(currentProcessedActorPtr->COL[i] == var_56)
						break;
				}

				if(i == 3)
				{
					actorTouchedPtr->flags &= 0xFFFE;
					stopAnim(var_56);
				}
			}
		}
	}

	if(currentProcessedActorPtr->END_FRAME) // key frame change
	{
		currentProcessedActorPtr->FRAME++;

		if(currentProcessedActorPtr->FRAME >= currentProcessedActorPtr->field_4C) // end of anim ?
		{
			currentProcessedActorPtr->END_ANIM = 1; // end of anim
			currentProcessedActorPtr->FRAME = 0; // restart anim

			if(!(currentProcessedActorPtr->field_40 & 1) && (currentProcessedActorPtr->field_44 == -1)) // is another anim waiting ?
			{
				currentProcessedActorPtr->field_40 &= 0xFFFD;

				anim(currentProcessedActorPtr->field_42, 1, -1);
			}
		}
		currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
		currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

		currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
		currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

		currentProcessedActorPtr->modX = 0;
		currentProcessedActorPtr->modZ = 0;
	}
	else // not the end of anim
	{
		if((currentProcessedActorPtr->ANIM == -1) && (currentProcessedActorPtr->speed != 0) && (currentProcessedActorPtr->speedChange.param == 0))
		{
			currentProcessedActorPtr->worldX += currentProcessedActorPtr->modX;
			currentProcessedActorPtr->roomX += currentProcessedActorPtr->modX;

			currentProcessedActorPtr->worldZ += currentProcessedActorPtr->modZ;
			currentProcessedActorPtr->roomZ += currentProcessedActorPtr->modZ;

			currentProcessedActorPtr->modX = 0;
			currentProcessedActorPtr->modZ = 0;

			startActorRotation(0,currentProcessedActorPtr->speed,60,&currentProcessedActorPtr->speedChange);
		}

		currentProcessedActorPtr->END_ANIM = 0;
	}
}

void drawInventoryActions(int arg)
{
	int var_2;
	int i;
	
	drawAITDBox(240,150,160,100);

	var_2 = 150 - ((numInventoryActions<<4)/2);

	for(i=0;i<numInventoryActions;i++)
	{
		if(arg == i)
		{
			fillBox(170,var_2,309,var_2+16,100);
			drawSlectedText(240,var_2,inventoryActionTable[i],15,4);
		}
		else
		{
			drawText(240,var_2,inventoryActionTable[i],4);
		}

		var_2 += 16;
	}
}

int drawTopStatusBox(int arg_0, int arg_2, int arg_4)
{
	int var_A = currentMenuTop+1;
	int var_6 = arg_0;
	int var_8;
	int i;
	int currentObj;
	objectStruct* objPtr;

	drawAITDBox(160,50,320,100);

	var_A = currentMenuTop+1;

	for(i=0;i<5;i++)
	{
		if(arg_0>=numObjInInventory)
			break;

		currentObj = inventory[arg_0];

		objPtr = &objectTable[currentObj];

		if(arg_0 == arg_2)
		{
			if(arg_4 == 15)
			{
				fillBox(0xA,var_A,0x135,var_A+0x10,0x64);
			}

			drawSlectedText(160,var_A,objPtr->foundName,arg_4,4);

			var_8 = currentObj;
		}
		else
		{
			drawText(160,var_A,objPtr->foundName,4);
		}

		var_A += 0x10;
		arg_0++;
	}

	printTextSub5(298,0x10,0x10,aitdBoxGfx);

	if(var_6+5 < numObjInInventory)
	{
		printTextSub5(298,74,9,aitdBoxGfx);
	}

	return(var_8);
}

void menuWaitVSync()
{
}

void renderInventoryObject(int arg)
{
	setClipSize(statusLeft,statusTop,statusRight,statusBottom);
	fillBox(statusLeft,statusTop,statusRight,statusBottom,0);

	statusVar1 -= 8;

	rotateModel(0,0,0,60,statusVar1,0,24000);
	renderModel(0,0,0,0,0,0,currentFoundBody);

	if(arg!=-1)
	{
		char buffer[256];
		initFont(fontData,4);
		sprintf(buffer,"%d",vars[arg]);
		renderText(statusLeft+4,statusTop+4,screen,buffer);
	}

	menuWaitVSync();
}

void makeStatusScreen(void)
{
	int exitMenu = 0;
	int var_10 = 0;
	int firstTime = 1;
	unsigned int chrono;
	int var_14;
	int var_16;
	int var_18;
	int var_1A;
	int var_1C;
	int var_8;
	int var_A;
	int var_C;
	int var_E;

	if(!numObjInInventory)
		return;

	var_1C = 0;
	var_14 = -1;
	var_18 = 0;
	var_A = 0;
	var_8 = 2;

	statusVar1 = 0;

	freezeTime();
	//pauseShaking();

	if(lightVar1!=0)
	{
		//makeBlackPalette();
	}

	drawAITDBox(80,150,160,100);

	statusLeft = currentMenuLeft;
	statusTop = currentMenuTop;
	statusRight = currentMenuRight;
	statusBottom = currentMenuBottom;

	setupSMCode(((statusRight-statusLeft)/2)+statusLeft,((statusBottom-statusTop)/2) + statusTop,128,400,390);

	while(!exitMenu)
	{
#ifdef USE_GL
		osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
		osystem_startFrame();
		osystem_cleanScreenKeepZBuffer();
#endif

		readKeyboard();
		process_events();

		input3 = input2;
		input4 = inputKey;
		button = input1;

		if(!input3 && !input4 && !button)
		{
			var_8 = 0;
		}

		if(input3 == 1)
		{
			var_10 = 0;
			exitMenu = 1;
		}

		if(var_A == 0)
		{
			if(var_8<1)
			{
				if((input3 == 0x1C) || (button != 0) || (input4 == 0xC))
				{
					drawTopStatusBox(var_1C,var_18,14);
					menuWaitVSync();
					var_A = 1;
					var_14 = -1;
					var_1A = 0;

					while(input2)
					{
						readKeyboard();
					}
				}
				else
				{
					if(input4&1 && var_18>0)
					{		
						var_18--;
					}

					if(input4&2 && var_18 < (numObjInInventory-1))
					{
						var_18++;
					}

					if(var_1C+5 <= var_18)
					{
						var_1C++;
					}

					if(var_18 < var_1C)
					{
						var_1C--;
					}

					if(input3 || input4 || button)
					{
						if(var_8==0)
						{
							var_8 = 1;
							startChrono(&chrono);
						}
					}
				}
			}
			else
			{
				if(var_8 == 1)
				{
					if(evalChrono(&chrono)>0x280000)
					{
						var_8 = -1;
					}
				}
			}

			if(var_14!=var_18)
			{
				var_16 = drawTopStatusBox(var_1C,var_18,15);

				currentFoundBodyIdx = objectTable[var_16].foundBody;

				currentFoundBody = HQR_Get(listBody,currentFoundBodyIdx);

				var_C = objectTable[var_16].flags2;

				numInventoryActions = 0;
				var_E = 0;

				while(var_E<11)
				{
					if(var_C & (1<<var_E))
					{
						if(numInventoryActions<5)
						{
							inventoryActionTable[numInventoryActions++] = var_E+23;
						}
					}
					var_E ++;
				}

				drawInventoryActions(-1);
				flipScreen();

				var_14 = var_18;
			}
		}
		else
		{
			if(var_8<1)
			{
				if(input3 == 0x1C || button)
				{
					var_18 = inventory[var_18];
					action = (1 << (233 + inventoryActionTable[var_1A]));
					var_10 = 1;
					exitMenu = 1;
				}

				if(input4 & 0xC)
				{
					drawInventoryActions(-1);
					var_A = 0;
					var_14 = -1;
					var_8 = 2;
					continue;
				}

				if(input4&1 && var_1A>0)
				{
					var_1A --;
				}

				if(input4&2 && var_1A<(numInventoryActions-1))
				{
					var_1A++;
				}

				if(input3 || input4 || button)
				{
					if(var_8==0)
					{
						var_8 = 1;
						startChrono(&chrono);
					}
				}
			}
			else
			{
				if(var_8==1)
				{
					if(evalChrono(&chrono)>0x280000)
					{
						var_8 = -1;
					}
				}

				if(var_14 != var_1A)
				{
					var_14 = var_1A;
					drawInventoryActions(var_14);
					menuWaitVSync();
				}
			}
		}
		renderInventoryObject(objectTable[var_16].field_24);

		if(firstTime)
		{
			firstTime = 0;
			if(lightVar1)
			{
				make3dTatouUnk1(0x40,0);
			}
		}

		flipScreen();
	}

	unfreezeTime();

	mainVar1 = 1;

	readKeyboard();
	while(input1 || input2 || inputKey)
	{
		readKeyboard();
	}

	input4 = 0;
	input3 = 0;
	button = 0;

	if(var_10 == 1)
	{
		updateInHand(var_18);
	}

	//updateShaking();
}

int changeCameraSub1Sub1(int x1, int z1, int x2, int z2, int x3, int z3, int x4, int z4)
{
	int returnFlag = 0;

	int var1 = x1 - x2;
	int var2 = z3 - z4;
	int var3 = x3 - x4;
	int var4 = z1 - z2;

	int var5 = x1 - x3;
	int var6 = z1 - z3;

	int result1 = (var1 * var2) - (var3 * var4);
	
	int result2;
	int result3;

	if(!result1)
	{
		return(returnFlag);
	}

	result2 = (var5 * var2) - (var3 * var6);

	result3 = (-var1 * var6) + (var5 * var4);

	if(result1<0)
	{
		result1 = -result1;
		result2 = -result2;
		result3 = -result3;
	}

	if(result2<=0 || result3<=0)
	{
		return(returnFlag);
	}

	if(result1 > result2 && result1 > result3)
	{
		returnFlag = 1;
	}

	return(returnFlag);
}

int changeCameraSub1(int x1, int x2, int z1, int z2, char* ptr, short int param)
{
	int xMid = (x1+x2)/2;
	int zMid = (z1+z2)/2;

	char* src = ptr;
	int i;

	for(i=0;i<param;i++)
	{
		char* dest = cameraBuffer;
		int j;
		int flag = 0;
		int var1 = *(short int*)src;
		src+=2;

		memcpy(dest,src,var1<<2);
		dest+= var1<<2;

		*(short int*)dest = *(short int*)src;
		dest+=2;
		*(short int*)dest = *(short int*)(src+2);
		dest+=2;

		src += (var1)<<2;

		dest = cameraBuffer;

		for(j=0;j<var1;j++)
		{
			int zoneX1;
			int zoneZ1;
			int zoneX2;
			int zoneZ2;
			
			zoneX1= *(short int*)dest;
			dest+=2;
			zoneZ1= *(short int*)dest;
			dest+=2;
			zoneX2= *(short int*)dest;
			zoneZ2= *(short int*)(dest+2);

			if(changeCameraSub1Sub1(xMid,zMid,xMid-10000,zMid,zoneX1,zoneZ1,zoneX2,zoneZ2))
			{
				flag |= 1;
			}

			if(changeCameraSub1Sub1(xMid,zMid,xMid+10000,zMid,zoneX1,zoneZ1,zoneX2,zoneZ2))
			{
				flag |= 2;
			}
		}

		if(flag == 3)
		{
			return(1);
		}
	}

	return(0);
}

int changeCameraSub2(void)
{
	int foundDistance = 32000;
	int foundCamera = -1;

	actorStruct* actorPtr = &actorTable[genVar9];

	int x1 = actorPtr->zv.ZVX1/10;
	int x2 = actorPtr->zv.ZVX2/10;
	int z1 = actorPtr->zv.ZVZ1/10;
	int z2 = actorPtr->zv.ZVZ2/10;

	int i;

	for(i=0;i<numCameraInRoom;i++)
	{
		char* cameraDataPtr = roomVar5[i] + 2*roomVar6[i];

		if(changeCameraSub1(x1,x2,z1,z2,cameraDataPtr+2,*(short int*)cameraDataPtr)) // if in camera i zone ?
		{
			int newAngle = actorPtr->beta + (((*(short int*)(roomVar5[i] + 2))+0x200)&0x3FF);

			if(newAngle)
			{
				newAngle = -newAngle;
			}

			if(newAngle<foundDistance)
			{
				foundDistance = newAngle;
				foundCamera = i;
			}
		}
	}

	return(foundCamera);
}

void checkIfCameraChangeIsRequired(void)
{
	int localCurrentCam = currentCamera;
	int newCamera;

	if(currentCamera!=-1)
	{
		char* cameraDataPtr = roomVar5[currentCamera] + 2*roomVar6[currentCamera];
		actorStruct* actorPtr;
		int zvx1;
		int zvx2;
		int zvz1;
		int zvz2;

		actorPtr = &actorTable[genVar9];

		zvx1 = actorPtr->zv.ZVX1/10;
		zvx2 = actorPtr->zv.ZVX2/10;

		zvz1 = actorPtr->zv.ZVZ1/10;
		zvz2 = actorPtr->zv.ZVZ2/10;

		if(changeCameraSub1(zvx1,zvx2,zvz1,zvz2,cameraDataPtr+2,*(short int*)cameraDataPtr)) // is still in current camera zone ?
		{
			return;
		}
	}

	newCamera = changeCameraSub2(); // find new camera

	if(newCamera!=-1)
	{
		localCurrentCam = newCamera;
	}

	if(currentCamera != localCurrentCam)
	{
		startGameVar1 = localCurrentCam;
		mainVar1 = 1;
	}

}

char* processActor2Sub(int x, int y, int z, char* zoneData)
{
	int i;
	int numOfZones;
	
	zoneData += *(short int*)(zoneData+2);
	numOfZones = *(short int*)zoneData;
	zoneData+=2;

	for(i=0;i<numOfZones;i++)
	{
		if(*(short int*)(zoneData) <= x && *(short int*)(zoneData+2) >= x)
		{
			if(*(short int*)(zoneData+4) <= y && *(short int*)(zoneData+6) >= y)
			{
				if(*(short int*)(zoneData+8) <= z && *(short int*)(zoneData+10) >= z)
				{
					return(zoneData);
				}
			}
		}

		zoneData += 16;
	}

	return(NULL);

}

void processActor2()
{
	char * ptr;
	int type;

	ptr = processActor2Sub(	currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
							currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY,
							currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
							etageVar0 + *(unsigned int*)(currentProcessedActorPtr->room*4 + etageVar0) );


	if(!ptr)
	{
		return;
	}

	type = *(short int*) (ptr + 0xE);

	switch(type)
	{
	case 0:
		{
			char* ptr2 = etageVar0 + *(unsigned int*)(etageVar0 + currentProcessedActorPtr->room*4);
			char* ptr3;
			int x;
			int y;
			int z;

			currentProcessedActorPtr->room = *(short int*) (ptr + 0xC);

			ptr3 = etageVar0 + *(unsigned int*)(etageVar0 + currentProcessedActorPtr->room*4);

			x = (*(short int*)(ptr3+4) - *(short int*)(ptr2+4)) * 10;
			y = (*(short int*)(ptr3+6) - *(short int*)(ptr2+6)) * 10;
			z = (*(short int*)(ptr3+8) - *(short int*)(ptr2+8)) * 10;
	
			currentProcessedActorPtr->roomX -= x;
			currentProcessedActorPtr->roomY += y;
			currentProcessedActorPtr->roomZ += z;

			currentProcessedActorPtr->zv.ZVX1 -= x;
			currentProcessedActorPtr->zv.ZVX2 -= x;

			currentProcessedActorPtr->zv.ZVY1 += y;
			currentProcessedActorPtr->zv.ZVY2 += y;

			currentProcessedActorPtr->zv.ZVZ1 += z;
			currentProcessedActorPtr->zv.ZVZ2 += z;

			if(currentProcessedActorIdx == genVar9)
			{
				needChangeRoom = 1;
				newRoom = *(short int*) (ptr + 0xC);
			}
			else
			{
				actorTurnedToObj = 1;
			}

			startChrono(&currentProcessedActorPtr->ROOM_CHRONO);


			break;
		}
	case 9:
		{
			currentProcessedActorPtr->HARD_DEC = *(short int*) (ptr + 0xC);
			break;
		}
	case 10:
		{
			int life;
// FIXME: fix the stairs zone in green room
			if(*(short int*) (ptr + 0xC) == 1/* && currentProcessedActorPtr->zv.ZVZ1 >= -1600*/)
				break;

			life = objectTable[currentProcessedActorPtr->field_0].field_24;

			if(life==-1)
				return;

			currentProcessedActorPtr->life = life;

			currentProcessedActorPtr->HARD_DEC = *(short int*) (ptr + 0xC);
			break;
		}
	}
}

int checkLineProjectionWithActors( int actorIdx, int X, int Y, int Z, int beta, int room, int param )
{
	ZVStruct localZv;
	int foundFlag = -2;
	int tempX;
	int tempZ;

	localZv.ZVX1 = X - param;
	localZv.ZVX2 = X + param;
	localZv.ZVY1 = Y - param;
	localZv.ZVY2 = Y + param;
	localZv.ZVZ1 = Z - param;
	localZv.ZVZ2 = Z + param;

	walkStep(param * 2, 0, beta);

	while(foundFlag == -2)
	{
		localZv.ZVX1 += animMoveX;
		localZv.ZVX2 += animMoveX;

		localZv.ZVZ1 += animMoveY;
		localZv.ZVZ2 += animMoveY;

		tempX = X;
		tempZ = Z;

		X += animMoveX;
		Z += animMoveY;

		if(X > 20000 || X < -20000 || Z > 20000 || Z < -20000)
		{
			foundFlag = -1;
			break;
		}

		if(checkForHardCol(&localZv, etageVar0 + *(unsigned int*)(etageVar0 + room *4)) <= 0)
		{
			foundFlag = -1;
		}
		else
		{
			actorStruct* currentActorPtr = actorTable;

			int i;

			for(i=0;i<50;i++)
			{
				if(currentActorPtr->field_0 != -1 && i != actorIdx && !(currentActorPtr->flags & 0x20))
				{
					ZVStruct* zvPtr = &currentActorPtr->zv;

					if(room != currentActorPtr->room)
					{
						ZVStruct localZv2;

						copyZv(&localZv, &localZv2);
						getZvRelativePosition(&localZv2, room, currentActorPtr->room);

						if(!checkZvCollision(&localZv2,zvPtr))
						{
							currentActorPtr++;
							continue;
						}
					}
					else
					{
						if(!checkZvCollision(&localZv,zvPtr))
						{
							currentActorPtr++;
							continue;
						}
					}

					foundFlag = i;
					break;
				}
				
				currentActorPtr++;
			}
		}
	}

	animMoveX = tempX;
	animMoveY = Y;
	animMoveZ = tempZ;

	return(foundFlag);
}

void putAt(int objIdx, int objIdxToPutAt)
{
	objectStruct* objPtr = &objectTable[objIdx];
	objectStruct* objPtrToPutAt = &objectTable[objIdxToPutAt];

	if(objPtrToPutAt->ownerIdx != -1)
	{
		actorStruct* actorToPutAtPtr = &actorTable[objPtrToPutAt->ownerIdx];

		removeObjFromInventory(objIdx);

		if(objPtr->ownerIdx == -1)
		{
			objPtr->x = actorToPutAtPtr->roomX;
			objPtr->y = actorToPutAtPtr->roomY;
			objPtr->z = actorToPutAtPtr->roomZ;
			objPtr->room = actorToPutAtPtr->room;
			objPtr->stage = actorToPutAtPtr->stage;
			objPtr->alpha = actorToPutAtPtr->alpha;
			objPtr->beta = actorToPutAtPtr->beta;
			objPtr->gamma = actorToPutAtPtr->gamma;

			objPtr->flags2 |= 0x4000;
			objPtr->flags |= 0x80;

//			objModifFlag1 = 1;
//			objModifFlag2 = 1;
		}
		else
		{
			currentProcessedActorPtr->roomX = actorToPutAtPtr->roomX;
			currentProcessedActorPtr->roomY = actorToPutAtPtr->roomY;
			currentProcessedActorPtr->roomZ = actorToPutAtPtr->roomZ;
			currentProcessedActorPtr->room = actorToPutAtPtr->room;
			currentProcessedActorPtr->stage = actorToPutAtPtr->stage;
			currentProcessedActorPtr->alpha = actorToPutAtPtr->alpha;
			currentProcessedActorPtr->beta = actorToPutAtPtr->beta;
			currentProcessedActorPtr->gamma = actorToPutAtPtr->gamma;

			objectTable[currentProcessedActorPtr->field_0].flags2 |= 0x4000;
			objectTable[currentProcessedActorPtr->field_0].flags |= 0x80;

//			objModifFlag1 = 1;
//			objModifFlag2 = 1;
		}

	}
	else
	{
		removeObjFromInventory(objIdx);

		if(objPtr->ownerIdx == -1)
		{
			objPtr->x = objPtrToPutAt->x;
			objPtr->y = objPtrToPutAt->y;
			objPtr->z = objPtrToPutAt->z;
			objPtr->room = objPtrToPutAt->room;
			objPtr->stage = objPtrToPutAt->stage;
			objPtr->alpha = objPtrToPutAt->alpha;
			objPtr->beta = objPtrToPutAt->beta;
			objPtr->gamma = objPtrToPutAt->gamma;

			objPtr->flags2 |= 0x4000;
			objPtr->flags |= 0x80;

//			objModifFlag1 = 1;
//			objModifFlag2 = 1;
		}
		else
		{
			currentProcessedActorPtr->roomX = objPtrToPutAt->x;
			currentProcessedActorPtr->roomY = objPtrToPutAt->y;
			currentProcessedActorPtr->roomZ = objPtrToPutAt->z;
			currentProcessedActorPtr->room = objPtrToPutAt->room;
			currentProcessedActorPtr->stage = objPtrToPutAt->stage;
			currentProcessedActorPtr->alpha = objPtrToPutAt->alpha;
			currentProcessedActorPtr->beta = objPtrToPutAt->beta;
			currentProcessedActorPtr->gamma = objPtrToPutAt->gamma;

			objectTable[currentProcessedActorPtr->field_0].flags2 |= 0x4000;
			objectTable[currentProcessedActorPtr->field_0].flags |= 0x80;

//			objModifFlag1 = 1;
//			objModifFlag2 = 1;
		}
	}
}

void objectHitActor(int x, int z)
{
}

void processAnimAction(void)
{
	switch(currentProcessedActorPtr->animActionType)
	{
	case 1: // PRE_HIT
		{
			if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
			{
				currentProcessedActorPtr->animActionType = 10;
			}
	
			if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
			{
				if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
				{
					currentProcessedActorPtr->animActionType = 2;
				}
			}
			else
			{
				currentProcessedActorPtr->animActionType = 0;
				return;
			}
			break;
		}
	case 2: // HIT
		{
			int x;
			int y;
			int z;
			int range;
			int collision;
			int i;
			ZVStruct rangeZv;
			
			if(currentProcessedActorPtr->ANIM != currentProcessedActorPtr->animActionANIM)
			{
				currentProcessedActorPtr->animActionType = 0;
			}

			x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A + currentProcessedActorPtr->modX;
			y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C + currentProcessedActorPtr->modY;
			z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E + currentProcessedActorPtr->modZ;

			range = currentProcessedActorPtr->animActionParam;

			rangeZv.ZVX1 = x - range;
			rangeZv.ZVX2 = x + range;
			rangeZv.ZVY1 = y - range;
			rangeZv.ZVY2 = y + range;
			rangeZv.ZVZ1 = z - range;
			rangeZv.ZVZ2 = z + range;

			collision = processActor1Sub1(currentProcessedActorIdx,&rangeZv);

			for(i=0;i<collision;i++)
			{
				actorStruct* actorPtr2;
				
				currentProcessedActorPtr->HIT = currentProcessedActorPtr->COL[i];
				actorPtr2 = &actorTable[currentProcessedActorPtr->COL[i]];

				actorPtr2->HIT_BY = currentProcessedActorIdx;
				actorPtr2->hitForce = currentProcessedActorPtr->hitForce;

				if(actorPtr2->flags & 1)
				{
					currentProcessedActorPtr->animActionType = 0;
					return;
				}
			}
			break;
		}
	case 4: // PRE_FIRE
		{
			if(currentProcessedActorPtr->ANIM != currentProcessedActorPtr->animActionANIM)
				return;

			if(currentProcessedActorPtr->FRAME != currentProcessedActorPtr->animActionFRAME)
				return;

			currentProcessedActorPtr->animActionType = 5;
	
			break;
		}
	case 5: // FIRE
		{
			int touchedActor;
			
			createFlow(	3,
						currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A,
						currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C,
						currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E,
						currentProcessedActorPtr->stage,
						currentProcessedActorPtr->room,
						0,
						currentProcessedActorPtr->beta,
						0,
						NULL);

			touchedActor = checkLineProjectionWithActors(
												currentProcessedActorIdx,
												currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A,
												currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C,
												currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E,
												currentProcessedActorPtr->beta - 0x100,
												currentProcessedActorPtr->room,
												currentProcessedActorPtr->animActionParam );

			if(touchedActor == -1) // no one has been touched
			{
				createFlow(	2, animMoveX, animMoveY, animMoveZ, currentProcessedActorPtr->stage, currentProcessedActorPtr->room,
							0, -currentProcessedActorPtr->beta, 0, NULL );

				currentProcessedActorPtr->animActionType = 0;
			}
			else
			{
				createFlow(	2, animMoveX, animMoveY, animMoveZ, currentProcessedActorPtr->stage, currentProcessedActorPtr->room,
							0, -currentProcessedActorPtr->beta, 0, NULL );

				currentProcessedActorPtr->field_9A = animMoveX - currentProcessedActorPtr->roomX;
				currentProcessedActorPtr->field_9C = animMoveY - currentProcessedActorPtr->roomY;
				currentProcessedActorPtr->field_9E = animMoveZ - currentProcessedActorPtr->roomZ;

				currentProcessedActorPtr->HIT = touchedActor;

				actorTable[touchedActor].HIT_BY = currentProcessedActorIdx;
				actorTable[touchedActor].hitForce = currentProcessedActorPtr->hitForce;

				currentProcessedActorPtr->animActionType = 0;
			}
			break;
		}
	case 6: // PRE_THROW
		{
			if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
			{
				int objIdx = currentProcessedActorPtr->animActionParam;

				objectStruct* objPtr = &objectTable[objIdx];

				int x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A + currentProcessedActorPtr->modX;
				int y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C + currentProcessedActorPtr->modY;
				int z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E + currentProcessedActorPtr->modZ;

				ZVStruct rangeZv;

				getZvNormal(HQR_Get(listBody, objPtr->field_2),&rangeZv);

				rangeZv.ZVX1 += x;
				rangeZv.ZVX2 += x;
				rangeZv.ZVY1 += y;
				rangeZv.ZVY2 += y;
				rangeZv.ZVZ1 += z;
				rangeZv.ZVZ2 += z;

				if(checkForHardCol(&rangeZv, etageVar0 + *(unsigned int*)(etageVar0 + currentProcessedActorPtr->room * 4)))
				{
					currentProcessedActorPtr->animActionType = 0;
					putAt(objIdx, currentProcessedActorPtr->field_0);
				}
				else
				{
					if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
					{
						int x;
						int y;
						int z;
						
						currentProcessedActorPtr->animActionType = 7;

						x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A + currentProcessedActorPtr->modX;
						y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C + currentProcessedActorPtr->modY;
						z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E + currentProcessedActorPtr->modZ;

						removeObjFromInventory(objIdx);

						objPtr->x = x;
						objPtr->y = y;
						objPtr->z = z;

						objPtr->room = currentProcessedActorPtr->room;
						objPtr->stage = currentProcessedActorPtr->stage;
						objPtr->alpha = currentProcessedActorPtr->alpha;
						objPtr->beta = currentProcessedActorPtr->beta;

						objPtr->flags2 &= 0xBFFF;
						objPtr->flags |= 0x85;
						objPtr->flags &= 0xFFDF;

						//objModifFlag1 = 1;
					}
				}
			}
			break;
		}
	case 7: // THROW
		{
			int x;
			int y;
			int z;
			int objIdx;
			int actorIdx;
			actorStruct* actorPtr;
			
			currentProcessedActorPtr->animActionType = 0;

			x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->field_9A + currentProcessedActorPtr->modX;
			y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->field_9C + currentProcessedActorPtr->modY;
			z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->field_9E + currentProcessedActorPtr->modZ;

			objIdx = currentProcessedActorPtr->animActionParam;

			actorIdx = objectTable[objIdx].ownerIdx;

			if(actorIdx == -1)
				return;

			actorPtr = &actorTable[actorIdx];

			actorPtr->roomX = x;
			actorPtr->roomY = y;
			actorPtr->roomZ = z;

			getZvNormal(HQR_Get(listBody,actorPtr->bodyNum),&actorPtr->zv);

			actorPtr->zv.ZVX1 += x;
			actorPtr->zv.ZVX2 += x;
			actorPtr->zv.ZVY1 += y;
			actorPtr->zv.ZVY2 += y;
			actorPtr->zv.ZVZ1 += z;
			actorPtr->zv.ZVZ2 += z;

			actorPtr->flags |= 1;
			actorPtr->flags &= 0xFFF7;

			objectTable[objIdx].x = x;
			objectTable[objIdx].y = y;
			objectTable[objIdx].z = z;

			objectTable[objIdx].alpha = currentProcessedActorPtr->field_0; // hu ?

			actorPtr->dynFlags = 0;
			actorPtr->animActionType = 9;
			actorPtr->animActionParam = 100;
			actorPtr->hitForce = currentProcessedActorPtr->hitForce;
			actorPtr->field_98 = -1;
			actorPtr->speed = 3000;

			startActorRotation(0, actorPtr->speed, 60, &actorPtr->speedChange);

			break;
		}
	case 8: // HIT_OBJ
		{
			break;
		}
	case 9: // during throw
		{
			objectStruct* objPtr = &objectTable[currentProcessedActorPtr->field_0];

			ZVStruct rangeZv;
			ZVStruct rangeZv2;
			int xtemp;
			int ytemp;
			int ztemp;
			int x1;
			int x2;
			int x3;
			int y1;
			int y2;
			int z1;
			int z2;
			int z3;
			int step;

			copyZv(&currentProcessedActorPtr->zv, &rangeZv);
			copyZv(&currentProcessedActorPtr->zv, &rangeZv2);

			xtemp = currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
			ytemp = currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
			ztemp = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

			rangeZv2.ZVX1 -= xtemp;
			rangeZv2.ZVX2 -= xtemp;
			rangeZv2.ZVY1 -= ytemp;
			rangeZv2.ZVY2 -= ytemp;
			rangeZv2.ZVZ1 -= ztemp;
			rangeZv2.ZVZ2 -= ztemp;

			x1 = objPtr->x;
			x2 = objPtr->x;
			x3 = objPtr->x;

			y1 = objPtr->y;
			y2 = objPtr->y;

			z1 = objPtr->z;
			z2 = objPtr->z;
			z3 = objPtr->z;

			step = 0;

			animMoveY = 0;
			animMoveX = 0;

			do
			{
				int collision;
				char* ptr;
				
				walkStep(0,-step,currentProcessedActorPtr->beta);
				step += 100;
				x2 = x1 + animMoveX;
				y2 = y1;
				z2 = z1 + animMoveY;

				copyZv(&rangeZv2,&rangeZv);

				rangeZv.ZVX1 = x2 - 200;
				rangeZv.ZVX2 = x2 + 200;
				rangeZv.ZVY1 = y2 - 200;
				rangeZv.ZVY2 = y2 + 200;
				rangeZv.ZVZ1 = z2 - 200;
				rangeZv.ZVZ2 = z2 + 200;

				collision =  processActor1Sub1(currentProcessedActorIdx,&rangeZv);

				if(collision)
				{
					 int collision2 = collision;
					 int i;

					currentProcessedActorPtr->field_9A = 0;
					currentProcessedActorPtr->field_9C = 0;
					currentProcessedActorPtr->field_9E = 0;

					 for(i=0;i<collision;i++)
					 {
						 int currentActorCol = currentProcessedActorPtr->COL[i];

						 if(actorTable[currentActorCol].field_0 == objPtr->alpha)
						 {
							 collision2--;
							 objPtr->x = xtemp;
							 objPtr->y = ytemp;
							 objPtr->z = ztemp;

							 return;
						 }

						 if(actorTable[currentActorCol].field_0 == defines.field_16)
						 {
							 objPtr->alpha = defines.field_16;
							 currentProcessedActorPtr->beta += 0x200;
							 xtemp = x3;
							 ztemp = z3;

							 currentProcessedActorPtr->worldX = currentProcessedActorPtr->roomX = x3;
							 currentProcessedActorPtr->worldY = currentProcessedActorPtr->roomY = y1;
							 currentProcessedActorPtr->worldZ = currentProcessedActorPtr->roomZ = z3;

							 currentProcessedActorPtr->modX = 0;
							 currentProcessedActorPtr->modZ = 0;

							 copyZv(&rangeZv2, &rangeZv);

							 rangeZv.ZVX1 += x3;
							 rangeZv.ZVX2 += x3;
							 rangeZv.ZVY1 += y1;
							 rangeZv.ZVY2 += y1;
							 rangeZv.ZVZ1 += z3;
							 rangeZv.ZVZ2 += z3;

							 copyZv(&rangeZv, &currentProcessedActorPtr->zv);

							 objPtr->x = xtemp;
							 objPtr->y = ytemp;
							 objPtr->z = ztemp;

							 return;

						 }
						 else
						 {
						 	 actorStruct* actorPtr;
						 	 
							 currentProcessedActorPtr->HIT = currentActorCol;
							 actorPtr = &actorTable[currentActorCol];
							 actorPtr->HIT_BY = currentProcessedActorIdx;
							 actorPtr->hitForce = currentProcessedActorPtr->hitForce;
						 }
					 }

					 if(collision2)
					 {
						 playSound(defines.field_12);
						 objectHitActor(x3,z3);
						 return;
					 }
				}

				ptr = processActor2Sub(x2,y2,z2, etageVar0 + *(unsigned int*)(etageVar0+currentProcessedActorPtr->room *4));

				if(ptr)
				{
					if(*(short int*)(ptr+0xE) == 0 || *(short int*)(ptr+0xE) == 10)
					{
						 playSound(defines.field_12);
						 objectHitActor(x3,z3);
						 return;
					}
				}

				if(checkForHardCol(&rangeZv, etageVar0 + *(unsigned int*)(etageVar0+currentProcessedActorPtr->room *4)))
				{
					currentProcessedActorPtr->field_9A = 0;
					currentProcessedActorPtr->field_9C = 0;
					currentProcessedActorPtr->field_9E = 0;

					playSound(defines.field_12);
					objectHitActor(x3,z3);
					return;
				}
			}while(		currentProcessedActorPtr->zv.ZVX1 - 100 > x2 ||
						currentProcessedActorPtr->zv.ZVX2 + 100 < x2 ||
						currentProcessedActorPtr->zv.ZVZ1 - 100 > z2 ||
						currentProcessedActorPtr->zv.ZVZ2 + 100 < z2 );

			objPtr->x = xtemp;
			objPtr->y = ytemp;
			objPtr->z = ztemp;

			break;
		}
	case 10: // PRE_HIT_IN_GOOD_ANIM
		{
			if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
			{
				if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
				{
					currentProcessedActorPtr->animActionType = 2;
				}
			}
			else
			{
				currentProcessedActorPtr->animActionType = 0;
			}
			break;
		}
	default:
		{
			printf("Unsupported processAnimAction type %d\n",currentProcessedActorPtr->animActionType);
			break;
		}
	}
}

int sortCompareFunction(void* param1, void* param2)
{
	int distance1 = 0;
	int distance2 = 0;
	actorStruct* actor1Ptr;
	actorStruct* actor2Ptr;
	ZVStruct* actor1ZvPtr;
	ZVStruct* actor2ZvPtr;
	ZVStruct localZv1;
	ZVStruct localZv2;
	int flag = 0;
	int y1;
	int y2;

	ASSERT(*(int*)param1 >=0 && *(int*)param1 < 50);
	ASSERT(*(int*)param2 >=0 && *(int*)param2 < 50);

	actor1Ptr = &actorTable[*(int*)param1];
	actor2Ptr = &actorTable[*(int*)param2];

	actor1ZvPtr = &actor1Ptr->zv;
	actor2ZvPtr = &actor2Ptr->zv;

	if(actor1Ptr->room != currentDisplayedRoom)
	{
		copyZv(actor1ZvPtr, &localZv1);
		getZvRelativePosition(&localZv1, actor1Ptr->room, currentDisplayedRoom);
		actor1ZvPtr = &localZv1;
	}

	if(actor2Ptr->room != currentDisplayedRoom)
	{
		copyZv(actor2ZvPtr, &localZv2);
		getZvRelativePosition(&localZv2, actor2Ptr->room, currentDisplayedRoom);
		actor2ZvPtr = &localZv2;
	}

	y1 = ((((actor1ZvPtr->ZVY1 + actor1ZvPtr->ZVY2) / 2) - 2000) / 2000) * 2000;
	y2 = ((((actor2ZvPtr->ZVY1 + actor2ZvPtr->ZVY2) / 2) - 2000) / 2000) * 2000;

	if(y1 == y2) // both y in the same range
	{
		if(
			((actor1ZvPtr->ZVX1 > actor2ZvPtr->ZVX1) && (actor2ZvPtr->ZVX2 > actor1ZvPtr->ZVX1)) ||
			((actor1ZvPtr->ZVX2 > actor2ZvPtr->ZVX1) && (actor1ZvPtr->ZVX2 < actor2ZvPtr->ZVX2)) ||
			((actor2ZvPtr->ZVX1 > actor1ZvPtr->ZVX1) && (actor1ZvPtr->ZVX2 > actor2ZvPtr->ZVX1)) ||
			((actor2ZvPtr->ZVX2 > actor1ZvPtr->ZVX1) && (actor2ZvPtr->ZVX2 < actor1ZvPtr->ZVX2)) )
		{
			flag |= 1;
		}

		if(
			(actor1ZvPtr->ZVZ1 > actor2ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ1 < actor2ZvPtr->ZVZ2) ||
			(actor1ZvPtr->ZVZ2 > actor2ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ2 < actor2ZvPtr->ZVZ2) ||
			(actor2ZvPtr->ZVZ1 > actor1ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ2 > actor2ZvPtr->ZVZ1) ||
			(actor2ZvPtr->ZVZ2 > actor1ZvPtr->ZVZ1 && actor2ZvPtr->ZVZ2 < actor1ZvPtr->ZVZ2) )
		{
			flag |= 2;
		}

		//TODO: remove hack and find the exact cause of the bug in the sorting algorithme
		flag = 0;

		if(flag == 0)
		{
			distance1 = computeDistanceToPoint(translateX,translateZ,(actor1ZvPtr->ZVX1+actor1ZvPtr->ZVX2)/2,(actor1ZvPtr->ZVZ1+actor1ZvPtr->ZVZ2)/2);
			distance2 = computeDistanceToPoint(translateX,translateZ,(actor2ZvPtr->ZVX1+actor2ZvPtr->ZVX2)/2,(actor2ZvPtr->ZVZ1+actor2ZvPtr->ZVZ2)/2);
		}
		else
		{
			if(flag & 2) // intersect on Z
			{
				if( abs(translateX - actor1ZvPtr->ZVX1) < abs(translateX - actor1ZvPtr->ZVX2) )
				{
					distance1 = translateX - actor1ZvPtr->ZVX1;
				}
				else
				{
					distance1 = translateX - actor1ZvPtr->ZVX2;
				}

				if( abs(translateX - actor2ZvPtr->ZVX1) < abs(translateX - actor2ZvPtr->ZVX2) )
				{
					distance2 = translateX - actor2ZvPtr->ZVX1;
				}
				else
				{
					distance2 = translateX - actor2ZvPtr->ZVX2;
				}
			}
			if(flag & 1) // intersect on X
			{
				if( abs(translateZ - actor1ZvPtr->ZVZ1) < abs(translateZ - actor1ZvPtr->ZVZ2) )
				{
					distance1 += translateZ - actor1ZvPtr->ZVZ1;
				}
				else
				{
					distance1 += translateZ - actor1ZvPtr->ZVZ2;
				}

				if( abs(translateZ - actor2ZvPtr->ZVZ1) < abs(translateZ - actor2ZvPtr->ZVZ2) )
				{
					distance2 += translateZ - actor2ZvPtr->ZVZ1;
				}
				else
				{
					distance2 += translateZ - actor2ZvPtr->ZVZ2;
				}
			}
		}

	}
	else
	{
		distance1 = abs(translateY - 2000 - y1);
		distance2 = abs(translateY - 2000 - y2);
	}

	if(distance1>distance2)
	{
		return(-1);
	}

	if(distance1<distance2)
	{
		return(1);
	}

	return(0);
}

void sortActorList()
{
	qsort(sortedActorTable, numActorInList, sizeof(int), sortCompareFunction);
}

void mainLoop(int allowSystemMenu)
{
	while(1)
	{
		timer = timeGlobal;
		process_events();
		readKeyboard();
		input3 = input2;
		input4 = inputKey;
		button = input1;

		if(input3)
		{
			if(input3 == 0x1C || input3 == 0x17)
			{
				if(allowSystemMenu == 0)
				{
					break;
				}

				if(statusScreenAllowed)
				{
					makeStatusScreen();
				}
			}
		}
		else
		{
//			input5 = 0;
		}

		if(button)
		{
			if(!allowSystemMenu)
			{
				break;
			}

			action = 0x2000;
		}
		else
		{
			action = 0;
		}

		updateInHand(inHand);

		if(changeFloor == 0)
		{
			if(defines.field_1A == -1)
			{
//				mainVar2 = 2000;
//				mainVar3 = 2000;
			}

			currentProcessedActorPtr = actorTable;

			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					currentProcessedActorPtr->COL_BY = -1;
					currentProcessedActorPtr->HIT_BY = -1;
					currentProcessedActorPtr->HIT = -1;
					currentProcessedActorPtr->HARD_DEC = -1;
					currentProcessedActorPtr->HARD_COL = -1;
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					int flag = currentProcessedActorPtr->flags;

					if(flag & 1)
					{
						processActor1();
					}

					if(flag & 0x40)
					{
						processActor2();
					}

					if(currentProcessedActorPtr->animActionType)
					{
						processAnimAction();
					}
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					if(currentProcessedActorPtr->life != -1 && currentProcessedActorPtr->lifeMode != -1)
					{
						processLife(currentProcessedActorPtr->life);
					}
				}

				if(changeFloor)
					break;

				currentProcessedActorPtr++;
			}

			if(giveUp)
				break;


		}

		if(changeFloor)
		{
			loadFloor(newFloor);
		}

		if(needChangeRoom)
		{
			loadRoom(newRoom);
			setupCamera();
		}
		else
		{
			checkIfCameraChangeIsRequired();
			if(mainVar1)
			{
				setupCamera();
			}
		}

//		if(objModifFlag1)
		{
			updateAllActorAndObjects();
		} 

		if(actorTurnedToObj)
		{
//			createActorList();
		}

		sortActorList();

//		if(objModifFlag2)
		{
//			setupCameraSub4();
		}

//		mainLoopSub1();

		mainDraw(setupCameraVar1);

		//osystem_delay(100);

//		updateSound2();
	}

//	mainLoopVar1 = 0;
//	shakingState = 0;

//	stopShaking();
//	stopSounds();
}

void startGame(int startupFloor, int startupRoom, int allowSystemMenu)
{
	initEngine();
	initVars();

	loadFloor(startupFloor);

	currentCamera = -1;

	loadRoom(startupRoom);

	startGameVar1 = 0;
	mainVar1 = 2;

	setupCamera();

	mainLoop(allowSystemMenu);

	/*freeScene();

	fadeOut(8,0);*/
}

int parseAllSaves(int arg)
{
	return(0);
	// TODO : make real implementation
}

unsigned int currentSaveEntrySize;

void* getSaveEntry(int index)
{
	currentSaveEntrySize = saveTable[index].size;

	return(saveTable[index].ptr);
}

void configureHqrHero(hqrEntryStruct* hqrPtr, char* name)
{
	strcpy(hqrPtr->string,"        ");
	strncpy(hqrPtr->string,name,8);
}

int initAnimInBody(int frame, char* anim, char* body)
{
	short int temp;
	short int ax;
	short int cx;
	short int bx;
	char* saveAnim;
	int i;

	temp = *(short int*)anim;
	anim+=2;

	if(frame >= temp)
	{
		return(0);
	}

	ax = *(short int*)anim;
	anim+=2;

	cx = ax;

	ax = ((ax+1)<<3)*frame;

	anim+=ax;

	animCurrentTime = *(short int*)anim;
	animKeyframeLength = animCurrentTime;

	if(!((*(short int*)body)&2))
	{
		return(0);
	}

	body +=14;

	*(char**)(body+2) = anim;
	*(unsigned short int*)(body+6) = timer;

	body+= *(short int*)body;
	body+=2;

	ax = *(short int*)body;
	bx = ax;

	body+=(((ax << 1) + bx) << 1) +2;

	bx = ax = *(short int*)body;

	body+=bx<<1;

	if(cx > ax)
		cx = ax;

	body+=10;

	saveAnim = anim;

	anim += 8;

	for(i=0;i<cx;i++)
	{
		*(short int*)(body) = *(short int*)(anim);
		body+=2;
		anim+=2;
		*(short int*)(body) = *(short int*)(anim);
		body+=2;
		anim+=2;
		*(short int*)(body) = *(short int*)(anim);
		body+=2;
		anim+=2;
		*(short int*)(body) = *(short int*)(anim);
		body+=2;
		anim+=2;

		body+=8;
	}

	anim = saveAnim;

	anim+=2;

	animRot2 = *(short int*)anim;
	anim+=2;
	animRot3 = *(short int*)anim;
	anim+=2;
	animRot1 = *(short int*)anim;
	anim+=2;

	return(1);

}

int loadSave(int saveNumber)
{
	char buffer[256];
	FILE* fHandle;
	unsigned int var28;
	int var_14 = 0;
	char* ptr;
	int var_E;
	int var_16;
	unsigned int offsetToVars;
	unsigned short int tempVarSize;
	unsigned int offsetToActors;
	int i;
	
	sprintf(buffer,"SAVE%d.ITD",saveNumber);

	fHandle = fopen(buffer,"rb");

	if(!fHandle)
	{
		return(0);
	}

	initEngine();
	initVars();

	fseek(fHandle,8,SEEK_SET);

	fread(&var28,4,1,fHandle);

	var28 = ((var28 & 0xFF) << 24) | ((var28 & 0xFF00) << 8) | (( var28 & 0xFF0000) >> 8) | ((var28 & 0xFF000000) >> 24);

	fseek(fHandle,var28,SEEK_SET);

	do
	{
		ptr = (char*)getSaveEntry(var_14);

		if(ptr)
		{
			fread(ptr,currentSaveEntrySize,1,fHandle);
		}
		var_14++;

	}while(ptr);

	//timerFreeze = 1;

	var_E = currentCamera;

	loadFloor(currentEtage);
	currentCamera = -1;
	loadRoom(currentDisplayedRoom);
	var_16 = currentMusic;
	currentMusic = -1;
//	changeMusic(var_16);

	fseek(fHandle,12,SEEK_SET);
	fread(&offsetToVars,4,1,fHandle);
	offsetToVars = ((offsetToVars & 0xFF) << 24) | ((offsetToVars & 0xFF00) << 8) | (( offsetToVars & 0xFF0000) >> 8) | ((offsetToVars & 0xFF000000) >> 24);
	fseek(fHandle,offsetToVars,SEEK_SET);

	fread(&tempVarSize,2,1,fHandle);
	varSize = tempVarSize;

	fread(vars,varSize,1,fHandle);

	configureHqrHero(listBody,listBodySelect[defines.hero]);
	configureHqrHero(listAnim,listAnimSelect[defines.hero]);

	fseek(fHandle,16,SEEK_SET);
	fread(&offsetToActors,4,1,fHandle);
	offsetToVars = ((offsetToActors & 0xFF) << 24) | ((offsetToActors & 0xFF00) << 8) | (( offsetToActors & 0xFF0000) >> 8) | ((offsetToActors & 0xFF000000) >> 24);
	fseek(fHandle,offsetToVars,SEEK_SET);

	fread(actorTable,8000,1,fHandle);
	fclose(fHandle);

	for(i=0;i<50;i++)
	{
		if(actorTable[i].field_0 != -1 && actorTable[i].bodyNum != -1)
		{
			char* bodyPtr = HQR_Get(listBody,actorTable[i].bodyNum);

			if(actorTable[i].ANIM != -1)
			{
				char* animPtr = HQR_Get(listAnim,actorTable[i].ANIM);
				initAnimInBody(actorTable[i].FRAME,animPtr,bodyPtr);
			}
		}
	}

	startGameVar1 = var_E;

	return(1);
}

int restoreSave(int arg0, int arg1)
{
	int selectedSave;
//	restoreSaveVar1 = arg0;

	if(arg1==0)
	{
		flushScreen();
		flipScreen();
		make3dTatouUnk1(0x40,0);
	}

	selectedSave = parseAllSaves(0);

	if(arg1==0)
	{
	//	fadeOut(8,0);
	}

	if(selectedSave == -1)
	{
		return(0);
	}

	if(arg1==0)
	{
		//freeScene();
	}

	return(loadSave(selectedSave));

}

int main(int argc, char** argv)
{
	int startupMenuResult;
//	int protectionToBeDone = 1;
	char version[256];
	getVersion(version);

	printf(version);

	osystem_init();
	osystem_initBuffer(scaledScreen,640,400);
	startThreadTimer();

	sysInit();

	paletteFill(palette,0,0,0);
	fadeIn(palette);

	preloadResource();

	if(!make3dTatou())
	{
		makeIntroScreens();
	}

	while(1)
	{
		startupMenuResult = processStartupMenu();

		switch(startupMenuResult)
		{
		case -1: // timeout
			{
				defines.hero = rand()&1;
			/*	startGame(7,1,0);

				if(!make3dTatou())
				{
					if(!makeIntroScreens())
					{
						makeSlideshow();
					}
				} */

				break;
			}
		case 0: // new game
			{
			/*	if(protectionToBeDone)
				{
					makeProtection();
					protectionToBeDone = 0;
				}*/

				//if(selectHero()!=-1)
				{
					readKeyboard();
					while(input2)
						readKeyboard();

					defines.hero = 0;
					startGame(7,1,0);

				/*	if(!protectionState)
					{
						freeAll();
						exit(-1);
					}
*/
					startGame(0,0,1);
/*
					if(giveUp == 0)
					{
						freeAll();
						exit(-1);
					}*/
				}

				break;
			}
		case 1: // continue
			{
			/*	if(protectionToBeDone)
				{
					makeProtection();
					protectionToBeDone = 0;
				}*/

				if(restoreSave(12,0))
				{
				/*	if(!protectionState)
					{
						freeAll();
						exit(-1);
					}*/

//					updateShaking();

					mainVar1 = 2;

					setupCamera();

					mainLoop(1);

//					freeScene();

					fadeOut(8,0);

				/*	if(giveUp == 0)
					{
						freeAll();
						exit(-1);
					} */
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
	}

	return(0);
}

int drawTextOverlay(void)
{
	int var_2 = 0;
	int var_14 = 0;
	int var_10 = 183;
	messageStruct* currentMessage;

	BBox3D4 = 199;
	BBox3D1 = 319;
	BBox3D3 = 0;

	currentMessage = messageTable;

	if(lightVar1==0)
	{
		int i;

		for(i=0;i<5;i++)
		{
			if(currentMessage->string)
			{
				int width = currentMessage->string->width;
				int X = 160 - width/2;
				int Y = X + width;

				if(X<BBox3D1)
				{
					BBox3D1 = X;
				}

				if(Y>BBox3D3)
				{
					BBox3D3 = Y;
				}

				if((currentMessage->time++)>55)
				{
					currentMessage->string = NULL;
				}
				else
				{
					if(currentMessage->time<26)
					{
						initFont(fontData,16);
					}
					else
					{
						initFont(fontData,16+(currentMessage->time-26)/2);
					}

					renderText(X,var_10+1,screen,currentMessage->string->textPtr);
				}

				var_10 -= 16;
				var_14 = 1;

			}

			currentMessage++;
		}
	}
	else
	{
	}

	BBox3D2 = var_10;
	return(var_14);
}

void makeMessage(int messageIdx)
{
	textEntryStruct* messagePtr;

	messagePtr = getTextFromIdx(messageIdx);

	if(messagePtr)
	{
		int i;

		for(i=0;i<5;i++)
		{
			if(messageTable[i].string == messagePtr)
			{
				messageTable[i].time = 0;
				return;
			}
		}

		for(i=0;i<5;i++)
		{
			if(messageTable[i].string == NULL)
			{
				messageTable[i].string = messagePtr;
				messageTable[i].time = 0;
				return;
			}
		}
	}
}

void hit(int animNumber,int arg_2,int arg_4,int arg_6,int hitForce,int arg_A)
{
	if(anim(animNumber, 0, arg_A))
	{
		currentProcessedActorPtr->animActionANIM = animNumber;
		currentProcessedActorPtr->animActionFRAME = arg_2;
		currentProcessedActorPtr->animActionType = 1;
		currentProcessedActorPtr->animActionParam = arg_6;
		currentProcessedActorPtr->field_98 = arg_4;
		currentProcessedActorPtr->hitForce = hitForce;
	}
}

void setClipSize(int left, int top, int right, int bottom)
{
	clipLeft = left;
	clipTop = top;
	clipRight = right;
	clipBottom = bottom;
}