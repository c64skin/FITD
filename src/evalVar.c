#include "common.h"

int getPosRelTable[] = {4,1,8,2,4,1,8,0};

int getPosRel(actorStruct* actor1, actorStruct* actor2)
{
	int beta1 = actor1->beta;
	int counter = 3;

	if(beta1 >= 0x80 && beta1 < 0x180)
	{
		counter = 2;
	}

	if(beta1 >= 0x180 && beta1 < 0x280)
	{
		counter = 1;
	}

	if(beta1 >= 0x280 && beta1 < 0x380)
	{
		counter = 0;
	}

	ZVStruct localZv;

	copyZv(&actor2->zv, &localZv);

	if(actor1->room != actor2->room)
	{
		getZvRelativePosition(&localZv, actor2->room, actor1->room);
	}

	int centerX = (localZv.ZVX1 + localZv.ZVX2) / 2;
	int centerZ = (localZv.ZVZ1 + localZv.ZVZ2) / 2;

	if(actor1->zv.ZVZ2 >= centerZ && actor1->zv.ZVZ1 <= centerZ)
	{
		if(actor1->zv.ZVX2 < centerX)
		{
			counter++;
		}
		else
		{
			if(actor1->zv.ZVX1 <= centerX)
			{
				return(0);
			}
			else
			{
				counter+=3;
			}
		}
	}
	else
	if(actor1->zv.ZVX2 >= centerX || actor1->zv.ZVX1 <= centerX)
	{
		if(actor1->zv.ZVZ2 < centerZ )
		{
			counter+=2;
		}
		else
		{
			if(actor1->zv.ZVZ1 <= centerZ)
			{
				return(0);
			}
		}
	}
	else
	{
		return(0);
	}

	return(getPosRelTable[counter]);
}

int calcDist(int X1, int Y1, int Z1, int X2, int Y2, int Z2)
{
	int Xdist = abs(X1 - X2);
	int Ydist = abs(Y1 - Y2);
	int Zdist = abs(Z1 - Z2);

	return(Xdist + Ydist + Zdist); // recheck overflow
}

