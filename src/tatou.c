// seg 4

#include "common.h"
#include "SDL.h"


//////////// stuff to move
void clearScreenTatou(void)
{
	int i;

	for(i=0;i<45120;i++)
	{
		screen[i] = 0;
	}
}

void blitScreenTatou(void)
{
	int i;

	for(i=0;i<45120;i++)
	{
		unkScreenVar[i] = screen[i];
	}
}

void copyPalette(char* source, char* dest)
{
	int i;

	for(i=0;i<768;i++)
	{
		dest[i] = source[i];
	}
}

void copyToScreen(char* source, char* dest)
{
	int i;

	for(i=0;i<64000;i++)
	{
		dest[i] = source[i];
	}
}

void paletteFill(void* palette, unsigned char r, unsigned char g, unsigned b)
{
	unsigned char* paletteLocal = (unsigned char*) palette;
	int offset = 0;
	int i;

	r<<=1;
	g<<=1;
	b<<=1;

	for(i=0;i<256;i++)
	{
		paletteLocal[offset] = r;
		paletteLocal[offset+1] = g;
		paletteLocal[offset+2] = b;
		offset += 3;
	}
}

void fadeIn(void* localPalette)
{
	flip();
	memcpy(palette,localPalette,0x300);
	flip();
	// TODO: implement fade
}

void flip()
{
	int i;
	char paletteRGBA[256*4];

	for(i=0;i<256;i++)
	{
		paletteRGBA[i*4] = palette[i*3];
		paletteRGBA[i*4+1] = palette[i*3+1];
		paletteRGBA[i*4+2] = palette[i*3+2];
		paletteRGBA[i*4+3] = 0xFF;
	}
	osystem.setPalette(paletteRGBA);
	osystem.Flip((unsigned char*)unkScreenVar);
}

void process_events( void )
{
    /* Our SDL event placeholder. */
    SDL_Event event;

    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
//            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            exit( 0 );
            break;
        }

    }

}

void startChrono(unsigned int* chrono)
{
	*chrono = timer;
}

int evalChrono(unsigned int* chrono)
{
	return(timer-*chrono);
}

////////////////////////

int make3dTatou(void)
{
	char* tatou2d;
	char* tatou3d;
	char* tatouPal;
	int time;
	int deltaTime;
	int rotation;
	int unk1;
	char paletteBackup[768];
	unsigned int localChrono;

	tatou2d = loadPakSafe("itd_ress",2);
	tatou3d = loadPakSafe("itd_ress",0);
	tatouPal = loadPakSafe("itd_ress",1);

	time = 8920;
	deltaTime = 50;
	rotation = 256;
	unk1 = 8;

	//setupSMCode(160,100,128,500,490);

	copyPalette(palette,paletteBackup);

	paletteFill(palette,0,0,0);

	fadeIn(palette);

	copyPalette(tatouPal,palette);
	copyToScreen(tatou2d+770,unkScreenVar);
	flip();
	copyToScreen(unkScreenVar,aux2);

	//make3dTatouUnk1(8,0);

	startChrono(&localChrono);

	return(0);

	do
	{
		process_events();

		if(evalChrono(&localChrono)<=0x0) // avant eclair
		{
		}
		else // eclair
		{
		/*	soundVar2 = -1;
			soundVar1 = -1;

			playSound(defines.field_8);

			soundVar2 = -1;
			soundVar1 = -1;*/

			paletteFill(palette,63,63,63);
			fadeIn(palette);
		/*	setClipSize(0,0,319,199);*/

			clearScreenTatou();

		/*	rotateModel(0,0,0,unk1,rotation,0,time); */

			renderModel(0,0,0,0,0,0,tatou3d);

			blitScreenTatou();

			copyPalette(tatouPal,palette);
			fadeIn(palette);

		//	while(input2==0 && input1 == 0 && inputKey == 0) // boucle de rotation du tatou
			while(1)
			{
				process_events();

				time = deltaTime;

				if(time>16000)
					break;

				rotation -=8;

			/*	clearScreenTatou();

				rotateModel(0,0,0,unk1,rotation,0,time);

				renderModel(0,0,0,0,0,0,tatou3d); */

				blitScreenTatou();
			}

			break;
		}
	}while(1);

	free(tatouPal);
	free(tatou3d);
	free(tatou2d);

	/*if(!(input2!=0 || input1 != 0 || inputKey != 0))
	{
		fadeOut(32,0);
		copyPalette(localPalette,palette);
		return(1);
	}
	else
	{
		fadeOut(16,0);
		copyPalette(localPalette,palette);
		return(0);
	}*/

	return(0);
}