#include "common.h"

short int numObjInInventoryTable[2];
short int inHandTable[2];
short int currentInHand = 0;

#define NUM_MAX_SEQUENCE_PARAM 30

int numSequenceParam = 0;

struct sequenceParamStruct
{
  unsigned int frame;
  unsigned int sample;
};

typedef struct sequenceParamStruct sequenceParamStruct;

sequenceParamStruct sequenceParams[NUM_MAX_SEQUENCE_PARAM];

void resetRotateParam(void )
{
  currentProcessedActorPtr->rotate.param = 0;
}

void convertPaletteIfRequired(unsigned char* lpalette)
{
  if(gameId >= JACK && gameId < AITD3)
  {
    int i;
    unsigned char* ptr2 = lpalette;
    for(i=0;i<256;i++)
    {
      int j;
      for(j=0;j<3;j++)
      {
        unsigned int composante = *(ptr2);
        composante*=255;
        composante/=63;
        *(ptr2++) = composante&0xFF;
      }
    }
  }
}

char* sequenceListAITD2[]=
{
  "BATL",
  "GRAP",
  "CLE1",
  "CLE2",
  "COOK",
  "EXPL",
  "FALA",
  "FAL2",
  "GLIS",
  "GREN",
  "JEND",
  "MANI",
  "MER",
  "TORD",
  "PANT",
  "VERE",
  "PL21",
  "PL22",
  "ENDX",
  "SORT",
  "EFER",
  "STAR",
  "MEDU",
  "PROL",
  "GRAS",
  "STRI",
  "ITRO",
  "BILL",
  "PIRA",
  "PIR2",
  "VENT",
  "FIN",
  "LAST"
};

void unapckSequenceFrame(unsigned char* source,unsigned char* dest)
{
  unsigned char byteCode;

  byteCode = *(source++);

  while(byteCode)
  {
    if(!(--byteCode)) // change pixel or skip pixel
    {
      unsigned char changeColor;

      changeColor = *(source++);

      if(changeColor)
      {
        *(dest++) = changeColor;
      }
      else
      {
        dest++;
      }
    }
    else
    if(!(--byteCode)) // change 2 pixels or skip 2 pixels
    {
      unsigned char changeColor;

      changeColor = *(source++);

      if(changeColor)
      {
        *(dest++) = changeColor;
        *(dest++) = changeColor;
      }
      else
      {
        dest+=2;
      }
    }
    else
    if(!(--byteCode)) // fill or skip
    {
      unsigned char size;
      unsigned char fillColor;

      size = *(source++);
      fillColor = *(source++);

      if(fillColor)
      {
        int i;

        for(i=0;i<size;i++)
        {
          *(dest++) = fillColor;
        }
      }
      else
      {
        dest+=size;
      }
    }
    else // large fill of skip
    {
      unsigned short int size;
      unsigned char fillColor;

      size = *(unsigned short int*)source;
      source+=2;
      fillColor = *(source++);

      if(fillColor)
      {
        int i;

        for(i=0;i<size;i++)
        {
          *(dest++) = fillColor;
        }
      }
      else
      {
        dest+=size;
      }
    }

    byteCode = *(source++);
  }
}

