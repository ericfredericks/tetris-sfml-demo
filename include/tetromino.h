#ifndef TETROMINO_H
#define TETROMINO_H

typedef struct
{
	int x,y,facing;
	const int W,H,TYPE;
	sfSprite[4] sprite;
} tetTetromino;

tetTetromino tetrisCreateTetromino(int piece)
{
	tetTetromino tetromino;
	switch (piece)
	{
		case TET_O:
			tetromino = {};
			return ;
		case TET_I:
			break;
		case TET_T:
			break;
		case TET_L:
			break;
		case TET_J:
			break;
		case TET_S:
			break;
		case TET_Z:
	}
}

#endif
