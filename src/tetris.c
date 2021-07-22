#include "tetris.h"

void tetrisInit()
{
	/* Input */
	sfmlInput_init();

	/* State */
	memset(&game,0,sizeof(game));
	srand((unsigned)time(NULL));
	tetrisNewArray(&game.nextQueue);
	game.fallSpdClock = sfClock_create();
	game.lockTimeClock = sfClock_create();

	memset(&piece,0,sizeof(piece));

	/* SFML objects */
	gfx.grid = sfmlVertexArray_createTetrisMatrix(MATX_ORIGIN,MATY_ORIGIN,TILE_SIZE);
	gfx.piece = sfVertexArray_create();
	gfx.blocks = sfVertexArray_create();
}
void tetrisDisplay()
{
	sfRenderWindow_clear(window,sfBlack);
	sfRenderWindow_drawVertexArray(window,gfx.grid,NULL);
	sfRenderWindow_drawVertexArray(window,gfx.blocks,NULL);
	sfRenderWindow_drawVertexArray(window,gfx.piece,NULL);
	sfRenderWindow_display(window);
}
void tetrisUpdate()
{
	sfmlInput_update();

	tetrisPieceUpdate();
	if (input.escPress)
		tetrisstatePop();

	tetrisGfxUpdate();
}
void tetrisExit()
{
	/* SFML objects */
	sfVertexArray_destroy(gfx.grid);
	sfVertexArray_destroy(gfx.piece);
	sfVertexArray_destroy(gfx.blocks);
}



void tetrisNewArray(int(*array)[NUM_PIECES])
{
	int result[NUM_PIECES] = {0};
	for (int i=0;i<NUM_PIECES;i++)
	{
		while (1)
		{
			result[i] = (rand() % NUM_PIECES)+1;
			int repeat = 0;
			for (int j=i;j>0;j--)
				repeat += (result[i] == result[j-1]);
			if (!repeat)
				break;
		}
		(*array)[i] = result[i];
	}
}


void tetrisPieceUpdate()
{
	float fallIntervalS;
	int increaseLockTime;
	switch (game.phase)
	{
		case GENERATION:
			game.level = 0;
			piece.type = tetrisPieceNewType();
			piece.facing = NORTH;
			tetrisPiecePlace(&piece);
			tetrisPieceNewPoints(&piece.point,&piece);
			if (!tetrisPieceCollision(&piece,0,-1))
				tetrisPieceMove(&piece,0,-1);
			sfClock_restart(game.fallSpdClock);
			game.phase = FALLING;
			break;

		case LOCK:
			increaseLockTime = 1;
			if (!tetrisPieceCollision(&piece,0,-1))
			{
				/* stop lock time while rotating */
				if (tetrisPieceGetY(&piece)>=game.yAtStartLock)
					increaseLockTime = 0;
				else
					game.phase = FALLING;
			}
			if (!increaseLockTime)
				game.lockTimeLostS += sfTime_asSeconds(sfClock_restart(game.lockTimeClock));
			else
			{
				game.lockTimeS = sfTime_asSeconds(sfClock_getElapsedTime(game.lockTimeClock)); 
				if (game.lockTimeLostS)
				{
					game.lockTimeS += game.lockTimeLostS;
					game.lockTimeLostS = 0;
				}
			}
			if (game.lockTimeS>=0.5f)
				game.phase = PATTERN;
		case FALLING:
			/* soft drop */
			fallIntervalS = FALL_SPD;
			if (input.yAxis>0)
				fallIntervalS = FALL_SPD/20;
			if (sfTime_asSeconds(sfClock_getElapsedTime(game.fallSpdClock))>=fallIntervalS)
			{
				if (!tetrisPieceCollision(&piece,0,-1))
					tetrisPieceMove(&piece,0,-1);
				sfClock_restart(game.fallSpdClock);
			}

			/* hard drop */
			if ((input.yAxis<0)
			&& (!input.upHold))
			{
				while (!tetrisPieceCollision(&piece,0,-1))
					tetrisPieceMove(&piece,0,-1);
				game.phase = PATTERN;
			}

			/* move */
			if ((input.xAxis<0)
			&& (!input.leftHold)
			&& (!tetrisPieceCollision(&piece,-1,0)))
				tetrisPieceMove(&piece,-1,0);
			if ((input.xAxis>0)
			&& (!input.rightHold)
			&& (!tetrisPieceCollision(&piece,1,0)))
				tetrisPieceMove(&piece,1,0);

			/* rotate */
			if ((input.zPress)
			&& (!input.zHold)
			&& (piece.type != TYPE_O))
				tetrisPieceTryRotate(&piece,1);
			if ((input.xPress)
			&& (!input.xHold)
			&& (piece.type != TYPE_O))
				tetrisPieceTryRotate(&piece,0);

			if (tetrisPieceCollision(&piece,0,-1)
			&& (game.phase == FALLING))
			{
				sfClock_restart(game.lockTimeClock);
				game.lockTimeS = 0.f;
				game.yAtStartLock = tetrisPieceGetY(&piece);
				game.phase = LOCK;
			}
			break;
		case PATTERN:
			tetrisPieceMakeBlocks(&piece);
			tetrisNewHitList(&game.hitList,&game.blockMatrix);
			game.phase = ITERATE;
			break;
		case ITERATE:
			game.phase = ANIMATE;
			break;
		case ANIMATE:
			game.phase = ELIMINATE;
			break;
		case ELIMINATE:
			tetrisUseHitList(&game.hitList,&game.blockMatrix);
			game.phase = COMPLETION;
			break;
		case COMPLETION:
			game.phase = GENERATION;
			break;
	}
}

