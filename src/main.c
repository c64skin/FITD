// 22 septembre 2003 14:25

// seg002

#include "common.h"

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

	while(character = (unsigned char)*(string++))
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

	fillBox(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom,0);
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
		data = *(unsigned short int*)dataPtr;
	
		data = ((data & 0xFF)<<8)| ((data&0xFF00)>>8);

		dx = data;

		data >>= 12;

		if(data&0xF) // real character (width != 0)
		{
			char* characterPtr;

			dx &= 0xFFF;

			characterPtr = (dx>>3) + fontVar4;

			fontSm9 = flagTable[dx & 7];

			int bp = fontSm7;

			fontSm8 = fontVar6;

			int ch;

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

/*	FILE* fileHandle;
	fileHandle = fopen("dump.raw","wb+");
	fwrite(screen,320,200,fileHandle);
	fclose(fileHandle);*/
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

	while(evalChrono(&chrono) <= 0x10000) // exit loop only if time out or if choice made
	{
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

int selectHero(void)
{
	int var_6 = 0;
	int var_4 = 1;
	int var_2 = 0;

	//sysInitSub1(aux,screen);

	while(var_2 == 0)
	{
		process_events();
		readKeyboard();
	
		loadPakToPtr("itd_ress",10,aux);
		copyToScreen(aux,screen);
		copyToScreen(screen,aux2);

		if(var_6 == 0)
		{
			drawAITDBox(80,100,160,200);
		//	selectHeroSub1(10,10,149,190);
		}
		else
		{
			drawAITDBox(240,100,160,200);
		//	selectHeroSub1(170,10,309,190);
		}

		flipScreen();

		if(var_4 != 0)
		{
			//make3dTatouUnk1(0x40,0);
		}
	}

	//fadeOut(0x40,0);

	//sysInitSub1(aux2,screen);

	return(-1);
}

int printTextSub1(hqrEntryStruct* hqrPtr,int size)
{
	hqrSubEntryStruct* dataPtr1;
	hqrSubEntryStruct* dataPtr2;
	int key;
	int entryNum;

	if(hqrPtr->sizeFreeData<size)
		return(-1);

	entryNum = hqrPtr->numUsedEntry;

	dataPtr1 = dataPtr2 = (hqrSubEntryStruct*)(hqrPtr+sizeof(hqrEntryStruct));

	key = hqrKeyGen;

	dataPtr1[entryNum].key = key;

	dataPtr1[entryNum].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
	dataPtr1[entryNum].size = size;

	hqrPtr->numUsedEntry++;
	hqrPtr->sizeFreeData -= size;

	hqrKeyGen++;

	return(key);
}

hqrSubEntryStruct* quickFindEntry(int index, int numMax, hqrSubEntryStruct* ptr) // no RE. Original was probably faster
{
	int i;

	for(i=0;i<numMax;i++)
	{
		if(ptr->key == index)
		{
			return(ptr);
		}
	}

	return(NULL);
}

char* printTextSub2(hqrEntryStruct* hqrPtr, int index)
{
	hqrSubEntryStruct* ptr;
	hqrSubEntryStruct* dataPtr;

	if(index<0)
		return NULL;

	dataPtr = (hqrSubEntryStruct*)(hqrPtr+sizeof(hqrEntryStruct));

	ptr = quickFindEntry(index, hqrPtr->numUsedEntry, dataPtr);

	if(!ptr)
		return NULL;

	return(hqrPtr->dataPtr + ptr->offset);
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
	int var_1A8 = 0;
	char tabString[] = "    ";
	int var_12 = 1;
	int currentPage = 0;
	int quit = 0;
	int var_2 = -1;
	int var_1C3;
	char* localTextTable[100];

	initFont(fontData, color);

	int maxStringWidth = right - left + 4;

	int var_8 = printTextSub1(hqrUnk,getPakSize(languageNameString,index)+300);

	char* textPtr = printTextSub2(hqrUnk, var_8);

	if(!loadPakToPtr( languageNameString, index, textPtr))
	{
		theEnd(1, languageNameString );
	}

	localTextTable[0] = textPtr;

//	soundVar2 = -1;
//	soundVar1 = -1;

	while(!quit)
	{
		copyToScreen(aux,screen);
		process_events();
		//setClipSize(left,top,right,bottom);

		char* var_1C2 = localTextTable[currentPage];

		int currentTextY = top;
		var_1A8 = 0;

		while(currentTextY <= bottom - 16)
		{
			int var_1AA = 1;
			int var_1BA = 0;

			regularTextEntryStruct* currentText = textTable;

			int maxTextIdx = 0;

			int var_1B6 = 0;

parseSpe:	while(*var_1C2 == '#')
			{
				char* var_1BE = var_1C2;
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
						break;
					}
				case 'C': // center
					{
						var_1AA &= 0xFFFE;
						var_1AA |= 8;
						break;
					}
				case 'G':
					{
						break;
					}
				}
			}

			currentText->textPtr = var_1C2;

			do
			{
				var_1C3 = *(var_1C2++);
			}while(var_1C3>' '); // go to the end of the string

			*(var_1C2-1) = 0; // add end of string marker to cut the word

			int currentStringWidth = computeStringWidth(currentText->textPtr) + 3;

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

					maxTextIdx++;
					currentText++;

					switch(var_1C3) // eval the character that caused the 'end of word' state
					{
					case 0x1A:
						{
							var_1AA &= 0xFFFE;
							var_1AA |= 4;
							var_1A8 = 1;

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

			if(var_1AA & 1)
			{
			//	var_1B6 = (maxStringWidth - var_1BA) / (maxTextIdx-1);
			}

			currentText = textTable;
			int currentTextX;

			if(var_1AA & 8)
			{
				currentTextX = left + ((maxStringWidth - var_1BA) / 2);
			}
			else
			{
				currentTextX = left;
			}

			int currentTextIdx = 0;

			while(currentTextIdx < maxTextIdx)
			{
				renderText(currentTextX,currentTextY,screen,currentText->textPtr);

				currentTextX += currentText->width + var_1B6;

				currentText++;
				currentTextIdx++;
			}

			if(var_1AA&2) // font size
			{
				currentTextY += 8;
			}

			currentTextY += 16;
		}

		if(!var_1A8 || bottom+16<currentTextY)
		{
pageChange:	if(var_1A8)
			{
				*(var_1C2-1) = 0x1A;
			}
			else
			{
				var_1C2 = localTextTable[currentPage];
			}

			if(mode==0)
			{
				if(currentPage>0)
				{
					printTextSub5(left-19,185,12,aitdBoxGfx);
				}

				if(var_1A8==0)
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

				if(var_1A8==0)
				{
					printTextSub5(right-10,191,14,aitdBoxGfx);
				}
			} 

			if(var_12)
			{
				if(mode!=1)
				{
					flipScreen();
//								make3dTatouUnk1(16,0);
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
				}
				while(input2 && inputKey && input1);

				input3 = input2 & 0xFF;
				input4 = inputKey & 0xFF;
				joy = input1;

				if(input3==1 || !joy)
				{
					quit = 1;
				}
				else
				{
					if(mode!=2 || input3 != 0x1C)
					{
						if(inputKey&0xA0 || input3 == 0x1C)
						{
							if(var_1A8 == 0) // flip to next page
							{
								var_2 = currentPage++;

								if(mode==2)
								{
//												playSound(defines.field_0);
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

			/*	do
				{
					if(evalChrono(&var_6) > 300)
					{
						break;
					}
				}while(!input2 && !input1);*/

				if(input2 || input1)
				{
					quit = 1;
				}

				if(var_1A8==0)
				{
				//	currentPage++;
//								playSound(defines.field_0);
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

	data = loadPak("itd_ress",13);
	copyToScreen(data+770,unkScreenVar);
	//make3dTatouUnk1(8,0);
	memcpy(screen,unkScreenVar,320*200);
	flipScreen();
	free(data);
	loadPakToPtr("itd_ress",7,aux);
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

	//playSound(defines.field_0);
/*	soundVar2 = -1;
	soundVar1 = -1;
	soundVar2 = -1;
	soundVar1 = 0; */
//	readVar = 1;
	printText(defines.field_6+1,48, 2,260,197,1,26);

	return(0);
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

	//if(!make3dTatou())
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

				if(selectHero()!=-1)
				{
				/*	startGame(7,1,0);

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
				}*/

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