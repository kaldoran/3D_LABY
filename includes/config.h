#include "k-tree.h"
#include "laby.h"

#ifndef __CONF
#define __CONF

#define SCREEN_POSITION_X	300
#define SCREEN_POSITION_Y	50
#define SCREEN_HEIGHT 		800
#define SCREEN_MID_HEIGHT 	(HEIGHT / 2)
#define SCREEN_WIDTH 		800
#define SCREEN_MID_WIDTH 	(WIDTH / 2)

#define HORIZON 		20
#define SENSITIVITY 	0.07
#define CELL_SIZE 		25
#define CHARACTER_SIZE	10
#define NEAR 			1
#define FAR				(WIDTH * CELL_SIZE / 2)
typedef struct Config {
	Point *eye;
	Point *center;
	Point *up;

	Point *eye_direction;
	Point *body_direction;

	float theta;
	float phi;
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