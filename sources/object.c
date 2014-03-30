/*
 * SAI project - 3D Laby
 * File : object.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <time.h>
#include "laby.h"
#include "config.h"
#include "k-tree.h"
#include "object.h"

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

Doubly_linked_node *doubly_linked_node_new()
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

Object_list *object_list_new()
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

Object_list *object_list_generatre_fir_trees(Object_list *ol)
{
	int x, y;
	int density = 3, gap = 14, space = 100;
	srand(time(NULL));
	for (x = -12; x > -100 ; x -= CELL_SIZE)
	{
		for (y = -100; y < (HEIGHT * CELL_SIZE) + 100; y += CELL_SIZE)
		{
			if (rand() % density == 1)
			{
				ol = object_list_push_object(ol, x, y, (rand() % 8) + 7, FIR_TREE);
				y += 5;
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
				x += 5;
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
				y += 5;
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
				x += 5;
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

void Object_border_print()
{
	glColor4f(1, 0, 0,0.1);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, 0, 10);

		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);

		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);

		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(0, 0, 5);
		glVertex3f(0, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
		/* LEFT_BORDER */
		glVertex3f(0, 0, 4.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, 0, 5);

		glVertex3f(0, 0, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);
		glVertex3f(0, 0, 10.5);

		/* TOP BORDER */
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);

		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);

		/* RIGHT BORDER */
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);

		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);

		/* BOTTOM BORDER */
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glVertex3f(0, 0, 4.5);
		glVertex3f(0, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);

		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(0, 0, 10);
		glVertex3f(0, 0, 10.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
	glEnd();
}

void Object_sun_print(Object *sun)
{
	glPushMatrix();
		glTranslatef((sun->anchor)->x, (sun->anchor)->y, (sun->anchor)->z); 
		glColor3f(1, 1, 0);
		glutWireSphere(30., 15, 15);
	glPopMatrix();
}

void Object_floor_print()
{
	int i, j;

	glColor3f(0, 1, 1);
	for ( i = 0; i < WIDTH * CELL_SIZE; i += CELL_SIZE ) {
		for ( j = 0; j < HEIGHT * CELL_SIZE; j += CELL_SIZE) { 
			glBegin(GL_LINE_LOOP);
				glVertex3f(i, j, 0);
				glVertex3f(i + CELL_SIZE, j, 0);
				glVertex3f(i + CELL_SIZE, j + CELL_SIZE, 0); 
				glVertex3f(i, j + CELL_SIZE, 0);
			glEnd();
		}
	}
}

void Object_fir_tree_print(Object *fir_tree) {
	float ratio = 0.2;
	/* TRONC Arbre */
	glColor3f(0.95, 0.7, 0.05);
	glPushMatrix();
		glTranslatef((fir_tree->anchor)->x, (fir_tree->anchor)->y, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-((fir_tree->anchor)->z*ratio), 0, 0);
		glVertex3f(((fir_tree->anchor)->z*ratio), 0, 0);
		glVertex3f(((fir_tree->anchor)->z*ratio), 0, (fir_tree->anchor)->z);
		glVertex3f(-((fir_tree->anchor)->z*ratio), 0, (fir_tree->anchor)->z);
		glEnd();

		/* Ligne milieu TRONC */
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, (fir_tree->anchor)->z);
		glEnd();

		/* Génération Arbre TRONC */
		glBegin(GL_LINE_LOOP);
		glVertex3f(0, -((fir_tree->anchor)->z*ratio), 0);
		glVertex3f(0, ((fir_tree->anchor)->z*ratio), 0);
		glVertex3f(0, ((fir_tree->anchor)->z*ratio), (fir_tree->anchor)->z);
		glVertex3f(0, -((fir_tree->anchor)->z*ratio), (fir_tree->anchor)->z); 		
		glEnd();

		/* Sapin les murs */
		glColor3f(0, 1, 0);
		glTranslatef(0, 0, (fir_tree->anchor)->z); 
		/*glutWireCone(Largeur Base, Hauteur Cone, Nombre de Facette, Nombre de Facette) */
		glutWireCone((fir_tree->anchor)->z, 3 * (fir_tree->anchor)->z, (fir_tree->anchor)->z, (fir_tree->anchor)->z);

		/*glutSolidCone(10., 25., 10, 10);
		glutWireSphere(10., 10, 10);*/
	glPopMatrix();
}
