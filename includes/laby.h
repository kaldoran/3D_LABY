/*
 * SAI project - 3D Laby
 * File : laby.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#ifndef __LABY
#define __LABY

/* WIDTH and HEIGHT must be odd numbers. */
#define WIDTH 	51
#define HEIGHT 	51
#define SIZE 	(WIDTH * HEIGHT)

#define COORD(I,J) 			((J * WIDTH) + I)
#define LINE(X)				(X / WIDTH)
#define COLUMN(X)			(X % WIDTH)

#define END_RIGHT(X) 		(X % WIDTH == WIDTH - 1)
#define END_LEFT(X) 		(X % WIDTH == 0)
#define END_TOP(X)			(X / WIDTH == HEIGHT - 1)
#define END_BOTTOM(X)		(X / WIDTH == 0)

#define BORDER 		100
#define PASS		101
#define ENTRY 		102
#define EXIT		103
#define WALL 		104

#define IS_IN(X)			(X >= 0  && X < SIZE)
#define IS_BORDER(X)		(END_TOP(X) || END_BOTTOM(X) || END_RIGHT(X) || END_LEFT(X))
#define IS_PLAYABLE(X)		(IS_IN(X) && (laby->matrix[X] == PASS || laby->matrix[X] == ENTRY || laby->matrix[X] == EXIT))
#define IS_WALL(X)			(IS_IN(X) && laby->matrix[X] == WALL)
#define IS_WALL_OR_OUT(X) 	(!IS_IN(X) || IS_WALL(X))

typedef struct Laby
{
	char *matrix;
} Laby;

/*
 * Create an empty labyrinth.
 * Return a maze or NULL in case of failure.
  */
Laby *laby_new(void);

/*
 * Free the memory of the labyrinth given.
  */
void laby_free(Laby *laby);

/*
 * Maze generation.
 * Take an empty labyrith and create walls in it by
 * modifying the matrix.
 * 1 will represente a wall, 0 somewhere the user can go.
 * 2 is the entry. 3 is the exit.
  */
Laby *maze_generation(Laby *laby);
Laby *maze_carving(Laby *laby, int x, int y);

/* 
 * Print the maze in the standard output.
  */
void laby_print(Laby *laby);
#endif