#include "common.h"

hqrSubEntryStruct* quickFindEntry(int index, int numMax, hqrSubEntryStruct* ptr) // no RE. Original was probably faster
{
	int i;

	for(i=0;i<numMax;i++)
	{
		if(ptr[i].key == index)
		{
			return(&ptr[i]);
		}
	}

	return(NULL);
}

hqrEntryStruct* HQR_InitRessource(char* name, int size, int numEntries)
{
	hqrEntryStruct* dest;
	char* dest2;

	dest = (hqrEntryStruct*)malloc(numEntries*sizeof(hqrSubEntryStruct)+sizeof(hqrEntryStruct));

	if(!dest)
		return NULL;

	dest2 = (char*)malloc(size + 300);

	if(!dest2)
		return NULL;

	strcpy(dest->string,"        ");
	strncpy(dest->string,name,8);

	dest->sizeFreeData = size;
	dest->maxFreeData = size;
	dest->numMaxEntry = numEntries;
	dest->numUsedEntry = 0;
	dest->dataPtr = dest2;

	return(dest);
}

int printTextSub1(hqrEntryStruct* hqrPtr,int size)
{
	hqrSubEntryStruct* dataPtr1;
	hqrSubEntryStruct* dataPtr2;
	int key;
	int entryNum;

	if(hqrPtr->sizeFreeData<size)
		return(-1);

	entryNum = hqrPtr->numUsedEntry;

	dataPtr1 = dataPtr2 = (hqrSubEntryStruct*)((char*)hqrPtr+sizeof(hqrEntryStruct));

	key = hqrKeyGen;

	dataPtr1[entryNum].key = key;

	dataPtr1[entryNum].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
	dataPtr1[entryNum].size = size;

	hqrPtr->numUsedEntry++;
	hqrPtr->sizeFreeData -= size;

	hqrKeyGen++;

	return(key);
}

char* printTextSub2(hqrEntryStruct* hqrPtr, int index)
{
	hqrSubEntryStruct* ptr;
	hqrSubEntryStruct* dataPtr;

	if(index<0)
		return NULL;

	dataPtr = (hqrSubEntryStruct*)((char*)hqrPtr+sizeof(hqrEntryStruct));

	ptr = quickFindEntry(index, hqrPtr->numUsedEntry, dataPtr);

	if(!ptr)
		return NULL;

	return(hqrPtr->dataPtr + ptr->offset);
}

void moveHqrEntry(hqrEntryStruct* hqrPtr, int index)
{
	hqrSubEntryStruct* hqrSubPtr = (hqrSubEntryStruct*)(((char*)hqrPtr)+sizeof(hqrEntryStruct));
	hqrSubEntryStruct* hqrSubPtr2 = hqrSubPtr;

	int size = hqrSubPtr[index].size;

	if(hqrPtr->numUsedEntry - 1 > index ) //if not last entry
	{
		char* dest = hqrPtr->dataPtr + hqrSubPtr2[index].offset;
		char* src = dest + size;

		memcpy(dest,src,hqrPtr->dataPtr + hqrPtr->maxFreeData - src);

		dest = (char*)&hqrSubPtr2[index];
		src = (char*)&hqrSubPtr2[index+1];
		memcpy(dest,src,hqrPtr->numMaxEntry-(index+1) * sizeof(hqrSubEntryStruct));
	}

	hqrPtr->numUsedEntry --;
	hqrPtr->sizeFreeData += size;
}

char* HQR_Get(hqrEntryStruct* hqrPtr, int index)
{
	hqrSubEntryStruct* hqrSubPtr;
	hqrSubEntryStruct* foundEntry;
	
	if(index<0)
		return NULL;

	hqrSubPtr = (hqrSubEntryStruct*)(((char*)hqrPtr)+sizeof(hqrEntryStruct));

	foundEntry = quickFindEntry(index,hqrPtr->numUsedEntry,hqrSubPtr);

	if(foundEntry)
	{
		foundEntry->lastTimeUsed = timer;
		hqrVar1 = 0;

		return(hqrPtr->dataPtr + foundEntry->offset);
	}
	else
	{
		int size;
		unsigned int time;
		char* ptr;
		
		freezeTime();
		size = getPakSize(hqrPtr->string,index);

		if(size>=hqrPtr->maxFreeData)
		{
			theEnd(1,hqrPtr->string);
		}

		time = timer;

		foundEntry = hqrSubPtr;

		while(size>hqrPtr->sizeFreeData || hqrPtr->numUsedEntry>= hqrPtr->numMaxEntry)
		{
			int bestEntry = 0;
			unsigned int bestTime = 0;
			int entryIdx = 0;

			for(entryIdx = 0; entryIdx<hqrPtr->numUsedEntry; entryIdx++)
			{
				if(time - foundEntry[entryIdx].lastTimeUsed > bestTime)
				{
					bestTime = time - foundEntry[entryIdx].lastTimeUsed;
					bestEntry = entryIdx;
				}
			}

			moveHqrEntry(hqrPtr,bestEntry);
		}

		ptr = hqrPtr->dataPtr + (hqrPtr->maxFreeData - hqrPtr->sizeFreeData);

		if(!loadPakToPtr(hqrPtr->string,index,ptr))
		{
			theEnd(1,hqrPtr->string);
		}

		hqrVar1 = 1;

		foundEntry[hqrPtr->numUsedEntry].key = index;
		foundEntry[hqrPtr->numUsedEntry].lastTimeUsed = timer;
		foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
		foundEntry[hqrPtr->numUsedEntry].size = size;

		hqrPtr->numUsedEntry++;
		hqrPtr->sizeFreeData -= size;

		unfreezeTime();

		return(ptr);
	}

}