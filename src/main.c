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
	{
		startupMenuResult = processStartupMenu();

		switch(startupMenuResult)
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