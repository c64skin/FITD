#include "common.h"
#include <SDL.h>

void readKeyboard(void)
{
    SDL_Event event;
    int localKey;
    int i;
    int find = 0;
    short int temp;
    unsigned char temp2;
    char found = 0;
    int size;
    int j;

    inputKey = 0;
	input1 = 0;
	input2 = 0;

	unsigned char *keyboard;
  /*  printTextVar12 = 0;
    key1 = 0;
    skipIntro = 0;

    unsigned char *keyboard;

    while (SDL_PollEvent(&event))
	{
	    switch (event.type)
		{
		case SDL_QUIT:
		    exit(0);
		    break;

		case SDL_MOUSEBUTTONDOWN:
		    if (event.button.button == 3)
			{
			    osystem->mouseRight = 1;
			}
		    else
			{
			    osystem->mouseLeft = 1;
			}
		    break;

		default:
		    break;
		}
	} */

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    for (j = 0; j < size; j++)
	{
	    if (keyboard[j])
		{
		    switch (j)
			{
			/*case SDLK_ESCAPE:
			    localKey = 0x1;
			    break; */

			case SDLK_RETURN:
			    input2 = 0x1C;
			    break;

			/*case SDLK_LSHIFT:
			    localKey = 0x2A;
			    break;

			case SDLK_RSHIFT:
			    localKey = 0x36;
			    break;

            case SDLK_LALT:
                localKey = 0x38;
                break;

			case SDLK_SPACE:
			    localKey = 0x39;
			    break;*/

			case SDLK_UP:
			    inputKey |= 1;
			    break;

			case SDLK_DOWN:
			    inputKey |= 2;
			    break;

			case SDLK_LEFT:
			    inputKey |= 3;
			    break;

			case SDLK_RIGHT:
			    inputKey |= 4;
			    break;

		/*	case SDLK_LCTRL:
			    localKey = 0x1D;
			    break;

			case SDLK_PAGEUP:
			    localKey = 0x49;
			    break;

			case SDLK_p:	// pause
			    localKey = 'p';
			    break;

			case SDLK_h:	// holomap
			    localKey = 'h';
			    break;

			case SDLK_u:	// next room
			    localKey = 'u';
			    break;

			case SDLK_j:	// previous room
			    localKey = 'j';
			    break;

			case SDLK_t:	// increase story state
			    localKey = 't';
			    break;

			case SDLK_g:	// decrease story state
			    localKey = 'g';
			    break;*/
			}

		 /*   for (i = 0; i < 28; i++)
			{
			    if (scanCodeTab1[i] == localKey)
				{
				    find = i;
				    found = 1;
				}
			}

		    if (found != 0)
			{
			    temp = scanCodeTab2[find];
			    temp2 = temp & 0x00FF;

			    if (temp2 == 0)
				{
				    if (!(localKey & 0x80))
					{
					    printTextVar12 |= (temp & 0xFF00) >> 8;
					}
				    else
					{
					    printTextVar12 &= -((temp & 0xFF00) >> 8);
					}
				}
			    else
				{
				   // mode non géré ici
				    key1 |= (temp & 0xFF00) >> 8;
				}
			}
		    else
			{
			   // no géré ici aussi
			    skipIntro = localKey;
			}

		    currentKey = event.key.keysym.scancode; */
		}
	}

   // printf("unicode= %X\n",event.key.keysym.unicode);
   // printf("scancode= %X\n",skipIntro);
}
