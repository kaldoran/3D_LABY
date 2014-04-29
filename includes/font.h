/*
 * SAI project - 3D Laby
 * File : font.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */

#include <SDL/SDL_ttf.h>
#include "config.h" 

#ifndef __FONT
#define __FONT

#define DEBUG_FONT    0
#define TIMER_FONT    1

#define NUMBER_OF_FONT 2
TTF_Font *font[NUMBER_OF_FONT];

TTF_Font *font_load(char *name, int size);

void font_new(void);
void font_close(TTF_Font *font[NUMBER_OF_FONT]);
void font_delete(void);

#endif
