#ifndef _OSYSTEM_H_
#define _OSYSTEM_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define byte char

class OSystem
{
  public:
    OSystem();

    int mouseRight;
    int mouseLeft;

    void delay(int time);
    void crossFade(char *buffer, char *palette);
    void fadeBlackToWhite();
	void updateImage();
    void initBuffer(char *buffer, int width, int height);
	void initVideoBuffer(char *buffer, int width, int height);
    void putpixel(int x, int y, int pixel);
    void setColor(byte i, byte R, byte G, byte B);
    void setPalette(byte * palette);
	void setPalette320x200(byte * palette);
    void flip(unsigned char *videoBuffer);
	void draw320x200BufferToScreen(unsigned char *videoBuffer);
    void CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom);
    void drawText(int X, int Y, char *text);
    void drawTextColor(int X, int Y, char *string, unsigned char R, unsigned char G, unsigned char B);
    void drawLine(int X1,int X2,int Y1,int Y2,unsigned char color, unsigned char* palette);
	void getPalette(char* palette);
	void playSample(char* sampleName);
//    void getMouseStatus(mouseStatusStruct * mouseData);

	void set320x200Mode(bool mode);

	void startFrame();
	void stopFrame();
	void startModelRender();
	void stopModelRender();
#ifdef USE_GL
	void fillPoly(float* buffer, int numPoint, unsigned char color);
	void draw3dLine(float x1, float y1, float z1, float x2, float y2, float z2, unsigned char color);
	void draw3dQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, unsigned char color);
#else
	void fillPoly(short int* buffer, int numPoint, unsigned char color);
	void draw3dLine(int x1, int y1, int z1, int x2, int y2, int z2, unsigned char color);
	void draw3dQuad(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, unsigned char color);
#endif

	void cleanScreenKeepZBuffer();

  private:
};

#endif
