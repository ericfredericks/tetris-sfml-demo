#ifndef TETRIS_SFML_H
#define TETRIS_SFML_H

#include <SFML/Graphics.h>
#include "tetristop.h"

/*sfVertexArray* sfmlVertexArray_createGrid(int,int,int,int,int);*/
sfVertexArray* sfmlVertexArray_createTetrisMatrix(int,int,int);
sfVertexArray* sfmlVertexArray_createTetrisBlocks(int,int,int(*)[W_MATRIX][H_MATRIX],int);
sfVertexArray* sfmlVertexArray_createTetrisPiece(int,int,int(*)[NUM_MINOS][2],int);



#endif
