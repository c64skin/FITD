// 22 septembre 2003 14:25

// seg002

#include "common.h"

char scaledScreen[640*400];

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

unsigned char flagTable[]= {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void renderText(int x, int y, char* surface, char* string)
{
	unsigned char character;

	fontVar6 = x;
	fontSm7 = y;

	while(character = *((unsigned char*)string++))
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

	memcpy(unkScreenVar,screen,320*200);

	for(i=0;i<256;i++)
	{
		paletteRGBA[i*4] = palette[i*3];
		paletteRGBA[i*4+1] = palette[i*3+1];
		paletteRGBA[i*4+2] = palette[i*3+2];
		paletteRGBA[i*4+3] = -1;
	}

	char* outPtr = scaledScreen;
	char* inPtr = unkScreenVar;

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

	osystem.setPalette(paletteRGBA);
	osystem.Flip((unsigned char*)scaledScreen);
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
	loadPakToPtr("ITD_RESS",3,aux);
	copyPalette(aux,palette);
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
			//make3dTatouUnk1(0x40,0);

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
//				fadeOut(0x40,0);
				return(-1);
			}
		}

//		fadeOut(0x40,0);
		readVar = 0;

		switch(choice)
		{
		case 0:
			{
				copyToScreen(unkScreenVar,screen);
//				setClipSize(0,0,319,199);
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
//				setClipSize(0,0,319,199);
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

	//fadeOut(0x40,0);

	sysInitSub1(aux2,screen);

	return(choice);
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

	dataPtr1 = dataPtr2 = (hqrSubEntryStruct*)((char*)hqrPtr+sizeof(hqrEntryStruct));

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
		if(ptr[i].key == index)
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

	dataPtr = (hqrSubEntryStruct*)((char*)hqrPtr+sizeof(hqrEntryStruct));

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
	bool lastPageReached = false;
	char tabString[] = "    ";
	int var_12 = 1;
	int currentPage = 0;
	int quit = 0;
	int var_2 = -1;
	int var_1C3;
	char* localTextTable[100];
	int currentTextIdx;

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
		lastPageReached = false;

		while(currentTextY <= bottom - 16)
		{
			int var_1AA = 1;
			int var_1BA = 0;

			regularTextEntryStruct* currentText = textTable;

			int numWordInLine = 0;

			int interWordSpace = 0;

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
			int currentTextX;

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
				}while(input2 || inputKey || input1);

				input3 = input2 & 0xFF;
				input4 = inputKey & 0xFF;
				joy = input1;

				if(input3==1 || joy)
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

	data = loadPak("ITD_RESS",13);
	copyToScreen(data+770,unkScreenVar);
	//make3dTatouUnk1(8,0);
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

	//playSound(defines.field_0);
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

	listLife = HQR_InitRessource("LISTLIFE", 10000, 100);
	listTrack = HQR_InitRessource("LISTTRAK", 1000, 10);

	// TODO: missing dos memory check here

	char* listBodySelect[] = {
		"LISTBODY",
		"LISTBOD2",
	};

	char* listAnimSelect[] = {
		"LISTANIM",
		"LISTANI2",
	};

	listBody = HQR_InitRessource(listBodySelect[defines.hero],100000, 50); // was calculated from free mem size
	listAnim = HQR_InitRessource(listAnimSelect[defines.hero],100000, 50); // was calculated from free mem size

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
		messageVar1[i] = NULL;
	}
}

void initVars()
{
	giveUp = 0;
	inHand = -1;
	numObjInInventory = 0;
	found = 0;

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

	currentCamera = 1;
	needChangeRoom = 1;
	changeFloor = 0;
}

