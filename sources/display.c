/*
 * SAI project - 3D Laby
 * File : object.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include "laby.h"
#include "config.h"
#include "object.h"
#include "display.h"
#include "portals.h"

void display()
{
	Doubly_linked_node *iterator = doubly_linked_node_new();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(conf->eye->x, conf->eye->y, conf->eye->z, conf->center->x, conf->center->y, conf->center->z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			break;
			case ENTRY:
				Object_entry_print(iterator->object);
			break;
			case EXIT:
				Object_exit_print(iterator->object);
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

	portal_maker();

	glFlush();
    SDL_GL_SwapBuffers();
}

void Object_border_print()
{
	UNUSED(conf);

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

void Object_sun_print(Object *sun)
{
	GLUquadric* params = gluNewQuadric();
	glPushMatrix();
		glTranslatef((sun->anchor)->x, (sun->anchor)->y, (sun->anchor)->z);
		time_color(conf);

		gluQuadricDrawStyle(params,GLU_LINE);
		gluSphere(params,40.,15,15);
		gluDeleteQuadric(params);
	glPopMatrix();
}

void Object_floor_print()
{
	int i, j;

	UNUSED(conf);

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

void Object_fir_tree_print(Object *fir_tree) {
	float ratio = 0.2;
	int i,j, size = CELL_SIZE / 5;
	GLUquadric* params;
	
	UNUSED(conf);
	
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
  		glTranslatef(-CELL_SIZE / 2, -CELL_SIZE / 2, 0);
  		for ( i = 0; i < CELL_SIZE; i += size) {
  			for ( j = i % (2 * size); j < CELL_SIZE; j += 2 * size) { 
  				glBegin(GL_LINE_LOOP);
  					glVertex3f(i, j, 0);
  					glVertex3f(i + size, j, 0);
  					glVertex3f(i + size, j + size, 0); 
  					glVertex3f(i, j + size, 0);
  				glEnd();
  			}
  		}
  		glTranslatef(CELL_SIZE / 2, CELL_SIZE / 2, (fir_tree->anchor)->z);

		params = gluNewQuadric();
		gluQuadricDrawStyle(params,GLU_LINE);
		/* gluCylinder(GLUquadric* params,base,top,height,slices,stacks); */
		gluCylinder(params,(fir_tree->anchor)->z,0,3 * (fir_tree->anchor)->z, (fir_tree->anchor)->z, (fir_tree->anchor)->z);

		gluDeleteQuadric(params);
	glPopMatrix();
}

void Object_wall_print(Object *wall)
{
	float x1 = (wall->anchor)->x, y1 = (wall->anchor)->y, z1 = (wall->anchor)->z;
	float x2 = (wall->anchor)->x + CELL_SIZE, y2 = (wall->anchor)->y + CELL_SIZE, z2 = (wall->anchor)->z + CELL_SIZE; 
	glColor3f(0.03, 0.03, 0.03);
	glBegin(GL_QUADS);
		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);

		glVertex3f(x2 - 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y2 - 1, z1 + 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);

		glVertex3f(x2 - 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);

		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);


		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(0, 1, 1);
		glVertex3f(x1, y1, z1);
		time_color(conf);
		glVertex3f(x1, y1, z2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y1, z1);
		time_color(conf);
		glVertex3f(x2, y1, z2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x1, y2, z1);
		time_color(conf);
		glVertex3f(x1, y2, z2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(0, 1, 1);
		glVertex3f(x2, y2, z1);
		time_color(conf);
		glVertex3f(x2, y2, z2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y2, z2);
	glEnd();
}

void Object_moving_wall_print(Object *wall)
{
	float x1 = (wall->anchor)->x, y1 = (wall->anchor)->y, z1 = (wall->anchor)->z;
	float x2 = (wall->anchor)->x + CELL_SIZE, y2 = (wall->anchor)->y + CELL_SIZE, z2 = (wall->anchor)->z + CELL_SIZE; 
	time_color(conf);
	glBegin(GL_QUADS);
		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);

		glVertex3f(x2 - 1, y1 + 1, z1 + 1);
		glVertex3f(x2 - 1, y2 - 1, z1 + 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);

		glVertex3f(x2 - 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);

		glVertex3f(x1 + 1, y1 + 1, z1 + 1);
		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z1 + 1);


		glVertex3f(x1 + 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y1 + 1, z2 - 1);
		glVertex3f(x2 - 1, y2 - 1, z2 - 1);
		glVertex3f(x1 + 1, y2 - 1, z2 - 1);
	glEnd();
}

void time_color()
{
	if (conf->time == NIGHT)
	{
		glColor3f(0.54, 0.18, 0.9);
	} else {
		glColor3f(1, 1, 0);
	}
}

void Object_entry_print(Object *entry)
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

