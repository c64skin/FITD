#include "common.h"

char* inputPtr;
char* outputPtr;

short int unV4 = 0;
unsigned char numBitInMask = 6;
unsigned char mask = 0;
short int compressParam = 0;
char* unV3 = 0;
short int unv13 = 0;
short int unV7 = 0;
char* unV1;
char* unV2;
unsigned short int unV14;
unsigned short int unV17;
unsigned short int unV8;
char* si;

unsigned long int decompPos;

char* unpackSub0(int param, char** outputInputPtr)
{
  unsigned short int bx;
  unsigned char cl;
  unsigned char ax;
  unsigned char di;
  unsigned short int dx;
  char* backupSource;

  int i;
  int j;

  unv13 = param;

  bx = *((unsigned char*)si);
  si++;

  di = 0;
  dx = 0;

  do
  {
    cl = ax = *((unsigned char*)si);
    si++;

    cl&=0x0F;
    cl++;

    ax >>=4;
    ax++;

    for(i=0;i<ax;i++)
    {
      outputPtr[di+3] = cl;
      outputPtr[di+2] = dx&0xFF;
      dx++;
      di+=4;
    }
  }while(!((--bx)&0x80));

  backupSource = si;

  {
    unsigned short int cx = param;
    unsigned short int si;
    bool sortFinished;

    cx--;
    si = cx/2;

    do // sort loop
    {
      unsigned char dh = 0;
      unsigned short int ax = 0;

      sortFinished = true;

      ax = cx&0xFF;
      ax -= si;

      unV7 = ax & 0xFF;

      while(dh<unV7)
      {
        unsigned short int backupSi;
        unsigned char bl;
        unsigned char bh;

        ax = dh & 0xFF;

        di = ax / 4;

        backupSi = si;

        si += dh;

        si *= 4;

        if( ( outputPtr[ax + 3] >  outputPtr[si + 3] ) || (( outputPtr[ax + 3] >  outputPtr[si + 3] ) && ( outputPtr[ax + 2] >  outputPtr[si + 2] ))) // TODO: recheck
        {
          unsigned short int temp;

          temp = *((unsigned short int*)&(outputPtr[ax]));
          *((unsigned short int*)&(outputPtr[ax])) = *((unsigned short int*)&(outputPtr[si]));
          *((unsigned short int*)&(outputPtr[si])) = temp;

          temp = *((unsigned short int*)&(outputPtr[ax+2]));
          *((unsigned short int*)&(outputPtr[ax+2])) = *((unsigned short int*)&(outputPtr[si+2]));
          *((unsigned short int*)&(outputPtr[si+2])) = temp;

          sortFinished = false;
        }

        si = backupSi;

        dh++;
      }
    }while(!sortFinished || (si/=2));

    {
      unsigned short int di = param;
      unsigned short int cx = 0;
      unsigned short int dx = 0;
      unsigned short int bx = 0;
      unsigned short int ax = 0;

      di--;
      di<<=2;

      do
      {
        cx += dx;

        if(bx != outputPtr[di+3]) // does dx need an update ?
        {
          bx = outputPtr[di+3];

          dx = 1 << (16 - bx);
        }

        {
          int i;
          unsigned short int si = cx;

          for(i=0;i<16;i++) // invert all bits
          {
            ax = (ax<<1) | (si&1);
            si>>=1;
          }
        }

        *(unsigned short int*)(&outputPtr[di]) = ax;

        di -= 4;
      }while(!(di&0x8000));

      inputPtr[2] = 0;
      inputPtr[1] = 0;
      inputPtr[0] = 0;

      unV17 = 1;
      {
        unsigned short int di =0;
        unsigned short int dx = unv13;
        unsigned char cl;
        unsigned short int ax;
        unsigned short int si;

        do
        {
          cl = outputPtr[di+3];
          unV7 = 0;
          unV8 = 0;

          ax = *(unsigned short int*)(&outputPtr[di]);

          do
          {
            int mod;
            si = unV7 * 3;
            unV14 = ax & 0xFF;

            mod = ax%2;
            ax /= 2;

            if(mod)
            {
              if(inputPtr[si] == 0)
              {
                inputPtr[si] = unV17 - unV7;

                unV8 = unV7;
                unV7 = unV17;
                unV17++;

                inputPtr[(unV7*3) + 2] = 0;
                inputPtr[(unV7*3) + 1] = 0;
                inputPtr[(unV7*3) + 0] = 0;
              }
              else
              {
                unV8 = unV7;
                unV7 += inputPtr[si];
              }
            }
            else
            {
              if(inputPtr[si+1] == 0)
              {
                inputPtr[si+1] = unV17 - unV7;

                unV8 = unV7;
                unV7 = unV17;
                unV17++;

                inputPtr[(unV7*3) + 2] = 0;
                inputPtr[(unV7*3) + 1] = 0;
                inputPtr[(unV7*3) + 0] = 0;
              }
              else
              {
                unV8 = unV7;
                unV7 += inputPtr[si+1];
              }
            }
          }while(--cl);

          si = unV8 * 3;

          if(unV14 & 1)
          {
            inputPtr[si+2] |= 1;
            inputPtr[si] = outputPtr[di+2];
          }
          else
          {
            inputPtr[si+2] |= 2;
            inputPtr[si+1] = outputPtr[di+2];
          }

          di += 4;
        }while(!((--dx)&0x8000));

        *(outputInputPtr) = inputPtr + (unV17*3);
      }
    }
  }

  si = backupSource;
  return inputPtr;
}

void unpack(short int param, char* source, char* dest, int uncompressedSize, char* ptr)
{
  unsigned char direction = 1;
  char* inputPtrTemp;

  si = source;

  inputPtr = ptr;
  outputPtr = dest;

  unV4 = 2;
  numBitInMask = 6;
  mask = 0x3F;

  compressParam = param;

  if(param&2)
  {
    numBitInMask = 7;
    mask = 0x7F;
  }

  if(param&4)
  {
    char* tempPtr;

    unV4++;
    unV3 = unpackSub0(0x100,&inputPtrTemp);
    inputPtr = inputPtrTemp;
  }

  unV2 = unpackSub0(0x40,&inputPtrTemp);
  inputPtr = inputPtrTemp;

  unV1 = unpackSub0(0x40,&inputPtrTemp);
  inputPtr = inputPtrTemp;

  decompPos = 0;

  
}
