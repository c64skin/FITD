#include "common.h"

/***************************************************************************
                          mainSDL.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "SDL.h"
#include "SDL_thread.h"
#include "osystem.h"

char *tempBuffer;
SDL_Surface *sdl_buffer;
SDL_Surface *sdl_buffer320x200;
SDL_Surface *sdl_buffer640x400;
SDL_Surface *sdl_bufferStretch;
SDL_Surface *sdl_bufferRGBA;
SDL_Surface *sdl_screen;	// that's the SDL global object for the screen
SDL_Color sdl_colors[256];
SDL_Surface *surfaceTable[16];
//TTF_Font *font;

void OSystem::delay(int time)
{
    SDL_Delay(time);
}

void OSystem::updateImage()
{
}

/*void OSystem::getMouseStatus(mouseStatusStruct * mouseData)
{

    SDL_GetMouseState(&mouseData->X, &mouseData->Y);

    mouseData->left = mouseLeft;
    mouseData->right = mouseRight;

    mouseLeft = 0;
    mouseRight = 0;
}*/

OSystem::OSystem()	// that's the constructor of the system dependent
						// object used for the SDL port
{
    unsigned char *keyboard;
    int size;

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
	    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
	    exit(1);
	}

    atexit(SDL_Quit);

/*    if (TTF_Init() < 0)
	{
	    fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
	    exit(1);
	}
    atexit(TTF_Quit);

    int rendersolid = 0;
    int renderstyle = 0;
    int rendertype = 0;

    int ptsize = 11;

    font = TTF_OpenFont("verdana.ttf", ptsize);

    if (font == NULL)
	{
	    fprintf(stderr, "Couldn't load %d pt font from %s: %s\n", ptsize, "verdana.ttf",
		    SDL_GetError());
	    exit(2);
	}

    TTF_SetFontStyle(font, renderstyle);*/

    SDL_WM_SetCaption("Alone in the dark", "AITD");

   // SDL_ShowCursor (SDL_DISABLE);

   // SDL_EnableUNICODE (SDL_ENABLE); // not much used in fact

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    keyboard[SDLK_RETURN] = 0;

    sdl_screen = SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE/*|SDL_FULLSCREEN*/);

    if (sdl_screen == NULL)
	{
	    fprintf(stderr, "Couldn't set 640x400x32 video mode: %s\n", SDL_GetError());
	    exit(1);
	}

    mouseLeft = 0;
    mouseRight = 0;
}

void OSystem::putpixel(int x, int y, int pixel)
{
    int bpp = sdl_screen->format->BytesPerPixel;

   /*
    * Here p is the address to the pixel we want to set 
    */
    Uint8 *p = (Uint8 *) sdl_screen->pixels + y * sdl_screen->pitch + x * bpp;

    *p = pixel;
}

void OSystem::setColor(byte i, byte R, byte G, byte B)	// cette fonction est vraiment vraiment
							// tres tres lente...
{
    sdl_colors[i].r = R;
    sdl_colors[i].g = G;
    sdl_colors[i].b = B;

    SDL_SetColors(sdl_buffer, sdl_colors, i, 1);
}

void OSystem::setPalette(byte * palette)
{
   // int i;
    SDL_Color *sdl_colorsTemp = (SDL_Color *) palette;

    SDL_SetColors(sdl_buffer, sdl_colorsTemp, 0, 256);

    SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void OSystem::setPalette320x200(byte * palette)
{
   // int i;
    SDL_Color *sdl_colorsTemp = (SDL_Color *) palette;

    SDL_SetColors(sdl_buffer320x200, sdl_colorsTemp, 0, 256);

//    SDL_BlitSurface(sdl_buffer320x200, NULL, sdl_screen, NULL);

  //  SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void OSystem::fadeBlackToWhite()
{
    int i;

    SDL_Color colorPtr[256];

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

    for (i = 0; i < 256; i += 3)
	{
	    memset(colorPtr, i, 1024);
	    SDL_SetPalette(sdl_screen, SDL_PHYSPAL, colorPtr, 0, 256);
	}
}

void OSystem::Flip(unsigned char *videoBuffer)
{
    SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void OSystem::draw320x200BufferToScreen(unsigned char *videoBuffer)
{
	SDL_BlitSurface(sdl_buffer320x200,NULL,sdl_bufferRGBA,NULL);

//	sdl_bufferStretch=zoomSurface(sdl_bufferRGBA, 2, 2.4, SMOOTHING_ON);

//	SDL_FillRect(sdl_screen,NULL,0);

    SDL_BlitSurface(sdl_bufferStretch, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

	SDL_FreeSurface(sdl_bufferStretch);
}

void OSystem::CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom)
{
    SDL_Rect rectangle;

    rectangle.x = left;
    rectangle.y = top;
    rectangle.w = right - left +1 ;
    rectangle.h = bottom - top +1 ;

    SDL_BlitSurface(sdl_buffer, &rectangle, sdl_screen, &rectangle);

    SDL_UpdateRect(sdl_screen, left, top, right - left +1, bottom - top+1);
}

void OSystem::initBuffer(char *buffer, int width, int height)
{   
	sdl_buffer = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, width, 0, 0, 0, 0);
}

void OSystem::crossFade(char *buffer, char *palette)
{
    SDL_Surface *backupSurface;
    SDL_Surface *newSurface;
    SDL_Surface *tempSurface;
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    backupSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, rmask, gmask, bmask, 0);
    newSurface =
	SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 640, 480, 32, rmask, gmask, bmask, 0);

    tempSurface = SDL_CreateRGBSurfaceFrom(buffer, 640, 480, 8, 640, 0, 0, 0, 0);
    SDL_SetColors(tempSurface, (SDL_Color *) palette, 0, 256);

    SDL_BlitSurface(sdl_screen, NULL, backupSurface, NULL);
    SDL_BlitSurface(tempSurface, NULL, newSurface, NULL);

#ifndef FASTDEBUG
    int i;

/*    for (i = 0; i < 16; i++)
	{
	    SDL_BlitSurface(backupSurface, NULL, surfaceTable[i], NULL);
	    SDL_SetAlpha(newSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 16);
	    SDL_BlitSurface(newSurface, NULL, surfaceTable[i], NULL);
	}

    for (i = 0; i < 16; i++)
	{
	    SDL_BlitSurface(surfaceTable[i], NULL, sdl_screen, NULL);
	    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
	}*/

    for (i = 0; i < 8; i++)
	{
	    SDL_BlitSurface(backupSurface, NULL, surfaceTable[i], NULL);
	    SDL_SetAlpha(newSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 32);
	    SDL_BlitSurface(newSurface, NULL, surfaceTable[i], NULL);
   	    SDL_BlitSurface(surfaceTable[i], NULL, sdl_screen, NULL);
	    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
        SDL_Delay(20);

	}

#endif

    SDL_BlitSurface(newSurface, NULL, sdl_screen, NULL);
    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

    SDL_FreeSurface(backupSurface);
    SDL_FreeSurface(newSurface);
    SDL_FreeSurface(tempSurface);
}

