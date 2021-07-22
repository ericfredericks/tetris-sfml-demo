#ifndef TETRISSTATE_H
#define TETRISSTATE_H

#include <stdio.h>
#include "tetris.h"
#include "tetristop.h"



#define MAX_STATES 30



int tetrisstateStack[MAX_STATES];
int tetrisstateCount;
int tetrisstateReady;



void tetrisstateInit();
void tetrisstatePush(int);
void tetrisstateUpdate();
void tetrisstateDisplay();
void tetrisstatePop();
void tetrisstateError();



#endif
