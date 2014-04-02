/*
 * SAI project - 3D Laby
 * File : my_glut.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "my_glut.h"
#include "laby.h"
#include "stack.h"
#include "k-tree.h"
#include "config.h"
#include "object.h"
#include "portals.h"

int shoot = 0;

/* calcule réellement le mouvement */
void move() {

	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	float speed = 1.5;
	
	if ( conf->free_fly) {
		speed = 8;
	}	

	if ( conf->keys[4] ) {
		speed = (conf->free_fly) ? 20 : 5.1337;
	}


	if ( conf->keys[7] ) {
		speed = (conf->free_fly) ? 4 : 0.5;
	}

	if ( conf->keys[0] ) {
		/* Go back */
		forward_move(save_eye, speed);
	}

	if ( conf->keys[1] ) {
		/* Go straight */
		backward_move(save_eye, speed);
	}
	
	if ( conf->keys[2] ) {
		/* Go left */
		left_move(save_eye, speed);
	}

	if ( conf->keys[3] ) {
		/* Go right */
		right_move(save_eye, speed);
	}
	
	if ( conf->keys[5] ) {
		/* Go down */
		save_eye->z -= 3;
	}

	if ( conf->keys[6] ) {
		/* Go up */
		save_eye->z += 3;
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

change_center();
point_free(save_eye);
}

void keyboard(unsigned char key, int x , int y) {

	int modifiers = glutGetModifiers();
	
	UNUSED(x);
	UNUSED(y);
	
	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT) {
		conf->keys[4] = 1;	
	}

	if (modifiers == GLUT_ACTIVE_CTRL ) {
		conf->keys[7] = 1;
	}

	if ( key == 'z' || key == 'Z') {
		conf->keys[0] = 1;
	}
	else if ( key == 's' || key == 'S') {	
		conf->keys[1] = 1;
	}
	else if ( key == 'q' || key == 'Q') { 
		conf->keys[2] = 1;
	}	
	else if ( key == 'd' || key == 'D') {
		conf->keys[3] = 1;
	}
	else if (key == 'r' || key == 'R' ) {
		/* Restart the maze */
		(conf->eye)->x = CELL_SIZE / 2;
		(conf->eye)->y = CELL_SIZE / 2;
		(conf->eye)->z = CHARACTER_SIZE;
		change_center();
		return;
	}
	else if (key == 'a' || key == 'A') {
		/* Make a 180° */
		conf->theta += 180;
		conf->phi = 0;
		modify_direction();
		return;
	}
	else if ( key == '2') {
		conf->keys[5] = 1;
	}
	else if ( key == '8' ) {
		conf->keys[6] = 1;
	}
	else if ( (int)key == 27) {
		object_list_free(ol);
		config_free(conf);
		laby_free(laby);
		/* Quit */
		glutDestroyWindow(conf->id_windows); /*glutLeaveMainLoop();*/
		exit(EXIT_SUCCESS);
	}
}

void keyboard_up(unsigned char key, int x, int y) {

	int modifiers = glutGetModifiers();
	
	UNUSED(x);
	UNUSED(y);
	
	if (modifiers != GLUT_ACTIVE_SHIFT && modifiers != GLUT_ACTIVE_ALT) {
		conf->keys[4] = 0;
	}

	if (modifiers != GLUT_ACTIVE_CTRL ) {
		conf->keys[7] = 0;
	}

	if ( key == 'z' || key == 'Z') {
		conf->keys[0] = 0;
	}
	else if ( key == 's' || key == 'S') {	
		conf->keys[1] = 0;
	}
	else if ( key == 'q' || key == 'Q') { 
		conf->keys[2] = 0;
	}	
	else if ( key == 'd' || key == 'D') {
		conf->keys[3] = 0;
	}
	else if ( key == '2' ) {
		conf->keys[5] = 0;
	}
	else if ( key == '8' ) {
		conf->keys[6] = 0;
	}

}

