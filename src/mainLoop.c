#include "common.h"

int mainLoopSwitch = 0;

void mainLoop(int allowSystemMenu)
{
#define SPEED 15              /* Ticks per Frame */
#define SLEEP_MIN 20          /* Minimum time a sleep takes, usually 2*GRAN */
#define SLEEP_GRAN 10         /* Granularity of sleep */

int frames=0;                   /* Number of frames displayed */
s32 t_start,t_end,t_left;
s32 q=0;                     /* Dummy */

	while(1)
	{
    frames++;
    t_start=SDL_GetTicks();

    callMusicUpdate();

    mainLoopSwitch++;

    if(mainLoopSwitch == 1)
    {
      timeGlobal++;
      mainLoopSwitch = 0;

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

			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++)
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
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++)
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
			for(currentProcessedActorIdx = 0; currentProcessedActorIdx < NUM_MAX_ACTOR; currentProcessedActorIdx++)
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

    t_end=t_start+SPEED;
    t_left=t_start-SDL_GetTicks()+SPEED;

    if(t_left>0){
        if(t_left>SLEEP_MIN)
            SDL_Delay(t_left-SLEEP_GRAN);
        while(SDL_GetTicks()<t_end){ q++; };
    }else{
       // printf("CPU to slow by %d ticks/round\n",-t_left);
    };
    }
	}

//	mainLoopVar1 = 0;
//	shakingState = 0;

//	stopShaking();
//	stopSounds();
}