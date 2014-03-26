/*
 * SAI project - 3D Laby
 * File : laby.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include "laby.h"
#include "stack.h"
 
/*
 * Maze allocation.
  */
Laby *laby_new(void)
{
	Laby *laby;

	if ((laby = malloc(sizeof *laby)) == NULL)
	{
		return NULL;
	}

	if ((laby->matrix = calloc(SIZE, sizeof(char))) == NULL)
	{
		return NULL;
	}
return laby;
}

void laby_free(Laby *laby)
{
	free(laby->matrix);
	free(laby);
}

