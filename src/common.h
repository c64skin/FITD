#ifndef _COMMON_
#define _COMMON_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <search.h>

#ifdef _WIN32
#include <windows.h>
#include <assert.h>
#endif

#ifdef _DEBUG
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#ifdef USE_GL
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
#endif

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

// scripting
#include "track.h"
#include "life.h"
#include "evalVar.h"

#include "osystem.h"


////

//typedef unsigned char byte;
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

#ifdef UNIX
#define FORCEINLINE inline
#else
#define FORCEINLINE __forceinline
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

