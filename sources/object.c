/*
 * SAI project - 3D Laby
 * File : object.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <time.h>
 
#include "laby.h"
#include "config.h"
#include "object.h"
#include "display.h"
/*
 * New Point allocation.
  */
Point *point_new(float x, float y, float z)
{
	Point *p;
	if ((p = malloc(sizeof *p)) == NULL)
	{
		return NULL;
	}
	
	p->x = x;
	p->y = y;
	p->z = z;
return p;
}

void point_free(Point *p)
{
	if(p != NULL)
	{
		free(p);
	}
}


Object *object_new(float x, float y, float z, unsigned int type)
{
	Object *object;

	if ((object = malloc(sizeof *object)) == NULL)
	{
		return NULL;
	}
	object->anchor = point_new(x, y, z);
	object->type = type;
return object;
}

void object_free(Object *object)
{
	if (object != NULL)
	{
		point_free(object->anchor);
		free(object);
	}
}

Doubly_linked_node *doubly_linked_node_new(void)
{
	Doubly_linked_node *node;

	if ((node = malloc(sizeof *node)) == NULL)
	{
		return NULL;
	}
	node->object             = NULL;
	node->previous           = NULL;
	node->next               = NULL;
return node;
}

/*
 * Use it on the last node of the list
  */
void doubly_linked_node_free(Doubly_linked_node *node)
{
	if (node != NULL)
	{
		if (node->next != NULL)
		{
			doubly_linked_node_free(node->next);
		}
		object_free(node->object);
		free(node);
	}
}

Object_list *object_list_new(void)
{
	Object_list *ol;
	if ((ol = malloc(sizeof *ol)) == NULL)
	{
		return NULL;
	}
	ol->first       = NULL;
	ol->last        = NULL;
	ol->size       = 0;
return ol;
}

void object_list_free(Object_list *ol)
{
	if (ol != NULL)
	{
		if (ol->last != NULL)
		{
			doubly_linked_node_free(ol->last);
		}
		free(ol);
	}
}

Object_list *object_list_push(Object_list *ol, Object *object)
{
	Doubly_linked_node *node;

	if ((node = doubly_linked_node_new()) == NULL)
	{
		return NULL;
	}

	if (ol == NULL)
	{
		return NULL;
	}

	if (ol->size == 0)
	{
		node->object   = object;
		node->previous = NULL;
		node->next     = NULL;

		ol->last       = node;
		ol->first      = node;
		++(ol->size);
	} else if (ol->size == 1) {
		node->object          = object;
		node->previous        = NULL;
		node->next            = ol->first;

		(ol->first)->previous = node;
		ol->last              = node;
		++(ol->size);
	} else {
		node->object         = object;
		node->previous       = NULL;
		node->next           = ol->last;

		(ol->last)->previous = node;
		(ol->last)           = node;
		++(ol->size);
	}
return ol;
}

Object_list *object_list_shift(Object_list *ol)
{
	Doubly_linked_node *node;

	if (ol == NULL)
	{
		return NULL;
	}

	if (ol->first == NULL)
	{
		return NULL;
	}

	if (ol->size == 1)
	{
		node      = ol->first;
		ol->first = NULL;
		ol->last  = NULL;
		--(ol->size);
		doubly_linked_node_free(node);
	} else {
		node = ol->first;
		ol->first = (ol->first)->previous;
		(ol->first)->next = NULL;
		doubly_linked_node_free(node);
	}
return ol;
}

Object_list *object_list_generate_fir_trees(Object_list *ol)
{
	int x, y, step = CELL_SIZE / 5;
	int density = 4, gap = CELL_SIZE, space = 5 * CELL_SIZE;
	srand(time(NULL));
	for (x = -gap; x > -space ; x -= CELL_SIZE)
	{
		for (y = -space; y < (HEIGHT * CELL_SIZE) + space; y += CELL_SIZE)
		{
			if (rand() % density == 1)
			{
				ol = object_list_push_object(ol, x, y, (rand() % 8) + 7, FIR_TREE);
				y += step;
			}
		}
	}

	for (x = 0; x < WIDTH * CELL_SIZE; x += CELL_SIZE)
	{
		for (y = -gap; y > -space; y -= CELL_SIZE)
		{
			if (rand() % density == 1)
			{
				ol = object_list_push_object(ol, x, y, (rand() % 8) + 7, FIR_TREE);
				x += step;
			}
		}
	}

	for (x = (WIDTH * CELL_SIZE) + gap; x < (WIDTH * CELL_SIZE) + space ; x += CELL_SIZE)
	{
		for (y = -space; y < (HEIGHT * CELL_SIZE) + space; y += CELL_SIZE)
		{
			if (rand() % density == 1)
			{
				ol = object_list_push_object(ol, x, y, (rand() % 8) + 7, FIR_TREE);
				y += step;
			}
		}
	}

	for (x = 0; x < WIDTH * CELL_SIZE; x += CELL_SIZE)
	{
		for (y = (HEIGHT * CELL_SIZE) + gap; y < (HEIGHT * CELL_SIZE) + space; y += CELL_SIZE)
		{
			if (rand() % density == 1)
			{
				ol = object_list_push_object(ol, x, y, (rand() % 8) + 7, FIR_TREE);
				x += step;
			}
		}
	}
return ol;
}

