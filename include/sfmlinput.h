#ifndef SFMLINPUT_H
#define SFMLINPUT_H

#include <string.h>
#include <SFML/Window.h>



typedef struct
{
	int xAxis;
	int yAxis;
	int zPress;
	int xPress;
	int cPress;
	int escPress;
	int enterPress;

	int leftHold;
	int rightHold;
	int upHold;
	int downHold;
       	int zHold;
	int xHold;
	int cHold;
	int escHold;
	int enterHold;
} input_t;
input_t input;



void sfmlInput_init();
void sfmlInput_update();



#endif


