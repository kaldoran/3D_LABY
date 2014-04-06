/*
 * SAI project - 3D Laby
 * File : main.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdint.h>
#include <unistd.h>

#include "laby.h"
#include "k-tree.h"
#include "config.h"
#include "object.h"
#include "display.h"
#include "portals.h"
#include "event.h"
#include "font.h" 
#include "music.h"

#define BUFFER_SIZE 16

int main( int argc, char* argv[] )
{
	const SDL_VideoInfo* info = NULL;
	int value_att = 0;
	char pc = '%', buffer[BUFFER_SIZE];
	GLfloat fogColor[4]     = {0, 0, 0, 1};

	Object *floor        = object_new(0, 0, 0, FLOOR);
	Object *border       = object_new(0, 0, 0, BORDER);
	/*Object *sun          = object_new(WIDTH * CELL_SIZE + (CELL_SIZE * WIDTH) / 2, CELL_SIZE * HEIGHT + 4 * CELL_SIZE * HEIGHT / 5, 500, SUN);*/
	Object *giant_teapot = object_new(-10 * CELL_SIZE, HEIGHT * CELL_SIZE / 2, 6 * CELL_SIZE, TEAPOT);

	laby    = laby_new();
	conf    = config_new();
	ol      = object_list_new();
	portals = portals_new();

	maze_generation();
	maze_moving_walls_generation();

	ol = object_list_push(ol, floor);
	ol = object_list_push(ol, border);
	/*ol = object_list_push(ol, sun);*/
	ol = object_list_generate_fir_trees(ol);
	ol = object_list_push_maze_walls(ol);
	ol = object_list_push(ol, giant_teapot);

	fprintf(stdout, "%s%s", CYEL, CBLINK);
	fprintf(stdout, "                             .,-:;//;:=\n");
	fprintf(stdout, "                        . :H@@@MM@M#H/.,+%c;,\n", pc);
	fprintf(stdout, "                     ,/X+ +M@@M@MM%c=,-%cHMMM@X/,\n", pc, pc);
	fprintf(stdout, "                   -+@MM; $M@@MH+-,;XMMMM@MMMM@+-\n");
	fprintf(stdout, "                  ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.\n");
	fprintf(stdout, "                ,%cMM@@MH ,@%c=              .------=.\n", pc, pc);
	fprintf(stdout, "                =@#@@@MX.                 -%cHX$$%c%c:;\n", pc, pc, pc);
	fprintf(stdout, "               =-./@M@M$                  .;@MMMM@MM:\n");
	fprintf(stdout, "               X@/ -$MM/                   . +MM@@@M$\n");
	fprintf(stdout, "              ,@M@H: :@:   Teapot Science   . =X#@@@@-\n");
	fprintf(stdout, "              ,@@@MMX,                      /H- ;@M@M=\n");
	fprintf(stdout, "              .H@@@@M@+,          & Co.     %cMM+..%c#$.\n", pc, pc);
	fprintf(stdout, "               /MMMM@MMH/.                  XM@MH; =;\n");
	fprintf(stdout, "                /%c+%c$XHH@$=              , .H@@@@MX,\n", pc, pc);
	fprintf(stdout, "                 .=--------.           -%cH.,@@@@@MX,\n", pc);
	fprintf(stdout, "                 .%cMM@@@HHHXX$$$%c+- .:$MMX =M@@MM%c.\n", pc, pc, pc);
	fprintf(stdout, "                   =XMMM@MM@MM#H;,-+HMM@M+ /MMMX\n");
	fprintf(stdout, "                     =%c@M@M#@$-.=$@MM@@@M; %cM%c=\n", pc, pc, pc);
	fprintf(stdout, "                       ,:+$+-,/H#MMMMMMM@= =,\n");
	fprintf(stdout, "                             =++%c%c%c%c+/:-.\n\n\n", pc, pc, pc, pc);
	fprintf(stdout, "%s%s", CRESET, CYEL);

	if(laby == NULL || conf == NULL || ol == NULL)
	{
		fprintf(stderr, "We are sorry an error as occurred.\n");
		fprintf(stderr, "%s\n", CRESET);
		exit(EXIT_FAILURE);
	}

	if(argc > 1)
	{
		if (!strcasecmp(argv[1], "Debug"))
		{
			fprintf(stdout, "Hello Human and Welcome to our new NON-computer-aided DEBUG MODE.\n\n");
			laby_print();
			fprintf(stdout, "%d Elements in the environment.\n", ol->size);
			fprintf(stderr, "%s\n", CRESET);
			
			object_list_free(ol);
			config_free(conf);
			laby_free(laby);
			portals_free(portals);
			fprintf(stdout, "Good bye !\n");
			return 0;
		}
	}

	fprintf(stdout, "Hello Human and Welcome to our new NON-computer-aided enrichment center.\n\n");
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
		fprintf(stderr, "%s\n", CRESET);

		buffer[0] = '\0';
		fgets (buffer, BUFFER_SIZE - 1, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		if (!strcasecmp(buffer,"Day") || !strcasecmp(buffer,"D")) {
			conf->time = DAY;
		}

		fprintf(stdout, "%s", CYEL);
		fprintf(stdout, "What music do you want ? (1 | 2)\n");
		fprintf(stderr, "%s\n", CRESET);

		buffer[0] = '\0';
		fgets (buffer, BUFFER_SIZE - 1, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		if (!strcasecmp(buffer,"2")) {
			conf->music = "music/music2.mp3";
		}

		fprintf(stdout, "%s", CYEL);
		fprintf(stdout, "Full screen ? (yes | No)\n");
		fprintf(stderr, "%s\n", CRESET);

		buffer[0] = '\0';
		fgets (buffer, BUFFER_SIZE - 1, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		if (!strcasecmp(buffer,"Yes") || !strcasecmp(buffer,"Y")) {
			conf->full_screen = 1;
		}
	}

	fprintf(stdout, "%s", CYEL);
	fprintf(stdout, "\n\nPor favor bordón de fallar Muchos gracias de fallar gracias.\n...\n");
	fprintf(stdout, "Here's a companion cube to help you in your test !\n");
	fprintf(stdout,"+@##########M/             :@#########@/\n");
	fprintf(stdout,"##############$;H#######@;+#############\n");
	fprintf(stdout,"###############M########################\n");
	fprintf(stdout,"##############X,-/++/+%c+/,%c#############\n", pc, pc);
	fprintf(stdout,"############M$:           -X############\n");
	fprintf(stdout,"##########H;.      ,--.     =X##########\n");
	fprintf(stdout,":X######M;     -$H@M##MH%c:    :H#######@\n", pc);
	fprintf(stdout,"  =%c#M+=,   ,+@#######M###H:    -=/M#%c\n", pc, pc);
	fprintf(stdout,"  %cM##@+   .X##$, ./+- ./###;    +M##%c\n", pc, pc);
	fprintf(stdout,"  %c####M.  /###=    .    @##M.   X###%c\n", pc, pc);
	fprintf(stdout,"  %c####M.  ;M##H:.     =$###X.   $###%c\n", pc, pc);
	fprintf(stdout,"  %c####@.   /####M$-./@#####:    %c###%c\n", pc, pc, pc);
	fprintf(stdout,"  %cH#M/,     /H###########@:     ./M#%c\n", pc, pc);
	fprintf(stdout," ;$H##@@H:    .;$HM#MMMH$;,   ./H@M##M$=\n");
	fprintf(stdout,"X#########%c.      ..,,.     .;@#########\n", pc);
	fprintf(stdout,"###########H+:.           ./@###########\n");
	fprintf(stdout,"##############/ ./%c%c%c%c+/.-M#############\n", pc, pc, pc, pc);
	fprintf(stdout,"##############H$@#######@@##############\n");
	fprintf(stdout,"##############X%c########M$M#############\n", pc);
	fprintf(stdout,"+M##########H:            .$##########X=\n");
	sleep(1);
	fprintf(stdout, "You wil have a cake if you win...\n");

	/*
	 * SDL initialisation. 
	  */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "We are sorry :SDL_Init Failure.\n");
		fprintf(stderr, "%s\n", CRESET);
		exit(EXIT_FAILURE);
	}

	info = SDL_GetVideoInfo();
	if( !info ) {
		/* This should probably never happen. */
		fprintf( stderr, "We are sorry : Video query failed: %s\n",
			SDL_GetError() );
		fprintf(stderr, "%s\n", CRESET);
		exit(EXIT_FAILURE);
    }
	
	if (conf->full_screen)
	{
		conf->pScreen = SDL_SetVideoMode(SCREEN_HEIGHT, SCREEN_WIDTH, info->vfmt->BitsPerPixel, SDL_OPENGL | SDL_FULLSCREEN);
	} else {
		conf->pScreen = SDL_SetVideoMode(SCREEN_HEIGHT, SCREEN_WIDTH, info->vfmt->BitsPerPixel, SDL_OPENGL);
		SDL_WM_SetCaption(TITLE, NULL);
	}

	if (!conf->pScreen)
	{
		fprintf(stderr, "We are sorry : SDL_SetVideoMode Failure.\n");
		fprintf(stderr, "%s\n", CRESET);
		exit(EXIT_FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if(SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value_att) < 0 || value_att != 1)
	{
		fprintf(stderr, "We are sorry : Double buffer failure\n");
		fprintf(stderr, "%s\n", CRESET);
		exit (EXIT_FAILURE);
	}
	
	if (TTF_Init() < 0) {
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit (EXIT_FAILURE);
	}
	
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit (EXIT_FAILURE);
	}

	/* Définit le nombre de channels à mixer */
	Mix_AllocateChannels(32);

	atexit(SDL_Quit);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, (double)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR, FAR);
	
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);

	glEnable(GL_FOG) ;
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_START, 1);
	glFogf(GL_FOG_END,CELL_SIZE * 15);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	sky_box_new();
	font_new();
	music_new();
	
	main_loop();

	font_delete();
	sky_box_delete();
	music_delete();
	
	object_list_free(ol);
	portals_free(portals);
	config_free(conf);
	laby_free(laby);
	

	fprintf(stdout, "Good bye !\n");
	fprintf(stderr, "%s\n", CRESET);
return EXIT_SUCCESS;
}
