#include "tetrisstate.h"

void tetrisstateInit()
{
	tetrisstateCount = 0;
	tetrisstateReady = 1;
}
void tetrisstatePush(int state)
{
	if (tetrisstateCount == MAX_STATES)
	{
		tetrisstateError(1);
		return;
	}
	if (!tetrisstateReady)
	{
		tetrisstateError(0);
		return;
	}
	switch (state)
	{
		default:
			tetrisstateError(2);
			return;
		case STATE_GAME:
			tetrisInit();
			break;
	}
	tetrisstateStack[tetrisstateCount] = state;
	tetrisstateCount++;
}		
void tetrisstateUpdate()
{
	if (!tetrisstateReady)
	{
		tetrisstateError(0);
		return;
	}
	if (!tetrisstateCount)
	{
		tetrisstateError(3);
		return;
	}
	switch (tetrisstateStack[tetrisstateCount-1])
	{
		case STATE_GAME:
			tetrisUpdate();
			break;
	}
}

void tetrisstateDisplay()
{
	if (!tetrisstateReady)
	{
		tetrisstateError(0);
		return;
	}
	if (!tetrisstateCount)
	{
		tetrisstateError(3);
		return;
	}
	switch (tetrisstateStack[tetrisstateCount-1])
	{
		case STATE_GAME:
			tetrisDisplay();
			break;
	}
}

void tetrisstatePop()
{
	if (!tetrisstateReady)
	{
		tetrisstateError(0);
		return;
	}
	if (!tetrisstateCount)
	{
		tetrisstateError(3);
		return;
	}
	switch (tetrisstateStack[tetrisstateCount-1])
	{
		case STATE_GAME:
			tetrisExit();
			break;
	}
	tetrisstateCount--;
}

void tetrisstateError(int error)
{
	switch (error)
	{
		case 0:
			printf("ERROR: state uninitialized\n");
			return;
		case 1:
			printf("ERROR: maximum states\n");
			return;
		case 2:
			printf("ERROR: set unknown state\n");
			return;
		case 3:
			printf("ERROR: state function with no states\n");
			return;
	}
}

