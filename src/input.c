#include "common.h"
#include <SDL.h>

void readKeyboard(void)
{
    int size;
    int j;

    inputKey = 0;
	input1 = 0;
	input2 = 0;

	unsigned char *keyboard;

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    for (j = 0; j < size; j++)
	{
	    if (keyboard[j])
		{
		    switch (j)
			{
			case SDLK_RETURN:
			    input2 = 0x1C;
			    break;
			case SDLK_ESCAPE:
				input2 = 0x1B;
				break;

			case SDLK_UP:
			    inputKey |= 1;
			    break;

			case SDLK_DOWN:
			    inputKey |= 2;
			    break;

			case SDLK_RIGHT:
			    inputKey |= 8;
			    break;

			case SDLK_LEFT:
			    inputKey |= 4;
			    break;
			}
		}
	}
}
