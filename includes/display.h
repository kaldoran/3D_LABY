/*
 * SAI project - 3D Laby
 * File : object.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "object.h"
 
#ifndef __DISPLAY
#define __DISPLAY


#define SKY_BACK   0
#define SKY_FRONT  1
#define SKY_LEFT   2
#define SKY_RIGHT  3
#define SKY_BOTTOM 4
#define SKY_TOP    5

unsigned int skybox[6];

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
void Object_teapot_print(Object *teapot);

void DrawEllipse(float radiusX, float radiusY);
void portal_maker(void);

void sky_box_print(float size);
void sky_box_new(void);
void sky_box_delete(void);
GLuint load_texture(const char* file);

void font_print(char *string, int x, int y);

#endif
