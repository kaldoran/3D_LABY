/*
 * SAI project - 3D Laby
 * File : laby.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#ifndef _LABY
#define _LABY

#define WIDTH 	20
#define HEIGHT 	20
#define COORD(I,J) ((I >= 0 && I <= HEIGHT && J >= 0 && J <= WIDTH) ? (I*WIDTH)+J : -1)
#define SIZE (WIDTH * HEIGHT)

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
 * '1' will represente a wall. '0' somewhere the user
 * can go.
  */
Laby *maze_generation(Laby *laby);


#endif