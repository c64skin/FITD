#include "common.h"

void processLife(int lifeNum)
{
	int exitLife = 0;
	//int switchVal = 0;
	int var_6;

	currentLifeActorIdx = currentProcessedActorIdx;
	currentLifeActorPtr = currentProcessedActorPtr;
	currentLifeNum = lifeNum;

	currentLifePtr = HQR_Get(listLife,lifeNum);

	int switchVal = 0;

	while(!exitLife)
	{
		int lifeTempVar1;
		int lifeTempVar2;
		int lifeTempVar3;
		int lifeTempVar4;
		int lifeTempVar5;

		var_6 = -1;

		short int currentOpcode = *(short int*)(currentLifePtr);
		currentLifePtr+=2;

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
					currentProcessedActorPtr = &actorTable[currentProcessedActorIdx];

					goto processOpcode;
				}
		}
		else
		{
processOpcode:
			switch(currentOpcode & 0x7FFF)
			{
			case 0x0: // DO_MOVE
				{
					processTrack();
					break;
				}
			case 0x1: // ANIM
				{
					printf("Anim\n");
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
			case 0x4: // IF_DIF
				{
					lifeTempVar1 = evalVar();
					lifeTempVar2 = evalVar();

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
			case 0xA: // GOTO
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr += lifeTempVar1*2;
					currentLifePtr += 2;	
					break;
				}
			case 0xC: // RETURN
				{
					exitLife = 1;
					break;
				}
			case 0xD: // ?
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					anim(lifeTempVar1, 1, -1);

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
			case 0x11: // MESSAGE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					makeMessage(lifeTempVar1);

					break;
				}
			case 0x19: // SWITCH
				{
					switchVal = evalVar();
					break;
				}
			case 0x1A: // CASE
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
			case 0x1F: // LIFE
				{
					currentProcessedActorPtr->life = *(short int*)(currentLifePtr);
					currentLifePtr+=2;
					break;
				}
			case 0x20: // DELETE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					deleteObject(lifeTempVar1);

					if(objectTable[lifeTempVar1].foundBody != -1)
					{
						objectTable[lifeTempVar1].flags2 &= 0x7FFF;
						objectTable[lifeTempVar1].flags2 |= 0x4000;
					}

					break;
				}
			case 0x27:
				{
					playSound(evalVar());
					break;
				}
			case 0x28: // TYPE
				{
					lifeTempVar1 = *(short int*)(currentLifePtr) + 0x1D1;
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
			case 0x2F: // STAGE TODO
				{
					currentLifePtr+=10;
					break;
				}
			case 0x33: // CAMERA_TARGET
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					if(lifeTempVar1 != currentCameraTarget)
					{
						lifeTempVar2 = objectTable[lifeTempVar1].ownerIdx;

						if(lifeTempVar1 != -1)
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
						else
						{
							printf("Partialy unimplemented life opcode 0x33\n");
							exit(1);
						}
					}

					break;
				}
			case 0x36: // TEST_COL
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
			case 0x3F: //todo
				{
					evalVar();
					evalVar();
					break;
				}
			case 0x40: // LIGHT
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
			case 0x41: // SHAKING
				{
					//shakingState = shakingAmplitude = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

/*					if(shakingState==0)
					{
						stopShaking();
					} */

					break;
				}
			case 0x4B: // sample
				{
					currentLifePtr+=2;
					break;
				}
			case 0x4D: // ? shaking related
				{
//					mainLoopVar1 = shakeVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

/*					if(mainLoopVar1)
					{
						//setupShaking(-600);
					}
					else
					{
						//setupShaking(1000);
					} */

					break;
				}
			case 0x51: // ? fade out music and play another music ?
				{
					lifeTempVar1 = *(short int*)(currentLifePtr);
					currentLifePtr+=2;

					printf("Play music %d\n",lifeTempVar1);

					/*if(currentMusic!=-1)
					{
						soundFunc1(0,0,0x8000); // fade out music
						startChrono(&musicChrono); // fade out music timer
						currentMusic = -2; // fade out mode
						genVar8 = lifeTempVar1; // next music to play
					}
					else
					{
						changeMusic(lifeTempVar1);
					}*/

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