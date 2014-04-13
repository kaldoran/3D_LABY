/*
 * SAI project - 3D Laby
 * File : config.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "object.h"
#include "laby.h"

#ifndef __CONF
#define __CONF

#define TITLE               "Teapot Science and Co. new NON-computer-aided enrichment center."

#define SCREEN_POSITION_X   300
#define SCREEN_POSITION_Y   600
#define SCREEN_HEIGHT       600
#define SCREEN_MID_HEIGHT   (SCREEN_HEIGHT / 2)
#define SCREEN_WIDTH        600
#define SCREEN_MID_WIDTH    (SCREEN_WIDTH / 2)

#define CELL_SIZE           16
#define SPIKE_HEIGHT	   0.5
#define HEIGHT_MIN_BORDER   3
#define HEIGHT_MAX_BORDER   7
#define HORIZON             1
#define CHARACTER_SIZE      7
#define FOVY                70
#define NEAR                1
#define FAR                 SIZE * 2 * CELL_SIZE
#define SENSITIVITY         0.05

#define DAY                 1
#define NIGHT               0

#define CRESET              "\x1B[0m"
#define CBLINK              "\x1B[5m"
#define CYEL                "\x1B[93m"

#define HEIGHT_CHAR         20

#define MAX_HEALTH          15
#define TIME_BETWEEN_DMG    7 /* Temps en Milliseconde */

typedef struct Config {
	char *path;
	
	Point *eye;
	Point *center;
	Point *up;

	Point *eye_direction;
	Point *body_direction;

	float theta;
	float phi;

	char free_fly;
	char time;
	char life;
	char shoot;
	char print_config;
	char quadTreeView;

	char full_screen;
	SDL_Surface* pScreen;

	char key[SDLK_LAST];
	int  mousex;
	int  mousey;
	int  mousexrel;
	int  mouseyrel;
	char mousebutton[8];
	char quit;
	
	char *music;
	int volume;

	GLuint text;
	unsigned int width_text;
	unsigned int height_text;
} Config;

/* Global config structure */
Config *conf;

Config *config_new(void);
void config_free(Config *conf);

Point *forward_move(Point *save_eye, float speed);
Point *backward_move(Point *save_eye, float speed);
Point *left_move(Point *save_eye, float speed);
Point *right_move(Point *save_eye, float speed);

void change_center(void);
void modify_direction(void);
char *my_strcat(char *start, char *end);

int pow2sup(int i);
#endif
