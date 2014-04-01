/*
 * SAI project - 3D Laby
 * File : config.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "k-tree.h"
#include "laby.h"

#ifndef __CONF
#define __CONF

#define SCREEN_POSITION_X	300
#define SCREEN_POSITION_Y	600
#define SCREEN_HEIGHT 		800
#define SCREEN_MID_HEIGHT 	(SCREEN_HEIGHT / 2)
#define SCREEN_WIDTH 		800
#define SCREEN_MID_WIDTH 	(SCREEN_WIDTH / 2)

#define CELL_SIZE 		25
#define HORIZON 		5 * CELL_SIZE
#define SENSITIVITY 	0.07
#define CHARACTER_SIZE	10
#define NEAR 			1
#define FAR				800

#define DAY		1
#define NIGHT 	0

typedef struct Config {
	Point *eye;
	Point *center;
	Point *up;

	Point *eye_direction;
	Point *body_direction;

	float theta;
	float phi;
	char keys[4];
	
	int time;
	int free_fly;

	int id_windows;
	int print_config;
} Config;

/*
 * void gluLookAt(
 *				GLdouble eyeX, 
 * 				GLdouble eyeY,
 * 				GLdouble eyeZ,
 *
 * 				GLdouble centerX,
 * 				GLdouble centerY,
 * 				GLdouble centerZ,
 *
 * 				GLdouble upX,
 * 				GLdouble upY,
 *				GLdouble upZ);
 */

/* Global config structure */
Config *conf;

Config *config_new();
void config_free();

Point *forward_move(Config *conf, Point *save_eye, float speed);
Point *backward_move(Config *conf, Point *save_eye, float speed);
Point *left_move(Config *conf, Point *save_eye, float speed);
Point *right_move(Config *conf, Point *save_eye, float speed);

Config *change_center(Config *conf);
Config *modify_direction(Config *conf);
#endif
