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

#ifdef USE_GL
char primBuffer[18000];
#else
char primBuffer[8000];
#endif
char* tempOutPtr;

int renderVar3;

void fillpoly(short int * datas, int n, char c);
#ifdef USE_GL
void transformPoint(float* ax, float* bx, float* cx)
{
	int X = *ax;
	int Y = *bx;
	int Z = *cx;
	{
		int* ax = &X;
		int* bx = &Y;
		int* cx = &Z;

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
	}

	*ax = X;
	*bx = Y;
	*cx = Z;
/*	float x = *ax;
	float y = *bx;
	float z = *cx;

	if(transformUseY)
	{
		float tempX = x;
		float tempZ = z;
		x = ((((tempX * transformYSin ) - (tempZ * transformYCos)))/65535.f)*2.f;
		z = ((((tempX * transformYCos ) + (tempZ * transformYSin)))/65535.f)*2.f;
	}

	if(transformUseX)
	{
		float tempY = y;
		float tempZ = z;
		y = ((((tempY * transformXSin) - ( tempZ * transformXCos)))/65535.f)*2.f;
		z = ((((tempY * transformXCos) + ( tempZ * transformXSin)))/65535.f)*2.f;
	}

	if(transformUseZ)
	{
		float tempX = x;
		float tempY = y;
		x = ((((tempX * transformZSin) - (tempY * transformZCos)))/65535.f)*2.f;
		z = ((((tempX * transformZCos) + (tempY * transformZSin)))/65535.f)*2.f;
	}


	*ax = x;
	*bx = y;
	*cx = z; */
}
#else
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
#endif

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
		int x = *(short int*)pointPtr;
		int y = *(short int*)(pointPtr+1);
		int z = *(short int*)(pointPtr+2);

		if(boneRotateZ)
		{
			int tempX = x;
			int tempY = y;
			x = ((((tempX * boneRotateZSin) - ( tempY * boneRotateZCos)))>>16)<<1;
			y = ((((tempX * boneRotateZCos) + ( tempY * boneRotateZSin)))>>16)<<1;
		}

		if(boneRotateY)
		{
			int tempX = x;
			int tempZ = z;

			x = ((((tempX * boneRotateYSin) - (tempZ * boneRotateYCos)))>>16)<<1;
			z = ((((tempX * boneRotateYCos) + (tempZ * boneRotateYSin)))>>16)<<1;
		}

		if(boneRotateX)
		{
			int tempY = y;
			int tempZ = z;
			y = ((((tempY * boneRotateXSin ) - (tempZ * boneRotateXCos)))>>16)<<1;
			z = ((((tempY * boneRotateXCos ) + (tempZ * boneRotateXSin)))>>16)<<1;
		}

		*(short int*)(pointPtr) = x;
		*(short int*)(pointPtr+1) = y;
		*(short int*)(pointPtr+2) = z;

		pointPtr+=3;
	}
}

