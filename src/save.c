#include "common.h"

unsigned int currentSaveEntrySize;

void* getSaveEntry(int index)
{
  currentSaveEntrySize = saveTable[index].size;

  return(saveTable[index].ptr);
}

int loadSave(int saveNumber)
{
  char buffer[256];
  FILE* fHandle;
  unsigned int var28;
  int var_14 = 0;
  char* ptr;
  int var_E;
  int var_16;
  unsigned int offsetToVars;
  unsigned short int tempVarSize;
  unsigned int offsetToActors;
  int i;
  
  sprintf(buffer,"SAVE%d.ITD",saveNumber);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
  {
    return(0);
  }

  initEngine();
  initVars();

  fseek(fHandle,8,SEEK_SET);

  fread(&var28,4,1,fHandle);

  var28 = ((var28 & 0xFF) << 24) | ((var28 & 0xFF00) << 8) | (( var28 & 0xFF0000) >> 8) | ((var28 & 0xFF000000) >> 24);

  fseek(fHandle,var28,SEEK_SET);

  do
  {
    ptr = (char*)getSaveEntry(var_14);

    if(ptr)
    {
      fread(ptr,currentSaveEntrySize,1,fHandle);
    }
    var_14++;

  }while(ptr);

  //timerFreeze = 1;

  var_E = currentCamera;

  loadFloor(currentEtage);
  currentCamera = -1;
  loadRoom(currentDisplayedRoom);
  var_16 = currentMusic;
  currentMusic = -1;
  playMusic(var_16);

  fseek(fHandle,12,SEEK_SET);
  fread(&offsetToVars,4,1,fHandle);
  offsetToVars = ((offsetToVars & 0xFF) << 24) | ((offsetToVars & 0xFF00) << 8) | (( offsetToVars & 0xFF0000) >> 8) | ((offsetToVars & 0xFF000000) >> 24);
  fseek(fHandle,offsetToVars,SEEK_SET);

  fread(&tempVarSize,2,1,fHandle);
  varSize = tempVarSize;

  fread(vars,varSize,1,fHandle);

  configureHqrHero(listBody,listBodySelect[defines.hero]);
  configureHqrHero(listAnim,listAnimSelect[defines.hero]);

  fseek(fHandle,16,SEEK_SET);
  fread(&offsetToActors,4,1,fHandle);
  offsetToVars = ((offsetToActors & 0xFF) << 24) | ((offsetToActors & 0xFF00) << 8) | (( offsetToActors & 0xFF0000) >> 8) | ((offsetToActors & 0xFF000000) >> 24);
  fseek(fHandle,offsetToVars,SEEK_SET);

  fread(actorTable,8000,1,fHandle);
  fclose(fHandle);

  for(i=0;i<NUM_MAX_ACTOR;i++)
  {
    if(actorTable[i].field_0 != -1 && actorTable[i].bodyNum != -1)
    {
      char* bodyPtr = HQR_Get(listBody,actorTable[i].bodyNum);

      if(actorTable[i].ANIM != -1)
      {
        char* animPtr = HQR_Get(listAnim,actorTable[i].ANIM);
        initAnimInBody(actorTable[i].FRAME,animPtr,bodyPtr);
      }
    }
  }

  startGameVar1 = var_E;

  return(1);
}

int restoreSave(int arg0, int arg1)
{
  int selectedSave;
//  restoreSaveVar1 = arg0;

  if(arg1==0)
  {
    flushScreen();
    flipScreen();
    make3dTatouUnk1(0x40,0);
  }

  selectedSave = parseAllSaves(0);

  if(arg1==0)
  {
  //  fadeOut(8,0);
  }

  if(selectedSave == -1)
  {
    return(0);
  }

  if(arg1==0)
  {
    //freeScene();
  }

  return(loadSave(selectedSave));

}