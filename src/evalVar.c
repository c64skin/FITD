#include "common.h"

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
				printf("actorIdx == -1 dans evalVar\n");
				exit(1);
			}
		}
		{
			
			var1&=0x7FFF;			

			var1--;

			switch(var1)
			{
			case 0x2:
				{
					return(actorPtr->HARD_COL);
					break;
				}
			case 0x5:
				{
					return(actorPtr->ANIM);
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
			case 0xF: // COL_BY
				{
					if(actorPtr->COL_BY == -1)
						return(-1);
					else
						return(actorTable[actorPtr->COL_BY].field_0);
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
