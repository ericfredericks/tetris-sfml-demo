#include "tetrissfml.h"

/*
sfVertexArray* sfmlVertexArray_createGrid(int x,int y,int width,int height,int size)
{
	sfVertexArray *va = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(va,sfLines);
	for (int i=0;i<height+1;i++)
	{
		sfVertex v1 = {(sfVector2f){left,top+i*size},sfRed,(sfVector2f){0.f,0.f}};
		sfVertex v2 = {(sfVector2f){left+width*size,top+i*size},sfRed,(sfVector2f){0.f,0.f}};
		sfVertexArray_append(va,v1);
		sfVertexArray_append(va,v2);
	}
	for (int i=0;i<width+1;i++)
	{
		sfVertex v3 = {(sfVector2f){left+i*size,top},sfRed,(sfVector2f){0.f,0.f}};
		sfVertex v4 = {(sfVector2f){left+i*size,top+height*size},sfRed,(sfVector2f){0.f,0.f}};
		sfVertexArray_append(va,v3);
		sfVertexArray_append(va,v4);
	}
	return va;
}
*/

sfVertexArray* sfmlVertexArray_createTetrisMatrix(int x,int y,int size)
{
	sfVertexArray *va = sfVertexArray_create();
	for (int i=1;i<H_MATRIX;i++)
	{
		for (int j=1;j<W_MATRIX;j++)
		{
			sfVertex v = {(sfVector2f){x+(j-1)*size,y-(i-1)*size},sfRed,(sfVector2f){0.f,0.f}};
			sfVertexArray_append(va,v);
		}
	}
	return va;
}

sfVertexArray* sfmlVertexArray_createTetrisBlocks(int x,int y,int (*block)[W_MATRIX][H_MATRIX],int size)
{
	sfVertexArray *va = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(va,sfQuads);
	for (int i=1;i<H_MATRIX;i++)
	{
		for (int j=1;j<W_MATRIX;j++)
		{
			if ((*block)[j][i])
			{
				sfVertex v1 = {(sfVector2f){x+(j-1)*size,y-i*size},sfBlue,(sfVector2f){0.f,0.f}};
				sfVertex v2 = {(sfVector2f){x+j*size,y-i*size},sfBlue,(sfVector2f){0.f,0.f}};
				sfVertex v3 = {(sfVector2f){x+j*size,y-(i-1)*size},sfBlue,(sfVector2f){0.f,0.f}};
				sfVertex v4 = {(sfVector2f){x+(j-1)*size,y-(i-1)*size},sfBlue,(sfVector2f){0.f,0.f}};
				sfVertexArray_append(va,v1);
				sfVertexArray_append(va,v2);
				sfVertexArray_append(va,v3);
				sfVertexArray_append(va,v4);
			}
		}
	}
	return va;
}

sfVertexArray* sfmlVertexArray_createTetrisPiece(int x,int y,int (*pos)[NUM_MINOS][2],int size)
{
	sfVertexArray *va = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(va,sfQuads);
	for (int i=0;i<NUM_MINOS;i++)
	{
		sfVertex v1 = {(sfVector2f){x+((*pos)[i][0]-1)*size,y-((*pos)[i][1])*size},sfGreen,(sfVector2f){0.f,0.f}};
		sfVertex v2 = {(sfVector2f){x+((*pos)[i][0])*size,y-((*pos)[i][1])*size},sfGreen,(sfVector2f){0.f,0.f}};
		sfVertex v3 = {(sfVector2f){x+((*pos)[i][0])*size,y-((*pos)[i][1]-1)*size},sfGreen,(sfVector2f){0.f,0.f}};
		sfVertex v4 = {(sfVector2f){x+((*pos)[i][0]-1)*size,y-((*pos)[i][1]-1)*size},sfGreen,(sfVector2f){0.f,0.f}};
		sfVertexArray_append(va,v1);
		sfVertexArray_append(va,v2);
		sfVertexArray_append(va,v3);
		sfVertexArray_append(va,v4);
	}
	sfVertexArray_getVertex(va,0)->color = sfMagenta;
	return va;
}