void tetrisPieceMakeBlocks(piece_t *p)
{
	for (int i=0;i<NUM_MINOS;i++)
	{
		game.blockMatrix[p->pos[i][0]][p->pos[i][1]]=p->type;
	}
}
int tetrisPieceGetY(piece_t *p)
{
	int result = 40;
	for (int i=0;i<NUM_MINOS;i++)
	{
		if (p->pos[i][1]<result)
			result = p->pos[i][1];
	}
	return result;
}
int tetrisPieceNewType()
{
	int result = game.nextQueue[0];
	if (game.amtBag == 0)
	{
		tetrisNewArray(&game.bag);
		game.amtBag = NUM_PIECES;
	}
	for (int i=1;i<NUM_PIECES;i++)
		game.nextQueue[i-1] = game.nextQueue[i];
	game.nextQueue[NUM_PIECES-1] = game.bag[NUM_PIECES-game.amtBag];
	game.amtBag--;
	return result;
}
void tetrisUseHitList(int(*hitlist)[H_MATRIX],int(*matrix)[W_MATRIX][H_MATRIX])
{
	for (int i=1;i<H_MATRIX;i++)
	{
		if ((*hitlist)[i])
		{
			for (int j=i;j<H_MATRIX-1;j++)
			{
				(*hitlist)[j]=(*hitlist)[j+1];
				for (int k=1;k<W_MATRIX;k++)
				{
					(*matrix)[k][j] = (*matrix)[k][j+1];
				}
			}
			i--;
		}
	}
}
void tetrisNewHitList(int(*hitlist)[H_MATRIX],int(*matrix)[W_MATRIX][H_MATRIX])
{
	for (int i=1;i<H_MATRIX;i++)
	{
		(*hitlist)[i]=0;
		int line = 0;
		for (int j=1;j<W_MATRIX;j++)
		{
			if ((*matrix)[j][i])
				line++;
		}
		if (line == W_MATRIX-1)
			(*hitlist)[i] = 1;
	}
}
void tetrisPiecePlace(piece_t *p)
{
	switch (p->type)
	{
		case TYPE_O:
			p->pos[0][0]=5;p->pos[0][1]=21;
			p->pos[1][0]=6;p->pos[1][1]=21;
			p->pos[2][0]=5;p->pos[2][1]=22;
			p->pos[3][0]=6;p->pos[3][1]=22;
			break;
		case TYPE_I:
			p->pos[0][0]=4;p->pos[0][1]=21;
			p->pos[1][0]=5;p->pos[1][1]=21;
			p->pos[2][0]=6;p->pos[2][1]=21;
			p->pos[3][0]=7;p->pos[3][1]=21;
			break;
		case TYPE_T:
			p->pos[0][0]=4;p->pos[0][1]=21;
			p->pos[1][0]=5;p->pos[1][1]=21;
			p->pos[2][0]=6;p->pos[2][1]=21;
			p->pos[3][0]=5;p->pos[3][1]=22;
			break;
		case TYPE_L:
			p->pos[0][0]=4;p->pos[0][1]=21;
			p->pos[1][0]=5;p->pos[1][1]=21;
			p->pos[2][0]=6;p->pos[2][1]=21;
			p->pos[3][0]=6;p->pos[3][1]=22;
			break;
		case TYPE_J:
			p->pos[0][0]=4;p->pos[0][1]=21;
			p->pos[1][0]=5;p->pos[1][1]=21;
			p->pos[2][0]=6;p->pos[2][1]=21;
			p->pos[3][0]=4;p->pos[3][1]=22;
			break;
		case TYPE_S:
			p->pos[0][0]=4;p->pos[0][1]=21;
			p->pos[1][0]=5;p->pos[1][1]=21;
			p->pos[2][0]=5;p->pos[2][1]=22;
			p->pos[3][0]=6;p->pos[3][1]=22;
			break;
		case TYPE_Z:
			p->pos[0][0]=5;p->pos[0][1]=21;
			p->pos[1][0]=6;p->pos[1][1]=21;
			p->pos[2][0]=4;p->pos[2][1]=22;
			p->pos[3][0]=5;p->pos[3][1]=22;
			break;
	}
}
void tetrisPieceNewPoints(int(*point)[NUM_SRPOINTS][2],piece_t *p)
{
	/* remember that greater y is higher */
	switch (p->type)
	{
		case TYPE_I:
			switch (p->facing)
			{
				case NORTH:
					(*point)[0][0]=1;(*point)[0][1]=0;
					(*point)[1][0]=0;(*point)[1][1]=0;
					(*point)[2][0]=3;(*point)[2][1]=0;
					(*point)[3][0]=0;(*point)[3][1]=0;
					(*point)[4][0]=3;(*point)[4][1]=0;
					break;
				case EAST:
					(*point)[0][0]=-1;(*point)[0][1]=-1;
					(*point)[1][0]=0;(*point)[1][1]=-1;
					(*point)[2][0]=0;(*point)[2][1]=-1;
					(*point)[3][0]=0;(*point)[3][1]=0;
					(*point)[4][0]=0;(*point)[4][1]=-3;
					break;
				case SOUTH:
					(*point)[0][0]=-2;(*point)[0][1]=1;
					(*point)[1][0]=0;(*point)[1][1]=1;
					(*point)[2][0]=-3;(*point)[2][1]=1;
					(*point)[3][0]=0;(*point)[3][1]=0;
					(*point)[4][0]=-3;(*point)[4][1]=0;
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=2;
					(*point)[1][0]=0;(*point)[1][1]=2;
					(*point)[2][0]=0;(*point)[2][1]=2;
					(*point)[3][0]=0;(*point)[3][1]=0;
					(*point)[4][0]=0;(*point)[4][1]=3;
			}
			break;
		case TYPE_T:
			switch (p->facing)
			{
				case NORTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=1;(*point)[i][1]=0;
					}
					break;
				case EAST:
					(*point)[0][0]=0;(*point)[0][1]=-1;
					(*point)[1][0]=1;(*point)[1][1]=-1;
					(*point)[2][0]=1;(*point)[2][1]=-2;
					(*point)[3][0]=0;(*point)[3][1]=1;
					(*point)[4][0]=1;(*point)[4][1]=1;
					break;
				case SOUTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=-1;(*point)[i][1]=0;
					}
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=1;
					(*point)[1][0]=-1;(*point)[1][1]=1;
					(*point)[2][0]=-1;(*point)[2][1]=0;
					(*point)[3][0]=0;(*point)[3][1]=3;
					(*point)[4][0]=-1;(*point)[4][1]=3;
			}
			break;
		case TYPE_L:
			switch (p->facing)
			{
				case NORTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=1;(*point)[i][1]=0;
					}
					break;
				case EAST:
					(*point)[0][0]=0;(*point)[0][1]=-1;
					(*point)[1][0]=1;(*point)[1][1]=-1;
					(*point)[2][0]=1;(*point)[2][1]=-2;
					(*point)[3][0]=0;(*point)[3][1]=1;
					(*point)[4][0]=1;(*point)[4][1]=1;
					break;
				case SOUTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=-1;(*point)[i][1]=0;
					}
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=1;
					(*point)[1][0]=-1;(*point)[1][1]=1;
					(*point)[2][0]=-1;(*point)[2][1]=0;
					(*point)[3][0]=0;(*point)[3][1]=3;
					(*point)[4][0]=-1;(*point)[4][1]=3;
			}
			break;
		case TYPE_J:
			switch (p->facing)
			{
				case NORTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=1;(*point)[i][1]=0;
					}
					break;
				case EAST:
					(*point)[0][0]=0;(*point)[0][1]=-1;
					(*point)[1][0]=1;(*point)[1][1]=-1;
					(*point)[2][0]=1;(*point)[2][1]=-2;
					(*point)[3][0]=0;(*point)[3][1]=1;
					(*point)[4][0]=1;(*point)[4][1]=1;
					break;
				case SOUTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=-1;(*point)[i][1]=0;
					}
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=1;
					(*point)[1][0]=-1;(*point)[1][1]=1;
					(*point)[2][0]=-1;(*point)[2][1]=0;
					(*point)[3][0]=0;(*point)[3][1]=3;
					(*point)[4][0]=-1;(*point)[4][1]=3;
			}
			break;
		case TYPE_S:
			switch (p->facing)
			{
				case NORTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=1;(*point)[i][1]=0;
					}
					break;
				case EAST:
					(*point)[0][0]=0;(*point)[0][1]=-1;
					(*point)[1][0]=1;(*point)[1][1]=-1;
					(*point)[2][0]=1;(*point)[2][1]=-2;
					(*point)[3][0]=0;(*point)[3][1]=1;
					(*point)[4][0]=1;(*point)[4][1]=1;
					break;
				case SOUTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=-1;(*point)[i][1]=0;
					}
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=1;
					(*point)[1][0]=-1;(*point)[1][1]=1;
					(*point)[2][0]=-1;(*point)[2][1]=0;
					(*point)[3][0]=0;(*point)[3][1]=3;
					(*point)[4][0]=-1;(*point)[4][1]=3;
			}
			break;
		case TYPE_Z:
			switch (p->facing)
			{
				case NORTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=0;(*point)[i][1]=0;
					}
					break;
				case EAST:
					(*point)[0][0]=0;(*point)[0][1]=0;
					(*point)[1][0]=1;(*point)[1][1]=0;
					(*point)[2][0]=1;(*point)[2][1]=-1;
					(*point)[3][0]=0;(*point)[3][1]=2;
					(*point)[4][0]=1;(*point)[4][1]=2;
					break;
				case SOUTH:
					for (int i=0;i<NUM_SRPOINTS;i++)
					{
						(*point)[i][0]=0;(*point)[i][1]=0;
					}
					break;
				case WEST:
					(*point)[0][0]=0;(*point)[0][1]=0;
					(*point)[1][0]=-1;(*point)[1][1]=0;
					(*point)[2][0]=-1;(*point)[2][1]=-1;
					(*point)[3][0]=0;(*point)[3][1]=2;
					(*point)[4][0]=-1;(*point)[4][1]=2;
			}

	}
}
int tetrisPieceCollision(piece_t *p,int dx,int dy)
{
	int result = 0;
	for (int i=0;i<NUM_MINOS;i++)
	{
		if ((p->pos[i][0]+dx==0)
		|| (p->pos[i][0]+dx==W_MATRIX)
		|| (p->pos[i][1]+dy==0)
		|| (p->pos[i][1]+dy==H_MATRIX))
			return 1;
		result += game.blockMatrix[p->pos[i][0]+dx][p->pos[i][1]+dy];
	}
	return result;
}
void tetrisPieceMove(piece_t *p,int dx,int dy)
{
	for (int i=0;i<NUM_MINOS;i++)
	{
		p->pos[i][0]+=dx;
		p->pos[i][1]+=dy;
	}
}

