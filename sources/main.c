/*
 * SAI project - 3D Laby
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "laby.h"
#include "stack.h"
#include "k-tree.h"

int main(int argc, char *argv[])
{
	Stack *stack = stack_new();
	Laby *laby = laby_new();

	laby = maze_generation(laby);
	

	stack_free(stack);
	laby_free(laby);
	return 0;
}