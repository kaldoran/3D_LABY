/*
 * SAI project - 3D Laby
 * File : my_glut.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "my_glut.h"
#include "laby.h"
#include "stack.h"
#include "k-tree.h"
#include "config.h"
#include "object.h"

int shoot = 0;

void keyboard(unsigned char key, int x , int y) {
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	int modifiers = glutGetModifiers();
	float speed = 1.5;
	
	if (conf->free_fly) 
	{
		speed = 8;
	} else if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT) {
		speed = 3.1337;	
	}

	if ( key == 's' || key == 'S')
	{	
		/* Go straight */
		save_eye = backward_move(conf, save_eye, speed);
	}
	else if ( key == 'z' || key == 'Z') {
		/* Go back */
		save_eye = forward_move(conf, save_eye, speed);
	}
	if ( key == 'q' || key == 'Q') { 
		/* Go left */
		save_eye = left_move(conf, save_eye, speed);
	}	
	else if ( key == 'd' || key == 'D') {
		/* Go right */
		save_eye = right_move(conf, save_eye, speed);
	}
	else if (key == 'r' || key == 'R' ) {
		/* Restart the maze */
		(conf->eye)->x = CELL_SIZE / 2;
		(conf->eye)->y = CELL_SIZE / 2;
		(conf->eye)->z = CHARACTER_SIZE;
		conf = change_center(conf);
		point_free(save_eye);
		return;
	}
	else if (key == 'a' || key == 'A') {
		/* Make a 180° */
		conf->theta += 180;
		conf->phi = 0;
		conf = modify_direction(conf);
		return;
	}
	else if ( key == '2') {
		/* Go down */
		save_eye->z -= 3;
	}
	else if ( key == '8' ) {
		/* Go up */
		save_eye->z += 3;
	}
	else if ( (int)key == 27) {
		/* Quit */
		glutDestroyWindow(conf->id_windows); /*glutLeaveMainLoop();*/
		exit(EXIT_SUCCESS);
	}

	if ((( save_eye->x > 2 && save_eye->y > 2 
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2 
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2 
		&& save_eye->z <= CHARACTER_SIZE 
		&& save_eye->z > 5 
		&& IS_PLAYABLE(COORD((int)(save_eye->x / CELL_SIZE),(int)(save_eye->y / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x + 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y + 2) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x - 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y - 2) / CELL_SIZE))) 
		) || conf->free_fly == 1
	)) {
		conf->eye->x = save_eye->x;
		conf->eye->y = save_eye->y;
		conf->eye->z = save_eye->z;
	} 
conf = change_center(conf);
point_free(save_eye);
}

void special_keyboard(int key, int x, int y) {
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	int modifiers = glutGetModifiers();
	float speed = 1.5;

	if (conf->free_fly) 
	{
		speed = 8;
	} else if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT) {
		speed = 3.1337;	
	}

	if ( key == GLUT_KEY_F1 ) {
		/* FREE FLY ! */
		conf->free_fly = !conf->free_fly;
		if (conf->free_fly == 0)
		{
			(conf->eye)->x = CELL_SIZE / 2;
			(conf->eye)->y = CELL_SIZE / 2;
			(conf->eye)->z = CHARACTER_SIZE;
			conf = change_center(conf);
			point_free(save_eye);
			return;
		}
	}

	if ( key == GLUT_KEY_F3 ) {
		conf->print_config = !conf->print_config;
	}
	
	if ( key == GLUT_KEY_F7 ) {
		/* Night or Day */
		conf->time = ( conf->time == DAY ) ? NIGHT : DAY;
	}

	if ( key == GLUT_KEY_DOWN) {
		/* Go back */
		save_eye = backward_move(conf, save_eye, speed);
	}
	else if ( key == GLUT_KEY_UP) {
		/* Go straight */
		save_eye = forward_move(conf, save_eye, speed);
	}
	if ( key == GLUT_KEY_LEFT) { 
		/* Go left */
		save_eye = left_move(conf, save_eye, speed);
	}	
	else if ( key == GLUT_KEY_RIGHT) {
		/* Go right */
		save_eye = right_move(conf, save_eye, speed);
	}

	if ((( save_eye->x > 2 && save_eye->y > 2 
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2 
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2 
		&& save_eye->z <= CHARACTER_SIZE 
		&& save_eye->z > 5 
		&& IS_PLAYABLE(COORD((int)(save_eye->x / CELL_SIZE),(int)(save_eye->y / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x + 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y + 2) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x - 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE))) 
		&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y - 2) / CELL_SIZE))) 
		) || conf->free_fly == 1
	)) {
		conf->eye->x = save_eye->x;
		conf->eye->y = save_eye->y;
		conf->eye->z = save_eye->z;
	}
