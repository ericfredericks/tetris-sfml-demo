#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include "tetristop.h"
#include "tetrisstate.h"



sfClock *gameClock;
const sfInt32 updateIntervalMs = 1000/25;
const sfInt32 displayIntervalMs = 1000/60;
sfInt32 lastUpdateMs; 
sfInt32 lastDisplayMs;
sfInt32 untilUpdateMs;
sfInt32 untilDisplayMs;
int gameClockDone;




#endif


