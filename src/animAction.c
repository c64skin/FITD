#include "common.h"

void processAnimAction(void)
{
  switch(currentProcessedActorPtr->animActionType)
  {
  case 1: // PRE_HIT
    {
      if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
      {
        currentProcessedActorPtr->animActionType = 10;
      }
  
      if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
      {
        if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
        {
          currentProcessedActorPtr->animActionType = 2;
        }
      }
      else
      {
        currentProcessedActorPtr->animActionType = 0;
        return;
      }
      break;
    }
  case 2: // HIT
    {
      int x;
      int y;
      int z;
      int range;
      int collision;
      int i;
      ZVStruct rangeZv;
      
      if(currentProcessedActorPtr->ANIM != currentProcessedActorPtr->animActionANIM)
      {
        currentProcessedActorPtr->animActionType = 0;
      }

      x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x + currentProcessedActorPtr->modX;
      y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y + currentProcessedActorPtr->modY;
      z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z + currentProcessedActorPtr->modZ;

      range = currentProcessedActorPtr->animActionParam;

      rangeZv.ZVX1 = x - range;
      rangeZv.ZVX2 = x + range;
      rangeZv.ZVY1 = y - range;
      rangeZv.ZVY2 = y + range;
      rangeZv.ZVZ1 = z - range;
      rangeZv.ZVZ2 = z + range;

      //drawProjectedBox(rangeZv.ZVX1,rangeZv.ZVX2,rangeZv.ZVY1,rangeZv.ZVY2,rangeZv.ZVZ1,rangeZv.ZVZ2,60,255);

      collision = processActor1Sub1(currentProcessedActorIdx,&rangeZv);

      for(i=0;i<collision;i++)
      {
        actorStruct* actorPtr2;
        
        currentProcessedActorPtr->HIT = currentProcessedActorPtr->COL[i];
        actorPtr2 = &actorTable[currentProcessedActorPtr->COL[i]];

        actorPtr2->HIT_BY = currentProcessedActorIdx;
        actorPtr2->hitForce = currentProcessedActorPtr->hitForce;

        if(actorPtr2->flags & 1)
        {
          currentProcessedActorPtr->animActionType = 0;
          return;
        }
      }
      break;
    }
  case 4: // PRE_FIRE
    {
      if(currentProcessedActorPtr->ANIM != currentProcessedActorPtr->animActionANIM)
        return;

      if(currentProcessedActorPtr->FRAME != currentProcessedActorPtr->animActionFRAME)
        return;

      currentProcessedActorPtr->animActionType = 5;
  
      break;
    }
  case 5: // FIRE
    {
      int touchedActor;
      
      createFlow( 3,
            currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x,
            currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y,
            currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z,
            currentProcessedActorPtr->stage,
            currentProcessedActorPtr->room,
            0,
            currentProcessedActorPtr->beta,
            0,
            NULL);

      touchedActor = checkLineProjectionWithActors(
                        currentProcessedActorIdx,
                        currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x,
                        currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y,
                        currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z,
                        currentProcessedActorPtr->beta - 0x100,
                        currentProcessedActorPtr->room,
                        currentProcessedActorPtr->animActionParam );

      if(touchedActor == -1) // no one has been touched
      {
        createFlow( 2, animMoveX, animMoveY, animMoveZ, currentProcessedActorPtr->stage, currentProcessedActorPtr->room, 0, -currentProcessedActorPtr->beta, 0, NULL );

        currentProcessedActorPtr->animActionType = 0;
      }
      else
      {
        createFlow( 2, animMoveX, animMoveY, animMoveZ, currentProcessedActorPtr->stage, currentProcessedActorPtr->room, 0, -currentProcessedActorPtr->beta, 0, NULL );

        currentProcessedActorPtr->hotPoint.x = animMoveX - currentProcessedActorPtr->roomX;
        currentProcessedActorPtr->hotPoint.y = animMoveY - currentProcessedActorPtr->roomY;
        currentProcessedActorPtr->hotPoint.z = animMoveZ - currentProcessedActorPtr->roomZ;

        currentProcessedActorPtr->HIT = touchedActor;

        actorTable[touchedActor].HIT_BY = currentProcessedActorIdx;
        actorTable[touchedActor].hitForce = currentProcessedActorPtr->hitForce;

        currentProcessedActorPtr->animActionType = 0;
      }
      break;
    }
  case 6: // PRE_THROW
    {
      if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
      {
        int objIdx = currentProcessedActorPtr->animActionParam;

        objectStruct* objPtr = &objectTable[objIdx];

        int x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x + currentProcessedActorPtr->modX;
        int y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y + currentProcessedActorPtr->modY;
        int z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z + currentProcessedActorPtr->modZ;

        ZVStruct rangeZv;

        getZvNormal(HQR_Get(listBody, objPtr->body),&rangeZv);

        rangeZv.ZVX1 += x;
        rangeZv.ZVX2 += x;
        rangeZv.ZVY1 += y;
        rangeZv.ZVY2 += y;
        rangeZv.ZVZ1 += z;
        rangeZv.ZVZ2 += z;

        if(checkForHardCol(&rangeZv, &roomDataTable[currentProcessedActorPtr->room]))
        {
          currentProcessedActorPtr->animActionType = 0;
          putAt(objIdx, currentProcessedActorPtr->field_0);
        }
        else
        {
          if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
          {
            int x;
            int y;
            int z;
            
            currentProcessedActorPtr->animActionType = 7;

            x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x + currentProcessedActorPtr->modX;
            y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y + currentProcessedActorPtr->modY;
            z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z + currentProcessedActorPtr->modZ;

            removeObjFromInventory(objIdx);

            objPtr->x = x;
            objPtr->y = y;
            objPtr->z = z;

            objPtr->room = currentProcessedActorPtr->room;
            objPtr->stage = currentProcessedActorPtr->stage;
            objPtr->alpha = currentProcessedActorPtr->alpha;
            objPtr->beta = currentProcessedActorPtr->beta+0x200; 

            objPtr->flags2 &= 0xBFFF;
            objPtr->flags |= 0x85;
            objPtr->flags &= 0xFFDF;

            //objModifFlag1 = 1;
          }
        }
      }
      break;
    }
  case 7: // THROW
    {
      int x;
      int y;
      int z;
      int objIdx;
      int actorIdx;
      actorStruct* actorPtr;
      
      currentProcessedActorPtr->animActionType = 0;

      x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->hotPoint.x + currentProcessedActorPtr->modX;
      y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->hotPoint.y + currentProcessedActorPtr->modY;
      z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->hotPoint.z + currentProcessedActorPtr->modZ;

      objIdx = currentProcessedActorPtr->animActionParam;

      actorIdx = objectTable[objIdx].ownerIdx;

      if(actorIdx == -1)
        return;

      actorPtr = &actorTable[actorIdx];

      actorPtr->roomX = x;
      actorPtr->roomY = y;
      actorPtr->roomZ = z;

      getZvNormal(HQR_Get(listBody,actorPtr->bodyNum),&actorPtr->zv);

      actorPtr->zv.ZVX1 += x;
      actorPtr->zv.ZVX2 += x;
      actorPtr->zv.ZVY1 += y;
      actorPtr->zv.ZVY2 += y;
      actorPtr->zv.ZVZ1 += z;
      actorPtr->zv.ZVZ2 += z;

      actorPtr->flags |= 1;
      actorPtr->flags &= 0xFFF7;

      objectTable[objIdx].x = x;
      objectTable[objIdx].y = y;
      objectTable[objIdx].z = z;

      objectTable[objIdx].alpha = currentProcessedActorPtr->field_0; // hu ?

      actorPtr->dynFlags = 0;
      actorPtr->animActionType = 9;
      actorPtr->animActionParam = 100;
      actorPtr->hitForce = currentProcessedActorPtr->hitForce;
      actorPtr->field_98 = -1;
      actorPtr->speed = 3000;

      startActorRotation(0, actorPtr->speed, 60, &actorPtr->speedChange);

      break;
    }
  case 8: // HIT_OBJ
    {
      break;
    }
  case 9: // during throw
    {
      objectStruct* objPtr = &objectTable[currentProcessedActorPtr->field_0];

      ZVStruct rangeZv;
      ZVStruct rangeZv2;
      int xtemp;
      int ytemp;
      int ztemp;
      int x1;
      int x2;
      int x3;
      int y1;
      int y2;
      int z1;
      int z2;
      int z3;
      int step;

      copyZv(&currentProcessedActorPtr->zv, &rangeZv);
      copyZv(&currentProcessedActorPtr->zv, &rangeZv2);

      xtemp = currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
      ytemp = currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
      ztemp = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

      rangeZv2.ZVX1 -= xtemp;
      rangeZv2.ZVX2 -= xtemp;
      rangeZv2.ZVY1 -= ytemp;
      rangeZv2.ZVY2 -= ytemp;
      rangeZv2.ZVZ1 -= ztemp;
      rangeZv2.ZVZ2 -= ztemp;

      x1 = objPtr->x;
      x2 = objPtr->x;
      x3 = objPtr->x;

      y1 = objPtr->y;
      y2 = objPtr->y;

      z1 = objPtr->z;
      z2 = objPtr->z;
      z3 = objPtr->z;

      step = 0;

      animMoveY = 0;
      animMoveX = 0;

      do
      {
        int collision;
        sceZoneStruct* ptr;
        
        walkStep(0,-step,currentProcessedActorPtr->beta);
        step += 100;
        x2 = x1 + animMoveX;
        y2 = y1;
        z2 = z1 + animMoveY;

        copyZv(&rangeZv2,&rangeZv);

        rangeZv.ZVX1 = x2 - 200;
        rangeZv.ZVX2 = x2 + 200;
        rangeZv.ZVY1 = y2 - 200;
        rangeZv.ZVY2 = y2 + 200;
        rangeZv.ZVZ1 = z2 - 200;
        rangeZv.ZVZ2 = z2 + 200;

        collision =  processActor1Sub1(currentProcessedActorIdx,&rangeZv);

        if(collision)
        {
          int collision2 = collision;
          int i;

          currentProcessedActorPtr->hotPoint.x = 0;
          currentProcessedActorPtr->hotPoint.y = 0;
          currentProcessedActorPtr->hotPoint.z = 0;

          for(i=0;i<collision;i++)
          {
            int currentActorCol = currentProcessedActorPtr->COL[i];

            if(actorTable[currentActorCol].field_0 == objPtr->alpha)
            {
              collision2--;
              objPtr->x = xtemp;
              objPtr->y = ytemp;
              objPtr->z = ztemp;

              return;
            }

            if(actorTable[currentActorCol].field_0 == CVars[getCVarsIdx(REVERSE_OBJECT)])
            {
              objPtr->alpha = CVars[getCVarsIdx(REVERSE_OBJECT)];
              currentProcessedActorPtr->beta += 0x200;
              xtemp = x3;
              ztemp = z3;

              currentProcessedActorPtr->worldX = currentProcessedActorPtr->roomX = x3;
              currentProcessedActorPtr->worldY = currentProcessedActorPtr->roomY = y1;
              currentProcessedActorPtr->worldZ = currentProcessedActorPtr->roomZ = z3;

              currentProcessedActorPtr->modX = 0;
              currentProcessedActorPtr->modZ = 0;

              copyZv(&rangeZv2, &rangeZv);

              rangeZv.ZVX1 += x3;
              rangeZv.ZVX2 += x3;
              rangeZv.ZVY1 += y1;
              rangeZv.ZVY2 += y1;
              rangeZv.ZVZ1 += z3;
              rangeZv.ZVZ2 += z3;

              copyZv(&rangeZv, &currentProcessedActorPtr->zv);

              objPtr->x = xtemp;
              objPtr->y = ytemp;
              objPtr->z = ztemp;

              return;

            }
            else
            {
              actorStruct* actorPtr;
              
              currentProcessedActorPtr->HIT = currentActorCol;
              actorPtr = &actorTable[currentActorCol];
              actorPtr->HIT_BY = currentProcessedActorIdx;
              actorPtr->hitForce = currentProcessedActorPtr->hitForce;
            }
          }

          if(collision2)
          {
            playSound(CVars[getCVarsIdx(SAMPLE_CHOC)]);
            throwStoppedAt(x3,z3);
            return;
          }
        }

        ptr = processActor2Sub(x2,y2,z2, &roomDataTable[currentProcessedActorPtr->room]);

        if(ptr)
        {
          if(ptr->type == 0 || ptr->type == 10)
          {
             playSound(CVars[getCVarsIdx(SAMPLE_CHOC)]);
             throwStoppedAt(x3,z3);
             return;
          }
        }

        if(checkForHardCol(&rangeZv, &roomDataTable[currentProcessedActorPtr->room]))
        {
          currentProcessedActorPtr->hotPoint.x = 0;
          currentProcessedActorPtr->hotPoint.y = 0;
          currentProcessedActorPtr->hotPoint.z = 0;

          playSound(CVars[getCVarsIdx(SAMPLE_CHOC)]);
          throwStoppedAt(x3,z3);
          return;
        }
      }while(   currentProcessedActorPtr->zv.ZVX1 - 100 > x2 ||
            currentProcessedActorPtr->zv.ZVX2 + 100 < x2 ||
            currentProcessedActorPtr->zv.ZVZ1 - 100 > z2 ||
            currentProcessedActorPtr->zv.ZVZ2 + 100 < z2 );

      objPtr->x = xtemp;
      objPtr->y = ytemp;
      objPtr->z = ztemp;

      break;
    }
  case 10: // PRE_HIT_IN_GOOD_ANIM
    {
      if(currentProcessedActorPtr->ANIM == currentProcessedActorPtr->animActionANIM)
      {
        if(currentProcessedActorPtr->FRAME == currentProcessedActorPtr->animActionFRAME)
        {
          currentProcessedActorPtr->animActionType = 2;
        }
      }
      else
      {
        currentProcessedActorPtr->animActionType = 0;
      }
      break;
    }
#ifdef INTERNAL_DEBUGGER
  default:
    {
      printf("Unsupported processAnimAction type %d\n",currentProcessedActorPtr->animActionType);
      break;
    }
#endif
  }
}
