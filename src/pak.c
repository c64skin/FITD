// seg 55

#include "common.h"

typedef struct  // warning: allignement unsafe
{
  long int discSize;
  long int uncompressedSize;
  char compressionFlag;
  char info5;
  short int offset;
}pakInfoStruct;

#define USE_UNPACKED_DATA

int loadPakToPtr(char* name, int index, char* ptr)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return(0);

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  fread(ptr,size,1,fHandle);
  fclose(fHandle);

  return(1);
#else
  char* lptr;

  lptr = loadPak(name,index);

  memcpy(ptr,lptr,getPakSize(name,index));

  free(lptr);

  return(1);
#endif
}

int getPakSize(char* name, int index)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return(0);

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  fclose(fHandle);

  return (size);
#else
  char bufferName[256];
  FILE* fileHandle;
  long int fileOffset;
  long int additionalDescriptorSize;
  pakInfoStruct pakInfo;
  char* ptr=0;
  long int size=0;

  strcpy(bufferName, name); // temporary until makeExtention is coded
  strcat(bufferName,".PAK");

  fileHandle = fopen(bufferName,"rb");

  if(fileHandle) // a bit stupid, should return NULL right away
  {
    fseek(fileHandle,(index+1)*4,SEEK_SET);

    fread(&fileOffset,4,1,fileHandle);

    fseek(fileHandle,fileOffset,SEEK_SET);

    fread(&additionalDescriptorSize,4,1,fileHandle);

    if(additionalDescriptorSize)
    {
      printf("Unimplemented additionalDescriptorSize in loadPak\n");
/*      return(0);
      exit(1); */
    }

    fread(&pakInfo,sizeof(pakInfoStruct),1,fileHandle);

    fseek(fileHandle,pakInfo.offset,SEEK_CUR);

    if(pakInfo.compressionFlag == 0) // uncompressed
    {
      size = pakInfo.discSize;
    }
    else if(pakInfo.compressionFlag == 1) // compressed
    {
      size = pakInfo.uncompressedSize;
    }

    fclose(fileHandle);
  }

  return size;
#endif
}

char* loadPak(char* name, int index)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;
  char* ptr;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return NULL;

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  ptr = (char*)malloc(size);

  fread(ptr,size,1,fHandle);
  fclose(fHandle);

  return ptr;
#else
  char bufferName[256];
  FILE* fileHandle;
  long int fileOffset;
  long int additionalDescriptorSize;
  pakInfoStruct pakInfo;
  char* ptr=0;

  //makeExtention(bufferName, name, ".PAK");
  strcpy(bufferName, name); // temporary until makeExtention is coded
  strcat(bufferName,".PAK");

  fileHandle = fopen(bufferName,"rb");

  if(fileHandle) // a bit stupid, should return NULL right away
  {
    fseek(fileHandle,(index+1)*4,SEEK_SET);

    fread(&fileOffset,4,1,fileHandle);

    fseek(fileHandle,fileOffset,SEEK_SET);

    fread(&additionalDescriptorSize,4,1,fileHandle);

    if(additionalDescriptorSize)
    {
      printf("Unimplemented additionalDescriptorSize in loadPak\n");
/*      return(0);
      exit(1); */
    }

    fread(&pakInfo,sizeof(pakInfoStruct),1,fileHandle);

    fseek(fileHandle,pakInfo.offset,SEEK_CUR);

    if(pakInfo.compressionFlag == 0) // uncompressed
    {
      ptr = (char*)malloc(pakInfo.discSize);
      fread(ptr,pakInfo.discSize,1,fileHandle);

    }
    else if(pakInfo.compressionFlag == 1) // compressed
    {
      char* compressedDataPtr;

      ptr = (char*)malloc(0xFFFFF);

      compressedDataPtr = ptr + pakInfo.uncompressedSize + 300 - pakInfo.discSize;

      fread(compressedDataPtr,pakInfo.discSize,1,fileHandle);

      unpack(pakInfo.info5, compressedDataPtr, ptr, pakInfo.uncompressedSize, ptr+pakInfo.uncompressedSize + 300);

      ptr = (char*)realloc(ptr,pakInfo.uncompressedSize);
    }

    fclose(fileHandle);
  }

  return ptr;
#endif
}