double dist(int a[2],int b[2])
{
	int result = (b[0]-a[0])*(b[0]-a[0]);
	result += (b[1]-a[1])*(b[1]-a[1]);
	return sqrt(abs(result));
}

int xoffset(piece_t *p,int mino)
{
	switch (p->type)
	{
		case TYPE_O:
			return (mino==1)*1+(mino==2)*0+(mino==3)*1;
		case TYPE_I:
			return (mino==1)*1+(mino==2)*2+(mino==3)*3;
		case TYPE_T:
			return (mino==1)*1+(mino==2)*2+(mino==3)*1;
		case TYPE_L:
			return (mino==1)*1+(mino==2)*2+(mino==3)*2;
		case TYPE_J:
			return (mino==1)*1+(mino==2)*2+(mino==3)*0;
		case TYPE_S:
			return (mino==1)*1+(mino==2)*1+(mino==3)*2;
		case TYPE_Z:
			return (mino==1)*1+(mino==2)*-1+(mino==3)*0;
	}
	return 0;
}
int yoffset(piece_t *p,int mino)
{
	switch (p->type)
	{
		case TYPE_O:
			return (mino==1)*0+(mino==2)*1+(mino==3)*1;
		case TYPE_I:
			return 0;
		case TYPE_T:
		case TYPE_L:
		case TYPE_J:
			return (mino==1)*0+(mino==2)*0+(mino==3)*1;
		case TYPE_S:
		case TYPE_Z:
			return (mino==1)*0+(mino==2)*1+(mino==3)*1;
	}
	return 0;
}

