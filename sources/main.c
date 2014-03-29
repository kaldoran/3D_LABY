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

int main(int argc, char *argv[])
{
	Stack *stack = stack_new();
	Laby *laby = laby_new();

	stack = stack_push(stack, 2);
	stack = stack_push(stack, 5);
	stack = stack_pop(stack);
	stack = stack_push(stack, 3);
	stack = stack_pop(stack);
	stack = stack_pop(stack);

	laby = maze_generation(laby);
	laby_print(laby);

	stack_free(stack);
	laby_free(laby);
	return 0;
}