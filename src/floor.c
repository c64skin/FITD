#include "common.h"

unsigned long int etageVar0Size = 0;

void loadFloor(int floorNumber)
{
  int i;
  int expectedNumberOfRoom;
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

  sprintf(buffer,"ETAGE%02d",floorNumber);

  etageVar0Size = getPakSize(buffer,0);

  etageVar0 = loadPakSafe(buffer,0);
  etageVar1 = loadPakSafe(buffer,1);

  currentCamera = -1;
  needChangeRoom = 1;
  changeFloor = 0;

  //////////////////////////////////

  if(roomDataTable)
  {
    free(roomDataTable);
    roomDataTable = NULL;
  }

  expectedNumberOfRoom = getNumberOfRoom();

  for(i=0;i<expectedNumberOfRoom;i++)
  {
    u32 j;
    u8* roomData;
    u8* hardColData;
    u8* sceZoneData;
    roomDataStruct* currentRoomDataPtr;

    if(roomDataTable)
    {
      roomDataTable = (roomDataStruct*)realloc(roomDataTable,sizeof(roomDataStruct)*(i+1));
    }
    else
    {
      roomDataTable = (roomDataStruct*)malloc(sizeof(roomDataStruct));
    }

    roomData = (etageVar0 + *(unsigned int*)(etageVar0 + i * 4));
    currentRoomDataPtr = &roomDataTable[i];

    currentRoomDataPtr->worldX = READ_LE_S16(roomData+4);
    currentRoomDataPtr->worldY = READ_LE_S16(roomData+6);
    currentRoomDataPtr->worldZ = READ_LE_S16(roomData+8);

    currentRoomDataPtr->numCameraInRoom = READ_LE_U16(roomData+0xA);

    // hard col read

    hardColData = roomData + READ_LE_U16(roomData);
    currentRoomDataPtr->numHardCol = READ_LE_U16(hardColData);
    hardColData+=2;

    if(currentRoomDataPtr->numHardCol)
    {
      currentRoomDataPtr->hardColTable = (hardColStruct*)malloc(sizeof(hardColStruct)*currentRoomDataPtr->numHardCol);

      for(j=0;j<currentRoomDataPtr->numHardCol;j++)
      {
        ZVStruct* zvData;

        zvData = &currentRoomDataPtr->hardColTable[j].zv;

        zvData->ZVX1 = READ_LE_U16(hardColData+0x00);
        zvData->ZVX2 = READ_LE_U16(hardColData+0x02);
        zvData->ZVY1 = READ_LE_U16(hardColData+0x04);
        zvData->ZVY2 = READ_LE_U16(hardColData+0x06);
        zvData->ZVZ1 = READ_LE_U16(hardColData+0x08);
        zvData->ZVZ2 = READ_LE_U16(hardColData+0x0A);

        currentRoomDataPtr->hardColTable[j].parameter = READ_LE_U16(hardColData+0x0C);
        currentRoomDataPtr->hardColTable[j].type = READ_LE_U16(hardColData+0x0E);

        hardColData+=0x10;
      }
    }
    else
    {
      currentRoomDataPtr->hardColTable = NULL;
    }

    // sce zone read

    sceZoneData = roomData + READ_LE_U16(roomData+2);
    currentRoomDataPtr->numSceZone = READ_LE_U16(sceZoneData);
    sceZoneData+=2;

    if(currentRoomDataPtr->numSceZone)
    {
      currentRoomDataPtr->sceZoneTable = (sceZoneStruct*)malloc(sizeof(sceZoneStruct)*currentRoomDataPtr->numSceZone);

      for(j=0;j<currentRoomDataPtr->numSceZone;j++)
      {
        ZVStruct* zvData;

        zvData = &currentRoomDataPtr->sceZoneTable[j].zv;

        zvData->ZVX1 = READ_LE_U16(sceZoneData+0x00);
        zvData->ZVX2 = READ_LE_U16(sceZoneData+0x02);
        zvData->ZVY1 = READ_LE_U16(sceZoneData+0x04);
        zvData->ZVY2 = READ_LE_U16(sceZoneData+0x06);
        zvData->ZVZ1 = READ_LE_U16(sceZoneData+0x08);
        zvData->ZVZ2 = READ_LE_U16(sceZoneData+0x0A);

        currentRoomDataPtr->sceZoneTable[j].parameter = READ_LE_U16(sceZoneData+0x0C);
        currentRoomDataPtr->sceZoneTable[j].type = READ_LE_U16(sceZoneData+0x0E);

        sceZoneData+=0x10;
      }
    }
    else
    {
      currentRoomDataPtr->sceZoneTable = NULL;
    }
  }
  ///////////////////////////////////
}
