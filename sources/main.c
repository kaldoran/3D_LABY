/*
 * SAI project - 3D Laby
 * File : main.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
	char pc = '%', buffer[BUFFER_SIZE];
	Object *floor = object_new(0, 0, 0, FLOOR);
	Object *border = object_new(0, 0, 0, BORDER);
	Object *sun = object_new(WIDTH * CELL_SIZE + (CELL_SIZE * WIDTH) / 2, CELL_SIZE * HEIGHT + 4 * CELL_SIZE * HEIGHT / 5, 500, SUN);
	Object *giant_teapot = object_new(-10 * CELL_SIZE, HEIGHT * CELL_SIZE / 2, 6 * CELL_SIZE, TEAPOT);

	portals = portals_new();
	laby = laby_new();
	conf = config_new();
	ol = object_list_new();

	fprintf(stdout, "%s %s", CYEL, CBLINK);
	fprintf(stdout, "                             .,-:;//;:=\n");
	fprintf(stdout, "                        . :H@@@MM@M#H/.,+%c;,\n", pc);
	fprintf(stdout, "                     ,/X+ +M@@M@MM%c=,-%cHMMM@X/,\n", pc, pc);
	fprintf(stdout, "                   -+@MM; $M@@MH+-,;XMMMM@MMMM@+-\n");
	fprintf(stdout, "                  ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.\n");
	fprintf(stdout, "                ,%cMM@@MH ,@%c=             .------=.\n", pc, pc);
	fprintf(stdout, "                =@#@@@MX.                 -%cHX$$%c%c:;\n", pc, pc, pc);
	fprintf(stdout, "               =-./@M@M$                   .;@MMMM@MM:\n");
	fprintf(stdout, "               X@/ -$MM/                    . +MM@@@M$\n");
	fprintf(stdout, "              ,@M@H: :@:   Teapot Science   . =X#@@@@-\n");
	fprintf(stdout, "              ,@@@MMX,                      /H- ;@M@M=\n");
	fprintf(stdout, "              .H@@@@M@+,          & Co.     %cMM+..%c#$.\n", pc, pc);
	fprintf(stdout, "               /MMMM@MMH/.                  XM@MH; =;\n");
	fprintf(stdout, "                /%c+%c$XHH@$=              , .H@@@@MX,\n", pc, pc);
	fprintf(stdout, "                 .=--------.           -%cH.,@@@@@MX,\n", pc);
	fprintf(stdout, "                 .%cMM@@@HHHXX$$$%c+- .:$MMX =M@@MM=%c.\n", pc, pc, pc);
	fprintf(stdout, "                   =XMMM@MM@MM#H;,-+HMM@M+ /MMMX\n");
	fprintf(stdout, "                     =%c@M@M#@$-.=$@MM@@@M; %cM%c=\n", pc, pc, pc);
	fprintf(stdout, "                       ,:+$+-,/H#MMMMMMM@= =,\n");
	fprintf(stdout, "                             =++%c%c%c%c+/:-.\n\n\n", pc, pc, pc, pc);
	fprintf(stdout, "%s %s", CRESET, CYEL);

	if(laby == NULL || conf == NULL || ol == NULL)
	{
		fprintf(stderr, "We are sorry an error as occurred.\n");
		fprintf(stderr, "%s \n", CRESET);
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

	if(argc > 1)
	{
		if (!strcasecmp(argv[1], "Debug"))
		{
			fprintf(stdout, "Hello Human and Welcome to our new computer-NON-aided DEBUG MODE.\n\n");
			laby_print();
			fprintf(stdout, "%d Elements in the environment.\n", ol->size);
			fprintf(stderr, "%s \n", CRESET);
			return 0;
		}
	}

	fprintf(stdout, "Hello Human and Welcome to our new computer-NON-aided enrichment center.\n\n");
	fprintf(stdout, "We hope your brief detention in this NON-relaxation maze will be a pleasant one.\n");
	fprintf(stdout, "Your specimen has not been processed and we hope we are now ready to begin the test proper.\n");
	fprintf(stdout, "Before we start, however, keep in mind that although fun and learning are the primary goals of\n");
	fprintf(stdout, "all enrichment center activities, serious injuries may occur.\n");
	fprintf(stdout, "For your own safety and the safety of others, please refrain from MM@MM@MM#H\n");
	fprintf(stdout, "Do you want to choose your environment ? (yes / No)\n");
	fprintf(stderr, "%s \n", CRESET);
	
	fgets (buffer, BUFFER_SIZE - 1, stdin);
	buffer[strlen(buffer) - 1] = '\0';

	if(!strcasecmp(buffer,"yes") || !strcasecmp(buffer,"y")) {
		fprintf(stdout, "%s", CYEL);
		fprintf(stdout, "Night or Day ? (day | Night)\n");
		fprintf(stderr, "%s \n", CRESET);

		buffer[0] = '\0';
		fgets (buffer, BUFFER_SIZE - 1, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		if (!strcasecmp(buffer,"Day") || !strcasecmp(buffer,"D")) {
			conf->time = DAY;
		}

		fprintf(stdout, "%s", CYEL);
		fprintf(stdout, "Full screen ? (yes | No)\n");
		fprintf(stderr, "%s \n", CRESET);

		buffer[0] = '\0';
		fgets (buffer, BUFFER_SIZE - 1, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		if (!strcasecmp(buffer,"Yes") || !strcasecmp(buffer,"Y")) {
			conf->full_screen = 1;
		}
	}

	fprintf(stdout, "%s", CYEL);
	fprintf(stdout, "\nPor favor bordón de fallar Muchos gracias de fallar gracias.\n");

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(SCREEN_POSITION_X, SCREEN_POSITION_Y);

	conf->id_windows = glutCreateWindow("SAI Project - 3D Laby Nicolas Reynaud && Kevin Hivert.");

	if (conf->full_screen)
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

