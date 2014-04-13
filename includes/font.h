/*
 * SAI project - 3D Laby
 * File : font.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */

#include <SDL/SDL_ttf.h>
#include "config.h" 

#ifndef __FONT
#define __FONT

TTF_Font *font;

TTF_Font *font_load(char *name, int size);

void font_new(void);
void font_close(TTF_Font *font);
void font_delete(void);

#endif
