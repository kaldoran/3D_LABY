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

void main_loop()
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

		if (conf->key[SDLK_F1])
		{
			conf->key[SDLK_F1] = 0;
			conf->free_fly = !conf->free_fly;
			if (!conf->free_fly)
			{
				save_eye->x = CELL_SIZE / 2;
				save_eye->y = CELL_SIZE / 2;
				save_eye->z = CHARACTER_SIZE;
				change_center();
			}
		}

		if (conf->key[SDLK_LSHIFT] || conf->key[SDLK_LALT])
		{
			speed = (conf->free_fly) ? 7.51337 : 0.91337;
		}

		if (conf->key[SDLK_LCTRL])
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
			/* Print Config. */
		}

		if (conf->key[SDLK_UP] || conf->key[SDLK_z])
		{
			forward_move(save_eye, speed);
		}

		if (conf->key[SDLK_DOWN] || conf->key[SDLK_s])
		{
			backward_move(save_eye, speed);
		}

		if (conf->key[SDLK_RIGHT] || conf->key[SDLK_d])
		{
			right_move(save_eye, speed);
		}

		if (conf->key[SDLK_LEFT] || conf->key[SDLK_q])
		{
			left_move(save_eye, speed);
		}

		if (conf->key[SDLK_a])
		{
			conf->key[SDLK_a] = 0;
			conf->theta += 180;
			conf->phi = 0;
			modify_direction();
		}

		if (conf->key[SDLK_r])
		{
			conf->key[SDLK_r] = 0;
			save_eye->x = CELL_SIZE / 2;
			save_eye->y = CELL_SIZE / 2;
			save_eye->z = CHARACTER_SIZE;
			
			portals->bleu->actif = 0;
			portals->orange->actif = 0;
			change_center();
		}

		if (conf->key[SDLK_KP2])
		{
			save_eye->z -= 1;
		}

		if (conf->key[SDLK_KP8] || (conf->key[SDLK_SPACE]))
		{
			save_eye->z += 1;
		}

		if (!conf->mousebutton[SDL_BUTTON_LEFT] && !conf->mousebutton[SDL_BUTTON_RIGHT])
		{
			conf->shoot = 0;
		}

		if (conf->mousebutton[SDL_BUTTON_LEFT])
		{
			conf->mousebutton[SDL_BUTTON_LEFT] = 0;
			conf->mousebutton[SDL_BUTTON_RIGHT] = 0;
			conf->shoot = 2;
		}

		if (conf->mousebutton[SDL_BUTTON_RIGHT])
		{
			conf->mousebutton[SDL_BUTTON_LEFT] = 0;
			conf->mousebutton[SDL_BUTTON_RIGHT] = 0;
			conf->shoot = 1;
		}

		/* Mouse motion */
		conf->theta -= (conf->mousex - SCREEN_MID_WIDTH) * SENSITIVITY;
		conf->phi -= (conf->mousey - SCREEN_MID_HEIGHT) * SENSITIVITY;
		SDL_WarpMouse(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT); 
		modify_direction();

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

		/*fprintf(stderr, "%d %d\n", conf->mousex - SCREEN_MID_WIDTH, conf->mousey - SCREEN_MID_HEIGHT);*/
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

		/* Display with FPS care */
		current_time = SDL_GetTicks();
		ellapsed_time = current_time - last_time;
		last_time = current_time;

		display();

		ellapsed_time = SDL_GetTicks() - start_time;
		if (ellapsed_time < 10)
		{
			SDL_Delay(10 - ellapsed_time);
		}
	}
}

void update_event()
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