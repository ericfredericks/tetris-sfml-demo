#include "main.h"



int main()
{
	sfVideoMode mode = {800,600,32};
	window = sfRenderWindow_create(mode,"Tetris",sfResize|sfClose,NULL);
	if (!window)
		return -1;

	tetrisstateInit();
	tetrisstatePush(STATE_GAME);

	gameClock = sfClock_create();
	lastUpdateMs = sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock));
	lastDisplayMs = sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock));



	while (sfRenderWindow_isOpen(window))
	{
		sfEvent event;
		while (sfRenderWindow_pollEvent(window,&event))
		{
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);
		}
		
		if (!tetrisstateCount)
		{
			sfRenderWindow_close(window);
			break;
		}

		gameClockDone = 0;
		untilUpdateMs = updateIntervalMs + (lastUpdateMs - sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock)));
		untilDisplayMs = displayIntervalMs + (lastDisplayMs - sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock)));
		if (untilUpdateMs <= 0)
		{
			tetrisstateUpdate();
			lastUpdateMs = sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock));
			gameClockDone = 1;
		}
		if (untilDisplayMs <= 0)
		{
			tetrisstateDisplay();
			lastDisplayMs = sfTime_asMilliseconds(sfClock_getElapsedTime(gameClock));
			gameClockDone = 1;
		}
		if (!gameClockDone)
		{
			if (untilDisplayMs < untilUpdateMs)
				sfSleep(sfMilliseconds(untilDisplayMs));
			else
				sfSleep(sfMilliseconds(untilUpdateMs));
		}
	}



	sfRenderWindow_destroy(window);
	return 0;
}