Object_list *object_list_push_maze_walls(Object_list *ol)
{
	int x, y;

	for (x = 0; x < WIDTH; ++x)
	{
		for (y = 0; y < HEIGHT; ++y)
		{
			if (laby->matrix[COORD(x,y)] == WALL)
			{
				ol = object_list_push_object(ol, x * CELL_SIZE, y * CELL_SIZE, 0, WALL);
			} else if (laby->matrix[COORD(x,y)] == ENTRY) {
				ol = object_list_push_object(ol, x * CELL_SIZE, y * CELL_SIZE, 0, ENTRY);
			} else if (laby->matrix[COORD(x,y)] == EXIT) {
				ol = object_list_push_object(ol, x * CELL_SIZE, y * CELL_SIZE, 0, EXIT);
			} else if (laby->matrix[COORD(x,y)] == MOVING_WALL) {
				ol = object_list_push_object(ol, x * CELL_SIZE, y * CELL_SIZE, 0, MOVING_WALL);
			}
		}
	}
return ol;
}

Object_list *object_list_generate_spikes(Object_list *ol) {
	int x, y;

	for (x = 0; x < WIDTH; ++x) {
		for (y = 0; y < HEIGHT; ++y) {
			if (laby->matrix[COORD(x,y)] == PASS && rand() % 27 == 0) {
				laby->matrix[COORD(x,y)] = SPIKES;
				ol = object_list_push_object(ol, x * CELL_SIZE, y * CELL_SIZE, SPIKE_HEIGHT, SPIKES);
			}
		}
	}
	return ol;
}

Object_list *object_list_push_object(Object_list *ol, float x, float y, float z, unsigned int type)
{
	Doubly_linked_node *node;
	Object *object = object_new(x, y, z, type);

	if ((node = doubly_linked_node_new()) == NULL)
	{
		return NULL;
	}

	if (ol == NULL)
	{
		return NULL;
	}

	if (ol->size == 0)
	{
		node->object   = object;
		node->previous = NULL;
		node->next     = NULL;

		ol->last       = node;
		ol->first      = node;
		++(ol->size);
	} else if (ol->size == 1) {
		node->object          = object;
		node->previous        = NULL;
		node->next            = ol->first;

		(ol->first)->previous = node;
		ol->last              = node;
		++(ol->size);
	} else {
		node->object         = object;
		node->previous       = NULL;
		node->next           = ol->last;

		(ol->last)->previous = node;
		(ol->last)           = node;
		++(ol->size);
	}
return ol;
}

void object_list_display(Object_list *ol)
{
	Doubly_linked_node *iterator = doubly_linked_node_new();

	if (ol == NULL || ol->size == 0)
	{
		return;
	}

	iterator = ol->last;
	while (1)
	{
		switch ((iterator->object)->type)
		{
			case FLOOR:
				Object_floor_print();
			break;
			case BORDER:
				Object_border_print();
			break;
			case SUN:
				Object_sun_print(iterator->object);
			break;
			case FIR_TREE:
				Object_fir_tree_print(iterator->object);
			break;
			case WALL:
				Object_wall_print(iterator->object);
			break;
			case MOVING_WALL:
				Object_moving_wall_print(iterator->object);
				/*Object_wall_print(iterator->object);*/
			break;
			case ENTRY:
				Object_entry_print(iterator->object);
			break;
			case EXIT:
				Object_exit_print(iterator->object);
			break;
			case SPIKES:
				Object_spikes_print(iterator->object);
			break;
			case TEAPOT:
				Object_teapot_print(iterator->object);
			break;
			default:
			break;
		}
		if (iterator->next != NULL)
		{
			iterator = iterator->next;
		} else {
			break;
		}
	}
}
