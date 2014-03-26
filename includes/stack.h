/*
 * SAI project - 3D Laby
 * File : stack.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#ifndef _STACK
#define _STACK

/*
 * In order to generate the maze we need a stack.
  */
typedef struct Node
{
	int value;
	struct Node *next;
} Node;

typedef struct Stack
{
	int size;
	Node *last;
} Stack;

/* Create an element. */
Node *node_new(void);

/* Free the element given. */
void node_free(Node *e);

/* Create a new empty stack. */
Stack *stack_new(void);

/* Free the given stack. */
void stack_free(Stack *stack);

/* Add a new element at the end of the
 * given stack. */
Stack *stack_push(Stack *stack, int value);

/* Remove the element at the end of the
 * given stack. */
Stack *stack_pop(Stack *stack);
#endif