#ifdef USE_GL

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
#include "SDL_sound.h"
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
char RGBA_Pal[256*4];
//TTF_Font *font;

GLuint		backTexture;
GLuint		modelsDisplayList;

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

	Sound_Init();

//	atexit(Sound_Quit);

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

    SDL_WM_SetCaption("Alone in the dark \"GL\"", "AITD");

   // SDL_ShowCursor (SDL_DISABLE);

   // SDL_EnableUNICODE (SDL_ENABLE); // not much used in fact

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    keyboard[SDLK_RETURN] = 0;

    sdl_screen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL/*|SDL_FULLSCREEN*/);

    if (sdl_screen == NULL)
	{
	    fprintf(stderr, "Couldn't set 640x480x32 video mode: %s\n", SDL_GetError());
	    exit(1);
	}

    mouseLeft = 0;
    mouseRight = 0;

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,320,200,0,0,100);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	modelsDisplayList = glGenLists(1);
}

void OSystem::setPalette(byte * palette)
{
	memcpy(RGBA_Pal,palette,256*4);
}

void OSystem::getPalette(char* palette)
{
	memcpy(palette,RGBA_Pal,256*4);
}

char tempBuffer2[1024*512*3];

void OSystem::flip(unsigned char *videoBuffer)
{
	glColor3ub(255,255,255);

	glBindTexture(GL_TEXTURE_2D, backTexture);

	glBegin(GL_TRIANGLES);

		glTexCoord2f(0,0); // triangle haut gauche
		glVertex3i(0,0,-100.0f);
		glTexCoord2f(640.f/1024.f,0);
		glVertex3i(640,0,-100.0f);
		glTexCoord2f(0.0f,480.f/512.f);
		glVertex3i(0,480,-100.0f);

		glTexCoord2f(640.f/1024.f,0); // triangle haut gauche
		glVertex3i(640,0,-100.0f);
		glTexCoord2f(640.f/1024.f,480.f/512.f);
		glVertex3i(640,480,-100.0f);
		glTexCoord2f(0.0f,480.f/512.f);
		glVertex3i(0,480,-100.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glCallList(modelsDisplayList);

	SDL_GL_SwapBuffers( );
}

char tempBuffer3[320*200*3];

void OSystem::CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom)
{
	char* out = tempBuffer3;
	char* in = (char*)videoBuffer + left + top * 320;

	int i;
	int j;

	while((right-left)%4)
	{
		right++;
	}

	while((bottom-top)%4)
	{
		bottom++;
	}

	for(i=top;i<bottom;i++)
	{
		in = (char*)videoBuffer + left + i * 320;
		for(j=left;j<right;j++)
		{
			unsigned char color= *(in++);

			*(out++) = palette[color*3];
			*(out++) = palette[color*3+1];
			*(out++) = palette[color*3+2];
		}
	}


	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, left, top, right-left, bottom-top, GL_RGB, GL_UNSIGNED_BYTE, tempBuffer3);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OSystem::initBuffer(char *buffer, int width, int height)
{   
	memset(tempBuffer2,0,1024*512*3);
	glGenTextures(1, &backTexture);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, tempBuffer2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void OSystem::crossFade(char *buffer, char *palette)
{
}

int posInStream = 0;
volatile bool deviceStatus = false;

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
	Sound_Sample *sample = (Sound_Sample *)userdata;
	Uint8* input = (Uint8*)sample->buffer;

	if(posInStream+len < sample->buffer_size)
	{
		memcpy(stream,input+posInStream,len);
		posInStream+=len;
	}
	else
	{
		len = sample->buffer_size - posInStream;
		memcpy(stream,input+posInStream,len);
		posInStream+=len;
	}
}

void OSystem::playSample(char* sampleName)
{
	return;
#ifdef UNIX
	return;
#endif
	Sound_Sample *sample;
	Sound_AudioInfo info;

	info.channels = 0;
	info.format = 0;
	info.rate = 0;

	sample = Sound_NewSampleFromFile(sampleName,&info,5000);
	Sound_DecodeAll(sample);

	if(deviceStatus)
	{
		SDL_CloseAudio();
	}

	posInStream = 0;
	{
		SDL_AudioSpec *desired, *obtained;
		SDL_AudioSpec *hardware_spec;

		/* Allocate a desired SDL_AudioSpec */
		desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

		/* Allocate space for the obtained SDL_AudioSpec */
		obtained = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

		/* 22050Hz - FM Radio quality */
		desired->freq=sample->actual.rate;

		/* 16-bit signed audio */
		desired->format=sample->actual.format;

		/* Mono */
		desired->channels=sample->actual.channels;

		/* Large audio buffer reduces risk of dropouts but increases response time */
		desired->samples=512;

		/* Our callback function */
		desired->callback=my_audio_callback;

		desired->userdata=(void*)sample;

		/* Open the audio device */
		if ( SDL_OpenAudio(desired, obtained) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
		}
		/* desired spec is no longer needed */
		free(desired);
		hardware_spec=obtained;
		/* Start playing */
		SDL_PauseAudio(0);
		deviceStatus = true;
	}
}

void OSystem::startFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void OSystem::stopFrame()
{
}

void OSystem::startModelRender()
{
	//glNewList(modelsDisplayList, GL_COMPILE);
}

void OSystem::stopModelRender()
{
	//glEndList();
}

void OSystem::fillPoly(short int* buffer, int numPoint, unsigned char color)
{
	int i;

	glColor3ub(palette[color*3],palette[color*3+1],palette[color*3+2]);
	glBegin(GL_POLYGON);

	for(i=0;i<numPoint;i++)
	{
		glVertex3f(buffer[0],buffer[1],-buffer[2]/1000.f);
		buffer+=3;
	}

	glEnd();
}

void OSystem::draw3dLine(int x1, int y1, int z1, int x2, int y2, int z2, unsigned char color)
{
	glColor3ub(palette[color*3],palette[color*3+1],palette[color*3+2]);

	glBegin(GL_LINES);

	glVertex3f(x1,y1,-z1/1000.f);
	glVertex3f(x2,y2,-z2/1000.f);

	glEnd();
}

void OSystem::draw3dQuad(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, unsigned char color)
{
	glColor4ub(palette[color*3],palette[color*3+1],palette[color*3+2],0.5f);

	glBegin(GL_QUADS);

	glVertex3f(x1,y1,-z1/1000.f);
	glVertex3f(x2,y2,-z2/1000.f);
	glVertex3f(x3,y3,-z3/1000.f);
	glVertex3f(x4,y4,-z4/1000.f);

	glEnd();
}

#endif