void playSequence(int sequenceIdx, int fadeStart, int fadeOutVar)
{
#define SPEED 70              /* Ticks per Frame */
#define SLEEP_MIN 20          /* Minimum time a sleep takes, usually 2*GRAN */
#define SLEEP_GRAN 1         /* Granularity of sleep */

int frames=0;                   /* Number of frames displayed */
s32 t_start,t_left;
u32 t_end;
s32 q=0; 

  int var_4 = 1;
  int var_6 = 0;
  int var_8 = 1;
  unsigned char localPalette[0x300];

  while(!var_6)
  {
    int si = 0;
    int sequenceParamIdx;

    while(si < var_8)
    {
      char buffer[256];
      frames++;
      t_start=SDL_GetTicks();

      timeGlobal++;

      timer = timeGlobal;

      if(gameId == AITD2)
      {
        strcpy(buffer,sequenceListAITD2[sequenceIdx]);
      }
      if(gameId == AITD3)
      {
        sprintf(buffer,"AN%d",sequenceIdx);
      }

      if(!loadPakToPtr(buffer,si,screen))
      {
        theEnd(0,buffer);
      }

      if(!si) // first frame
      {
        memcpy(localPalette,screen,0x300); // copy palette
        memcpy(aux,screen+0x300,64000);
        var_8 = *(unsigned short int*)(screen+64768);

        if(gameId < AITD3)
          convertPaletteIfRequired(localPalette);

        if(var_4 != 0)
        {
    /*      if(fadeStart & 1)
          {
            fadeOut(0x10,0);
          }
          if(fadeStart & 4)
          {
             //memset(palette,0,0); // hu ?
            fadeInSub1(localPalette);
            flipOtherPalette(palette);
          } */

          osystem_setPalette(localPalette);
          copyPalette(localPalette,palette);
        }
      }
      else // not first frame
      {
        unsigned long int frameSize;

        frameSize = *(unsigned long int*)screen;

        if(frameSize < 64000) // key frame
        {
          unapckSequenceFrame(screen+4,aux);
        }
        else // delta frame
        {
          copyToScreen(screen,aux);
        }
      }

      for(sequenceParamIdx = 0; sequenceParamIdx < numSequenceParam; sequenceParamIdx++)
      {
        if(sequenceParams[sequenceParamIdx].frame == si)
        {
          playSound(sequenceParams[sequenceParamIdx].sample);
        }
      }

      // TODO: here, timming management
      // TODO: fade management

      osystem_CopyBlockPhys(aux,0,0,320,200);
      osystem_startFrame();
      flipScreen();

      si++;

      process_events();
      readKeyboard();

      t_end=t_start+SPEED;
      t_left=t_start-SDL_GetTicks()+SPEED;

      if(t_left>0){
          if(t_left>SLEEP_MIN)
              SDL_Delay(t_left-SLEEP_GRAN);
          while(SDL_GetTicks()<t_end){ q++; };
      }
    }

    fadeOutVar--;

    if(fadeOutVar==0)
    {
      var_6=1;
    }
  }
}

