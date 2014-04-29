/*
 * SAI project - 3D Laby
 * File : event.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
 *
  */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdint.h>
#include <math.h>

#include "event.h"
#include "k-tree.h"
#include "config.h"
#include "laby.h"
#include "object.h"
#include "display.h"
#include "portals.h"
#include "music.h"

void main_loop(void)
{
	Doubly_linked_node *iterator;
	int direction, dx, dy, count;
	int x, y;

	uint32_t last_time;
	uint32_t current_time;
	uint32_t ellapsed_time;
	uint32_t start_time;

	Point *save_eye;
	float speed;

	last_time = SDL_GetTicks();
	while (!conf->key[SDLK_ESCAPE] && !conf->quit)
	{
		start_time = SDL_GetTicks(); 
		save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
		speed  = (conf->free_fly) ? 5 : 0.5;

		update_event();

		if (!conf->viewMode && conf->key[SDLK_F1])
		{
			conf->key[SDLK_F1] = 0;
			conf->free_fly = !conf->free_fly;
			if (!conf->free_fly)
			{
				save_eye->x = CELL_SIZE / 2;
				save_eye->y = CELL_SIZE / 2;
				save_eye->z = CHARACTER_SIZE;
			}
			else {
				conf->jump_duration = 0;
			}
		}

		if (!conf->viewMode && (conf->key[SDLK_LSHIFT] || conf->key[SDLK_LALT]))
		{
			speed = (conf->free_fly) ? 7.51337 : 0.91337;
		}

		if (!conf->viewMode &&  conf->key[SDLK_LCTRL])
		{
			speed = (conf->free_fly) ? 0.1 : 0.3;
		}

		if (conf->key[SDLK_F2])
		{
			conf->key[SDLK_F2] = 0;
			conf->time = !conf->time;
		}

		if (conf->key[SDLK_F3])
		{
			conf->key[SDLK_F3] = 0;
			conf->display = !conf->display;
		}

		if (!conf->viewMode &&  conf->key[SDLK_F4])
		{
			conf->key[SDLK_F4] = 0;
			conf->quadTreeView = !conf->quadTreeView;
		}

		if (conf->key[SDLK_F5])
		{
			conf->key[SDLK_F5] = 0;
			if (!strcmp(conf->music, "music/music2.mp3"))
			{
				conf->music = "music/music.mp3";
				music_new();
			} else if (!strcmp(conf->music, "music/music.mp3")) {
				conf->music = "music/music2.mp3";
				music_new();
			}
		}
		
		if (!conf->viewMode && (conf->key[SDLK_UP] || conf->key[SDLK_z]))
		{
			forward_move(save_eye, speed);
		}

		if (!conf->viewMode && (conf->key[SDLK_DOWN] || conf->key[SDLK_s]))
		{
			backward_move(save_eye, speed);
		}

		if (!conf->viewMode && (conf->key[SDLK_RIGHT] || conf->key[SDLK_d]))
		{
			right_move(save_eye, speed);
		}

		if (!conf->viewMode && (conf->key[SDLK_LEFT] || conf->key[SDLK_q]))
		{
			left_move(save_eye, speed);
		}

		if (!conf->viewMode && conf->key[SDLK_a])
		{
			conf->key[SDLK_a] = 0;
			conf->theta += 180;
		}

		if (!conf->viewMode && conf->key[SDLK_r])
		{
			conf->key[SDLK_r] = 0;
			conf->life = MAX_HEALTH;
			save_eye->x = CELL_SIZE / 2;
			save_eye->y = CELL_SIZE / 2;
			save_eye->z = CHARACTER_SIZE;
			
			portals->bleu->actif = 0;
			portals->orange->actif = 0;
			conf->timer = SDL_GetTicks();
		}

		if (conf->key[SDLK_KP2] || (conf->key[SDLK_n]))
		{
			if (conf->free_fly) {
				save_eye->z -= 1;
			}
		}

		if (conf->key[SDLK_KP8] || (conf->key[SDLK_SPACE]))
		{
			if (!conf->free_fly)
			{
				if ( !conf->viewMode && conf->jump_duration == 0 ) {
					conf->jump_duration = 120;
				}

			} 
			else {
				save_eye->z += 1;
			}
		}
		
		conf->eye->z += jump(save_eye);
		
		if ( conf->key[SDLK_p] ) {
			conf->key[SDLK_p] = 0;
			if(Mix_PausedMusic() == 1) {
				Mix_ResumeMusic();
			}
			else {
				Mix_PauseMusic();
			}
		}
		
		if ( conf->key[SDLK_PLUS] || conf->key[SDLK_m] || conf->key[SDLK_KP_PLUS] ) {
			change_volume(CHANG_VOL);
		}

		if ( conf->key[SDLK_MINUS] || conf->key[SDLK_l] || conf->key[SDLK_KP_MINUS] ) {
			change_volume(-CHANG_VOL);
		}
		
		if (!conf->mousebutton[SDL_BUTTON_LEFT] && !conf->mousebutton[SDL_BUTTON_RIGHT])
		{
			conf->shoot = 0;
		}

		if (!conf->viewMode && conf->mousebutton[SDL_BUTTON_LEFT])
		{
			conf->mousebutton[SDL_BUTTON_LEFT] = 0;
			conf->mousebutton[SDL_BUTTON_RIGHT] = 0;
			conf->shoot = 1;
		}

		if (!conf->viewMode && conf->mousebutton[SDL_BUTTON_RIGHT])
		{
			conf->mousebutton[SDL_BUTTON_LEFT] = 0;
			conf->mousebutton[SDL_BUTTON_RIGHT] = 0;
			conf->shoot = 2;
		}
		
		if (!conf->viewMode && portals->orange->actif && portals->bleu->actif ) {
			if ( abs(save_eye->x - portals->bleu->portail->x ) < TRIGGER_DISTANCE && abs(save_eye->y - portals->bleu->portail->y) < TRIGGER_DISTANCE) {
				save_eye->x = portals->orange->portail->x - (sin(portals->orange->rotation) * PUSH_DISTANCE);
				save_eye->y = portals->orange->portail->y + (cos(portals->orange->rotation) * PUSH_DISTANCE);
				conf->theta += 180 + ( portals->orange->rotation - portals->bleu->rotation);
			}
			else if ( abs(save_eye->x - portals->orange->portail->x ) < TRIGGER_DISTANCE && abs(save_eye->y - portals->orange->portail->y) < TRIGGER_DISTANCE) {
				save_eye->x = portals->bleu->portail->x - (sin(portals->bleu->rotation) * PUSH_DISTANCE);
				save_eye->y = portals->bleu->portail->y + (cos(portals->bleu->rotation) * PUSH_DISTANCE);
				conf->theta += 180 + ( portals->bleu->rotation - portals->orange->rotation);
			}
		}

		if (COORD((int)(save_eye->x / CELL_SIZE), (int)(save_eye->y / CELL_SIZE))
				!= COORD((int)((conf->eye)->x / CELL_SIZE), (int)((conf->eye)->y / CELL_SIZE)))
		{
			iterator = mwl->last;
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
					 		if(laby->matrix[COORD(x,y)] == SPIKES_MW) {
					 			laby->matrix[COORD(x,y)] = SPIKES;
					 		} else if (laby->matrix[COORD(x,y)] == MOVING_WALL) {
					 			laby->matrix[COORD(x,y)] = PASS;
					 		}

							if(laby->matrix[COORD((x+dx),(y+dy))] == PASS)
							{
								laby->matrix[COORD((x+dx),(y+dy))] = MOVING_WALL;
							} else if (laby->matrix[COORD((x+dx),(y+dy))] == SPIKES) {
								laby->matrix[COORD((x+dx),(y+dy))] = SPIKES_MW;
							}

							((iterator->object)->anchor)->x += (dx * CELL_SIZE);
							((iterator->object)->anchor)->y += (dy * CELL_SIZE);
							count = 4;
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

		/*fprintf(stderr, "%d %d\n", conf->mousex - SCREEN_MID_WIDTH, conf->mousey - SCREEN_MID_HEIGHT);*/
		if (!conf->viewMode && (( save_eye->x > 2 && save_eye->y > 2 
			&& save_eye->x < (CELL_SIZE * WIDTH) - 2 
			&& save_eye->y < (CELL_SIZE * HEIGHT) - 2 
			/*&& save_eye->z <= CHARACTER_SIZE 
			&& save_eye->z > CHARACTER_SIZE - 3*/
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

		/* Mouse motion */
		if(!conf->viewMode)
		{
			conf->theta -= (conf->mousex - SCREEN_MID_WIDTH) * SENSITIVITY;
			conf->phi -= (conf->mousey - SCREEN_MID_HEIGHT) * SENSITIVITY;
			SDL_WarpMouse(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT); 
			modify_direction();
			change_center();
		} else {
			conf->eye->x = conf->center->x + CELL_SIZE * WIDTH * cos(conf->theta * M_PI / 180);
			conf->eye->y = conf->center->y + CELL_SIZE * HEIGHT * sin(conf->theta * M_PI / 180);
			conf->theta += 0.5;
			if (conf->theta >= 360)
			{
				conf->theta = 0;
			}
		}

		if (conf->life <= 0)
		{
			fprintf(stderr,"                          .,---.\n");
			fprintf(stderr,"                        ,/XM#MMMX;,\n");
			fprintf(stderr,"                      -%%##########M%%,\n");
			fprintf(stderr,"                     -@######%%  $###@=\n");
			fprintf(stderr,"      .,--,         -H#######$   $###M:\n");
			fprintf(stderr,"   ,;$M###MMX;     .;##########$;HM###X=\n");
			fprintf(stderr," ,/@##########H=      ;################+\n");
			fprintf(stderr,"-+#############M/,      %%##############+\n");
			fprintf(stderr,"%%M###############=      /##############:\n");
			fprintf(stderr,"H################      .M#############;.\n");
			fprintf(stderr,"@###############M      ,@###########M:.\n");
			fprintf(stderr,"X################,      -$=X#######@:\n");
			fprintf(stderr,"/@##################%%-     +######$-\n");
			fprintf(stderr,".;##################X     .X#####+,\n");
			fprintf(stderr," .;H################/     -X####+.\n");
			fprintf(stderr,"   ,;X##############,       .MM/\n");
			fprintf(stderr,"      ,:+$H@M#######M#$-    .$$=\n");
			fprintf(stderr,"           .,-=;+$@###X:    ;/=.\n");
			fprintf(stderr,"                  .,/X$;   .::,\n");
			fprintf(stderr,"                      .,    ..\n");
			fprintf(stderr,"Haw Haw ! You lose !\n");
			conf->quit = 1;
		}

		if (IS_EXIT(COORD((int)(conf->eye->x / CELL_SIZE), (int)(conf->eye->y / CELL_SIZE))) && !conf->free_fly)
		{
			if (strcmp(conf->music, "music/music3.mp3")) {
				fprintf(stderr, "\n                         #,\n");
				fprintf(stderr, "                        ###\n");
				fprintf(stderr, "                       ## ##\n");
				fprintf(stderr, "                      ##  ##\n");
				fprintf(stderr, "                       ####\n");
				fprintf(stderr, "                         |\n");
				fprintf(stderr, "                        #####\n");
				fprintf(stderr, "                       ######\n");
				fprintf(stderr, "                       ##  ##\n");
				fprintf(stderr, "                       ##  ##\n");
				fprintf(stderr, "                       ##  ##\n");
				fprintf(stderr, "                       ##  ##########\n");
				fprintf(stderr, "                       ##  #############\n");
				fprintf(stderr, "                  #######  ###############\n");
				fprintf(stderr, "              #############################\n");
				fprintf(stderr, "        .###################################\n");
				fprintf(stderr, "       #####################################;\n");
				fprintf(stderr, "       ##                                 ##.\n");
				fprintf(stderr, "       ##                                 ##\n");
				fprintf(stderr, "       #####################################\n");
				fprintf(stderr, "       ##                                 ##\n");
				fprintf(stderr, "       ##                                 ##\n");
				fprintf(stderr, "       ##                                 ###\n");
				fprintf(stderr, "    #####                                 #####\n");
				fprintf(stderr, "   ### ##################################### ###\n");
				fprintf(stderr, "  ###  ##                                 ##  ###\n");
				fprintf(stderr, "  ##   ## ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ##   ##\n");
				fprintf(stderr, "   ##  #####################################  ##\n");
				fprintf(stderr, "    ##                                       ##\n");
				fprintf(stderr, "     ####                                 ####\n");
				fprintf(stderr, "       ######                         ######\n");
				fprintf(stderr, "          ###############################\n");
				fprintf(stderr, "The cake was not a lie !\n");
				conf->music = "music/music3.mp3";
				music_new();
			}
		}

		/* Display with FPS care */
		current_time = SDL_GetTicks();
		ellapsed_time = current_time - last_time;
		last_time = current_time;

		display();

		ellapsed_time = SDL_GetTicks() - start_time;
		if (ellapsed_time < 25)
		{
			SDL_Delay(25 - ellapsed_time);
		}

		point_free(save_eye);
	}
}

void update_event(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				conf->key[event.key.keysym.sym] = 1;
			break;

			case SDL_KEYUP:
				conf->key[event.key.keysym.sym] = 0;
			break;

			case SDL_QUIT:
				conf->quit = 1;
			break;

			case SDL_MOUSEMOTION:
				conf->mousex = event.motion.x;
				conf->mousey = event.motion.y;
				conf->mousexrel = event.motion.xrel;
				conf->mouseyrel = event.motion.yrel;
			break;

			case SDL_MOUSEBUTTONDOWN:
				/*
				 * We don't need the wheel.
				  */
				if (event.button.button!=SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
				{
					conf->mousebutton[event.button.button]=1;
				}
			break;
			
			case SDL_MOUSEBUTTONUP:
				/*
				 * We don't need the wheel.
				  */
				if (event.button.button!=SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
				{
					conf->mousebutton[event.button.button]=0;
				}
			break;
			
			default:
			break;
		}
	}
}
