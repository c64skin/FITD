#include "common.h"

int computeAngleModificatorToPositionSub1(int ax)
{
	int xOut;
	int yOut;

	makeRotationMtx(ax,0,1000,&xOut,&yOut);

	yOut *= angleCompX;
	xOut *= angleCompZ;

	yOut -= xOut;

	return(abs(yOut));
}

int computeAngleModificatorToPosition(int x1,int z1, int beta, int x2, int z2)
{
	angleCompX = x2 - x1;
	angleCompZ = z2 - z1;
	angleCompBeta = beta;

	int resultMin = computeAngleModificatorToPositionSub1(beta - 4);
	int resultMax = computeAngleModificatorToPositionSub1(beta + 4);

	if(resultMax == -1 && resultMin == 1) // in the middle
	{
		return(computeAngleModificatorToPositionSub1(beta));
	}
	else
	{
		return(((resultMax+resultMin)+1)>>1);
	}
}

void processTrack(void)
{
	switch(currentProcessedActorPtr->trackMode)
	{
	case 1: // manual
		{
			printf("Manual move !\n");
			break;
		}
	case 2: // ?
		{
			printf("unsupported move mode 2\n");
			exit(1);
			break;
		}
	case 3: // track
		{
			char* trackPtr = HQR_Get(listTrack,currentProcessedActorPtr->trackNumber);
			
			trackPtr+=currentProcessedActorPtr->positionInTrack * 2;

			short int trackMacro = *(short int*)trackPtr;
			trackPtr += 2;

			switch(trackMacro)
			{
			case 1: // goToPosition
				{
					int roomNumber = *(short int*)(trackPtr);
					trackPtr += 2;

					int x = *(short int*)(trackPtr);
					trackPtr += 2;
					int y = 0;
					int z = *(short int*)(trackPtr);
					trackPtr += 2;
		
					if(roomNumber != currentProcessedActorPtr->room)
					{
						char* roomDestDataPtr = etageVar0 + *(unsigned int*)(etageVar0+roomNumber*4);
						char* roomSourceDataPtr = etageVar0 + *(unsigned int*)(etageVar0+currentProcessedActorPtr->room*4);

						x -= ((*(short int*)(roomSourceDataPtr+4)) - (*(short int*)(roomDestDataPtr+4))) * 10;
						z -= ((*(short int*)(roomSourceDataPtr+8)) - (*(short int*)(roomDestDataPtr+8))) * 10;
					}

					int distanceToPoint = computeDistanceToPoint(	currentProcessedActorPtr->x + currentProcessedActorPtr->field_5A,
																	currentProcessedActorPtr->z + currentProcessedActorPtr->field_5E,
																	x,z );


					if(distanceToPoint >= 400) // not yet at position
					{
						int angleModif = computeAngleModificatorToPosition(	currentProcessedActorPtr->x + currentProcessedActorPtr->field_5A,
																			currentProcessedActorPtr->z + currentProcessedActorPtr->field_5E,
																			currentProcessedActorPtr->beta,
																			x,z );

						if(currentProcessedActorPtr->rotate.param == 0 || (currentProcessedActorPtr->rotate.param != 0 && currentProcessedActorPtr->field_72 != angleModif))
						{
							startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<6), 15, &currentProcessedActorPtr->rotate);
						}

						currentProcessedActorPtr->field_72 = angleModif;

						if(angleModif)
						{
							currentProcessedActorPtr->rotate.param = 0;
						}
						else
						{
							currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
						}
					}
					else // reached position
					{
						currentProcessedActorPtr->positionInTrack += 4;
					}
						
					break;
				}
			case 2: // stop
				{
					currentProcessedActorPtr->speed = 0;
					currentProcessedActorPtr->trackNumber = -1;
					setMoveMode(0,0);
					break;
				}
			case 4: // MARK
				{
					currentProcessedActorPtr->MARK = *(short int*)(trackPtr);
					trackPtr += 2;
					currentProcessedActorPtr->positionInTrack += 2;
					break;
				}
			case 0x13: // rotate
				{
					currentProcessedActorPtr->alpha = *(short int*)(trackPtr);
					trackPtr += 2;
					currentProcessedActorPtr->beta = *(short int*)(trackPtr);
					trackPtr += 2;
					currentProcessedActorPtr->gamma = *(short int*)(trackPtr);
					trackPtr += 2;

					currentProcessedActorPtr->field_72 = 0;

					currentProcessedActorPtr->positionInTrack +=4;

					break;
				}
			default:
				{
					printf("Unknown track macro %X\n",trackMacro);
					exit(1);
					break;
				}
			}

			break;
		}
	}

	currentProcessedActorPtr->beta &= 0x3FF;
}
