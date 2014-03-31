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
Laby *laby;

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
	glutSpecialFunc(keyboard);
	
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

void keyboard(unsigned char key, int x , int y) {
	int modifiers = glutGetModifiers();
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	float speed = 1.5;

	if ( key == GLUT_KEY_F3 ) {
		printf("Prepare uranus for Debug mode\n");
	}

	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
	{
		speed = 3.1337;	
	}

	if (save_eye->z > CELL_SIZE + 7) 
	{
		speed = 8;
	}

	if ( key == 's' || key == 'S' || key == GLUT_KEY_DOWN)
	{
		save_eye->x -= speed * conf->body_direction->x;
		save_eye->y -= speed * conf->body_direction->y;
	}
	else if ( key == 'z' || key == 'Z' || key == GLUT_KEY_UP) {
		save_eye->x += speed * conf->body_direction->x;
		save_eye->y += speed * conf->body_direction->y;
	}
	else if ( key == 'q' || key == 'Q' || key == GLUT_KEY_LEFT) { 
		save_eye->x += -(speed * conf->body_direction->y);
		save_eye->y += speed * conf->body_direction->x;
	}	
	else if ( key == 'd' || key == 'D' || key == GLUT_KEY_RIGHT) {
		save_eye->x += speed * conf->body_direction->y;
		save_eye->y += -(speed * conf->body_direction->x);
	}
	else if ( key == '2') {
		save_eye->z -= 3;
	}
	else if ( key == '8' ) {
		save_eye->z += 3;
	}
	else if ( (int)key == 27) {
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

	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	glutPostRedisplay();
	glutSwapBuffers();
}