conf = change_center(conf);
point_free(save_eye);
}

void mouse_motion(int x, int z) {

	conf->theta -= (x - SCREEN_MID_WIDTH) * SENSITIVITY;
	conf->phi -= (z - SCREEN_MID_HEIGHT) * SENSITIVITY;

	conf = modify_direction(conf);
	return;
}



void mouse_trigger(int button, int state, int x, int y) {

	if ( state != GLUT_UP && button == GLUT_LEFT_BUTTON ) {
		shoot = 1;

	}
	else if ( state != GLUT_UP && button == GLUT_RIGHT_BUTTON ) {
		shoot = 2;
	}
	
	if ( state == GLUT_UP ) {
		printf("Relache\n");
		shoot = 0;
	}
	else {
		printf("shoot\n");
	}	


}

void write_string(char* string, int x, int y, void* font) {	
	int i, width;

	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glPushMatrix();
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	glScalef(1, -1, 1);
	glTranslatef(0, -SCREEN_HEIGHT + 15, 0);
	
	width = x - (glutBitmapLength(font, (unsigned char *)string) / 2);
	if ( width > 0 ) {
		x = width; 
	}
	
	glRasterPos2i(x, y);
	for (i=0; string[i] != '\0'; i++) {
		glutBitmapCharacter(font, string[i]);
	}
	
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void display() {
	Doubly_linked_node *iterator = doubly_linked_node_new();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR, 1600);
	/*glFrustum(-5, 5, -5, 5, 5, 500);*/
	gluLookAt(conf->eye->x, conf->eye->y, conf->eye->z, conf->center->x, conf->center->y, conf->center->z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	iterator = ol->last;
	while (1)
	{
		switch ((iterator->object)->type)
		{
			case FLOOR:
				Object_floor_print(conf);
			break;
			case BORDER:
				Object_border_print(conf);
			break;
			case SUN:
				Object_sun_print(iterator->object, conf);
			break;
			case FIR_TREE:
				Object_fir_tree_print(iterator->object, conf);
			break;
			case WALL:
				Object_wall_print(iterator->object, conf);
			break;
			case ENTRY:
				Object_entry_print(iterator->object, conf);
			break;
			case EXIT:
				Object_exit_print(iterator->object, conf);
			break;
			case TEAPOT:
				Object_teapot_print(iterator->object, conf);
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
	
	/************************************************* ULTRA TEMPORAIRE ******************************************************/
	if ( shoot == 1 ) glColor3f(0, 1, 1);
	if ( shoot == 2 ) glColor3f(1, 1, 0);
	if ( shoot != 0 ) {
		
		printf("%f %f %f - %f %f %f\n", conf->body_direction->x, conf->body_direction->y, conf->body_direction->z, conf->eye->x, conf->eye->y + 2, conf->eye->z);
		glPushMatrix();
		glTranslatef((conf->eye)->x, (conf->eye)->y, (conf->eye)->z);
		glRotatef(90,1,0,0);
		glRotatef(conf->theta,0,1,0);
		glColor3f(0,1,1);
		glutWireTeapot(3);
		glPopMatrix();

		glLineWidth(1); 
	}
		/* Balance le text franky */
	if ( conf->print_config ) {
		glColor3f(1, 0.7, 0.5);
		write_string("Salut le pd, si tu me lis c'est que tu as fais F3 alors arrete de jouer et travail", 0, 0, GLUT_BITMAP_HELVETICA_18);
	}
	else {
		glColor3f(0.5, 0.7, 0.5);
		write_string("M'en fou que tu puisse pas jouer", SCREEN_MID_HEIGHT, SCREEN_MID_WIDTH, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	/************************************************* FIN ULTRA TEMPORAIRE ***************************************************/
		
	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	glutPostRedisplay();
	glutSwapBuffers();
}