void computeRotationMatrix(char* ptr)
{
	int baseBone = *(short int*)(ptr);
	int numPoints = *(short int*)((ptr)+2);
	int temp;
	int temp2;

	computeBoneRotation(pointBuffer+baseBone/2,numPoints);

	temp = *((ptr)+7);

	temp2 = numOfBones - temp;

	do
	{
		if(*((ptr)+6) == temp) // is it a child of me ?
		{
			computeRotationMatrix(ptr); // yes, so apply the transformation to him
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
	char* tempPtr;
	int i;
	char* si;

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

	tempPtr = ptr;

	*(short int*)(ptr+0xA) = alpha;
	*(short int*)(ptr+0xC) = beta;
	*(short int*)(ptr+0xE) = gamma;

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

	si = tempPtr;

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

	//(*ptr) = si;
	tempOutPtr = si;

	{
		char* ptr = (char*)pointBuffer;
		short int* outPtr = pointBuffer;
		

#ifdef USE_GL
		float* outPtr2;
#else
		short int* outPtr2;
#endif

		numPointInPoly = numOfPoints;

		for(i=0;i<numOfPoints;i++)
		{
#ifdef USE_GL
			float X = *(short int*)ptr;
			float Y = *(short int*)(ptr+2);
			float Z = *(short int*)(ptr+4);
#else
			int X = *(short int*)ptr;
			int Y = *(short int*)(ptr+2);
			int Z = *(short int*)(ptr+4);
#endif
			ptr+=6;

			X += renderX;
			Y += renderY;

			if(Y>10000) // depth clamp
			{
				*(outPtr++) = -10000;
				*(outPtr++) = -10000;
				*(outPtr++) = -10000;
			}
			else
			{
				Y -= translateY;
				Z += renderZ;

				transformPoint(&X,&Y,&Z);

				*(outPtr++) = X;
				*(outPtr++) = Y;
				*(outPtr++) = Z;
			}
		}

		ptr = (char*)pointBuffer;
		outPtr2 = renderPointList;
		
		do
		{
			float X;
			float Y;
			float Z;
			
			X = *(short int*)ptr;
			ptr+=2;
			Y = *(short int*)ptr;
			ptr+=2;
			Z = *(short int*)ptr;
			ptr+=2;

			Z += cameraX;

			if( Z<=50 ) // clipping
			{
				*(outPtr2++) = -10000;
				*(outPtr2++) = -10000;
				*(outPtr2++) = -10000;
			}
			else
			{
				float transformedX = ((X * cameraY) / Z) + cameraCenterX;
				float transformedY;

				*(outPtr2++) = transformedX;

				if(transformedX < BBox3D1)
					BBox3D1 = transformedX;

				if(transformedX > BBox3D3)
					BBox3D3 = transformedX;

				transformedY = ((Y * cameraZ) / Z) + cameraCenterY;

				*(outPtr2++) = transformedY;

				if(transformedY < BBox3D2)
					BBox3D2 = transformedY;

				if(transformedY > BBox3D4)
					BBox3D4 = transformedY;

				*(outPtr2++) = Z;
			}

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

	int var1;
#ifdef USE_GL
		float* outPtr;
#else
		short int* outPtr;
#endif

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

	var1 = *(short int*)ptr;
	ptr+=2;

// DEBUG
	numPointInPoly = var1;
//

	outPtr = renderPointList;

	do
	{
#ifdef USE_GL
		float X;
		float Y;
		float Z;
		
		X = *(short int*)ptr;
		ptr+=2;
		Y = *(short int*)ptr;
		ptr+=2;
		Z = *(short int*)ptr;
		ptr+=2;
#else
		int X;
		int Y;
		int Z;
		
		X = *(short int*)ptr;
		ptr+=2;
		Y = *(short int*)ptr;
		ptr+=2;
		Z = *(short int*)ptr;
		ptr+=2;
#endif

		if(!noModelRotation)
		{
			{
				float tempX = X;
				float tempY = Y;

				X = (((modelSinGamma * tempX) - (modelCosGamma * tempY))/65536.f)*2.f;
				Y = (((modelCosGamma * tempX) + (modelSinGamma * tempY))/65536.f)*2.f;
			}

			{
				float tempX = X;
				float tempZ = Z;

				X = (((modelSinBeta * tempX) - (modelCosBeta * tempZ))/65536.f)*2.f;
				Z = (((modelCosBeta * tempX) + (modelSinBeta * tempZ))/65536.f)*2.f;
			}

			{
				float tempY = Y;
				float tempZ = Z;

				Y = (((modelSinAlpha * tempY) - (modelCosAlpha * tempZ))/65536.f)*2.f;
				Z = (((modelCosAlpha * tempY) + (modelSinAlpha * tempZ))/65536.f)*2.f;
			}
		}

		X += renderX;
		Y += renderY;

		if(Y>10000) // depth clamp
		{
			*(outPtr++) = -10000;
			*(outPtr++) = -10000;
			*(outPtr++) = -10000;
		}
		else
		{
			float transformedX;
			float transformedY;
			
			Y -= translateY;
			Z += renderZ;

			transformPoint(&X,&Y,&Z);

			Z += cameraX;

			transformedX = ((X * cameraY) / Z) + cameraCenterX;

			*(outPtr++) = transformedX;

			if(transformedX < BBox3D1)
				BBox3D1 = transformedX;

			if(transformedX > BBox3D3)
				BBox3D3 = transformedX;

			transformedY = ((Y * cameraZ) / Z) + cameraCenterY;

			*(outPtr++) = transformedY;

			if(transformedY < BBox3D2)
				BBox3D2 = transformedY;

			if(transformedY > BBox3D4)
				BBox3D4 = transformedY;

			*(outPtr++) = Z;

		}

		var1--;
		if(var1==0)
		{
			tempOutPtr = ptr;
			return(1);
		}

	}while(renderVar1 == 0);

	tempOutPtr = ptr;
	renderVar1 = 2;

	return(0);
}

char* primVar1;
char* primVar2;

void primFunctionDefault(int primType,char** ptr,char** out)
{
	printf("UnHandled primType %d\n",primType);
	exit(1);
}

void primType0(int primType, char** ptr, char** out) // line tested
{
	int ax;
	int depth1;
	int depth2;
	int depthLow;
	int depthHi;
	
	primVar1 = *(out);
	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=2;
	*ptr+=3;

	ax = *(short int*)(*ptr);
	*ptr+=2;

#ifdef USE_GL
	*(float*)(*out) = renderPointList[ax/2]; // X
	*out+=sizeof(float);
	*(float*)(*out) = renderPointList[ax/2+1]; // Y
	*out+=sizeof(float);
	depth1 = *(float*)(*out) = renderPointList[ax/2+2]; // Z
	*out+=sizeof(float);
#else
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // X
	ax+=2;
	*out+=2;
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Y
	ax+=2;
	*out+=2;
	depth1 = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;
#endif

	ax = *(short int*)(*ptr);
	*ptr+=2;

#ifdef USE_GL
	*(float*)(*out) = renderPointList[ax/2]; // X
	*out+=sizeof(float);
	*(float*)(*out) = renderPointList[ax/2+1]; // Y
	*out+=sizeof(float);
	depth2 = *(float*)(*out) = renderPointList[ax/2+2]; // Z
	*out+=sizeof(float);
#else
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // X
	ax+=2;
	*out+=2;
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Y
	ax+=2;
	*out+=2;
	depth2 = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;
#endif

	primVar2 = *out;


	if(depth1 <= depth2)
	{
		depthLow = depth1;
		depthHi = depth2;
	}
	else
	{
		depthLow = depth2;
		depthHi = depth1;
	}

	if(depthLow<=0) // behind camera
	{
		*out = primVar1; // do not add the prim
	}
	else 
	{
		numOfPolyToRender++;

		*out = renderVar2;

		*(short int*)(*out) = depthHi;
		*out+=2;
		*(short int*)(*out) = depthHi;
		*out+=2;
		*(short int*)(*out) = 0;
		*out+=2;
		*(char**)(*out) = primVar1;
		*out+=4;

		renderVar2 = *out;
		*out = primVar2;
	}
}

void primType1(int primType, char** ptr, char** out) // poly
{
	int ax;
	int cx;
	int min = 32000;
	int max = -32000;
	int depth;
	int i;

	char* saveDi;

	primVar1 = *out;

	ax = **ptr;
	**out = ax;
	cx = ax;
	(*out)++;
	(*ptr)++;

	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=3;
	*ptr+=2;

	saveDi = *out;

	for(i=0;i<cx;i++)
	{
		int pointNumber = *(short int*)(*ptr);
		*ptr+=2;

#ifdef USE_GL
		*(float*)(*out) = renderPointList[pointNumber/2]; // X
		ax+=2;
		*out+=sizeof(float);
		*(float*)(*out) = renderPointList[pointNumber/2+1]; // Y
		ax+=2;
		*out+=sizeof(float);
		depth = *(float*)(*out) = renderPointList[pointNumber/2+2]; // Z
		ax+=2;
		*out+=sizeof(float);
#else
		*(short int*)(*out) = *(short int*)(((char*)renderPointList) + pointNumber); // X
		ax+=2;
		*out+=2;
		*(short int*)(*out) = *(short int*)(((char*)renderPointList) + pointNumber + 2); // Y
		ax+=2;
		*out+=2;
		depth = *(short int*)(((char*)renderPointList) + pointNumber + 4); // Z
		ax+=2;
#endif

		if(depth<min)
			min = depth;
		if(depth>max)
			max = depth;
	}

	primVar2 = *out;

	*out = saveDi;

	if(min<=0) // behind camera
	{
		*out = primVar1; // do not add the prim
	}
	else
	{
		
		int prod1;
		int prod2;
		int prod;
		int cx;
#ifdef USE_GL
		int bx = *(short int*)((*out)+6) - *(short int*)((*out));
		int ax = *(short int*)((*out)+2) - *(short int*)((*out)+14);
#else
		int bx = *(short int*)((*out)+4) - *(short int*)((*out));
		int ax = *(short int*)((*out)+2) - *(short int*)((*out)+10);
#endif
		renderVar3 = max;
		ax *= bx;

		prod1 = ax;

#ifdef USE_GL
		cx = *(short int*)((*out)+8) - *(short int*)((*out)+2);
		ax = *(short int*)((*out)) - *(short int*)((*out)+12);
#else
		cx = *(short int*)((*out)+6) - *(short int*)((*out)+2);
		ax = *(short int*)((*out)) - *(short int*)((*out)+8);
#endif
		ax *= cx;

		prod2 = ax;

		prod = prod2 - prod1;

#ifdef USE_GL
		prod = 1;
#endif

		if(prod<0)
		{
			*out = primVar1; // do not add the prim
		}
		else 
		{
			numOfPolyToRender++;

			*(out) = renderVar2;

			*(short int*)(*out) = renderVar3;
			*out+=2;
			*(short int*)(*out) = renderVar3;
			*out+=2;
			*(short int*)(*out) = 1;
			*out+=2;

			*(char**)(*out) = primVar1;
			*out+=4;

			renderVar2 = *out;
			*out = primVar2;
		}
	}
}

void primType2(int primType, char** ptr, char** out)
{
	return;
/*	primVar1 = *out;

	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=2;
	*ptr+=3;

	int ax = *(short int*)(*ptr);
	*ptr+=2;

	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // X
	ax+=2;
	*out+=2;
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Y
	ax+=2;
	*out+=2;
	int ax2 = *(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;
	*out+=2;

	primVar2 = *out;

	//debug: dummy
	ax2 = 0;

	if(ax2<=0)
	{
		*out = primVar1; // do not add the prim
	}
	else
	{
		numOfPolyToRender++;

		*out = renderVar2;

		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = primType;
		*out+=2;

		*(char**)(*out) = primVar1;
		*out+=4;

		renderVar2 = *out;
		*out = primVar2;
	} */

}

void primType3(int primType, char** ptr, char** out)
{
	return;
/*	primVar1 = *out;

	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=2;
	*ptr+=3;

	int ax = *(short int*)(*ptr);
	*ptr+=2;

	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // X
	ax+=2;
	*out+=2;
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Y
	ax+=2;
	*out+=2;
	int ax2 = *(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;
	*out+=2;

	primVar2 = *out;

	ax2 = 0;
	if(ax2<=0)
	{
		*out = primVar1; // do not add the prim
	}
	else
	{
		numOfPolyToRender++;

		*out = renderVar2;

		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = 3;
		*out+=2;

		*(char**)(*out) = primVar1;
		*out+=4;

		renderVar2 = *out;
		*out = primVar2;
	}*/
}

void line(int x1, int y1, int x2, int y2, char c);

void renderStyle0(char* buffer)
{
	char color;
#ifdef USE_GL
	float X1;
	float Y1;
	float Z1;

	float X2;
	float Y2;
	float Z2;

#else
	short int X1;
	short int Y1;
	short int X2;
	short int Y2;
#endif

	
	
	buffer++;

	color = *(buffer++);

#ifdef USE_GL
	X1 = *(float*)buffer;
	buffer+=sizeof(float);
	Y1 = *(float*)buffer;
	buffer+=sizeof(float);
	Z1 = *(float*)buffer;
	buffer+=sizeof(float);

	X2 = *(float*)buffer;
	buffer+=sizeof(float);
	Y2 = *(float*)buffer;
	buffer+=sizeof(float);
	Z2 = *(float*)buffer;
	buffer+=sizeof(float);

#else
	X1 = *(short int*)buffer;
	buffer+=2;
	Y1 = *(short int*)buffer;
	buffer+=2;
	X2 = *(short int*)buffer;
	buffer+=2;
	Y2 = *(short int*)buffer;
	buffer+=2;
#endif

#ifdef USE_GL
	osystem_draw3dLine(X1,Y1,Z1,X2,Y2,Z2,color);
#else
	line(X1,Y1,X2,Y2,color);
#endif
}

// buffer is made of:
// numPoint (short int)
// color (short int)
// {
//  X
//  Y
//  Z
// } * numPoint
void renderStyle1(char* buffer)
{
	int i;

	int max = 3000;
	int min = -3000;

	int numPoint = *(short int*)buffer;
	int color;
	buffer+=2;

	numPoint &= 0xF;

	color = *(short int*)buffer;
	buffer+=2;

	{
		short int* ptr = (short int*) buffer;

		for(i=0;i<numPoint;i++)
		{
			ptr++;

			if(*ptr<max)
				max = *ptr;

			if(*ptr>min)
				min = *ptr;

			ptr++;
			ptr++;
		}
	}

#ifdef USE_GL
	osystem_fillPoly((float *)buffer,numPoint,color);
#else
	if(max>=0 && min <320)
		fillpoly((short *)buffer,numPoint,color);
#endif
}

void defaultRenderFunction(char* buffer)
{
	printf("Unsupported renderType\n");
}

typedef void (*renderFunction)(char* buffer);

renderFunction renderFunctions[]={
	renderStyle0,
	renderStyle1,
	defaultRenderFunction,
	defaultRenderFunction,
	renderStyle1,
	defaultRenderFunction,
	defaultRenderFunction,
	defaultRenderFunction,
};

typedef void (*primFunction)(int primType,char** ptr, char** out);

primFunction primFunctionTable[]={
	primType0,
	primType1,
	primType2,
	primType3,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
	primFunctionDefault,
};

int renderModel(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr)
{
	char* ptr = (char*)modelPtr;
	int numPrim;
	int i;
	char* out;

	char* source;
#ifndef USE_GL
	char sortedBuffer[32000];

	char* inBuffer;
	char* outBuffer;
#endif

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
		if(!computeModel(x,y,z,alpha,beta,gamma,modelPtr,ptr))
		{
			BBox3D3 = -32000;
			BBox3D4 = -32000;
			BBox3D1 = 32000;
			BBox3D2 = 32000;
			return(2);
		}
	}
	else
	if(!(modelFlags&4))
	{
		if(!prerenderFlag0(x,y,z,alpha,beta,gamma,modelPtr,ptr))
		{
			BBox3D3 = -32000;
			BBox3D4 = -32000;
			BBox3D1 = 32000;
			BBox3D2 = 32000;
			return(2);
		}
	}
	else
	{
		printf("unsupported model type prerenderFlag4 in renderer !\n");

		BBox3D3 = -32000;
		BBox3D4 = -32000;
		BBox3D1 = 32000;
		BBox3D2 = 32000;
		return(2);
	}

	ptr = tempOutPtr;
	numPrim = *(short int*)ptr;
	ptr+=2;

	if(!numPrim)
	{
		BBox3D3 = -32000;
		BBox3D4 = -32000;
		BBox3D1 = 32000;
		BBox3D2 = 32000;
		return(2);
	}

	out = primBuffer;

	for(i=0;i<numPrim;i++)
	{
		unsigned char primType = *(ptr++);

		if(primType==1)
		{
			primFunctionTable[primType](primType,&ptr,&out);
		}
		else
		{
			break;
		}
	}

	// TODO: poly sorting by depth
#ifdef USE_GL
	source = renderBuffer;
#else
	inBuffer = renderBuffer;
	outBuffer = sortedBuffer;

	for(i=0;i<numOfPolyToRender;i++)
	{
		int j;
		int bestIdx;
		int bestDepth = -32000;
		char* readBuffer = renderBuffer;

		for(j=0;j<numOfPolyToRender;j++)
		{
			int depth = *(short int*)(readBuffer);

			if(depth>bestDepth)
			{
				bestIdx = j;
				bestDepth = depth;
			}

			readBuffer+=10;
		}

		memcpy(outBuffer,renderBuffer+10*bestIdx,10);
		*(short int*)(renderBuffer+10*bestIdx) = -32000;
		outBuffer+=10;
	}
	source = sortedBuffer;

#endif
	
//	

	if(!numOfPolyToRender)
	{
		BBox3D3 = -32000;
		BBox3D4 = -32000;
		BBox3D1 = 32000;
		BBox3D2 = 32000;
		return(1); // model ok, but out of screen
	}

//	source += 10 * 1;
	for(i=0;i<numOfPolyToRender;i++)
	{
		int renderType;
		char* bufferSource;
		
		source+=4;

		renderType = *(short int*)(source);
		source+=2;
		bufferSource = *(char**)(source);
		source+=4;

		if(renderType == 0 || renderType == 1)
			renderFunctions[renderType](bufferSource);
	}

//DEBUG
/*	for(i=0;i<numPointInPoly;i++)
	{
		int x;
		int y;

		x = renderPointList[i*3];
		y = renderPointList[i*3+1];

		if(x>=0 && x < 319 && y>=0 && y<199)
		{
			screen[y*320+x] = 15;
		}
	} */
//

	return(0);
}

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char* bodyPtr)
{
	char* ptr = (char*)bodyPtr;

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
		computeModel(x,y,z,alpha,beta,gamma,bodyPtr,ptr);
	}
}
