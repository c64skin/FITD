#include "common.h"

void loadRoom(int roomNumber)
{
	int i;
	int cameraVar0;
	int cameraVar1;
	int cameraVar2;
	int currentCameraIdx;
	roomDefStruct* roomDataPtr;
	char* var_20;
	int var_1A = 0;
	int var_10 = -1;
	int var_1C;

	freezeTime();
	printf("Load room %d\n",roomNumber);

	if(currentCamera == -1)
	{
		currentCameraIdx = -1;
	}
	else
	{
		cameraVar0 = *(short int*)(cameraPtr+4);
		cameraVar1 = *(short int*)(cameraPtr+6);
		cameraVar2 = *(short int*)(cameraPtr+8);

		currentCameraIdx = *(short int*)(cameraPtr + (currentCamera+6)*2);
	}

	cameraPtr = etageVar0+*(unsigned int*)(etageVar0 + (roomNumber * 4));
	roomDataPtr = (roomDefStruct*)(etageVar0+*(unsigned int*)(etageVar0 + (roomNumber * 4)));

	currentDisplayedRoom = roomNumber;

	numCameraInRoom = roomDataPtr->numCameraInRoom;

	var_20 = cameraPtr + roomDataPtr->offsetToPosDef;
	numCameraZone = *(short int*)var_20;
	var_20 += 2;
	cameraZoneData = var_20;

	var_20 = cameraPtr + roomDataPtr->offsetToCameraDef;
	numRoomZone = *(short int*)var_20;
	var_20 += 2;
	roomZoneData = var_20;
	
	for(i=0;i<numCameraInRoom;i++) // build all the camera list
	{
		int cameraIdx = *(short int*)(cameraPtr + (i+6)*2); // indexes are between the roomDefStruct and the first zone data
		int j;
		char* var_8;

		if(currentCameraIdx == cameraIdx)
		{
			var_1A = i;
			var_10 = cameraIdx;
		}

		roomVar5[i] = etageVar1 + *(unsigned int*)(etageVar1 + cameraIdx * 4);
		var_20 = roomVar5[i] + 0x12;
		cameraIdx = *(short int*)var_20;
		var_20 +=2;

	/*	for(j=0;*(short int*)(var_20+=2)!=currentDisplayedRoom;(j++) && (var_20+=0xA))
		{
			if(j>= cameraIdx)
			{
				break;
			}
		} */

		for(j=0;j<cameraIdx;j++)
		{
			if(*(short int*)var_20 == currentDisplayedRoom)
				break;

			var_20+=2;
			var_20+=0xA;
		}

		var_1C = j;

		var_8 = roomVar5[i] + (var_1C*12) + 0x18;

		roomVar6[i] = (*(short int*)var_8)/2;
	}

	if(currentCameraIdx != -1) // if the two room have the current camera in common, update the actor transform
	{
		int var_E = (*(short int*)(cameraPtr + 4) - cameraVar0) * 10;
		int var_C = (*(short int*)(cameraPtr + 6) - cameraVar1) * 10;
		int var_A = (*(short int*)(cameraPtr + 8) - cameraVar2) * 10;

		for(i=0;i<50;i++)
		{
			if(actorTable[i].field_0 != -1)
			{
				actorTable[i].worldX -= var_E;
				actorTable[i].worldY += var_C;
				actorTable[i].worldZ += var_A;
			}
		}
	}

	startGameVar1 = var_1A;
	mainVar1 = 1;
	needChangeRoom = 0;
	unfreezeTime();
}