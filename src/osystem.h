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
    void Flip(unsigned char *videoBuffer);
	void draw320x200BufferToScreen(unsigned char *videoBuffer);
    void CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom);
    void drawText(int X, int Y, char *text);
    void drawTextColor(int X, int Y, char *string, unsigned char R, unsigned char G, unsigned char B);
    void drawLine(int X1,int X2,int Y1,int Y2,unsigned char color, unsigned char* palette);
//    void getMouseStatus(mouseStatusStruct * mouseData);

	void set320x200Mode(bool mode);

  private:
};

#endif
