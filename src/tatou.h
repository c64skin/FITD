#ifndef _TATOU_H_
#define _TATOU_H_

int make3dTatou(void);

//////////////// to mode

void paletteFill(void* palette, unsigned char r, unsigned char g, unsigned b);
void fadeIn(void* palette);
void flip();
void startChrono(unsigned int* chrono);
int evalChrono(unsigned int* chrono);
void process_events( void );
void copyPalette(char* source, char* dest);
void copyToScreen(char* source, char* dest);

//////////////
#endif