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

int numOfPoints;
int numOfBones;

short int pointBuffer[400*3];
short int bonesBuffer[59];

bool boneRotateX;
bool boneRotateY;
bool boneRotateZ;

int boneRotateXCos;
int boneRotateXSin;
int boneRotateYCos;
int boneRotateYSin;
int boneRotateZCos;
int boneRotateZSin;

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

void prepareRotationMatrix(int transX,int transY,int transZ)
{
	if(transX)
	{
		boneRotateXCos = cosTable[transX&0x3FF];
		boneRotateXSin = cosTable[(transX+0x100)&0x3FF];

		boneRotateX = true;
	}
	else
	{
		boneRotateX = false;
	}

	if(transY)
	{
		boneRotateYCos = cosTable[transY&0x3FF];
		boneRotateYSin = cosTable[(transY+0x100)&0x3FF];

		boneRotateY = true;
	}
	else
	{
		boneRotateY = false;
	}

	if(transZ)
	{
		boneRotateZCos = cosTable[transZ&0x3FF];
		boneRotateZSin = cosTable[(transZ+0x100)&0x3FF];

		boneRotateZ = true;
	}
	else
	{
		boneRotateZ = false;
	}
}

void computeBoneRotation(short int* pointPtr, int numOfPoint)
{
	int i;

	for(i=0;i<numOfPoint;i++)
	{
		int ax = *(short int*)pointPtr;
		int bx = *(short int*)(pointPtr+2);
		int cx = *(short int*)(pointPtr+4);

		int x;
		int y;
		int z;

		if(boneRotateY)
		{
			x = (((((ax) * boneRotateYSin) - ((cx) * boneRotateYCos)))>>16)<<1;
			z = (((((ax) * boneRotateYCos) + ((cx) * boneRotateYSin)))>>16)<<1;
		}
		else
		{
			x = (ax);
			z = (cx);
		}

		//si = x
		//ax = z

		if(boneRotateX)
		{
			int tempY = (bx);
			int tempZ = z;
			y = ((((tempY * boneRotateXSin ) - (tempZ * boneRotateXCos)))>>16)<<1;
			z = ((((tempY * boneRotateXCos ) + (tempZ * boneRotateXSin)))>>16)<<1;
		}
		else
		{
			y = (bx);
		}

		// cx = y
		// bx = z

		if(boneRotateZ)
		{
			int tempX = x;
			int tempY = y;
			x = ((((tempX * boneRotateZSin) - ( tempY * boneRotateZCos)))>>16)<<1;
			y = ((((tempX * boneRotateZCos) + ( tempY * boneRotateZSin)))>>16)<<1;
		}

		*(short int*)(pointPtr) = x;
		*(short int*)(pointPtr+2) = y;
		*(short int*)(pointPtr+4) = z;

		pointPtr+=6;
	}
}

void computeRotationMatrix(char* ptr)
{
	int baseBone = *(short int*)(ptr);
	int numPoints = *(short int*)((ptr)+2);

	computeBoneRotation(pointBuffer+baseBone/2,numPoints);

	int temp = *((ptr)+7);

	int temp2 = numOfBones - temp;

	do
	{
		if(*((ptr)+6) == temp)
		{
			computeRotationMatrix(ptr);
		}

		(ptr)+=0x10;

	}while(--temp2);
}

void computeTranslation1(int transX,int transY,int transZ,char* ptr)
{
	int i;
	short int* ptrSource = &pointBuffer[(*(short int*)ptr)/2];
	int number = *(short int*)(ptr+2);

	for(i=0;i<number;i++)
	{
		*(ptrSource++) += transX;
		*(ptrSource++) += transY;
		*(ptrSource++) += transZ;
	}
}

