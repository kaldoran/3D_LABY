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
#include "object.h"
#include "laby.h"

Config *config_new()
{
	Config *conf;
	if ((conf = malloc(sizeof *conf)) == NULL)
	{
		return NULL;
	}
	
	conf->eye_direction = point_new(0, HORIZON, 0);
	conf->body_direction = point_new(0, 1, 0);
	conf->eye = point_new(CELL_SIZE / 2, CELL_SIZE / 2, CHARACTER_SIZE);
	conf->center = point_new(conf->eye->x + conf->eye_direction->x,
							conf->eye->y + conf->eye_direction->y,
							conf->eye->z + conf->eye_direction->z);
	
	conf->theta = 90.;
	conf->phi = 0.;
	conf->time = NIGHT;
	conf->id_windows = 0;
	
	conf->print_config = 0;
	conf->free_fly = 0;
	conf->full_screen = 0;

	conf->keys[0] = 0; /* Up */
	conf->keys[1] = 0; /* Down */
	conf->keys[2] = 0; /* Left */
	conf->keys[3] = 0; /* Right */
	conf->keys[4] = 0; /* Maj - Alt */
	conf->keys[5] = 0; /* 2 - Down on Z */
	conf->keys[6] = 0; /* 8 - Up on Z */
	conf->keys[7] = 0; /* CTRL */
return conf;
}

void config_free(Config *conf)
{
	point_free(conf->eye_direction);
	point_free(conf->body_direction);
	point_free(conf->eye);
	point_free(conf->center);
free(conf);
}

Point *forward_move(Point *save_eye, float speed) {

	save_eye->x += speed * conf->body_direction->x;
	save_eye->y += speed * conf->body_direction->y;
	/*
	 * If we go on an other cell
	  */
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

void change_center()
{
	conf->center->x = conf->eye->x + conf->eye_direction->x;
	conf->center->y = conf->eye->y + conf->eye_direction->y;
	conf->center->z = conf->eye->z + conf->eye_direction->z;
}


void modify_direction()
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
	
	change_center(conf);
}
