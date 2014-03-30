/*
 * SAI project - 3D Laby
 * File : config.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include "config.h"

Config *config_new()
{
	Config *conf = malloc(sizeof *conf);
	conf->eye_direction = point_new(0, HORIZON, 0);
	conf->body_direction = point_new(0, 1, 0);
	conf->eye = point_new(CELL_SIZE / 2, CELL_SIZE / 2, CHARACTER_SIZE);
	conf->center = point_new(conf->eye->x + conf->eye_direction->x,
							conf->eye->y + conf->eye_direction->y,
							conf->eye->z + conf->eye_direction->z);
	conf->theta = 90.;
	conf->phi = 0.;
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
