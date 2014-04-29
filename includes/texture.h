/*
 * SAI project - 3D Laby
 * File : texture.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_ttf.h>

#ifndef __TEXTURES
#define __TEXTURES

GLuint load_texture(const char* file);
void create_texture_from_text(TTF_Font *font, char *string, int r, int g, int b);

#endif