void loadRoom(int roomNumber)
{
	int i;
	int cameraVar0;
	int cameraVar1;
	int cameraVar2;
	int cameraVar3;

//	freezeTime();

	if(currentCamera == -1)
	{
		cameraVar3 = -1;
	}
	else
	{
		cameraVar0 = *(short int*)(cameraPtr+4);
		cameraVar1 = *(short int*)(cameraPtr+6);
		cameraVar2 = *(short int*)(cameraPtr+8);

		cameraVar3 = *(short int*)(cameraPtr + (currentCamera+6)*2);
	}

	cameraPtr = etageVar0+*(unsigned int*)(etageVar0 + (roomNumber * 4));

	currentDisplayedRoom = roomNumber;

	roomVar0 = *(short int*)(cameraPtr + 0xA);

	char* var_20 = cameraPtr + *(unsigned short int*)(cameraPtr + 2);

	roomVar1 = *(short int*)var_20;
	var_20 += 2;

	roomVar2 = var_20;

	var_20 = cameraPtr + *(unsigned short int*)cameraPtr;

	roomVar3 = *(short int*)var_20;
	var_20 += 2;

	roomVar4 = var_20;

	int var_1A = 0;
	int var_10 = -1;
	int var_1C;
	
	for(i=0;i<roomVar0;i++) // search ?
	{
		int var_22 = *(short int*)(cameraPtr + (i+6)*2);

		if(cameraVar3 == var_22)
		{
			var_1A = i;
			var_10 = var_22;
		}

		roomVar5[i] = etageVar1 + *(unsigned int*)(etageVar1 + var_22 * 4);
		var_20 = roomVar5[i];
		var_22 = *(short int*)var_20;
		var_20 +=2;
		int var_24 = 0;

		for(;*(short int*)(var_20+=2)!=currentDisplayedRoom;(var_24++) && (var_20+=0xA))
		{
			if(var_24>= var_22)
			{
				break;
			}
		}

		var_1C = var_24;

		char* var_8 = roomVar5[i] + (var_1C << 3) + (var_1C<<2) + 0x18;

		roomVar6[i] = (*(short int*)var_8)*2;
	}

	if(cameraVar3 != -1)
	{
		int var_E = (*(short int*)(cameraPtr + 4) - cameraVar0) * 10;
		int var_C = (*(short int*)(cameraPtr + 6) - cameraVar1) * 10;
		int var_A = (*(short int*)(cameraPtr + 8) - cameraVar2) * 10;

		for(i=0;i<50;i++)
		{
			if(actorTable[i].field_0 != -1)
			{
				actorTable[i].field_22 -= var_E;
				actorTable[i].field_24 -= var_C;
				actorTable[i].field_26 -= var_A;
			}
		}
	}

	startGameVar1 = var_1A;
	mainVar1 = 1;
	needChangeRoom = 0;
//	unfreezeTime();
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

	short int* ptr2 = (short int*)(roomVar5[currentCamera] + 18);

	int var_12 = *(ptr2++);

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

void updateAllActorAndObjectsSub1(int index)
{
	actorStruct *actorPtr = &actorTable[index];

	if(actorPtr->field_0 == -2)
	{
		actorPtr->field_0 = -1;

		if(actorPtr->hitBy == 4 )
		{
			defines.field_1C = 0;
		}

		printTextSub6(hqrUnk,actorPtr->endAnim);
	}
	else
	{
		if(actorPtr->field_0 >= 0)
		{
			objectStruct* objectPtr = &objectTable[actorPtr->field_0];

			objectPtr->ownerIdx = -1;
			actorPtr->field_0 = -1;

			objectPtr->field_2 = actorPtr->bodyNum;
			objectPtr->field_26 = actorPtr->hitBy;
			objectPtr->field_28 = actorPtr->endAnim;
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

			objectPtr->x = actorPtr->x + actorPtr->field_5A;
			objectPtr->y = actorPtr->y + actorPtr->field_5C;
			objectPtr->z = actorPtr->z + actorPtr->field_5E;

			objectPtr->alpha = actorPtr->alpha;
			objectPtr->beta = actorPtr->beta;
			objectPtr->gamma = actorPtr->gamma;

			objectPtr->stage = actorPtr->stage;
			objectPtr->room = actorPtr->room;

			actorTurnedToObj = 1;
		}
	}
}

char* HQR_Get(hqrEntryStruct* hqrPtr, int index)
{
	if(index<0)
		return NULL;

	hqrSubEntryStruct* hqrSubPtr = (hqrSubEntryStruct*)(((char*)hqrPtr)+sizeof(hqrEntryStruct));

	hqrSubEntryStruct* foundEntry = quickFindEntry(index,hqrPtr->numUsedEntry,hqrSubPtr);

	if(foundEntry)
	{
		foundEntry->lastTimeUsed = timer;
//		hqrVar1 = 0;

		return(hqrPtr->dataPtr + foundEntry->offset);
	}
	else
	{
		//freezeTime();
		int size = getPakSize(hqrPtr->string,index);

		if(size>=hqrPtr->maxFreeData)
		{
			theEnd(1,hqrPtr->string);
		}

		unsigned int time = timer;

		foundEntry = hqrSubPtr;

		while(size>hqrPtr->sizeFreeData || hqrPtr->numUsedEntry>= hqrPtr->numMaxEntry)
		{
			printf("Unimplemented code int HQR_Get\n");
			exit(1);
		}

		char* ptr = hqrPtr->dataPtr + (hqrPtr->maxFreeData - hqrPtr->sizeFreeData);

		if(!loadPakToPtr(hqrPtr->string,index,ptr))
		{
			theEnd(1,hqrPtr->string);
		}

//		hqrVar1 = 1;

		foundEntry[hqrPtr->numUsedEntry].key = index;
		foundEntry[hqrPtr->numUsedEntry].lastTimeUsed = timer;
		foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
		foundEntry[hqrPtr->numUsedEntry].size = size;

		hqrPtr->numUsedEntry++;
		hqrPtr->sizeFreeData -= size;

		return(ptr);
	}

}

int copyObjectToActor(int flag2, int var1, int foundName, int flag, int x, int y, int z, int stage, int room, int alpha, int beta, int gamma, int var2, int var3, int var4, int var5)
{
	int i;
	int j;
	actorStruct* actorPtr = actorTable;

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
	actorPtr->field_22 = actorPtr->x = x;
	actorPtr->field_24 = actorPtr->y = y;
	actorPtr->field_26 = actorPtr->z = z;

	if(room == currentDisplayedRoom)
	{
		char* roomPtr = etageVar0+room*4;

		actorPtr->field_22 = ((*(short int*)(cameraPtr+4)) - (*(short int*)(roomPtr+4))) * 10;
		actorPtr->field_24 = ((*(short int*)(cameraPtr+6)) - (*(short int*)(roomPtr+6))) * 10;
		actorPtr->field_26 = ((*(short int*)(cameraPtr+8)) - (*(short int*)(roomPtr+8))) * 10;
	}

	actorPtr->alpha = alpha;
	actorPtr->beta = beta;
	actorPtr->gamma = gamma;

	actorPtr->dynFlags = 1;
	
	actorPtr->hitBy = var2;
	actorPtr->endAnim = var3;

	actorPtr->field_40 = var4;
	actorPtr->field_42 = var5;

	actorPtr->frame = 1;
	actorPtr->anim = 1;
	actorPtr->field_44 = -1;
	actorPtr->field_46 = 0;
	actorPtr->field_48 = -1;
	actorPtr->field_5A = 0;
	actorPtr->field_5C = 0;
	actorPtr->field_5E = 0;

	for(j=0;j<3;j++)
	{
		actorPtr->field_7E[j] = -1;
	}

	actorPtr->field_84 = -1;
	actorPtr->col = -1;
	actorPtr->hardDec = -1;

	actorPtr->field_6A = 0;
	actorPtr->field_6C = 0;
	actorPtr->field_6E = 0;

	actorPtr->field_60 = 0;
	actorPtr->field_62 = 0;
	actorPtr->field_64 = 0;

	actorPtr->falling = 0;

	actorPtr->field_72 = 0;

	actorPtr->speed = 0;

	actorPtr->trackMode = 0;
	actorPtr->trackNumber = -1;

	actorPtr->field_8E = 0;
	actorPtr->hardCol = -1;
	actorPtr->hit = -1;

	if(flag2 != -1)
	{
		char* bodyPtr = HQR_Get(listBody,actorPtr->bodyNum);

		if(var2 != -1)
		{
			char* animPtr = HQR_Get(listAnim,var2);

//			initAnimInBody(var3,animPtr,bodyPtr);

//			actorPtr->field_4C = getAnimParam(animPtr);
			actorPtr->anim = 0;
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

	startChrono(&actorPtr->chronoStructure);
	startChrono(&actorPtr->field_36);

	ZVStruct* zvPtr = &actorPtr->zv;

	switch(var1)
	{
	case 0:
		{
			break;
		}
	case 1:
		{		
			break;
		}
	case 2:
		{
			break;
		}
	case 3:
		{
			break;
		}
	case 4:
		{
			break;
		}

	}

	return(0);
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
			currentProcessedActorPtr->body = -1;
			break;
		}
	case 3:
		{
			currentProcessedActorPtr->trackNumber = trackNumber;
			currentProcessedActorPtr->body = -1;
			currentProcessedActorPtr->positionInTrack = 0;
			currentProcessedActorPtr->body = -1;
			break;
		}
	}
}

void updateAllActorAndObjects()
{
	int i;
	actorStruct *currentActor = actorTable;

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

	objectStruct* currentObject = objectTable;

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

						int var_C = currentObject->flags & 0xFFDF;
						int var_E = currentObject->flags2;
						int var_A = currentObject->field_26;

						int actorIdx = copyObjectToActor(	currentObject->flags2, currentObject->field_6, currentObject->foundName,
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

							currentProcessedActorPtr->flags = (currentObject->flags & 0x20) / 0x20; // recheck
							currentProcessedActorPtr->life = currentObject->life;
							currentProcessedActorPtr->lifeMode = currentObject->lifeMode;

							currentProcessedActorPtr->field_0 = i;

							setMoveMode(currentProcessedActorPtr->trackMode, currentProcessedActorPtr->trackNumber);

							currentProcessedActorPtr->positionInTrack = currentObject->positionInTrack;

							actorTurnedToObj = 1;
						}
					}
				}
			}
		}

		currentObject++;
	}

