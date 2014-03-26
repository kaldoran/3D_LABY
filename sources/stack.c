/*
 * SAI project - 3D Laby
 * File : stack.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

Node *node_new()
{
	Node *n;

	if ((n = malloc(sizeof *n)) == NULL)
	{
		/*
		 * Bad allocation.
		  */
		return NULL;
	}
	n->next = NULL;
return n;
}

void node_free(Node *n)
{
	if (n == NULL)
	{
		return;
	}

	if (n->next != NULL)
	{
		node_free(n->next);
	}
free(n);
}

Stack *stack_new(void)
{
	Stack *stack;

	if ((stack = malloc(sizeof *stack)) == NULL)
	{
		/*
		 * Bad allocation.
		  */
		return NULL;
	}

	stack->last = NULL;
	stack->size = 0;
return stack;
}

void stack_free(Stack *stack)
{
	if (stack == NULL)
	{
		return;
	}

	if (stack->last != NULL)
	{
		node_free(stack->last);
	}
free(stack);
}

Stack *stack_push(Stack *stack, int value)
{
	Node *node;

	if ((node = node_new()) == NULL)
	{
		/*
		 * Bad allocation.
		  */
		return NULL;
	}

	if (stack == NULL)
	{
		fprintf(stderr, "You need to initialise the stack before puting some values in.\n");
		return NULL;
	}

	node->value = value;
	node->next = stack->last;
	stack->last = node;
	++(stack->size);

return stack;
}

Stack *stack_pop(Stack *stack)
{
	Node *node;

	if (stack == NULL)
	{
		return NULL;
	}

	if (stack->last == NULL)
	{
		return stack;
	}

	if ((stack->last)->next == NULL)
	{
		node = stack->last;
		stack->last = NULL;
		stack->size = 0;
		free(node);
		return stack;
	}

	node = stack->last;
	stack->last = node->next;
	--(stack->size);

	free(node);
return stack;
}