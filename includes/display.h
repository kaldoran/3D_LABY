/*
 * SAI project - 3D Laby
 * File : object.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "object.h"
 
#ifndef __DISPLAY
#define __DISPLAY

void display();

void time_color();
void Object_floor_print();
void Object_border_print();
void Object_sun_print(Object *sun);
void Object_fir_tree_print(Object *fir_tree);
void Object_wall_print(Object *wall);
void Object_moving_wall_print(Object *wall);
void Object_entry_print(Object *object);
void Object_exit_print(Object *object);
void Object_teapot_print(Object *teapot);

void DrawEllipse(float radiusX, float radiusY);
void portal_maker ();
#endif