int evalVar(void)
{
	int var1;

	var1 = *(short int*)(currentLifePtr);
	currentLifePtr+=2;

	if(var1 == -1)
	{
		int temp = *(short int*)(currentLifePtr);
		currentLifePtr+=2;

		return(temp);
	}
	else
	if(var1 == 0)
	{
		int temp = *(short int*)(currentLifePtr);
		currentLifePtr+=2;

		return(vars[temp]);
	}
	else
	{
		actorStruct* actorPtr = currentLifeActorPtr;
		int actorIdx = currentLifeActorIdx;

		if(var1 & 0x8000)
		{
			int objectNumber;

			objectNumber = *(short int*)currentLifePtr;

			actorIdx = objectTable[objectNumber].ownerIdx;

			currentLifePtr+=2;
			actorPtr = &actorTable[actorIdx];

			if(actorIdx==-1)
			{
				switch(var1 & 0x7FFF)
				{
				case 0x1F:
					{
						return(objectTable[objectNumber].room);
						break;
					}
				case 0x26:
					{
						return(objectTable[objectNumber].stage);
						break;
					}
				default:
					{
						printf("Unsupported evalVar %X when actor not in room !\n", var1 & 0x7FFF);
						exit(1);
					}
				}
			}
		}
		{
			
			var1&=0x7FFF;			

			var1--;

			switch(var1)
			{
			case 0x0:
				{
					return(actorPtr->COL[0]);
					break;
				}
			case 0x1:
				{
					return(actorPtr->HARD_DEC);
					break;
				}
			case 0x2:
				{
					return(actorPtr->HARD_COL);
					break;
				}
			case 0x3:
				{
					int temp = actorPtr->HIT;

					if(temp == -1)
					{
						return(-1);
					}
					else
					{
						return(actorTable[temp].field_0);
					}

					break;
				}
			case 0x4: // TODO
				{
					int temp = actorPtr->HIT_BY;

					if(temp == -1)
					{
						return(-1);
					}
					else
					{
						return(actorTable[temp].field_0);
					}

					break;
				}
			case 0x5:
				{
					return(actorPtr->ANIM);
					break;
				}
			case 0x6:
				{
					return(actorPtr->END_ANIM);
					break;
				}
			case 0x7:
				{
					return(actorPtr->FRAME);
					break;
				}
			case 0x8:
				{
					return(actorPtr->END_FRAME);
					break;
				}
			case 0x9:
				{
					return(actorPtr->bodyNum);
					break;
				}
			case 0xA: // MARK
				{
					return(actorPtr->MARK);
					break;
				}
			case 0xB: // NUM_TRACK
				{
					return(actorPtr->trackNumber);
					break;
				}
			case 0xC: // CHRONO
				{
					return(evalChrono(&actorPtr->CHRONO) / 0x3C0000); // recheck
					break;
				}
			case 0xD:
				{
					return(evalChrono(&actorPtr->ROOM_CHRONO) / 0x3C0000); // recheck
					break;
				}
			case 0xE: // DIST
				{
					int actorNumber = objectTable[*(short int*)currentLifePtr].ownerIdx;
					currentLifePtr+=2;

					if(actorNumber == -1)
					{
						return(32000);
					}
					else
					{
						int tempX = actorTable[actorNumber].worldX;
						int tempY = actorTable[actorNumber].worldY;
						int tempZ = actorTable[actorNumber].worldZ;

						return(calcDist(actorPtr->worldX, actorPtr->worldY, actorPtr->worldZ, tempX, tempY, tempZ));
					}

					break;
				}
			case 0xF: // COL_BY
				{
					if(actorPtr->COL_BY == -1)
						return(-1);
					else
						return(actorTable[actorPtr->COL_BY].field_0);
					break;
				}
			case 0x10:
				{
					if(objectTable[evalVar()].flags2 & 0x8000)
					{
						return(1);
					}
					else
					{
						return(0);
					}

					break;
				}
			case 0x12: // POSREL
				{
					int objNum;

					objNum = *(short int*)currentLifePtr;
					currentLifePtr+=2;

					if(objectTable[objNum].ownerIdx == -1)
					{
						return 0;
					}

					int posRel = getPosRel(actorPtr, &actorTable[objectTable[objNum].ownerIdx]);
					printf("PosRel: %d\n",posRel);
					return(posRel);

					break;
				}
			case 0x14:
				{
					return(button);
					break;
				}
			case 0x15: // TODO
				{
					return(-1);
					break;
				}
			case 0x16:
				{
					return(actorPtr->alpha);
					break;
				}
			case 0x17:
				{
					return(actorPtr->beta);
					break;
				}
			case 0x18:
				{
					return(actorPtr->gamma);
					break;
				}
			case 0x19:
				{
					return(inHand);
					break;
				}
			case 0x1E:
				{
					return(actorPtr->room);
					break;
				}
			case 0x1D:
				{
					return(actorPtr->falling);
					break;
				}
			case 0x20:
				{
					int objNum;

					objNum = *(short int*)currentLifePtr;
					currentLifePtr+=2;

					if(objectTable[objNum].flags2 & 0xC000)
					{
						return(1);
					}
					else
					{
						return(0);
					}

					break;
				}
			case 0x21:
				{
					return(actorPtr->roomY);
					break;
				}
			case 0x23: // TODO: music
				{
					return(1);
					break;
				}
			case 0x24:
				{
					int temp = *((short int*)&defines)+(*(short int*)currentLifePtr);
					currentLifePtr+=2;
					return(temp);
					break;
				}
			case 0x25:
				{
					return(actorPtr->stage);
					break;
				}
			case 0x26: // THROW
				{
					int objNum;

					objNum = *(short int*)currentLifePtr;
					currentLifePtr+=2;

					if(objectTable[objNum].flags2 & 0x1000)
					{
						return 1;
					}
					else
					{
						return 0;
					}
					break;
				}
			default:
				{
					printf("Unhandled test type %X in evalVar\n",var1);
					exit(1);
					break;
				}
			}
		}
	}
}
