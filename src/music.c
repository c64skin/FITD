#include "common.h"

typedef int(*musicDrvFunctionType)(void);

int initialialize(void)
{
}

int getSignature(void)
{
  return 1;
}

musicDrvFunctionType musicDrvFunc[14]=
{
  NULL,
  initialialize,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  getSignature,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};

int callMusicDrv(int commandArg)
{
  if(!musicDrvFunc[commandArg])
  {
    exit(1);
  }

  return musicDrvFunc[commandArg]();
}

int initMusicDriver(void)
{
  callMusicDrv(1);
  
  return callMusicDrv(8);
}