/*
 * SAI project - 3D Laby
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <setjmp.h>
#include "laby.h"
#include "stack.h"
#include "k-tree.h"
#include "config.h"
#include "object.h"


void forward_move(Point *save_eye, float speed);
void backward_move(Point *save_eye, float speed);
void left_move(Point *save_eye, float speed);
void right_move(Point *save_eye, float speed);

void keyboard(unsigned char key, int x , int y);
void special_keyboard(int key, int x, int y);
void mouse_motion(int x, int z);
void mouse_trigger(int button, int state, int x, int y);
void display();
void change_center();

Config *conf;
Object_list *ol;
Laby *laby;
int shoot = 0;
int main(int argc, char *argv[])
{
	Object *floor = object_new(0, 0, 0, FLOOR);
	Object *border = object_new(0, 0, 0, BORDER);
	Object *sun = object_new(CELL_SIZE * WIDTH / 2, CELL_SIZE * HEIGHT / 2, 500, SUN);
	Object *giant_teapot = object_new(-WIDTH * CELL_SIZE / 2, HEIGHT * CELL_SIZE	 / 2, 300, TEAPOT);
	Object *teapot1 = object_new(-CELL_SIZE / 2, CELL_SIZE / 2, 5, TEAPOT);

	laby = laby_new();
	conf = config_new();
	ol = object_list_new();
	laby = maze_generation(laby);

	ol = object_list_push(ol, floor);
	ol = object_list_push(ol, border);
	ol = object_list_push(ol, sun);
	ol = object_list_generate_fir_trees(ol);
	ol = object_list_push_maze_walls(ol, laby);
	ol = object_list_push(ol, giant_teapot);
	ol = object_list_push(ol, teapot1);

	if(argc == 2)
	{
		if (argv[1] == 0)
		{
			laby_print(laby);
			return 0;
		} else if (strcmp(argv[1], "day") == 0) {
			conf->time = DAY;
		} else {
			conf->time = NIGHT;
		}
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); 
	glutInitWindowPosition(SCREEN_POSITION_X, SCREEN_POSITION_Y);

	conf->id_windows = glutCreateWindow("SAI Project - 3D Laby Nicolas Reynaud && Kevin Hivert.");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(SCREEN_MID_HEIGHT, SCREEN_MID_HEIGHT);

	glutDisplayFunc(display);
	glutIdleFunc(display);
		
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutMouseFunc(mouse_trigger);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	
	glutMainLoop();

	object_list_free(ol);
	config_free(conf);
	laby_free(laby);
return 0;
}

void change_center()
{
	conf->center->x = conf->eye->x + conf->eye_direction->x;
	conf->center->y = conf->eye->y + conf->eye_direction->y;
	conf->center->z = conf->eye->z + conf->eye_direction->z;
}

void forward_move(Point *save_eye, float speed) {
	save_eye->x += speed * conf->body_direction->x;
	save_eye->y += speed * conf->body_direction->y;
}

void backward_move(Point *save_eye, float speed) {
	save_eye->x -= speed * conf->body_direction->x;
	save_eye->y -= speed * conf->body_direction->y;
}

void left_move(Point *save_eye, float speed) {
	save_eye->x += -(speed * conf->body_direction->y);
	save_eye->y += speed * conf->body_direction->x;
}

void right_move(Point *save_eye, float speed) {
	save_eye->x += speed * conf->body_direction->y;
	save_eye->y += -(speed * conf->body_direction->x);
}

void keyboard(unsigned char key, int x , int y) {
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	int modifiers = glutGetModifiers();
	float speed = 1.5;
	
	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
	{
		speed = 3.1337;	
	}

	if (save_eye->z > CELL_SIZE + 7) 
	{
		speed = 8;
	}

	if ( key == 's' || key == 'S')
	{	
		backward_move(save_eye, speed);
	}
	else if ( key == 'z' || key == 'Z') {
		forward_move(save_eye, speed);
	}
	if ( key == 'q' || key == 'Q') { 
		left_move(save_eye, speed);
	}	
	else if ( key == 'd' || key == 'D') {
		right_move(save_eye, speed);
	}
	else if ( key == '2') {
		save_eye->z -= 3;
	}
	else if ( key == '8' ) {
		save_eye->z += 3;
	}
	else if ( (int)key == 27) {
		glutDestroyWindow(conf->id_windows);
		/*glutLeaveMainLoop();*/
		exit(EXIT_SUCCESS);
	}
	
	if ( save_eye->x > 2 && save_eye->y > 2
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2
		&& save_eye->z > 5
		&& ((IS_PLAYABLE(COORD((int)(save_eye->x / CELL_SIZE),(int)(save_eye->y / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x + 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y + 2) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x - 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y - 2) / CELL_SIZE))))
			|| save_eye->z > CELL_SIZE + 7)
	) {
		conf->eye->x = save_eye->x;
		conf->eye->y = save_eye->y;
		conf->eye->z = save_eye->z;
	} 
