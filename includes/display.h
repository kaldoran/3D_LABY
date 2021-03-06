/*
 * SAI project - 3D Laby
 * File : object.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <SDL/SDL_ttf.h>
#include "object.h"

 
#ifndef __DISPLAY
#define __DISPLAY

#define SKY_BACK      0
#define SKY_FRONT     1
#define SKY_LEFT      2
#define SKY_RIGHT     3
#define SKY_BOTTOM    4
#define SKY_TOP       5

#define CURSOR_BLUE   0
#define CURSOR_ORANGE 1
#define CURSOR_BOTH   2
#define CURSOR_NONE   3

#define HEIGHT_CURSOR 64
#define WIDHT_CURSOR  64

#define HEIGHT_HEART  17
#define WIDTH_HEART   20
#define MARGING_HEART 5

#define MARGING_FONT  20

unsigned int skybox[6];
unsigned int cursors[4];
unsigned int heart;

void display(void);

void time_color(void);
void Object_floor_print(void);
void Object_border_print(void);
void Object_sun_print(Object *sun);
void Object_fir_tree_print(Object *fir_tree);
void Object_wall_print(Object *wall);
void Object_moving_wall_print(Object *wall);
void Object_entry_print(Object *object);
void Object_exit_print(Object *object);
void Object_spikes_print(Object *spikes);

void portal_maker(void);

void sky_box_print(float size);
void sky_box_new(void);
void sky_box_delete(void);

void cursor_print(void);
void cursors_new(void);
void cursors_delete(void);

void change_to_2d(void);
void change_to_3d(void);

void heart_new(void);
void heart_delete(void);
void life_print(void);

void text_print_coord(int x, int y);
void text_print(TTF_Font *font, char *buffer, int r, int g, int b, int x, int y);

void check_dommage();

void moving_wall_list_display();

void timer_convert(Uint32 timer);

float jump(Point *save_eye);
#endif