//	objModifFlag1 = 0;

	//TODO: object update
}

void setupCamera()
{
//	freezeTime();

	currentCamera = startGameVar1;

	loadCamera(*(cameraPtr+(startGameVar1+6)*2));

	char* ptr = roomVar5[currentCamera];

	setupPointTransformSM(*(short int*)(ptr),*(short int*)(ptr+2),*(short int*)(ptr+4));
	ptr+= 6;

	int x = (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+4))) << 3) + (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+4))) << 1);
	ptr+=2;
	int y = (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+6))) << 3) + (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+6))) << 1);
	ptr+=2;
	int z = (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+8))) << 3) + (((*(short int*)(ptr)) - (*(short int*)(cameraPtr+8))) << 1);
	ptr+=2;

	setupSelfModifyingCode(x,y,z);
	setupSMCode(160,100,*(short int*)(ptr),*(short int*)(ptr+2),*(short int*)(ptr+4));
	ptr+=6;

	setupCameraSub1();
	updateAllActorAndObjects();
/*	setupCameraSub2();
	setupCameraSub3(); */
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
//unfreezeTime();
}

int evalVar(void)
{
	int var1;

	var1 = *(short int*)(currentLifePtr);
	currentLifePtr+=2;

	if(var1 == -1)
	{
		int temp = *(short int*)(currentLifePtr);
		currentLifePtr+=2;

		return(temp);
	}
	else
	if(var1 == 0)
	{
		printf("Unimplemented code int evalVar: var1 == 0\n");
		exit(1);
	}
	else
	{
		if(var1 & 0x8000)
		{
			printf("Unimplemented code int evalVar: var1 & 0x8000\n");
			exit(1);
		}
		else
		{
			int actorIdx = currentLifeActorIdx;
			actorStruct* actorPtr = currentLifeActorPtr;

			switch(var1)
			{
			case 0xC: // NUM_TRACK ?
				{
					return(evalChrono(&actorPtr->field_36) / 0x3C0000); // recheck
					break;
				}
			case 0xB: // MARK ?
				{
					return(actorPtr->trackNumber);
					break;
				}
			default:
				{
					printf("Unhandled test type %X in evalVar\n",var1);
					exit(1);
					break;
				}
			}
		}
	}
}

