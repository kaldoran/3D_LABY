/*
 * SAI project - 3D Laby
 * File : my_glut.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */

#ifndef __GLUT
#define __GLUT

#define ESC 27
 
void move();
void keyboard(unsigned char key, int x , int y);
void keyboard_up(unsigned char key, int x, int y);
void special_keyboard(int key, int x, int y);
void special_keyboard_up(int key, int x, int y);
void mouse_motion(int x, int z);
void mouse_trigger(int button, int state, int x, int y);
void display();
void DrawEllipse(float radiusX, float radiusY);
void write_string(char* string, int x, int y, void* font);
#endif
