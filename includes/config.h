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
#define SCREEN_POSITION_Y	50
#define SCREEN_HEIGHT 		800
#define SCREEN_MID_HEIGHT 	(SCREEN_HEIGHT / 2)
#define SCREEN_WIDTH 		800
#define SCREEN_MID_WIDTH 	(SCREEN_WIDTH / 2)

#define HORIZON 		20
#define SENSITIVITY 	0.07
#define CELL_SIZE 		25
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

	int time;
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

Config *config_new();
void config_free();
#endif