void Object_exit_print(Object *exit)
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

void Object_teapot_print(Object *teapot)
{
	GLUquadric* params;
	UNUSED(conf);

/*	glPushMatrix();
  		glTranslatef((teapot->anchor)->x, (teapot->anchor)->y, (teapot->anchor)->z * 2 / 3);
  		glRotatef(90,1,0,0);
  		glRotatef(90,0,1,0);
  		glColor3f((float)((int)(teapot->anchor)->x % 10)/10,
  					(float)((int)(teapot->anchor)->z % 10)/10,
  					(float)((int)(teapot->anchor)->y % 10)/10);
  		params = gluNewQuadric();
  		gluQuadricDrawStyle(params,GLU_LINE);
  		
  		glutSolidTeapot((teapot->anchor)->z);

		gluDeleteQuadric(params);
	glPopMatrix();*/


/* TEST
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
	
  glDrawArrays(GL_LINE_STRIP, 0, 306);

  glDisableClientState(GL_VERTEX_ARRAY);*/
}


void DrawEllipse(float radiusX, float radiusY)
{
	int i;
	float rad;
	glBegin(GL_LINE_LOOP);
	for(i=0;i<360;i++) {
		rad = i*(M_PI / 180.0);
		glVertex2f(cos(rad)*radiusX, sin(rad)*radiusY);
	}
	glEnd();
}

void portal_maker ()
{
  	int coord_previous_bloc = 0, coord_current_bloc = 0;
  	Point *tmp;
  	if (conf->shoot) {
  		tmp = point_new((conf->eye)->x, (conf->eye)->y, (conf->eye)->z);
  		for ( ; tmp->x < SIZE && tmp->x > 0 && tmp->y < SIZE && tmp->y > 0 && tmp->z > 0 && tmp->z < CELL_SIZE; tmp->x += conf->eye_direction->x, tmp->y += conf->eye_direction->y, tmp->z += conf->eye_direction->z) {
  			coord_current_bloc = COORD((int)(tmp->x / CELL_SIZE), (int)(tmp->y / CELL_SIZE));
  			if ( laby->matrix[coord_current_bloc] == WALL) {
  				coord_previous_bloc = COORD((int)((tmp->x - conf->eye_direction->x) / CELL_SIZE), 
  								(int)((tmp->y - conf->eye_direction->y) / CELL_SIZE));
  				coord_previous_bloc -= coord_current_bloc; /* contient la différence entre les bloc trouvé */
  				if ( coord_previous_bloc == 1) {
  					coord_previous_bloc = -90;	   /* contient maintnant la rotation a faire */
  				}
  				else if ( coord_previous_bloc == -1) {
  					coord_previous_bloc = 90;
  				}
  				else if ( coord_previous_bloc == -WIDTH ) {
  					coord_previous_bloc = 180;
  				}
  				else {
  					coord_previous_bloc = 0;
  				}
  			fprintf(stderr,"Value : %d \n", coord_previous_bloc); 
  				if ( conf->shoot == 2 ) {
  					portals->orange->rotation = coord_previous_bloc;
  					portals->orange->actif = 1;
  					portals->orange->portail->x = tmp->x;
  					portals->orange->portail->y = tmp->y;
  				}
  				if ( conf->shoot == 1 ) {
  					portals->bleu->rotation = coord_previous_bloc;
  					portals->bleu->actif = 1;
  					portals->bleu->portail->x = tmp->x;
  					portals->bleu->portail->y = tmp->y;
  				}
/*				fprintf(stderr,"WALLLLLLLL !! %f %f %f - %d - angle %f \n",tmp->x, tmp->y, tmp->z, COORD((int)(tmp->x / CELL_SIZE), (int)(tmp->y / CELL_SIZE)), conf->theta);
*/			break;
  			}
  		}
  	}

	if ( portals->bleu->actif) {
		glColor3f(0, 1, 1);

		glPushMatrix();
		glLineWidth(5);
		glTranslatef(portals->bleu->portail->x, portals->bleu->portail->y, portals->bleu->portail->z);
		glRotatef(90, 1, 0, 0);
		glRotatef(portals->bleu->rotation, 0, 1, 0);
		DrawEllipse(5.0, 8.0);
		glLineWidth(1);
		glPopMatrix();
 
	}

	if ( portals->orange->actif) {
		glColor3f(1, 1, 0);

		glPushMatrix();
		glLineWidth(5);
		glTranslatef(portals->orange->portail->x, portals->orange->portail->y, portals->orange->portail->z);
			
		glRotatef(90, 1, 0, 0);
		glRotatef(portals->orange->rotation, 0, 1, 0);

		DrawEllipse(5.0, 8.0);
		glLineWidth(1);
		glPopMatrix();

	}
}