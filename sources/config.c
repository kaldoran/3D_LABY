/*
 * SAI project - 3D Laby
 * File : config.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "config.h"

Config *config_new(void)
{
	Config *conf;
	if ((conf = malloc(sizeof *conf)) == NULL)
	{
		return NULL;
	}

	conf->eye_direction  = point_new(0, HORIZON, 0);
	conf->body_direction = point_new(0, 1, 0);
	conf->eye            = point_new(CELL_SIZE / 2, CELL_SIZE / 2, CHARACTER_SIZE);
	conf->center         = point_new(conf->eye->x + conf->eye_direction->x,
									conf->eye->y + conf->eye_direction->y,
									conf->eye->z + conf->eye_direction->z);
	conf->up             = point_new(0, 0, 1);

	conf->theta          = 90.;
	conf->phi            = 0.;

	conf->time           = NIGHT;
	conf->life           = MAX_HEALTH;
	conf->shoot          = 0;
	
	conf->print_config   = 0;
	conf->free_fly       = 0;
	conf->full_screen    = 0;

	conf->quadTreeView   = 0;
	conf->viewMode       = 0;
	
	memset(conf->key,0,sizeof(conf->key));
	memset(conf->mousebutton,0,sizeof(conf->mousebutton));
	conf->mousex         = SCREEN_MID_WIDTH;
	conf->mousey         = SCREEN_MID_HEIGHT;
	conf->mousexrel      = 0;
	conf->mouseyrel      = 0;
	conf->quit           = 0;

	conf->music 		 = "music/music.mp3";
	conf->volume	     = MIX_MAX_VOLUME;

	conf->text = 0;
	conf->display = 0;
	conf->timer = 0;
	conf->last_time_dmg = 0;
	conf->jump_duration = 0;
return conf;
}

void config_free(Config *conf)
{
	point_free(conf->eye_direction);
	point_free(conf->body_direction);
	point_free(conf->eye);
	point_free(conf->center);
	point_free(conf->up);
	free(conf->path);
free(conf);
}

Point *forward_move(Point *save_eye, float speed) {

	save_eye->x += speed * conf->body_direction->x;
	save_eye->y += speed * conf->body_direction->y;
return save_eye;
}

Point *backward_move(Point *save_eye, float speed) {
	save_eye->x -= speed * conf->body_direction->x;
	save_eye->y -= speed * conf->body_direction->y;
return save_eye;
}

Point *left_move(Point *save_eye, float speed) {
	save_eye->x += -(speed * conf->body_direction->y);
	save_eye->y += speed * conf->body_direction->x;
return save_eye;
}

Point *right_move(Point *save_eye, float speed) {
	save_eye->x += speed * conf->body_direction->y;
	save_eye->y += -(speed * conf->body_direction->x);
return save_eye;
}

void change_center(void)
{
	conf->center->x = conf->eye->x + conf->eye_direction->x;
	conf->center->y = conf->eye->y + conf->eye_direction->y;
	conf->center->z = conf->eye->z + conf->eye_direction->z;
}

void modify_direction(void)
{
	float tmp;
	if (conf->theta > 360 )
	{
		conf->theta = (int)conf->theta % 360;
	} else if (conf->theta < -360 ) {
		conf->theta = abs((int)conf->theta) % 360;
	}

	if (conf->phi > 89 )
	{
		conf->phi = 89;
	} else if (conf->phi < -89) {
		conf->phi = -89;
	}

	tmp = HORIZON * cos(conf->phi * M_PI / 180);

	conf->body_direction->x = cos(conf->theta * M_PI / 180);
	conf->body_direction->y = sin(conf->theta * M_PI / 180);

	conf->eye_direction->x = tmp * conf->body_direction->x;
	conf->eye_direction->y = tmp * conf->body_direction->y;
	conf->eye_direction->z = HORIZON * sin(conf->phi * M_PI / 180);
	
	change_center();
}

char *my_strcat(char *start, char *end) {
	char* temp;
	
	temp = malloc(strlen(start) +strlen(end) + 1);
	strcpy(temp, start);
	strcat(temp, end);
	
	return temp;	
}

int pow2sup(int i)
{
    double logbase2 = log(i) / log(2);
    return (int) floor(pow(2.0, ceil(logbase2)));
}
