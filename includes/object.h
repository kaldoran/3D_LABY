/*
 * SAI project - 3D Laby
 * File : object.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "k-tree.h"
 
#ifndef __OBJECT
#define __OBJECT

#define FLOOR		0
#define FIR_TREE 	1
#define SUN			2
#define TEAPOT		418

typedef struct Object
{
	Point *anchor;
	unsigned int type;
} Object;

typedef struct Doubly_linked_node
{
	Object *object;
	struct Doubly_linked_node *previous;
	struct Doubly_linked_node *next;
} Doubly_linked_node;

typedef struct Object_list
{
	Doubly_linked_node *first;
	Doubly_linked_node *last;
	unsigned int size;
} Object_list;

/* Global list */
Object_list *ol;

Object *object_new(float x, float y, float z, unsigned int type);
void object_free(Object *object);

Doubly_linked_node *doubly_linked_node_new();
void doubly_linked_node_free(Doubly_linked_node *node);

Object_list *object_list_new();
void object_list_free(Object_list *ol);

/* Add an object at the end of the list.  */
Object_list *object_list_push(Object_list *ol, Object *object);

/*  Shift the first element of the list. */
Object_list *object_list_shift(Object_list *ol);

Object_list *object_list_push_maze_walls(Object_list *ol);
Object_list *object_list_generate_fir_trees(Object_list *ol);
Object_list *object_list_push_object(Object_list *ol, float x, float y, float z, unsigned int type);

void time_color();
void Object_floor_print();
void Object_border_print();
void Object_sun_print(Object *sun);
void Object_fir_tree_print(Object *fir_tree);
void Object_wall_print(Object *wall);
void Object_moving_wall_print(Object *wall);
void Object_entry_print(Object *object);
void Object_exit_print(Object *object);
void Object_teapot_print(Object *teapot);

#endif