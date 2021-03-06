#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SFML/Graphics.h>
#include "tetristop.h"
#include "tetrissfml.h"
#include "sfmlinput.h"

void tetrisstatePush(int);
void tetrisstatePop();


/* Constants */
#define PI 3.14159265
#define TILE_SIZE 8
#define MATX_ORIGIN TILE_SIZE
#define MATY_ORIGIN (600-TILE_SIZE)
#define FALL_SPD (pow(0.8-((game.level-1)*0.007),game.level-1))
enum {GENERATION,FALLING,LOCK,PATTERN,ITERATE,ANIMATE,ELIMINATE,COMPLETION};



/* State */
typedef struct
{
	sfClock *lockTimeClock;
	float lockTimeS;
	float lockTimeLostS;
	int yAtStartLock;
	int level;
	sfClock *fallSpdClock;
	int bag[NUM_PIECES];
	int nextQueue[NUM_PIECES];
	int amtBag;
	int phase;
	int blockMatrix[W_MATRIX][H_MATRIX];
	int hitList[H_MATRIX];
} game_t;
game_t game;
typedef struct
{
	int type;
	int facing;
	int pos[NUM_MINOS][2];
	int point[NUM_SRPOINTS][2];
} piece_t;
piece_t piece;

/* SFML objects */
typedef struct
{
	sfVertexArray *grid;
	sfVertexArray *piece;
	sfVertexArray *blocks;
} gfx_t;
gfx_t gfx;



/* Functions */
void tetrisInit();
void tetrisDisplay();
void tetrisUpdate();
void tetrisExit();

void tetrisNewArray(int(*)[NUM_PIECES]);

void tetrisPieceUpdate();
int tetrisPieceNewType();
void tetrisPiecePlace(piece_t*);
void tetrisPieceNewPoints(int(*)[NUM_SRPOINTS][2],piece_t*);
int tetrisPieceGetY(piece_t*);
int tetrisPieceCollision(piece_t*,int,int);
void tetrisPieceMove(piece_t*,int,int);
void tetrisPieceTryRotate(piece_t*,int);
void tetrisPieceMakeBlocks(piece_t*);
void tetrisNewHitList(int(*)[H_MATRIX],int(*)[W_MATRIX][H_MATRIX]);
void tetrisUseHitList(int(*)[H_MATRIX],int(*)[W_MATRIX][H_MATRIX]);


void tetrisGfxUpdate();
#endif
