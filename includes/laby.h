/*
 * SAI project - 3D Laby
 * File : laby.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#ifndef _LABY
#define _LABY

#define WIDTH 	40
#define HEIGHT 	40
#define SIZE 	(WIDTH * HEIGHT)

#define COORD(I,J) 		((I >= 0 && I <= HEIGHT && J >= 0 && J <= WIDTH) ? (I * WIDTH)+J : -1)
#define LINE(X)			(X / WIDTH)
#define COLUMN(X)		(X % WIDTH)
#define END_RIGHT(X) 	(X % WIDTH == WIDTH - 1)
#define END_LEFT(X) 	(X % WIDTH == 0)
#define END_TOP(X)		(X / WIDTH == HEIGHT - 1)
#define END_BOTTOM(X)	(X / WIDTH == 0)
#define IS_IN(X)		(X >= 0  && X < SIZE)
#define BORDER(X)		(END_TOP(X) || END_BOTTOM(X) || END_RIGHT(X) || END_LEFT(X))
typedef struct Laby
{
	int end;
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
 * '1' will represente a wall. '0' somewhere the user can go.
  */
Laby *maze_generation(Laby *laby);

/* 
 * Print the maze in the standard output.
  */
void laby_print(Laby *laby);
#endif