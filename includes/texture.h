/*
 * SAI project - 3D Laby
 * File : texture.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef __TEXTURES
#define __TEXTURES

GLuint load_texture(const char* file);
void create_texture_from_text(char *string, char *font_name);
int pow2sup(int i);

#endif
