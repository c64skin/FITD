#include "common.h"

void mainLoop(int allowSystemMenu)
{
	while(1)
	{
		timer = timeGlobal;
		process_events();
		readKeyboard();
		input3 = input2;
		input4 = inputKey;
		button = input1;

		if(input3)
		{
			if(input3 == 0x1C || input3 == 0x17)
			{
				if(allowSystemMenu == 0)
				{
					break;
				}

				if(statusScreenAllowed)
				{
					makeStatusScreen();
				}
			}
		}
		else
		{
//			input5 = 0;
		}

		if(button)
		{
			if(!allowSystemMenu)
			{
				break;
			}

			action = 0x2000;
		}
		else
		{
			action = 0;
		}

		updateInHand(inHand);

		if(changeFloor == 0)
		{
			if(defines.field_1A == -1)
			{
//				mainVar2 = 2000;
//				mainVar3 = 2000;
			}

			currentProcessedActorPtr = actorTable;

			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					currentProcessedActorPtr->COL_BY = -1;
					currentProcessedActorPtr->HIT_BY = -1;
					currentProcessedActorPtr->HIT = -1;
					currentProcessedActorPtr->HARD_DEC = -1;
					currentProcessedActorPtr->HARD_COL = -1;
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					int flag = currentProcessedActorPtr->flags;

					if(flag & 1)
					{
						processActor1();
					}

					if(flag & 0x40)
					{
						processActor2();
					}

					if(currentProcessedActorPtr->animActionType)
					{
						processAnimAction();
					}
				}

				currentProcessedActorPtr++;
			}

			currentProcessedActorPtr = actorTable;
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < 50; currentProcessedActorIdx++)
			{
				if(currentProcessedActorPtr->field_0 >= 0)
				{
					if(currentProcessedActorPtr->life != -1 && currentProcessedActorPtr->lifeMode != -1)
					{
						processLife(currentProcessedActorPtr->life);
					}
				}

				if(changeFloor)
					break;

				currentProcessedActorPtr++;
			}

			if(giveUp)
				break;


		}

		if(changeFloor)
		{
			loadFloor(newFloor);
		}

		if(needChangeRoom)
		{
			loadRoom(newRoom);
			setupCamera();
		}
		else
		{
			checkIfCameraChangeIsRequired();
			if(mainVar1)
			{
				setupCamera();
			}
		}

//		if(objModifFlag1)
		{
			updateAllActorAndObjects();
		} 

//		if(actorTurnedToObj)
		{
			createActorList();
		}

		sortActorList();

//		if(objModifFlag2)
		{
//			setupCameraSub4();
		}

//		mainLoopSub1();

		mainDraw(setupCameraVar1);

		//osystem_delay(100);

//		updateSound2();
	}

//	mainLoopVar1 = 0;
//	shakingState = 0;

//	stopShaking();
//	stopSounds();
}