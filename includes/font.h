/*
 * SAI project - 3D Laby
 * File : font.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */

#include <SDL/SDL_ttf.h>

#ifndef __FONT
#define __FONT


TTF_Font *font;

TTF_Font *load_font(char *name, int size);

void font_new(void);
void close_font(TTF_Font *font);
void font_delete(void);



#endif
