/*
 * SAI project - 3D Laby
 * File : laby.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
 *
 * Thanks to : https://github.com/joewing/maze for the maze generation.
  */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "laby.h"
#include "object.h"
#include "config.h"
 
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
		laby->matrix[x++] = WALL;
	}
return laby;
}

void laby_free(Laby *laby)
{
	if(laby->matrix != NULL)
	{
		free(laby->matrix);
	}
	if (laby != NULL)
	{
		free(laby);
	}
}

void maze_carving (int x, int y)
{
	int x1, y1, x2, y2, dx, dy;
	int direction = rand() % 4, count = 0;

	while (count < 4)
	{
		dx = 0;
		dy = 0;
		switch (direction)
		{
			case 0:
				dx = 1;
			break;
			case 1:
				dy = 1;
			break;
			case 2:
				dx = -1;
			break;
			default:
				dy = -1;
			break;
		}

		x1 = x + dx;
		y1 = y + dy;
		x2 = x1 + dx;
		y2 = y1 + dy;

		if (!IS_BORDER(COORD(x2,y2)) && IS_WALL(COORD(x2,y2)) && IS_WALL(COORD(x1,y1)))
		{
			laby->matrix[COORD(x1,y1)] = PASS;
			laby->matrix[COORD(x2,y2)] = PASS;
			x = x2;
			y = y2;
			direction = rand() % 4;
			count = 0;
		} else {
			direction = (direction + 1) % 4;
			++count;
		}
	}
}

void maze_generation(void)
{
	int i, x, y;
	int rand_room_width, rand_room_height;
	int cell_x, cell_y;

	srand(time(0));
	laby->matrix[WIDTH + 1] = PASS;
	laby->matrix[WIDTH] = PASS;

	for (x = 1; x < WIDTH; x += 2)
	{
		for (y = 1; y < HEIGHT; y += 2)
		{
			maze_carving(x, y);
		}
	}

	/*
	 * We generate some rooms at borders of the maze.
	  */
	for (i = 0; i < 6; ++i)
	{
		rand_room_width  = (rand() % (3)) + 2;
		rand_room_height = (rand() % (3)) + 2;
		rand_room_width  = rand_room_width / 2;
		rand_room_height = rand_room_height / 2;
		
		rand_room_width  = (rand_room_width == 0) ? 1 : rand_room_width;
		rand_room_height = (rand_room_height == 0) ? 1 : rand_room_height;

		do {
			cell_x = rand() % WIDTH;
			cell_y = rand() % HEIGHT;
		} while (!IS_BORDER(COORD(cell_x,cell_y)));

		for (x = cell_x - rand_room_width ; x <=  cell_x + rand_room_width; ++x)
		{
			for (y = cell_y - rand_room_height; y <= cell_y + rand_room_height; ++y)
			{
				if (IS_IN(COORD(x,y)))
				{
					laby->matrix[COORD(x,y)] = PASS;
				}
			}
		}
	}

	laby->matrix[0] = ENTRY;
	laby->matrix[COORD((WIDTH - 2), (HEIGHT - 1))] = EXIT;
}

void laby_print(void)
{
	int x,y;
	fprintf(stdout, "%i : %d\n",HEIGHT, WIDTH);


	fprintf(stdout, "↑Y\n");

	for (y = HEIGHT - 1; y >= 0; --y)
	{
		fprintf(stdout, "│");
		for (x = 0; x < WIDTH; ++x)
		{
			if (laby->matrix[COORD(x,y)] == WALL)
			{
				fprintf(stdout, "█");
			} else if (laby->matrix[COORD(x,y)] == MOVING_WALL) {
				fprintf(stdout, "?");
			} else if (laby->matrix[COORD(x,y)] == PASS) {
				fprintf(stdout, "░");
			} else if (laby->matrix[COORD(x,y)] == ENTRY || laby->matrix[COORD(x,y)] == EXIT) {
				fprintf(stdout, "X");
			} else if (laby->matrix[COORD(x,y)] == SPIKES) {
				fprintf(stdout, "^");
			}
			
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "└");

	for (x = 0; x < WIDTH; ++x)
	{
		fprintf(stdout, "─");
	}

	fprintf(stdout, "→ X\n");
	fprintf(stdout, "(0,0)\n");
}

void maze_moving_walls_generation(void)
{
	int x, y;
	for (x = 1; x < WIDTH; ++x)
	{
		for (y = 1; y < HEIGHT; ++y)
		{
			if (laby->matrix[COORD(x,y)] == PASS && rand() % SIZE <= 10)
			{
				laby->matrix[COORD(x,y)] = MOVING_WALL;
				mwl = object_list_push_object(mwl, x * CELL_SIZE, y * CELL_SIZE, SPIKE_HEIGHT, MOVING_WALL);
			}
		}
	}
	fprintf(stderr, "\n");
}
