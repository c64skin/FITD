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
		printf("Unimplemented code int evalVar: var1 == 0\n");
		exit(1);
	}
	else
	{
		if(var1 & 0x8000)
		{
			printf("Unimplemented code int evalVar: var1 & 0x8000\n");
			exit(1);
		}
		else
		{
			//int actorIdx = currentLifeActorIdx;
			actorStruct* actorPtr = currentLifeActorPtr;

			var1--;

			switch(var1)
			{
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
