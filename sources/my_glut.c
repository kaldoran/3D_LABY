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

	Doubly_linked_node *iterator;
	int direction, dx, dy, count;
	int x, y;
	
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	float speed = 0.45;
	
	if ( conf->free_fly) {
		speed = 4.5;
	}	

	if ( conf->keys[4] ) {
		speed = (conf->free_fly) ? 7.51337 : 0.751337;
	}


	if ( conf->keys[7] ) {
		speed = (conf->free_fly) ? 0.1 : 0.3;
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

	if (COORD((int)(save_eye->x / CELL_SIZE), (int)(save_eye->y / CELL_SIZE))
			!= COORD((int)((conf->eye)->x / CELL_SIZE), (int)((conf->eye)->y / CELL_SIZE)))
	{
		iterator = ol->last;
		while (1)
		{
			if ((iterator->object)->type == MOVING_WALL)
			{
				x = (((int)((iterator->object)->anchor)->x ) / CELL_SIZE);
				y = (((int)((iterator->object)->anchor)->y ) / CELL_SIZE);

				count = 0;
				direction = rand() % 4;
				while (count < 4)
				{	
					dx = 0;
					dy = 0;
					switch (direction)
					{
						case 0:
							dx = 1;
						break;
						case 1:
							dy = 1;
						break;
						case 2:
							dx = -1;
						break;
						default:
							dy = -1;
						break;
					}
					if(COORD((x+dx),(y+dy)) == COORD(((int)save_eye->x / CELL_SIZE), ((int)save_eye->y / CELL_SIZE))
						|| !IS_PLAYABLE(COORD((x+dx),(y+dy)))
						|| (dx == 1 && END_RIGHT(COORD(x,y)))
						|| (dy == 1 && END_TOP(COORD(x,y)))
						|| (dx == -1 && END_LEFT(COORD(x,y)))
						|| (dy == -1 && END_BOTTOM(COORD(x,y)))
					) {
						direction = (direction + 1) % 4;
						++count;
					} else {
						laby->matrix[COORD(x,y)] = PASS;
						laby->matrix[COORD((x+dx),(y+dy))] = MOVING_WALL;

						((iterator->object)->anchor)->x += (dx * CELL_SIZE);
						((iterator->object)->anchor)->y += (dy * CELL_SIZE);
						count = 7;
					}
				}
			}

			if (iterator->next != NULL)
			{
				iterator = iterator->next;
			} else {
				break;
			}
		}
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
		fprintf(stdout, "%s\n", CRESET);
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
		rad = i*(M_PI / 180.0);
		glVertex2f(cos(rad)*radiusX, sin(rad)*radiusY);
	}
	glEnd();
}

void display() {
	int height_char = 0;
	char buffer[256];
	int coord_previous_bloc = 0, coord_current_bloc = 0;
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
<<<<<<< HEAD
/*	fprintf(stderr,"Eye see what you did there %f %f %f\n", conf->eye_direction->x, conf->eye_direction->y, conf->eye_direction->z);
*/	if ( shoot != 0 ) {
=======
	if ( shoot != 0 ) {
>>>>>>> 4e6e3ff5c323791ff2660170248888df4b1d7b11
		tmp = point_new((conf->eye)->x, (conf->eye)->y, (conf->eye)->z);
		for ( ; tmp->x < 1250 && tmp->x > 0 && tmp->y < 1250 && tmp->y > 0 && tmp->z > 0 && tmp->z < CELL_SIZE; tmp->x += conf->eye_direction->x, tmp->y += conf->eye_direction->y, tmp->z += conf->eye_direction->z) {
			coord_current_bloc = COORD((int)(tmp->x / CELL_SIZE), (int)(tmp->y / CELL_SIZE));
			if ( laby->matrix[coord_current_bloc] == WALL) {
				coord_previous_bloc = COORD((int)((tmp->x - conf->eye_direction->x) / CELL_SIZE), 
								(int)((tmp->y - conf->eye_direction->y) / CELL_SIZE));
				coord_previous_bloc -= coord_current_bloc; /* contient la différence entre les bloc trouvé */
				if ( coord_previous_bloc == 1) {
					coord_previous_bloc = -90; 	   /* contient maintnant la rotation a faire */
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
				if ( shoot == 2) {
					portals->orange->rotation = coord_previous_bloc;
					portals->orange->actif = 1;
					portals->orange->portail->x = tmp->x;
					portals->orange->portail->y = tmp->y;
				}
				if ( shoot == 1) { 
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

	if ( portals->orange->actif && portals->bleu->actif ) {
		if ( abs(conf->eye->x - portals->bleu->portail->x ) < TRIGGER_DISTANCE && abs(conf->eye->y - portals->bleu->portail->y) < TRIGGER_DISTANCE) {
			conf->eye->x = portals->orange->portail->x - (sin(portals->orange->rotation) * PUSH_DISTANCE);
			conf->eye->y = portals->orange->portail->y + (cos(portals->orange->rotation) * PUSH_DISTANCE);
			conf->theta += 180 + ( portals->orange->rotation - portals->bleu->rotation);
			conf->phi = 0;
			modify_direction();
		}
		else if ( abs(conf->eye->x - portals->orange->portail->x ) < TRIGGER_DISTANCE && abs(conf->eye->y - portals->orange->portail->y) < TRIGGER_DISTANCE) {
			conf->eye->x = portals->bleu->portail->x - (sin(portals->bleu->rotation) * PUSH_DISTANCE);
			conf->eye->y = portals->bleu->portail->y + (cos(portals->bleu->rotation) * PUSH_DISTANCE);
			conf->theta += 180 + ( portals->bleu->rotation - portals->orange->rotation);
			conf->phi = 0;
			modify_direction();
		}
	}

	/* Balance le text franky */
	if ( conf->print_config ) {
		glColor3f(0.2, 0.7, 0.7);
		sprintf(buffer,"X : %.2f - Y : %.2f - Z : %.2f - Angle X : %.2f - Angle Z : %.2f", conf->eye->x, conf->eye->y, conf->eye->z, conf->theta, conf->phi);
		write_string(buffer, 5, height_char, GLUT_BITMAP_HELVETICA_18);
		if ( portals->bleu->actif ) {
			height_char += HEIGHT_CHAR;
			sprintf(buffer,"Portail bleu : X : %.2f - Y : %.2f - Z : %.2f",portals->bleu->portail->x, portals->bleu->portail->y, portals->bleu->portail->z);
			write_string(buffer, 5, height_char, GLUT_BITMAP_HELVETICA_18);
		}
		if ( portals->orange->actif ) {
			height_char += HEIGHT_CHAR;
			sprintf(buffer,"Portail orange : X : %.2f - Y : %.2f - Z : %.2f",portals->orange->portail->x, portals->orange->portail->y, portals->orange->portail->z);
			write_string(buffer, 5, height_char, GLUT_BITMAP_HELVETICA_18);
		}
		
		height_char += HEIGHT_CHAR;
		sprintf(buffer,"Life : %d", conf->life);
		write_string(buffer, 5, height_char, GLUT_BITMAP_HELVETICA_18);

	}

	glColor3f(0.5, 0.7, 0.5);
	write_string("+", SCREEN_MID_HEIGHT, SCREEN_MID_WIDTH, GLUT_BITMAP_TIMES_ROMAN_24);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, conf->textu);
	glutSolidTeapot(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	/************************************************* FIN ULTRA TEMPORAIRE ***************************************************/
	move();
	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	glutPostRedisplay();
	glutSwapBuffers();
}

