#ifndef _ROOM_H_
#define _ROOM_H_

struct hardColStruct
{
  ZVStruct zv;
  u32 type;
  u32 parameter;
};

typedef struct hardColStruct hardColStruct;

struct sceZoneStruct
{
  ZVStruct zv;
  u32 type;
  u32 parameter;
};

typedef struct sceZoneStruct sceZoneStruct;

struct roomDataStruct
{
  u32 numCameraInRoom;

  u32 numHardCol;
  hardColStruct* hardColTable;

  u32 numSceZone;
  sceZoneStruct* sceZoneTable;

  s32 worldX;
  s32 worldY;
  s32 worldZ;
};

typedef struct roomDataStruct roomDataStruct;

extern roomDataStruct* roomDataTable;

roomDefStruct* getRoomData(int roomNumber);
void loadRoom(int roomNumber);

#endif