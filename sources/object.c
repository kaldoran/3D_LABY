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

Object_list *object_list_generate_fir_trees(Object_list *ol)
{
	int x, y;
	int density = 3, gap = 14, space = 300;
	srand(time(NULL));
	for (x = -12; x > -space ; x -= CELL_SIZE)
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

Object_list *object_list_push_maze_walls(Object_list *ol, Laby *laby)
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

void Object_border_print(Config *conf)
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

	glBegin(GL_QUADS);
		/* LEFT_BORDER */
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, 0, 4.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, 0, 5);

		glColor4f(1, 0, 0,0.1);
		glVertex3f(0, 0, 10);
		glColor4f(1, 0, 0,0.1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glColor4f(1, 0, 0,1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);
		glColor4f(1, 0, 0,1);
		glVertex3f(0, 0, 10.5);

		/* TOP BORDER */
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);

		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);

		/* RIGHT BORDER */
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);

		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);

		/* BOTTOM BORDER */
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, 0, 4.5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, 0, 5);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);

		glColor4f(1, 0, 0, 0.1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glColor4f(1, 0, 0, 0.1);
		glVertex3f(0, 0, 10);
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, 0, 10.5);
		glColor4f(1, 0, 0, 1);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
	glEnd();
}

void Object_sun_print(Object *sun, Config *conf)
{
	glPushMatrix();
		glTranslatef((sun->anchor)->x, (sun->anchor)->y, (sun->anchor)->z);
		time_color(conf);
		glutWireSphere(30., 15, 15);
	glPopMatrix();
}

void Object_floor_print(Config *conf)
{
	int i, j;

	glColor3f(0, 1, 1);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
		glVertex3f(WIDTH * CELL_SIZE, 0, 0);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 0); 
		glVertex3f(0, HEIGHT * CELL_SIZE, 0);
	glEnd();
	for ( i = 0; i < WIDTH * CELL_SIZE; i += CELL_SIZE ) {
		for ( j = i % (2 * CELL_SIZE); j < HEIGHT * CELL_SIZE; j += CELL_SIZE * 2) { 
			glBegin(GL_LINE_LOOP);
				glVertex3f(i, j, 0);
				glVertex3f(i + CELL_SIZE, j, 0);
				glVertex3f(i + CELL_SIZE, j + CELL_SIZE, 0); 
				glVertex3f(i, j + CELL_SIZE, 0);
			glEnd();
		}
	}
}

void Object_fir_tree_print(Object *fir_tree, Config *conf) {
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

void Object_wall_print(Object *wall, Config *conf)
{
	float x1 = (wall->anchor)->x, y1 = (wall->anchor)->y, z1 = 0;
	float x2 = (wall->anchor)->x + CELL_SIZE, y2 = (wall->anchor)->y + CELL_SIZE, z2 = (wall->anchor)->z + CELL_SIZE; 
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z1 + 1);

		glVertex3f(x2 - 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z1 + 1);

		glVertex3f(x2 - 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);

		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);

	glEnd();

	/*glColor3f(0, 0, 1);*/
	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x1, y1, z1);
		time_color(conf);
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y1, z1);
	glEnd();
	/*glColor3f(0, 1, 0);*/
	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y1, z1);
		time_color(conf);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y2, z1);
	glEnd();
	/*glColor3f(1, 0, 0);*/
	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x1, y2, z1);
		time_color(conf);
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glColor3f(0, 1, 1);
	glEnd();
	/*glColor3f(1, 0, 1);*/
	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x1, y1, z1);
		time_color(conf);
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glColor3f(0, 1, 1);
		glVertex3f(x1, y2, z1);
	glEnd();
	/*glColor3f(1, 1, 0);*/
	time_color(conf);
	glBegin(GL_LINE_LOOP);
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y1, z2);
	glEnd();
}

void time_color(Config *conf)
{
	if (conf->time == NIGHT)
	{
		glColor3f(0.54, 0.18, 0.9);
	} else {
		glColor3f(1, 1, 0);
	}
}

void Object_entry_print(Object *entry, Config *conf)
{
	float x1 = (entry->anchor)->x, y1 = (entry->anchor)->y;
	
	glBegin(GL_QUADS);
		glColor3f(1, 0.5, 0);
		glVertex3f(x1 + 1, y1 + 1, 0);
		glVertex3f(x1 + 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(1, 0.5, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + 1, 0);

		glVertex3f(x1 + CELL_SIZE - 1, y1 + 1, 0);
		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(1, 0.5, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + 1, 0);

		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE - 1, 0);
		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(1, 0.5, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE - 1, 0);

		glVertex3f(x1 + 1, y1 + CELL_SIZE - 1, 0);
		glVertex3f(x1 + 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(1, 0.5, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE - 1, 0);
	glEnd();
}

void Object_exit_print(Object *exit, Config *conf)
{
	float x1 = (exit->anchor)->x, y1 = (exit->anchor)->y;
	
	glBegin(GL_QUADS);
		glColor3f(0, 0.8, 0);
		glVertex3f(x1 + 1, y1 + 1, 0);
		glVertex3f(x1 + 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(0, 0.8, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + 1, 0);

		glVertex3f(x1 + CELL_SIZE - 1, y1 + 1, 0);
		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(0, 0.8, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + 1, 0);

		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE - 1, 0);
		glVertex3f(x1 + CELL_SIZE - 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(0, 0.8, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE - 1, 0);

		glVertex3f(x1 + 1, y1 + CELL_SIZE - 1, 0);
		glVertex3f(x1 + 1, y1 + CELL_SIZE / 2, 0);
		time_color(conf);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE / 2, 0);
		glColor3f(0, 0.8, 0);
		glVertex3f(x1 + CELL_SIZE / 2, y1 + CELL_SIZE - 1, 0);
	glEnd();
}

void Object_teapot_print(Object *teapot, Config *conf)
{
	glPushMatrix();
		glTranslatef((teapot->anchor)->x, (teapot->anchor)->y, (teapot->anchor)->z * 2 / 3);
		glRotatef(90,1,0,0);
		glRotatef(90,0,1,0);
		glColor3f((float)((int)(teapot->anchor)->x % 10)/10,
					(float)((int)(teapot->anchor)->z % 10)/10,
					(float)((int)(teapot->anchor)->y % 10)/10);
		glutWireTeapot((teapot->anchor)->z);
	glPopMatrix();
}