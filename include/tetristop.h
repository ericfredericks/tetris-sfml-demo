#ifndef TETRISTOP_H
#define TETRISTOP_H

#include <SFML/Graphics.h>



/* 
 *	This Tetris uses the matrix coordinates
 *	from the Tetris Design Guideline
 *		[1-10][1-40]
 *
 *	SRS points are given as an offset from the first mino
 */
#define NUM_PIECES 7
#define TYPE_O 1
#define TYPE_I 2
#define TYPE_T 3
#define TYPE_L 4
#define TYPE_J 5
#define TYPE_S 6
#define TYPE_Z 7
#define TYPE_NONE 8
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define W_MATRIX (10+1)
#define H_MATRIX (40+1)
#define H_SKYLINE 21
#define STATE_GAME 0
#define NUM_SRPOINTS 5
#define NUM_MINOS 4
sfRenderWindow *window;



#endif