void computeTranslation2(int transX,int transY,int transZ,char* ptr)
{
	int i;
	short int* ptrSource = &pointBuffer[(*(short int*)ptr)/2];
	int number = *(short int*)(ptr+2);

	for(i=0;i<number;i++)
	{
		*(ptrSource++) = (*(ptrSource) * transX)>>8;
		*(ptrSource++) = (*(ptrSource) * transY)>>8;
		*(ptrSource++) = (*(ptrSource) * transZ)>>8;
	}
}

int computeModel(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr, char* ptr)
{
	renderX = x - translateX;
	renderY = y;
	renderZ = z - translateZ;

	numOfPoints = *(short int*)ptr;
	ptr+=2;

	memcpy(pointBuffer,ptr,numOfPoints*3*2);
	ptr+=numOfPoints*3*2;

	numOfBones = *(short int*)ptr;
	ptr+=2;

	memcpy(bonesBuffer,ptr,numOfBones*2);
	ptr+=numOfBones*2;

	char* tempPtr = ptr;

	*(short int*)(ptr+0xA) = alpha;
	*(short int*)(ptr+0xC) = beta;
	*(short int*)(ptr+0xE) = gamma;

	int i;

	for(i=0;i<numOfBones;i++)
	{
		int boneDataOffset = bonesBuffer[i];
		char* boneDataPtr = tempPtr + boneDataOffset;

		int transX;
		int transY;
		int transZ;

		transX = *(short int*)(boneDataPtr+0xA);
		transY = *(short int*)(boneDataPtr+0xC);
		transZ = *(short int*)(boneDataPtr+0xE);

		if(transX || transY || transZ)
		{
			int type = *(short int*)(boneDataPtr+0x8);

			switch(type)
			{
			case 0:
				{
					prepareRotationMatrix(transX,transY,transZ);
					computeRotationMatrix(boneDataPtr);
					break;
				}
			case 1:
				{
					computeTranslation1(transX,transY,transZ,boneDataPtr);
					break;
				}
			case 2:
				{
					computeTranslation2(transX,transY,transZ,boneDataPtr);
					break;
				}
			}
		}
	}

	char* si = tempPtr;

	for(i=0;i<numOfBones;i++)
	{
		int j;

		short int* ptr1 = (short int*)(((char*)pointBuffer)+*(short int*)(si+4));
		short int* ptr2 = (short int*)(((char*)pointBuffer)+*(short int*)(si));

		int number = *(short int*)(si+2);

		int ax = ptr1[0];
		int bx = ptr1[1];
		int dx = ptr1[2];

		for(j=0;j<number;j++)
		{
			*(ptr2++) += ax;
			*(ptr2++) += bx;
			*(ptr2++) += dx;
		}

		si += 0x10;
	}

	{
		numPointInPoly = numOfPoints;
		char* ptr = (char*)pointBuffer;

		short int* outPtr = pointBuffer;

		for(i=0;i<numOfPoints;i++)
		{

			int var2 = *(short int*)ptr;
			int var3 = *(short int*)(ptr+2);
			int var4 = *(short int*)(ptr+4);
			ptr+=6;

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

				*(outPtr++) = var2;
				*(outPtr++) = var3;
				*(outPtr++) = var4;
			}
		}

		ptr = (char*)pointBuffer;
		outPtr = renderPointList;
		
		do
		{
			int var2 = *(short int*)ptr;
			ptr+=2;
			int var3 = *(short int*)ptr;
			ptr+=2;
			int var4 = *(short int*)ptr;
			ptr+=2;

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

			numOfPoints--;
			if(numOfPoints==0)
			{
				return(1);
			}

		}while(renderVar1 == 0);
	}

	return(0);
}

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

	short int* outPtr = renderPointList;

	do
	{

		int var2 = *(short int*)ptr;
		ptr+=2;
		int var3 = *(short int*)ptr;
		ptr+=2;
		int var4 = *(short int*)ptr;
		ptr+=2;

		if(!noModelRotation)
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
		}

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
		computeModel(x,y,z,alpha,beta,gamma,modelPtr,ptr);
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
			screen[y*320+x] = 15;
		}
	}
//

	return(0);
}