void special_keyboard(int key, int x, int y) {

	UNUSED(x);
	UNUSED(y);

	if ( key == GLUT_KEY_F1 ) {
		/* FREE FLY ! */
		conf->free_fly = !conf->free_fly;
		if (conf->free_fly == 0)
		{
			(conf->eye)->x = CELL_SIZE / 2;
			(conf->eye)->y = CELL_SIZE / 2;
			(conf->eye)->z = CHARACTER_SIZE;
			change_center();
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

	if ( key == GLUT_KEY_UP) {
		conf->keys[0] = 1;
	}
	else if ( key == GLUT_KEY_DOWN) {
		conf->keys[1] = 1;
	}
	else if ( key == GLUT_KEY_LEFT) { 
		conf->keys[2] = 1;
	}	
	else if ( key == GLUT_KEY_RIGHT) {
		conf->keys[3] = 1;
	}
}

void special_keyboard_up(int key, int x, int y) {

	UNUSED(x);
	UNUSED(y);

	if ( key == GLUT_KEY_UP) {
		conf->keys[0] = 0;
	}
	else if ( key == GLUT_KEY_DOWN) {	
		conf->keys[1] = 0;
	}
	else if ( key == GLUT_KEY_LEFT) { 
		conf->keys[2] = 0;
	}	
	else if ( key == GLUT_KEY_RIGHT) {
		conf->keys[3] = 0;
	}
}

void mouse_motion(int x, int z) {

	conf->theta -= (x - SCREEN_MID_WIDTH) * SENSITIVITY;
	conf->phi -= (z - SCREEN_MID_HEIGHT) * SENSITIVITY;

	modify_direction();
	return;
}



void mouse_trigger(int button, int state, int x, int y) {

	UNUSED(x);
	UNUSED(y);

	if ( state != GLUT_UP && button == GLUT_LEFT_BUTTON ) {
		shoot = 1;

	}
	else if ( state != GLUT_UP && button == GLUT_RIGHT_BUTTON ) {
		shoot = 2;
	}
	fprintf(stderr, "%d", shoot);	
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

void DrawEllipse(float radiusX, float radiusY)
{
	int i;
	float rad;
	glBegin(GL_LINE_LOOP);
	for(i=0;i<360;i++) {
		rad = i*(3.14159 / 180.0);
		glVertex2f(cos(rad)*radiusX, sin(rad)*radiusY);
	}
	glEnd();
}

void display() {
	Point *tmp;
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
	
	/************************************************* ULTRA TEMPORAIRE ******************************************************/
	fprintf(stderr,"Eye see what you did there %f %f %f\n", conf->eye_direction->x, conf->eye_direction->y, conf->eye_direction->z);
	if ( shoot != 0 ) {
		tmp = point_new((conf->eye)->x, (conf->eye)->y, (conf->eye)->z);
		for ( ; tmp->x < 1250 && tmp->x > 0 && tmp->y < 1250 && tmp->y > 0; tmp->x += conf->eye_direction->x, tmp->y += conf->eye_direction->y) {
			if ( laby->matrix[COORD((int)(tmp->x / CELL_SIZE), (int)(tmp->y / CELL_SIZE))] == WALL) {
				if ( shoot == 2) {
					portals->orange->actif = 1;
					portals->orange->portail->x = tmp->x;
					portals->orange->portail->y = tmp->y;
				}
				if ( shoot == 1) { 
					portals->bleu->actif = 1;
					portals->bleu->portail->x = tmp->x;
					portals->bleu->portail->y = tmp->y;
				}
				fprintf(stderr,"WALLLLLLLL !! %f %f %f - %d - angle %f \n",tmp->x, tmp->y, tmp->z, COORD((int)(tmp->x / CELL_SIZE), (int)(tmp->y / CELL_SIZE)), conf->theta);
			break;
			}
		}
	}

	if ( portals->bleu->actif) {
		glColor3f(0, 1, 1);

		glPushMatrix();
		glLineWidth(5);
		glTranslatef(portals->bleu->portail->x, portals->bleu->portail->y, portals->bleu->portail->z);
		glRotatef(90,1, 0, 0);

		DrawEllipse(5.0, 8.0);
		glLineWidth(1);
		glPopMatrix();
 
	}

	if ( portals->orange->actif) {
		glColor3f(1, 1, 0);

		glPushMatrix();
		glLineWidth(5);
		glTranslatef(portals->orange->portail->x, portals->orange->portail->y, portals->orange->portail->z);
		glRotatef(90,1, 0, 0);

		DrawEllipse(5.0, 8.0);
		glLineWidth(1);
		glPopMatrix();

	}

	if ( portals->orange->actif && portals->bleu->actif ) {
		if ( abs(conf->eye->x - portals->bleu->portail->x ) <= 10 && abs(conf->eye->y - portals->bleu->portail->y) <= 10) {
			conf->eye->x = portals->orange->portail->x;
			conf->eye->y = portals->orange->portail->y + 10;
			conf->theta += 180;
			conf->phi = 0;
			modify_direction();
		}
		else if ( abs(conf->eye->x - portals->orange->portail->x ) <= 10 && abs(conf->eye->y - portals->orange->portail->y) <= 10) {
			conf->eye->x = portals->bleu->portail->x;
			conf->eye->y = portals->bleu->portail->y + 10;
			conf->theta += 180;
			conf->phi = 0;
			modify_direction();
		}
	}

	/* Balance le text franky */
	if ( conf->print_config ) {
		glColor3f(1, 0.7, 0.5);
		write_string("Salut le pd, si tu me lis c'est que tu as fais F3 alors arrete de jouer et travail", 0, 0, GLUT_BITMAP_HELVETICA_18);
	}
	else {
		glColor3f(0.5, 0.7, 0.5);
		write_string("+", SCREEN_MID_HEIGHT, SCREEN_MID_WIDTH, GLUT_BITMAP_TIMES_ROMAN_24);
	}


	/************************************************* FIN ULTRA TEMPORAIRE ***************************************************/
	move();
	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	glutPostRedisplay();
	glutSwapBuffers();
}

