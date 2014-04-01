/*
 * SAI project - 3D Laby
 * File : config.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "k-tree.h"

#ifndef __CONF
#define __CONF

#define UNUSED(x) (void)(x)

#define SCREEN_POSITION_X	300
#define SCREEN_POSITION_Y	600
#define SCREEN_HEIGHT 		800
#define SCREEN_MID_HEIGHT 	(SCREEN_HEIGHT / 2)
#define SCREEN_WIDTH 		800
#define SCREEN_MID_WIDTH 	(SCREEN_WIDTH / 2)

#define CELL_SIZE 		25

#define HORIZON 		1
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
	char keys[8];		/* Up , Down , Left , Right, Alt - Maj, 8, 2 */
	
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
void config_free(Config *conf);

Point *forward_move(Point *save_eye, float speed);
Point *backward_move(Point *save_eye, float speed);
Point *left_move(Point *save_eye, float speed);
Point *right_move(Point *save_eye, float speed);

void change_center();
void modify_direction();
#endif