change_center();
free(save_eye);
}

void special_keyboard(int key, int x, int y) {
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	int modifiers = glutGetModifiers();
	float speed = 1.5;

	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT) {
		speed = 3.1337;	
	}


	if ( key == GLUT_KEY_F3 ) {
		conf->print_config = !conf->print_config;
	}
	
	if ( key == GLUT_KEY_F7 ) {
		conf->time = ( conf->time == DAY ) ? NIGHT : DAY;
	}

	if ( key == GLUT_KEY_DOWN) {
		backward_move(save_eye, speed);
	}
	else if ( key == GLUT_KEY_UP) {
		forward_move(save_eye, speed);
	}
	if ( key == GLUT_KEY_LEFT) { 
		left_move(save_eye, speed);
	}	
	else if ( key == GLUT_KEY_RIGHT) {
		right_move(save_eye, speed);
	}

	if ( save_eye->x > 2 && save_eye->y > 2
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2
		&& ((IS_PLAYABLE(COORD((int)(save_eye->x / CELL_SIZE),(int)(save_eye->y / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x + 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y + 2) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x - 2) / CELL_SIZE),(int)((save_eye->y) / CELL_SIZE)))
			&& IS_PLAYABLE(COORD((int)((save_eye->x) / CELL_SIZE),(int)((save_eye->y - 2) / CELL_SIZE)))))
	) {
		conf->eye->x = save_eye->x;
		conf->eye->y = save_eye->y;
		conf->eye->z = save_eye->z;
	} 
change_center();
free(save_eye);
}

void mouse_motion(int x, int z) {
	float tmp;

	conf->theta -= (x - SCREEN_MID_WIDTH) * SENSITIVITY;
	conf->phi -= (z - SCREEN_MID_HEIGHT) * SENSITIVITY;

	if (conf->theta > 360 )
	{
		conf->theta = 0;
	} else if (conf->theta < -360 ) {
		conf->theta = 0;
	}

	if (conf->phi > 89 )
	{
		conf->phi = 89;
	} else if (conf->phi < -89) {
		conf->phi = -89;
	}

	tmp = cos(conf->phi * M_PI / 180);

	conf->body_direction->x = cos(conf->theta * M_PI / 180);
	conf->body_direction->y = sin(conf->theta * M_PI / 180);

	conf->eye_direction->x = tmp * conf->body_direction->x;
	conf->eye_direction->y = tmp * conf->body_direction->y;
	conf->eye_direction->z = sin(conf->phi * M_PI / 180);
	
	change_center();
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
		glLineWidth(15); 
		glBegin( GL_LINES );
		printf("%f %f %f - %f %f %f\n", conf->center->x, conf->center->y, conf->center->z, conf->eye->x, conf->eye->y + 2, conf->eye->z);
		glVertex3f(conf->center->x, conf->center->y, conf->center->z);
		glVertex3f(conf->eye->x, conf->eye->y + 2, conf->eye->z);
		glEnd();
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

