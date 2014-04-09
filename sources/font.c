/*
 * SAI project - 3D Laby
 * File : font.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */

#include "font.h"


TTF_Font *font_load(char *name, int size) {

    TTF_Font *font = TTF_OpenFont(name, size);

    if (font == NULL) {
        printf("Failed to open Font %s: %s\n", name, TTF_GetError());
        return NULL;
    }

    return font;
}

void font_new(void) {
	font = font_load(my_strcat(conf->path, "font/zelda.ttf"), 32);
}

void font_close(TTF_Font *font)
{
    if (font != NULL) {
        TTF_CloseFont(font);
    }
}

void font_delete(void) {
    font_close(font);
    TTF_Quit();
}
