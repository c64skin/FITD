#include "common.h"

void loadFloor(int floorNumber)
{
	char buffer[256];

	if(etageVar1)
	{
		free(etageVar1);
		free(etageVar0);
	}

	//stopSounds();

	HQR_Reset(listBody);
	HQR_Reset(listAnim);

	currentEtage = floorNumber;

	sprintf(buffer,"ETAGE0%d",floorNumber);

	etageVar0 = loadPakSafe(buffer,0);
	etageVar1 = loadPakSafe(buffer,1);

	currentCamera = -1;
	needChangeRoom = 1;
	changeFloor = 0;
}