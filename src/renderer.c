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

char primBuffer[8000];

char* tempOutPtr;

int renderVar3;

void fillpoly(short unsigned int * datas, int n, char c);

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

	/////////////////////////
	// DEBUG
		transX = 0;
		transY = 0;
		transZ = 0;

	/////////////////////////

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

	//(*ptr) = si;
	tempOutPtr = si;

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
	primVar1 = *(out);
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
	int depth1 = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;

	ax = *(short int*)(*ptr);
	*ptr+=2;

	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // X
	ax+=2;
	*out+=2;
	*(short int*)(*out) = *(short int*)(((char*)renderPointList) + ax); // Y
	ax+=2;
	*out+=2;
	int depth2 = *(short int*)(((char*)renderPointList) + ax); // Z
	ax+=2;

	primVar2 = *out;

	int depthLow;
	int depthHi;

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
	primVar1 = *out;

	int ax = **ptr;
	**out = ax;
	int cx = ax;
	(*out)++;
	(*ptr)++;

	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=3;
	*ptr+=2;

	int min = 32000;
	int max = -32000;

	int i;

	char* saveDi = *out;

	for(i=0;i<cx;i++)
	{
		int pointNumber = *(short int*)(*ptr);
		*ptr+=2;

		*(short int*)(*out) = *(short int*)(((char*)renderPointList) + pointNumber); // X
		ax+=2;
		*out+=2;
		*(short int*)(*out) = *(short int*)(((char*)renderPointList) + pointNumber + 2); // Y
		ax+=2;
		*out+=2;
		int depth = *(short int*)(((char*)renderPointList) + pointNumber + 4); // Z
		ax+=2;

		if(depth<min)
			min = depth;
		if(depth>max)
			max = depth;
	}

	primVar2 = *out;

	*out = saveDi;

	// debug: dummy
	//min = 0;
	
	if(min<=0) // behind camera
	{
		*out = primVar1; // do not add the prim
	}
	else
	{
		renderVar3 = max;

		int bx = *(short int*)((*out)+4) - *(short int*)((*out));
		int ax = *(short int*)((*out)+2) - *(short int*)((*out)+10);

		ax *= bx;

		int prod1 = ax;

		int cx = *(short int*)((*out)+6) - *(short int*)((*out)+2);
		ax = *(short int*)((*out)) - *(short int*)((*out)+8);

		ax *= cx;

		int prod2 = ax;

		int prod = prod2 - prod1;

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
	primVar1 = *out;

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
	}

}

void primType3(int primType, char** ptr, char** out)
{
	primVar1 = *out;

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
	}

}

void line(int x1, int y1, int x2, int y2, char c);

void renderStyle0(char* buffer)
{
	buffer++;

	char color = *(buffer++);

	short int X1 = *(short int*)buffer;
	buffer+=2;
	short int Y1 = *(short int*)buffer;
	buffer+=2;
	short int X2 = *(short int*)buffer;
	buffer+=2;
	short int Y2 = *(short int*)buffer;
	buffer+=2;

	line(X1,Y1,X2,Y2,color);
}

void renderStyle1(char* buffer)
{
	int i;

	int max = 3000;
	int min = -3000;

	int numPoint = *(short int*)buffer;
	buffer+=2;

	int color = *(short int*)buffer;
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
		}
	}

	if(max>=0 && min <200)
		fillpoly((unsigned short *)buffer,numPoint,color);

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
	int numPrim = *(short int*)ptr;
	ptr+=2;

	if(!numPrim)
	{
		BBox3D3 = -32000;
		BBox3D4 = -32000;
		BBox3D1 = 32000;
		BBox3D2 = 32000;
		return(2);
	}

	int i;
	char* out = primBuffer;

	for(i=0;i<numPrim;i++)
	{
		unsigned char primType = *(ptr++);

		if(primType>3)
			break;

		primFunctionTable[primType](primType,&ptr,&out);
	}

	// TODO: poly sorting by depth

	char* source = renderBuffer;

	if(!numOfPolyToRender)
	{
		BBox3D3 = -32000;
		BBox3D4 = -32000;
		BBox3D1 = 32000;
		BBox3D2 = 32000;
		return(1); // model ok, but out of screen
	}

	for(i=0;i<numOfPolyToRender;i++)
	{
		source+=4;

		int renderType = *(short int*)(source);
		source+=2;
		char* bufferSource = *(char**)(source);
		source+=4;

		renderFunctions[renderType](bufferSource);
	}

//DEBUG
	for(i=0;i<numPointInPoly;i++)
	{
		int x;
		int y;

		x = renderPointList[i*3];
		y = renderPointList[i*3+1];

		if(x>=0 && x < 319 && y>=0 && y<199)
		{
	//		screen[y*320+x] = 15;
		}
	}
//

	return(0);
}
