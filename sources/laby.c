/*
 * SAI project - 3D Laby
 * File : laby.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "laby.h"
#include "stack.h"
 
/*
 * Maze allocation.
  */
Laby *laby_new(void)
{
	int x = 0;
	Laby *laby;

	if ((laby = malloc(sizeof *laby)) == NULL)
	{
		return NULL;
	}

	if ((laby->matrix = calloc(SIZE, sizeof(char))) == NULL)
	{
		return NULL;
	}

	while (IS_IN(x))
	{
		laby->matrix[x] = '1';
		++x;
	}

	laby->end = 0;
return laby;
}

void laby_free(Laby *laby)
{
	free(laby->matrix);
	free(laby);
}


Laby *maze_generation(Laby *laby)
{
	int i, x, y;
	int rand_cell, rand_room_width, rand_room_height;
	int cell_x, cell_y;
	Stack *stack = stack_new();
	/*
	 * We start the maze generation from the position 0.
	  */
	stack = stack_push(stack, 0);
	
	/*
	 * We generate some rooms in the middle of the maze.
	  */
	for (i = 0; i < 3; ++i)
	{

		rand_room_width = (rand() % ((WIDTH / 2) - (WIDTH / 10))) + (WIDTH / 10);
		rand_room_height = (rand() % ((HEIGHT / 2) - (HEIGHT / 10))) +  (HEIGHT / 10);
		rand_room_width = rand_room_width / 2;
		rand_room_height = rand_room_height / 2;

		rand_cell = rand() % (SIZE - 1);
		cell_x = COLUMN(rand_cell);
		cell_y = LINE(rand_cell);

		fprintf(stderr, "%d %d, %d %d\n",
			cell_x, cell_y, rand_room_width, rand_room_height );
		for (x = cell_x - rand_room_width ; x <=  cell_x + rand_room_width; ++x)
		{
			for (y = cell_y - rand_room_height; y <= cell_y + rand_room_height; ++y)
			{
				if (IS_IN(COORD(x,y)) && !BORDER(COORD(x,y)))
				{
					laby->matrix[COORD(x,y)] = '0';
				}
			}
		}
	}
stack_free(stack);
return laby;
}

void laby_print(Laby *laby)
{
	int x,y;
	fprintf(stdout, "%i : %d\n",HEIGHT, WIDTH);

	for (x = 0; x < WIDTH; ++x)
	{
		fprintf(stdout, "─");
	}
	fprintf(stdout, "→ X\n");

	for (y = HEIGHT - 1; y >= 0; --y)
	{
		fprintf(stdout, "│");
		for (x = 0; x < WIDTH; ++x)
		{
			if (laby->matrix[(y * WIDTH) + x] == '1')
			{
				fprintf(stdout, "%c", laby->matrix[(y * WIDTH) + x]);
			} else {
				fprintf(stdout, " ");
			}
		}
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "↓\nY\n");

	fprintf(stdout, "\n");
}