void processTrack(void)
{
	switch(currentProcessedActorPtr->trackMode)
	{
	case 1: // manual
		{
			printf("Manual move !\n");
			break;
		}
	case 2: // ?
		{
			printf("unsupported move mode 2\n");
			exit(1);
			break;
		}
	case 3: // track
		{
			char* trackPtr = HQR_Get(listTrack,currentProcessedActorPtr->trackNumber);
			
			trackPtr+=currentProcessedActorPtr->positionInTrack * 2;

			short int trackMacro = *(short int*)trackPtr;
			trackPtr += 2;

			switch(trackMacro)
			{
			case 4: // BODY
				{
					currentProcessedActorPtr->body = *(short int*)(trackPtr);
					trackPtr += 2;
					currentProcessedActorPtr->positionInTrack += 2;
					break;
				}
			default:
				{
					printf("Unknown track macro %X\n",trackMacro);
					exit(1);
					break;
				}
			}

			break;
		}
	}

	currentProcessedActorPtr->beta &= 0x3FF;
}

void processLife(int lifeNum)
{
	int exitLife = 0;
	int switchVal = 0;
	int var_6;

	currentLifeActorIdx = currentProcessedActorIdx;
	currentLifeActorPtr = currentProcessedActorPtr;
	currentLifeNum = lifeNum;

	currentLifePtr = HQR_Get(listLife,lifeNum);

	while(!exitLife)
	{
		int lifeTempVar1;
		int lifeTempVar2;
		int lifeTempVar3;

		int switchVal = 0;

		var_6 = -1;

		short int currentOpcode = *(short int*)(currentLifePtr);
		currentLifePtr+=2;

		if(currentOpcode & 0x8000)
		{
			printf("Unimplemented code in processLife\n");
			exit(1);
		}
		else
		{
			switch(currentOpcode & 0x7FFF)
			{
			case 0x0: // DO_MOVE
				{
					processTrack();
					break;
				}
			case 0x4: // IF_DIF
				{
					lifeTempVar1 = evalVar();
					lifeTempVar2 = evalVar();

					if(lifeTempVar1 == lifeTempVar2)
					{
						currentLifePtr+=2;
					}
					else
					{
						lifeTempVar2 = *(short int*)(currentLifePtr);
						currentLifePtr += lifeTempVar2*2;
						currentLifePtr += 2;
					}

					break;
				}
			case 0xC: // RETURN
				{
					exitLife = 1;
					break;
				}
			case 0xD: // ?
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					//anim(lifeTempVar1, 1, -1);

					break;
				}
			case 0xF: // MOVE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					lifeTempVar2 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					setMoveMode(lifeTempVar1,lifeTempVar2);

					break;
				}
			case 0x11: // MESSAGE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					//makeMessage(lifeTempVar1);

					break;
				}
			case 0x19: // SWITCH
				{
					switchVal = evalVar();
					break;
				}
			case 0x1A: // CASE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					if(lifeTempVar1 == switchVal)
					{
						currentLifePtr+=2;
					}
					else
					{
						lifeTempVar2 = *(short int*)(currentLifePtr);
						currentLifePtr += lifeTempVar2*2;
						currentLifePtr += 2;
					}
		
					break;
				}
			case 0x1F: // LIFE
				{
					currentProcessedActorPtr->life = *(short int*)(currentLifePtr);
					currentLifePtr+=2;
					break;
				}
			case 0x33: // CAMERA_TARGET
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					if(lifeTempVar1 != currentCameraTarget)
					{
						lifeTempVar2 = objectTable[lifeTempVar1].ownerIdx;

						if(lifeTempVar1 != -1)
						{
							currentCameraTarget = lifeTempVar1;
							genVar9 = lifeTempVar2;

							lifeTempVar3 = actorTable[genVar9].room;

							if(lifeTempVar3 != currentDisplayedRoom)
							{
								needChangeRoom = 1;
//								newRoom = lifeTempVar3;
							}
						}
						else
						{
							printf("Partialy unimplemented life opcode 0x33\n");
							exit(1);
						}
					}

					break;
				}
			case 0x36: // TEST_COL
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					if(lifeTempVar1)
					{
						currentProcessedActorPtr->dynFlags |= 1;
					}
					else
					{
						currentProcessedActorPtr->dynFlags &= 0xFFFE;
					}

					break;
				}
			case 0x40: // LIGHT
				{
					lifeTempVar1 = 2-((*(short int*)(currentLifePtr))<<1);
					currentLifePtr+=2;

					if(!defines.lightVar)
					{
						if(lightVar1 != lifeTempVar1)
						{
							lightVar1 = lifeTempVar1;
							lightVar2 = 1;
						}
					}

					break;
				}
			case 0x41: // SHAKING
				{
					//shakingState = shakingAmplitude = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

/*					if(shakingState==0)
					{
						stopShaking();
					} */

					break;
				}
			case 0x4D: // ? shaking related
				{
//					mainLoopVar1 = shakeVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

/*					if(mainLoopVar1)
					{
						//setupShaking(-600);
					}
					else
					{
						//setupShaking(1000);
					} */

					break;
				}
			case 0x51: // ? fade out music and play another music ?
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					/*if(currentMusic!=-1)
					{
						soundFunc1(0,0,0x8000); // fade out music
						startChrono(&musicChrono); // fade out music timer
						currentMusic = -2; // fade out mode
						genVar8 = lifeTempVar1; // next music to play
					}
					else
					{
						changeMusic(lifeTempVar1);
					}*/

					break;
				}
			default:
				{
					printf("Unknown opcode %X in processLife\n",currentOpcode & 0x7FFF);
					exit(1);
				}
			}
		}

		if(var_6 != -1)
		{
			currentProcessedActorIdx = currentLifeActorIdx;
			currentProcessedActorPtr = currentLifeActorPtr;
		}

	}

	currentLifeNum = -1;
}

