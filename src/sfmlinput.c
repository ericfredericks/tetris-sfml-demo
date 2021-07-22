#include "sfmlinput.h"

void sfmlInput_init()
{
	memset(&input,0,sizeof(input));
}

void sfmlInput_update()
{
	input.leftHold = input.xAxis < 0;
	input.rightHold = input.xAxis > 0;
	input.upHold = input.yAxis < 0;
	input.downHold = input.yAxis > 0;
	input.zHold = input.zPress;
	input.xHold = input.xPress;
	input.cHold = input.cPress;
	input.escHold = input.escPress;
	input.enterHold = input.enterPress;

	input.xAxis = -sfKeyboard_isKeyPressed(sfKeyLeft)+sfKeyboard_isKeyPressed(sfKeyRight);
	input.yAxis = -sfKeyboard_isKeyPressed(sfKeyUp)+sfKeyboard_isKeyPressed(sfKeyDown);
	input.zPress = sfKeyboard_isKeyPressed(sfKeyZ);
	input.xPress = sfKeyboard_isKeyPressed(sfKeyX);
	input.cPress = sfKeyboard_isKeyPressed(sfKeyC);
	input.escPress = sfKeyboard_isKeyPressed(sfKeyEscape);
	input.enterPress = sfKeyboard_isKeyPressed(sfKeyEnter);
}
