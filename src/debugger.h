#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#ifdef INTERNAL_DEBUGGER

////// debug var used in engine
extern bool debuggerVar_drawModelZv;
extern bool debuggerVar_drawCameraCoverZone;

extern bool debuggerVar_useBlackBG;
///////////////////////////////

void debugger_enterMainDebug(void);
void debugger_processDebugMenu(void);
void debugger_draw(void);

#endif // INTERNAL_DEBUGGEr

#endif //_DEBUGGER_H_