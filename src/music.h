#ifndef _MUSIC_H_
#define _MUSIC_H_

int initMusicDriver(void);
int musicUpdate(void *udata, uint8 *stream, int len);
void playMusic(int musicNumber);
void callMusicUpdate(void);

#endif