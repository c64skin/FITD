#ifndef _COMMON_
#define _COMMON_

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef WIN32
#include <search.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <assert.h>
#endif

#ifdef _DEBUG
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#ifdef _DEBUG
#define ASSERT_PTR(exp) assert(exp)
#else
#define ASSERT_PTR(exp)
#endif

#ifdef USE_GL
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
//#include <gl\glaux.h>   // Header File For The Glaux Library
#endif

//////////////// GAME SPECIFIC DEFINES

#define NUM_MAX_OBJ         300
#define NUM_MAX_ACTOR       50
#define NUM_MAX_TEXT        40
#define NUM_MAX_MESSAGE     5
#define INVENTORY_SIZE      30
#define NUM_MAX_TEXT_ENTRY  250


///////////////

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned int uint;
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed short S16;
typedef signed long S32;

// temp triangulation stuff
//#include <bool.h>
//#include <geometry.h>
int triangulate_polygon(int ncontours,int cntr[],double (*vertices)[2],int (*triangles)[3]);
#include "vars.h"

#include "main.h"
#include "fileAccess.h"
#include "screen.h"
#include "videoMode.h"
#include "pak.h"
#include "unpack.h"
#include "tatou.h"
#include "threadCode.h"
#include "renderer.h"
#include "input.h"
#include "version.h"
#include "cosTable.h"
#include "hqr.h"
#include "time.h"
#include "font.h"
#include "aitdBox.h"
#include "save.h"
#include "anim.h"
#include "animAction.h"
#include "actorList.h"
#include "mainLoop.h"
#include "inventory.h"
#include "startupMenu.h"
#include "room.h"
#include "floor.h"
#include "object.h"
#include "zv.h"
#include "music.h"
#include "fmopl.h"

// debugger
#ifdef INTERNAL_DEBUGGER
#include "debugger.h"
#endif

// scripting
#include "track.h"
#include "life.h"
#include "evalVar.h"

#include "osystem.h"


////

//typedef unsigned char byte;

#ifdef UNIX
#define FORCEINLINE static inline
#else
#ifdef WIN32
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif
#endif

FORCEINLINE uint16 READ_LE_U16(void *ptr)
{
  return (((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE int16 READ_LE_S16(void *ptr)
{
  return (((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE uint32 READ_LE_U32(void *ptr)
{
  return (((byte*)ptr)[3]<<24)|(((byte*)ptr)[2]<<16)|(((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE int32 READ_LE_S32(void *ptr)
{
  return (((byte*)ptr)[3]<<24)|(((byte*)ptr)[2]<<16)|(((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE void WRITE_LE_U16(void *ptr, uint16 value)
{
  unsigned char val0;
  unsigned char val1;

  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_S16(void *ptr, int16 value)
{
  unsigned char val0;
  unsigned char val1;

  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_U32(void *ptr, uint32 value)
{
  unsigned char val0;
  unsigned char val1;
  unsigned char val2;
  unsigned char val3;

  val3 = (unsigned char)((value>>24)&0xFF);
  val2 = (unsigned char)((value>>16)&0xFF);
  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
  ((byte*)ptr)[2] = val2;
  ((byte*)ptr)[3] = val3;
}

FORCEINLINE void WRITE_LE_S32(void *ptr, int32 value)
{
  unsigned char val0;
  unsigned char val1;
  unsigned char val2;
  unsigned char val3;

  val3 = (unsigned char)((value>>24)&0xFF);
  val2 = (unsigned char)((value>>16)&0xFF);
  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
  ((byte*)ptr)[2] = val2;
  ((byte*)ptr)[3] = val3;
}

#endif

