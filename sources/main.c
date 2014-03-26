/*
 * SAI project - 3D Laby
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "laby.h"
#include "stack.h"

int main(int argc, char *argv[])
{
	Laby *laby = laby_new();
	Stack *stack = stack_new();

	stack = stack_push(stack, 2);
	stack = stack_push(stack, 5);
	stack = stack_pop(stack);
	stack = stack_push(stack, 3);
	stack = stack_pop(stack);
	stack = stack_pop(stack);

	if (stack->last != NULL)
	{
		fprintf(stderr, "%i : %i\n", (stack->last)->value, stack->size);
	}

	stack_free(stack);
	laby_free(laby);
	return 0;
}