void mainDraw(int mode)
{
	if(mode == 0)
		mode = 1; // DEBUG

	if(mode== 0)
	{
		//restoreDirtyRects();
	}
	else
	{
		genVar5 = 0;
		copyToScreen(aux2,screen);
	}

//	setClipSize(0,0,319,199);
	genVar6 = 0;

	int i;

	/*for(i=0;i<numActorInList;i++)
	{
	}*/

	/*if(drawTextOverlay())
	{
		addToRedrawBox();
	}*/

	if(!lightVar1)
	{
		if(mode)
		{
			if(mode!=2 || lightVar2)
			{
				//makeBlackPalette();
				flipScreen();
				//make3dTatouUnk1(0x10,0);
				lightVar2 = 0;
			}
			else
			{
				flipScreen();
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

	flipScreen();
}

void mainLoop(int allowSystemMenu)
{
	while(1)
	{
		process_events();
		input3 = input2;
		input4 = inputKey;
		joy = input1;

		if(input3)
		{
		}
		else
		{
//			input5 = 0;
		}

		if(joy)
		{
			if(!allowSystemMenu)
			{
				break;
			}

			found = 0x2000;
		}
		else
		{
			found = 0;
		}

//		updateInHand(inHand);

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
					currentProcessedActorPtr->field_84 = -1;
					currentProcessedActorPtr->hit = -1;
					currentProcessedActorPtr->hardCol = -1;
					currentProcessedActorPtr->col = -1;
					currentProcessedActorPtr->hardDec = -1;
				}

				currentProcessedActorPtr++;
				currentProcessedActorIdx++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					int flag = currentProcessedActorPtr->flags;

					if(flag & 1)
					{
//						processActor1();
					}

					if(flag & 0x40)
					{
//						processActor2();
					}

					if(currentProcessedActorPtr->field_8E)
					{
//						processActor3();
					}
				}

				currentProcessedActorPtr++;
				currentProcessedActorIdx++;
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
			}

			if(giveUp)
				break;


		}

		if(changeFloor)
		{
			//loadFloor(newFloor);
		}

		if(needChangeRoom)
		{
//			loadRoom(newRoom);
			setupCamera();
		}
		else
		{
//			checkIfCameraChangeIsRequired();
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

//		sortActorList();

//		if(objModifFlag2)
		{
//			setupCameraSub4();
		}

//		mainLoopSub1();

		mainDraw(setupCameraVar1);

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

int main(int argc, char** argv)
{
	int startupMenuResult;
	int protectionToBeDone = 1;
	char version[256];
	getVersion(version);

	printf(version);

	osystem.initBuffer(scaledScreen,640,400);
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
					startGame(7,1,0);

				/*	if(!protectionState)
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