void processLife2(int lifeNum)
{
  int exitLife = 0;
  //int switchVal = 0;
  int var_6;
  int switchVal = 0;

  currentLifeActorIdx = currentProcessedActorIdx;
  currentLifeActorPtr = currentProcessedActorPtr;
  currentLifeNum = lifeNum;

  currentLifePtr = HQR_Get(listLife,lifeNum);

  while(!exitLife)
  {
    int lifeTempVar1;
    int lifeTempVar2;
    int lifeTempVar3;
    int lifeTempVar4;
    int lifeTempVar5;
    int lifeTempVar6;
    int lifeTempVar7;
    short int currentOpcode;

    var_6 = -1;

    currentOpcode = *(short int*)(currentLifePtr);
    currentLifePtr+=2;

    //printf("%d:opcode: %04X\n",lifeNum, currentOpcode);

    if(currentOpcode & 0x8000)
    {
        var_6 = *(short int*)(currentLifePtr);
        currentLifePtr+=2;

        if(var_6==-1)
        {
          printf("Unsupported newVar = -1\n");
          exit(1);
        }
        else
        {
          currentProcessedActorIdx = objectTable[var_6].ownerIdx;

          if(currentProcessedActorIdx != -1)
          {
            currentProcessedActorPtr = &actorTable[currentProcessedActorIdx];

            goto processOpcode;
          }
          else
          {
            switch(currentOpcode & 0x7FFF)
            {
            case 0x2: // anim
              {
                objectTable[var_6].field_26 = *(short int*)(currentLifePtr);
                currentLifePtr+=2;
                objectTable[var_6].field_2C = *(short int*)(currentLifePtr);
                currentLifePtr+=2;
                objectTable[var_6].field_2A  = 0;
                objectTable[var_6].field_28  = 0;
                break;
              }
            case 0x3:
              {
                objectTable[var_6].field_2 = evalVar();
                break;
              }
            case 0xD: // anim2
              {
                objectTable[var_6].field_26 = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].field_2C = -1;
                objectTable[var_6].field_2A  = 0;
                objectTable[var_6].field_28  = 0;
                break;
              }
            case 0xF: // move
              {
                objectTable[var_6].trackMode = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].trackNumber= *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].positionInTrack = 0;

                objectTable[var_6].mark = -1;
                break;
              }
            case 0x1A: // occure in original interpreter too
              {
                break;
              }
            case 0x1D: // life
              {
                objectTable[var_6].life = *(short int*)(currentLifePtr);
                currentLifePtr+=2;
                break;
              }
            case 0x2D: // stage
              {
                objectTable[var_6].stage = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].room = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].x = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].y = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                objectTable[var_6].z = *(short int*)(currentLifePtr);
                currentLifePtr+=2;

                //objModifFlag1 = 1;

                break;
              }
            case 0x35:
              {
                objectTable[var_6].foundBody = *(short int*)(currentLifePtr);
                currentLifePtr+=2;
                break;
              }
            case 0x43: // ANGLE
              {
                objectTable[var_6].alpha = *(short int*)currentLifePtr;
                currentLifePtr+=2;
                objectTable[var_6].beta = *(short int*)currentLifePtr;
                currentLifePtr+=2;
                objectTable[var_6].gamma = *(short int*)currentLifePtr;
                currentLifePtr+=2;

                break;
              }
            default:
              {
                printf("Unsupported opcode %X when actor isn't in floor\n",currentOpcode & 0x7FFF);
                exit(1);
                break;
              }
            }
          }
        }
    }
    else
    {
processOpcode:
      switch(currentOpcode & 0x7FFF)
      {
      case 0x0: // DO_MOVE
        {
          processTrack2();
          break;
        }
      case 0x1: // ANIM
        {
          lifeTempVar1 = *(short int*)currentLifePtr;
          currentLifePtr +=2;
          lifeTempVar2 = *(short int*)currentLifePtr;
          currentLifePtr +=2;

          if(lifeTempVar1==-1)
          {
            currentProcessedActorPtr->ANIM = -1;
            currentProcessedActorPtr->field_44 = -2;
          }
          else
          {
            anim(lifeTempVar1,0,lifeTempVar2);
          }

          break;
        }
      case 0x2: // ANIM_ALL
        {
          lifeTempVar1 = *(short int*)currentLifePtr;
          currentLifePtr +=2;
          lifeTempVar2 = *(short int*)currentLifePtr;
          currentLifePtr +=2;

          anim(lifeTempVar1, 2, lifeTempVar2);

          break;
        }
      case 0x3: // BODY
        {
          lifeTempVar1 = evalVar2();

          objectTable[currentProcessedActorPtr->field_0].field_2 = lifeTempVar1;

          if(currentProcessedActorPtr->bodyNum != lifeTempVar1)
          {
            currentProcessedActorPtr->bodyNum = lifeTempVar1;

            if(currentProcessedActorPtr->flags & 1)
            {
              if(currentProcessedActorPtr->ANIM != -1 && currentProcessedActorPtr->bodyNum != -1 )
              {
                processAnim(currentProcessedActorPtr->FRAME, HQR_Get(listAnim,currentProcessedActorPtr->ANIM),HQR_Get(listBody,currentProcessedActorPtr->bodyNum));
              }
            }
            else
            {
              mainVar1 = 1;
            }
          }
          break;
        }
      case 0x4: // IF_DIF
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 == lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0x5: // IF_EQU
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 != lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0x6: // IF_INF
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 >= lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0x7: // IF_INFEQU
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 > lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0x8: // IF_SUP
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 <= lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0x9: // IF_SUPEQU
        {
          lifeTempVar1 = evalVar2();
          lifeTempVar2 = evalVar2();

          if(lifeTempVar1 < lifeTempVar2)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }

          break;
        }
      case 0xA: // GOTO
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr += lifeTempVar1*2;
          currentLifePtr += 2;  
          break;
        }
      case 0xB: // RETURN
        {
          exitLife = 1;
          break;
        }
      case 0xC: // END
        {
          exitLife = 1;
          break;
        }
      case 0xD: // anim2
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          anim(lifeTempVar1, 1, -1);

          break;
        }
      case 0xE:
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar3 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar4 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar5 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar6 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar7 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          
          animMove(lifeTempVar1,lifeTempVar2,lifeTempVar3,lifeTempVar4,lifeTempVar5,lifeTempVar6,lifeTempVar7);

          break;
        }
      case 0xF: // MOVE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          setMoveMode(lifeTempVar1,lifeTempVar2);

          break;
        }
      case 0x10: // HIT
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar3 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar4 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar5 = evalVar();
          lifeTempVar6 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          hit(lifeTempVar1,lifeTempVar2,lifeTempVar3,lifeTempVar4,lifeTempVar5,lifeTempVar6);

          break;
        }
      case 0x11: // MESSAGE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          makeMessage(lifeTempVar1);

          break;
        }
      case 0x12: // VAR
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          vars[lifeTempVar1] = evalVar2();
          break;
        }
      case 0x13: // INC_VAR
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          vars[lifeTempVar1]++;
          break;
        }
      case 0x14: // DEC_VAR
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          vars[lifeTempVar1]--;
          break;
        }
      case 0x15: // ADD_VAR
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          vars[lifeTempVar1]+=evalVar2();
          break;
        }
      case 0x16: // SUB_VAR
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          vars[lifeTempVar1]-=evalVar2();
          break;
        }
      case 0x17: // LIFE_MODE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          lifeTempVar2 = currentProcessedActorPtr->lifeMode;

          if(lifeTempVar1 != lifeTempVar2)
          {
            currentProcessedActorPtr->lifeMode = lifeTempVar1;
//            objModifFlag1 = 1;
          }
          break;
        }
      case 0x18: // SWITCH
        {
          switchVal = evalVar2();
          break;
        }
      case 0x19: // CASE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(lifeTempVar1 == switchVal)
          {
            currentLifePtr+=2;
          }
          else
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }
    
          break;
        }
      case 0x1A: // START_CHRONO
        {
          startChrono(&currentProcessedActorPtr->CHRONO);
          break;
        }
      case 0x1B: // MULTI_CASE
        {
          int i;
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          lifeTempVar2 = 0;

          for(i=0;i<lifeTempVar1;i++)
          {
            if(*(short int*)(currentLifePtr) == switchVal)
            {
              lifeTempVar2 = 1;
            }
            currentLifePtr+=2;
          }

          if(!lifeTempVar2)
          {
            lifeTempVar2 = *(short int*)(currentLifePtr);
            currentLifePtr += lifeTempVar2*2;
            currentLifePtr += 2;
          }
          else
          {
            currentLifePtr+=2;
          }
          break;
        }
      case 0x1D: // LIFE
        {
          currentProcessedActorPtr->life = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          break;
        }
      case 0x1E: // DELETE
        {
          int objectIdx = evalVar2();

          deleteObject(objectIdx);

          if(objectTable[objectIdx].foundBody != -1)
          {
            objectTable[objectIdx].flags2 |= 0x4000;
          }
          break;
        }
      case 0x1F: // TAKE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          take(lifeTempVar1);

          break;
        }
      case 0x20:
        {
          if(gameId == JACK)
          {
            inHandTable[currentInHand] = *(short int*)(currentLifePtr);
            currentLifePtr+=2;
          }
          else
          {
            inHandTable[currentInHand] = evalVar2();
          }
          break;
        }
      case 0x21: //READ
        {
          currentLifePtr+=2;
          currentLifePtr+=2;
          break;
        }
      case 0x22:
        {
          lifeTempVar1 = evalVar();

          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar3 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(currentProcessedActorPtr->END_FRAME != 0)
          {
            if(currentProcessedActorPtr->ANIM == lifeTempVar2)
            {
              if(currentProcessedActorPtr->FRAME == lifeTempVar3)
              {
                playSound(lifeTempVar1);
              }
            }
          }
          break;
        }
      case 0x23: // SPECIAL
        {
          lifeTempVar1 = *(short int*)(currentLifePtr) & 0x1D1;
          currentLifePtr+=2;

          switch(lifeTempVar1)
          {
          case 0:
            {
              createFlow( 0,
                    currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                    currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY,
                    currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                    currentProcessedActorPtr->stage,
                    currentProcessedActorPtr->room,
                    currentProcessedActorPtr->alpha,
                    currentProcessedActorPtr->beta,
                    currentProcessedActorPtr->gamma,
                    &currentProcessedActorPtr->zv );
              break;
            }
          case 1:
            {
              currentProcessedActorPtr = &actorTable[currentProcessedActorPtr->HIT_BY];

              createFlow( 1,
                    currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX + currentProcessedActorPtr->hotPoint.x,
                    currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY + currentProcessedActorPtr->hotPoint.y,
                    currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ + currentProcessedActorPtr->hotPoint.z,
                    currentProcessedActorPtr->stage,
                    currentProcessedActorPtr->room,
                    0,
                    -currentProcessedActorPtr->beta,
                    0,
                    NULL );
              
              currentProcessedActorPtr = currentLifeActorPtr;
                    

              break;
            }
          case 4:
            {
              createFlow( 4,
                    currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                    currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY,
                    currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                    currentProcessedActorPtr->stage,
                    currentProcessedActorPtr->room,
                    currentProcessedActorPtr->alpha,
                    currentProcessedActorPtr->beta,
                    currentProcessedActorPtr->gamma,
                    &currentProcessedActorPtr->zv );
              break;
            }
          }
          break;
        }
      case 0x24:
        {
          doRealZv(currentProcessedActorPtr);
          break;
        }
      case 0x25:
        {
          if(gameId == JACK)
          {
            lifeTempVar1 = evalVar();
          }
          else
          {
            lifeTempVar1 = *(short int*)(currentLifePtr);
            currentLifePtr+=2;
          }

          playSound(lifeTempVar1);
          break;
        }
      case 0x26: // TYPE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr) & 0x1D1;
          currentLifePtr+=2;

          lifeTempVar2 = currentProcessedActorPtr->flags;

          currentProcessedActorPtr->flags = (currentProcessedActorPtr->flags & 0xFE2E) + lifeTempVar1;

          if(lifeTempVar2 & 1)
          {
            if(!(lifeTempVar1 & 1))
            {
              stopAnim(currentProcessedActorIdx);
            }
          }

          if(lifeTempVar1 & 1)
          {
            if(!(lifeTempVar2 & 8))
            {
              deleteSub(currentProcessedActorIdx);
            }
          }

          break;
        }
      case 0x27: // GAME_OVER
        {
          fadeMusic(0,0,0); // TODO: fix

          // TODO: implement music fadeout

          giveUp = 1;
          break;
        }
      case 0x28: // MANUAL_ROT
        {
          manualRot(90);
          break;
        }
      case 0x29: // TODO
        {
          currentLifePtr+=2;
          break;
        }
      case 0x2A:
        {
          int newMusicIdx = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          playMusic(newMusicIdx);
          break;
        }
      case 0x2B: // SET_BETA
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(currentProcessedActorPtr->beta != lifeTempVar1)
          {
            if(currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->rotate.newAngle != lifeTempVar1)
            {
              startActorRotation(currentProcessedActorPtr->beta, lifeTempVar1, lifeTempVar2, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
          }

          break;
        }
      case 0x2C: // DO_ROT_ZV
        {
          getZvRot(HQR_Get(listBody, currentProcessedActorPtr->bodyNum),&currentProcessedActorPtr->zv,
                            currentProcessedActorPtr->alpha,
                            currentProcessedActorPtr->beta,
                            currentProcessedActorPtr->gamma);
          
          currentProcessedActorPtr->zv.ZVX1 += currentProcessedActorPtr->roomX;
          currentProcessedActorPtr->zv.ZVX2 += currentProcessedActorPtr->roomX;
          currentProcessedActorPtr->zv.ZVY1 += currentProcessedActorPtr->roomY;
          currentProcessedActorPtr->zv.ZVY2 += currentProcessedActorPtr->roomY;
          currentProcessedActorPtr->zv.ZVZ1 += currentProcessedActorPtr->roomZ;
          currentProcessedActorPtr->zv.ZVZ2 += currentProcessedActorPtr->roomZ;

          break;
        }
      case 0x2D: // STAGE
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar3 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar4 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar5 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          setStage(lifeTempVar1,lifeTempVar2,lifeTempVar3,lifeTempVar4,lifeTempVar5);

          break;
        }
      case 0x31: // CAMERA_TARGET
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(lifeTempVar1 != currentCameraTarget) // same stage
          {
            lifeTempVar2 = objectTable[lifeTempVar1].ownerIdx;

            if(lifeTempVar2 != -1)
            {
              currentCameraTarget = lifeTempVar1;
              genVar9 = lifeTempVar2;

              lifeTempVar3 = actorTable[genVar9].room;

              if(lifeTempVar3 != currentDisplayedRoom)
              {
                needChangeRoom = 1;
                newRoom = lifeTempVar3;
              }
            }
            else // different stage
            {
              if(objectTable[lifeTempVar1].stage != currentEtage)
              {
                changeFloor = 1;
                newFloor = objectTable[lifeTempVar1].stage;
                newRoom = objectTable[lifeTempVar1].room;
              }
              else
              {
                if(currentDisplayedRoom!=objectTable[lifeTempVar1].room)
                {
                  needChangeRoom = 1;
                  newRoom = objectTable[lifeTempVar1].room;
                }
              }
            }
          }
          break;
        }
      case 0x33: // FIRE
        {
          evalVar();
          currentLifePtr+=2;
          currentLifePtr+=2;
          currentLifePtr+=2;
          currentLifePtr+=2;
          currentLifePtr+=2;
          evalVar();

          //TODO: finish
          
          break;
        }
      case 0x34: // TEST_COL
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(lifeTempVar1)
          {
            currentProcessedActorPtr->dynFlags |= 1;
          }
          else
          {
            currentProcessedActorPtr->dynFlags &= 0xFFFE;
          }

          break;
        }
      case 0x3A: // DO_CARRE_ZV
        {
          getZvCube(HQR_Get(listBody,currentProcessedActorPtr->bodyNum),&currentProcessedActorPtr->zv);

          currentProcessedActorPtr->zv.ZVX1 += currentProcessedActorPtr->roomX;
          currentProcessedActorPtr->zv.ZVX2 += currentProcessedActorPtr->roomX;
          currentProcessedActorPtr->zv.ZVY1 += currentProcessedActorPtr->roomY;
          currentProcessedActorPtr->zv.ZVY2 += currentProcessedActorPtr->roomY;
          currentProcessedActorPtr->zv.ZVZ1 += currentProcessedActorPtr->roomZ;
          currentProcessedActorPtr->zv.ZVZ2 += currentProcessedActorPtr->roomZ;

          break;
        }
      case 0x3B:
        {
          int newSample;

          if(gameId == JACK)
          {
            newSample = evalVar();
            genVar7 = evalVar();
          }
          else
          {
            newSample = *(short int*)currentLifePtr;
            currentLifePtr+=2;

            genVar7 = *(short int*)currentLifePtr;
            currentLifePtr+=2;
          }

          playSound(newSample);
          break;
        }
      case 0x3C: // LIGHT
        {
          lifeTempVar1 = 2-((*(short int*)(currentLifePtr))<<1);
          currentLifePtr+=2;

          if(!defines.lightVar)
          {
            if(lightVar1 != lifeTempVar1)
            {
              lightVar1 = lifeTempVar1;
              lightVar2 = 1;
            }
          }

          break;
        }
      case 0x3D: // SHAKING
        {
          printf("Shaking\n");
          //shakingState = shakingAmplitude = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

/*          if(shakingState==0)
          {
            stopShaking();
          } */
          break;
        }
      case 0x3E: // pluie
        {
          // TODO!
          currentLifePtr+=2;
          break;
        }
      case 0x40: // PUT_AT
        {
          int objIdx1;
          int objIdx2;

          objIdx1 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          objIdx2 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          putAt(objIdx1,objIdx2);
          break;
        }
      case 0x41: // DEF_ZV
        {
          currentProcessedActorPtr->zv.ZVX1 = currentProcessedActorPtr->roomX + *(short int*)currentLifePtr + currentProcessedActorPtr->modX;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVX2 = currentProcessedActorPtr->roomX + *(short int*)currentLifePtr + currentProcessedActorPtr->modX;
          currentLifePtr+=2;

          currentProcessedActorPtr->zv.ZVY1 = currentProcessedActorPtr->roomY + *(short int*)currentLifePtr + currentProcessedActorPtr->modY;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVY2 = currentProcessedActorPtr->roomY + *(short int*)currentLifePtr + currentProcessedActorPtr->modY;
          currentLifePtr+=2;

          currentProcessedActorPtr->zv.ZVZ1 = currentProcessedActorPtr->roomZ + *(short int*)currentLifePtr + currentProcessedActorPtr->modZ;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVZ2 = currentProcessedActorPtr->roomZ + *(short int*)currentLifePtr + currentProcessedActorPtr->modZ;
          currentLifePtr+=2;

          break;
        }
      case 0x42: // HIT_OBJECT
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          currentProcessedActorPtr->animActionType = 8;
          currentProcessedActorPtr->animActionParam = lifeTempVar1;
          currentProcessedActorPtr->hitForce = lifeTempVar2;
          currentProcessedActorPtr->field_98 = -1;
          break;
        }
      case 0x43: // ANGLE
        {
          currentProcessedActorPtr->alpha = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          currentProcessedActorPtr->beta = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          currentProcessedActorPtr->gamma = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          break;
        }
      case 0x44:
        {
          int param1;
          int param2;

          param1 = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          param2 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          playSound(param1);

          break;
        }
      case 0x45: // throw
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar3 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar4 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar5 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar6 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;
          lifeTempVar7 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          throwObj(lifeTempVar1,lifeTempVar2,lifeTempVar3,lifeTempVar4,lifeTempVar5,lifeTempVar6,lifeTempVar7);

          break;
        }
      case 0x47: // tatou screen
        {
          unsigned int chrono;
          unsigned char lpalette[0x300];
          short int ressourceIdx;

          ressourceIdx = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          loadPakToPtr("ITD_RESS",ressourceIdx,aux);

          copyPalette(aux+64000,lpalette);
          
          if(gameId < AITD3)
          convertPaletteIfRequired(lpalette);

          fadeOut(0x10,0);
          fadeIn(lpalette);

          osystem_setPalette(lpalette);
          copyPalette(lpalette,palette);

          copyToScreen(aux,screen);

          startChrono(&chrono);

          do
          {
            unsigned int time;
            process_events();
            readKeyboard();

            osystem_CopyBlockPhys(screen,0,0,320,200);
            osystem_startFrame();
            flipScreen();
            
            timeGlobal++;
            timer = timeGlobal;

            time = evalChrono(&chrono);

            if(time>100)
              break;
          }while(!input2 && !input1);

          unfreezeTime();

          currentLifePtr+=4;

          break;
        }
      case 0x48:
        {
          //todo!
          break;
        }
      case 0x49: // next music
        {
          int lNewMusic = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(currentMusic!=-1)
          {
            nextMusic = lNewMusic;
          }
          else
          {
            playMusic(lNewMusic);
          }

          break;
        }
      case 0x4A: // next music and fade
        {
          int lNewMusic = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(currentMusic!=-1)
          {
            fadeMusic(0,0,0x8000);    // fade out music
            startChrono(&musicChrono); // fade out music timer
            currentMusic = -2;         // waiting next music
            nextMusic = lNewMusic;    // next music to play
          }
          else
          {
            playMusic(lNewMusic);
          }

          break;
        }
      case 0x50:
        {
          objectTable[currentProcessedActorPtr->field_0].field_24 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          break;
        }
      case 0x51:
        {
          char* ptr;

          ptr = HQR_Get(listTrack,currentProcessedActorPtr->trackNumber);

          ptr += currentProcessedActorPtr->positionInTrack *2;

          if(*(short int*)ptr == 5)
          {
            currentProcessedActorPtr->positionInTrack++;
          }
          break;
        }
      case 0x52:
        {
          int param1;
          int param2;

          param1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          param2 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          if(param1==-1)
          {
            currentProcessedActorPtr->ANIM = -1;
            currentProcessedActorPtr->field_44 = -2;
          }
          else
          {
            anim(param1,4,param2);
          }

          break;
        }
      case 0x53:
        {
          resetRotateParam();
          break;
        }
      case 0x54:
        {
          // TODO
          break;
        }
      case 0x55:
        {
          // TODO
          currentLifePtr+=4;
          break;
        }
      case 0x57:
        {
          lifeTempVar1 = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          *(((short int*)(&defines))+lifeTempVar1) = evalVar2();
          break;
        }
      case 0x59:
        {
          int param1;
          int param2;

          param1 = evalVar();
          param2 = evalVar();

          objectTable[currentProcessedActorPtr->field_0].field_2 = param1;
          objectTable[currentProcessedActorPtr->field_0].field_26 = param2;

          currentProcessedActorPtr->bodyNum = param1;

          if(currentProcessedActorPtr->flags&1)
          {
            initAnimInBody(0, HQR_Get(listAnim, currentProcessedActorPtr->ANIM), HQR_Get(listBody,currentProcessedActorPtr->bodyNum));
            anim(param2,4,-1);
          }
          else
          {
            mainVar1 = 1;
          }
          break;
        }
      case 0x5B:
        {
          int inventoryIndex = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

/*          if(indeventoyIndex != currentInHand)
          {
            if(currentInHand<2)
            {
              int i;

              for(i=0;i<inventory[currentInHand];i++)
              {
                objectTable[inventoryTable[currentInHand][i]].flags2&=0x7FFF;
              }

              currentInHand = inventoryIndex
            }
          }*/

          break;
        }
      case 0x5C: // sequence
        {
          unsigned short int sequenceIdx;
          unsigned short int fadeEntry;
          unsigned short int fadeOut;

          sequenceIdx = *(unsigned short int*)(currentLifePtr);
          currentLifePtr+=2;

          fadeEntry = *(unsigned short int*)(currentLifePtr);
          currentLifePtr+=2;

          fadeOut = *(unsigned short int*)(currentLifePtr);
          currentLifePtr+=2;

          playSequence(sequenceIdx,fadeEntry,fadeOut);

          break;
        }
      case 0x5E:
        {
          // todo!
          currentLifePtr+=2;
          break;
        }
      case 0x5F: // protection opcode
        {
          //protection = 1;
          break;
        }
      case 0x60:
        {
          currentProcessedActorPtr->zv.ZVX1 = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVX2 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          currentProcessedActorPtr->zv.ZVY1 = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVY2 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          currentProcessedActorPtr->zv.ZVZ1 = *(short int*)currentLifePtr;
          currentLifePtr+=2;
          currentProcessedActorPtr->zv.ZVZ2 = *(short int*)currentLifePtr;
          currentLifePtr+=2;

          break;
        }
      case 0x61:
        {
          unsigned short int numParams;
          int i;

          numParams = *(short int*)(currentLifePtr);
          currentLifePtr+=2;

          ASSERT(numParams<=NUM_MAX_SEQUENCE_PARAM);
          
          for(i=0;i<numParams;i++)
          {
            sequenceParams[i].frame = READ_LE_U16(currentLifePtr);
            currentLifePtr+=2;
            sequenceParams[i].sample = READ_LE_U16(currentLifePtr);
            currentLifePtr+=2;
          }

          numSequenceParam = numParams;
          break;
        }
      case 0x62: // TODO
        {
          currentLifePtr+=16;
          break;
        }
      case 0x63: // TODO AITD3 only
        {
          evalVar();
          currentLifePtr+=12;
          evalVar();
          break;
        }
      default:
        {
          printf("Unknown opcode %X in processLife\n",currentOpcode & 0x7FFF);
          exit(1);
        }
      }
    }

    if(var_6 != -1)
    {
      currentProcessedActorIdx = currentLifeActorIdx;
      currentProcessedActorPtr = currentLifeActorPtr;
    }

  }

  currentLifeNum = -1;
}
