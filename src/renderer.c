#include "common.h"

int BBox3D1=0;
int BBox3D2=0;
int BBox3D3=0;
int BBox3D4=0;

int renderVar1=0;

int numOfPolyToRender=0;

char renderBuffer[3261];

char* renderVar2=NULL;

int modelFlags = 0;

int renderModel(int var0,int var1,int var2,int var3,int var4,int var5,void* modelPtr)
{
	char* ptr = (char*)modelPtr;

	BBox3D1 = 0x7FFF;
	BBox3D2 = 0x7FFF;

	BBox3D3 = -0x7FFF;
	BBox3D4 = -0x7FFF;

	renderVar1 = 0;

	numOfPolyToRender = 0;
	
	renderVar2 = renderBuffer;

	modelFlags = *(short int*)ptr;

	ptr+=12;

	ptr+=*(short int*)ptr;

	if(modelFlags&2)
	{
	}
	else
	if(!(modelFlags&4))
	{
		//prenderFlag0();
	}

	return(0);
}