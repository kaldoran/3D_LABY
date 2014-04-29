/*
 * SAI project - 3D Laby
 * File : laby.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#ifndef __LABY
#define __LABY

/* WIDTH and HEIGHT must be odd numbers. */
#define WIDTH               51
#define HEIGHT              51
#define SIZE                (WIDTH * HEIGHT)

#define COORD(I,J)          (((J) * WIDTH) + (I))
#define LINE(X)             (X / WIDTH)
#define COLUMN(X)           (X % WIDTH)

#define END_RIGHT(X)        (X % WIDTH == WIDTH - 1)
#define END_LEFT(X)         (X % WIDTH == 0)
#define END_TOP(X)          (X / WIDTH == HEIGHT - 1)
#define END_BOTTOM(X)       (X / WIDTH == 0)

#define BORDER              100
#define PASS                101
#define ENTRY               102
#define EXIT                103
#define WALL                104
#define MOVING_WALL         105
#define SPIKES              106
#define SPIKES_MW		   107

#define IS_IN(X)            (X >= 0  && X < SIZE)
#define IS_BORDER(X)        (END_TOP(X) || END_BOTTOM(X) || END_RIGHT(X) || END_LEFT(X))
#define IS_PLAYABLE(X)      (IS_IN(X) && (laby->matrix[X] == PASS || laby->matrix[X] == ENTRY || laby->matrix[X] == EXIT || laby->matrix[X] == SPIKES))
#define IS_WALL(X)          (IS_IN(X) && (laby->matrix[X] == WALL || laby->matrix[X] == MOVING_WALL || laby->matrix[X] == SPIKES_MW))
#define IS_WALL_OR_OUT(X)   (!IS_IN(X) || IS_WALL(X))
#define IS_EXIT(X)          (IS_IN(X) && (laby->matrix[X] == EXIT))
#define IS_SPIKES(X)        (IS_IN(X) && (laby->matrix[X] == SPIKES))

typedef struct Laby
{
    char *matrix;
} Laby;

/* Global laby */
Laby *laby;

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
  */
void maze_generation(void);
void maze_carving(int x, int y);
void maze_moving_walls_generation(void);

/* 
 * Print the maze in the standard output.
  */
void laby_print(void);

int laby_is_playable(void);
#endif
