/*
 * SAI project - 3D Laby
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "laby.h"
#include "stack.h"
#include "k-tree.h"
#include "config.h"
#include "object.h"

void keyboard(unsigned char touche, int x , int y);
void mouse_motion(int x, int z);
void display();
void change_center();

Config *conf;
Object_list *ol;

int main(int argc, char *argv[])
{
	Laby *laby = laby_new();
	Object *floor = object_new(0, 0, 0, FLOOR);
	Object *border = object_new(0, 0, 0, BORDER);
	Object *sun = object_new(CELL_SIZE * WIDTH / 2, CELL_SIZE * HEIGHT / 2, 300, SUN);

	conf = config_new();
	ol = object_list_new();

	ol = object_list_push(ol, floor);
	ol = object_list_push(ol, border);
	ol = object_list_push(ol, sun);
	ol = object_list_generatre_fir_trees(ol);

	fprintf(stderr, "%u\n", ol->size);
	laby = maze_generation(laby);

	if(argc == 1)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		
		glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); 
		glutInitWindowPosition(SCREEN_POSITION_X, SCREEN_POSITION_Y);

		glutCreateWindow("SAI Project - 3D Laby Nicolas Reynaud && Kevin Hivert.");
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(SCREEN_MID_HEIGHT, SCREEN_MID_HEIGHT);

		glutDisplayFunc(display);
		glutIdleFunc(display);
			
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);
		glutKeyboardFunc(keyboard);
		
		glutMainLoop();

		object_list_free(ol);
		config_free(conf);
		laby_free(laby);
	}
return 0;
}

void change_center()
{
	conf->center->x = conf->eye->x + conf->eye_direction->x;
	conf->center->y = conf->eye->y + conf->eye_direction->y;
	conf->center->z = conf->eye->z + conf->eye_direction->z;
}

void keyboard(unsigned char key, int x , int y) {
	int modifiers = glutGetModifiers();
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	float speed = 1.5;

	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
	{
		speed = 3.1337;	
	}

	if ( key == 's' || key == 'S')
	{
		save_eye->x -= speed * conf->body_direction->x;
		save_eye->y -= speed * conf->body_direction->y;
	}
	else if ( key == 'z' || key == 'Z') {
		save_eye->x += speed * conf->body_direction->x;
		save_eye->y += speed * conf->body_direction->y;
	}
	else if ( key == 'q' || key == 'Q') { 
		save_eye->x += -(speed * conf->body_direction->y);
		save_eye->y += speed * conf->body_direction->x;
	}	
	else if ( key == 'd' || key == 'D') {
		save_eye->x += speed * conf->body_direction->y;
		save_eye->y += -(speed * conf->body_direction->x);
	}
	else if ( key == '2') {
		save_eye->z--;
	}
	else if ( key == '8' ) {
		save_eye->z++;
	}
	else if ( (int)key == 27) {
		/*glutLeaveMainLoop();*/
		exit(EXIT_SUCCESS);
	}
	
	if ( save_eye->x > 2 && save_eye->y > 2
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2
		&& save_eye->z > 5
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

void display() {
	Doubly_linked_node *iterator = doubly_linked_node_new();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR, 1600);
	/*glFrustum(-5, 5, -5, 5, 5, 500);*/
	gluLookAt(conf->eye->x, conf->eye->y, conf->eye->z, conf->center->x, conf->center->y, conf->center->z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	iterator = ol->last;
	while (iterator->next != NULL)
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
			default:
			break;
		}
		iterator = iterator->next;
	}
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
		default:
		break;
	}

	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	glutPostRedisplay();
	glutSwapBuffers();
}