void tetrisPieceTryRotate(piece_t *p,int counterclockwise)
{
	piece_t p1 = *p;

	/* change facing */
	p1.facing += counterclockwise*-2+1;
	p1.facing -= (p1.facing==4)*4;
	p1.facing += (p1.facing==-1)*4;
	
	/* rotate piece about first mino */
	for (int i=1;i<NUM_MINOS;i++)
	{
		p1.pos[i][0]=p1.pos[0][0]
		+(int)cos(p1.facing*90*PI/180)*xoffset(p,i)
		+(int)sin(p1.facing*90*PI/180)*yoffset(p,i);
		p1.pos[i][1]=p1.pos[0][1]
		+(int)cos(p1.facing*90*PI/180)*yoffset(p,i)
		-(int)sin(p1.facing*90*PI/180)*xoffset(p,i);
	}

	/* rotation points */
	for (int i=0;i<NUM_SRPOINTS;i++)
	{
		piece_t p2 = p1;
		tetrisPieceNewPoints(&p2.point,&p2);
		tetrisPieceMove(&p2,p1.point[i][0],p1.point[i][1]);
		if (!tetrisPieceCollision(&p2,-p2.point[i][0],-p2.point[i][1]))
		{
			tetrisPieceMove(&p2,-p2.point[i][0],-p2.point[i][1]);
			*p = p2;
			return;
		}
	}
}



void tetrisGfxUpdate()
{

	gfx.blocks = sfmlVertexArray_createTetrisBlocks(MATX_ORIGIN,MATY_ORIGIN,&game.blockMatrix,TILE_SIZE);	
	gfx.piece = sfmlVertexArray_createTetrisPiece(MATX_ORIGIN,MATY_ORIGIN,&piece.pos,TILE_SIZE);
}
