#include "common.h"
#include "math.h"
// From LBA:
int DoTrackVar1;
int GetAngle(int X1, int Y1, int X2, int Y2)
{
    int newX;
    int newY;
    int ebp;
    int edi;
    int eax;
    int tempExchange;
    int esi;
    int ebx;
    int flag;
    int difX;
    int difY;
	short int* tab3 = &cosTable[384];
	short int* tab2 = &cosTable[256];

    difY = edi = Y2 - Y1;
    newY = edi * edi;

    difX = ebp = X2 - X1;
    newX = ebp * ebp;

    if (newX < newY)		// exchange X and Y
	{
	    tempExchange = difX;
	    difX = difY;
	    difY = tempExchange;

	    flag = 1;
	}
    else
	{
	    flag = 0;
	}

    DoTrackVar1 = (int) sqrt(newX + newY);

    if (!DoTrackVar1)
	return (0);

    int destVal;
    int startAngle;
    int stopAngle;
    int finalAngle;

    destVal = (difY << 14) / DoTrackVar1;

    startAngle = 0;
    stopAngle = 0x100;

    while (tab3[startAngle] > destVal)
	{
	    startAngle++;
	   /*
	    * if (startAngle > 256) { printf ("Can't find angle %d...\n", destVal); exit (1); } 
	    */
	}

    if (tab3[startAngle] != destVal)
	if ((tab3[startAngle - 1] + tab3[startAngle]) / 2 <= destVal)
	    {
		startAngle--;
	    }

    finalAngle = 128 + startAngle;

    if (difX <= 0)		// if we are going left
	{
	    finalAngle = -finalAngle;
	}

    if (flag & 1)		// X and Y are exchanged -> 90° rotation needed
	{
	    finalAngle = -finalAngle + 0x100;
	}

    return (finalAngle & 0x3FF);

   /*
    * do { currentAngle=(startAngle+stopAngle)/2;
    * 
    * if(destVal>tab3[currentAngle]) { stopAngle=currentAngle; } else { startAngle=currentAngle;
    * if(destVal==tab3[currentAngle]) { goto endCalc; } currentAngle=stopAngle; } }
    * while(--currentAngle);
    */

    esi = (int) tab3;
    edi = esi + 0x200;

    do
	{
	    ebx = esi;
	    ebx += edi;
	    ebx >>= 1;

	    if (eax > READ_LE_S16((void*)ebx))
		{
		    edi = ebx;
		}
	    else
		{
		    esi = ebx;
		    if (eax == READ_LE_S16((void*)ebx))
			{
			    goto endCalc;
			}
		    ebx = edi;
		}
	    ebx -= esi;
	}
    while (--ebx > 2);

    if ((READ_LE_S16((void*)esi) + READ_LE_S16((void*)edi)) / 2 <= eax)
	{
	    esi = edi;
	}

  endCalc:

    esi -= (int) tab2;
    eax = esi;
    eax >>= 1;

    if (ebp <= 0)
	{
	    eax = -eax;
	}

    if (ebp & 1)		// (newX < newY) ie plus loin sur Y que sur X 
	{
	    eax = -eax;
	    eax += 0x100;
	}

    return (eax & 0x3FF);

}

int computeAngleModificatorToPositionSub1(int ax)
{
	int xOut;
	int yOut;

	makeRotationMtx(ax,0,1000,&yOut,&xOut);

	yOut *= angleCompX;
	xOut *= angleCompZ;

	yOut -= xOut;

	if(yOut==0)
		return(0);

	if(yOut>0)
		return(1);
	else
		return(-1);
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

					unsigned int distanceToPoint = computeDistanceToPoint(	currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
																			currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
																			x,z );


					if(distanceToPoint >= 400) // not yet at position
					{
						currentProcessedActorPtr->beta = GetAngle(currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
																			currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
																			x,z );
				/*		int angleModif = computeAngleModificatorToPosition(	currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
																			currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
																			currentProcessedActorPtr->beta,
																			x,z );

						if(currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->field_72 != angleModif)
						{
							startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<6), 15, &currentProcessedActorPtr->rotate);
						}

						currentProcessedActorPtr->field_72 = angleModif;

						if(!angleModif)
						{
							currentProcessedActorPtr->rotate.param = 0;
						}
						else
						{
							currentProcessedActorPtr->beta = -updateActorRotation(&currentProcessedActorPtr->rotate);
						}*/
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
