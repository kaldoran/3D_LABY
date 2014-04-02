/*
 * SAI project - 3D Laby
 * File : main.c
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
#include "my_glut.h"
#include "portals.h"

int main(int argc, char *argv[])
{
	Object *floor = object_new(0, 0, 0, FLOOR);
	Object *border = object_new(0, 0, 0, BORDER);
	Object *sun = object_new(CELL_SIZE * WIDTH / 2, CELL_SIZE * HEIGHT / 2, 500, SUN);
	Object *giant_teapot = object_new(-WIDTH * CELL_SIZE / 2, HEIGHT * CELL_SIZE / 2, 300, TEAPOT);
	Object *teapot1 = object_new(-CELL_SIZE / 2, CELL_SIZE / 2, 5, TEAPOT);

	portals = portals_new();
	laby = laby_new();
	conf = config_new();
	ol = object_list_new();

	if(laby == NULL || conf == NULL || ol == NULL)
	{
		exit(EXIT_FAILURE);
	}
	
	maze_generation();
	maze_moving_walls_generation();

	ol = object_list_push(ol, floor);
	ol = object_list_push(ol, border);
	ol = object_list_push(ol, sun);

	ol = object_list_generate_fir_trees(ol);
	ol = object_list_push_maze_walls(ol);

	ol = object_list_push(ol, giant_teapot);
	ol = object_list_push(ol, teapot1);

	if(argc > 1)
	{
		if (strcmp(argv[1], "0") == 0)
		{
			laby_print();
			fprintf(stdout, "%d\n", ol->size);
			return 0;
		} else if (strcmp(argv[1], "day") == 0) {
			conf->time = DAY;
		} else if (strcmp(argv[1], "night") == 0){
			conf->time = NIGHT;
		}
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(SCREEN_POSITION_X, SCREEN_POSITION_Y);

	conf->id_windows = glutCreateWindow("SAI Project - 3D Laby Nicolas Reynaud && Kevin Hivert.");

	if ((argc == 2 && strcmp(argv[1], "full") == 0) || (argc == 3 && strcmp(argv[2], "full") == 0))
	{
		glutFullScreen();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(SCREEN_MID_HEIGHT, SCREEN_MID_HEIGHT);

	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutMouseFunc(mouse_trigger);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special_keyboard);
	glutSpecialUpFunc(special_keyboard_up);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	
	glutMainLoop();

return 0;
}

