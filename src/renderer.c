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

// DEBUG
int numPointInPoly;
//

void transformPoint(int* ax, int* bx, int* cx)
{
	int x;
	int y;
	int z;

	if(transformUseY)
	{
		x = (((((*ax) * transformYSin) - ((*cx) * transformYCos)))>>16)<<1;
		z = (((((*ax) * transformYCos) + ((*cx) * transformYSin)))>>16)<<1;
	}
	else
	{
		x = (*ax);
		z = (*cx);
	}

	//si = x
	//ax = z

	if(transformUseX)
	{
		int tempY = (*bx);
		int tempZ = z;
		y = ((((tempY * transformXSin ) - (tempZ * transformXCos)))>>16)<<1;
		z = ((((tempY * transformXCos ) + (tempZ * transformXSin)))>>16)<<1;
	}
	else
	{
		y = (*bx);
	}

	// cx = y
	// bx = z

	if(transformUseZ)
	{
		int tempX = x;
		int tempY = y;
		x = ((((tempX * transformZSin) - ( tempY * transformZCos)))>>16)<<1;
		y = ((((tempX * transformZCos) + ( tempY * transformZSin)))>>16)<<1;
	}

	*ax = x;
	*bx = y;
	*cx = z;
}

int modelCosAlpha;
int modelSinAlpha;
int modelCosBeta;
int modelSinBeta;
int modelCosGamma;
int modelSinGamma;

bool noModelRotation;

int renderX;
int renderY;
int renderZ;



int prerenderFlag0(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr, char* ptr)
{
	renderX = x - translateX;
	renderY = y;
	renderZ = z - translateZ;

	if(!alpha && !beta && !gamma)
	{
		noModelRotation = true;
	}
	else
	{
		noModelRotation = false;

		modelCosAlpha = cosTable[alpha&0x3FF];
		modelSinAlpha = cosTable[(alpha+0x100)&0x3FF];

		modelCosBeta = cosTable[beta&0x3FF];
		modelSinBeta = cosTable[(beta+0x100)&0x3FF];

		modelCosGamma = cosTable[gamma&0x3FF];
		modelSinGamma = cosTable[(gamma+0x100)&0x3FF];
	}

	int var1 = *(short int*)ptr;
	ptr+=2;

// DEBUG
	numPointInPoly = var1;
//

	unsigned short int* outPtr = renderPointList;

	do
	{

		int var2 = *(short int*)ptr;
		ptr+=2;
		int var3 = *(short int*)ptr;
		ptr+=2;
		int var4 = *(short int*)ptr;
		ptr+=2;

		/*if(!noModelRotation)
		{
			int xrot = ((modelSinBeta * var2) - (modelCosBeta * var4))<<1;
			int yrot = var3;
			int zrot = ((modelCosBeta * var4) + (modelSinBeta * var2))<<1;

			var2 = xrot;
			var3 = yrot;
			var4 = zrot;

			yrot = ((modelSinAlpha * var3) - (modelCosAlpha * var4))<<1;
			zrot = ((modelCosAlpha * var4) + (modelSinAlpha * var3))<<1;

			var2 = xrot;
			var3 = yrot;
			var4 = zrot;

			xrot = ((modelSinGamma * var2) - (modelCosGamma * var3))<<1;
			yrot = ((modelCosGamma * var3) + (modelSinGamma * var2))<<1;

			var2 = xrot;
			var3 = yrot;
			var4 = zrot;
		}*/

		var4 += renderX;
		var3 += renderY;

		if(var3>10000) // depth clamp
		{
			*(outPtr++) = -10000;
			*(outPtr++) = -10000;
			*(outPtr++) = -10000;
		}
		else
		{
			var3 -= translateY;
			var2 += renderZ;

			transformPoint(&var4,&var3,&var2);

			var2 += cameraX;

			int transformedX = ((var4 * cameraY) / var2) + cameraCenterX;

			*(outPtr++) = transformedX;

			if(transformedX < BBox3D1)
				BBox3D1 = transformedX;

			if(transformedX > BBox3D3)
				BBox3D3 = transformedX;

			int transformedY = ((var3 * cameraZ) / var2) + cameraCenterY;

			*(outPtr++) = transformedY;

			if(transformedY < BBox3D2)
				BBox3D2 = transformedY;

			if(transformedY > BBox3D4)
				BBox3D4 = transformedY;

			*(outPtr++) = var2;

		}

		var1--;
		if(var1==0)
		{
			return(1);
		}

	}while(renderVar1 == 0);

	renderVar1 = 2;

	return(0);
}

int renderModel(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr)
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
	ptr+=2;
	ptr+=12;

	ptr+=*(short int*)ptr + 2;
	
	if(modelFlags&2)
	{
	}
	else
	if(!(modelFlags&4))
	{
		prerenderFlag0(x,y,z,alpha,beta,gamma,modelPtr,ptr);
	}

//DEBUG
	int i;

	for(i=0;i<numPointInPoly;i++)
	{
		int x;
		int y;

		x = renderPointList[i*3];
		y = renderPointList[i*3+1];

		if(x>=0 && x < 319 && y>=0 && y<199)
		{
			screen[y*320+x] = 10;
		}
	}
//

	return(0);
}
