/*
 * SAI project - 3D Laby
 * File : font.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */

#include "font.h"


TTF_Font *load_font(char *name, int size) {

    TTF_Font *font = TTF_OpenFont(name, size);

    if (font == NULL) {
        printf("Failed to open Font %s: %s\n", name, TTF_GetError());
        exit(EXIT_FAILURE);
    }

    return font;
}

void font_new(void) {
	font = load_font("font/zelda.ttf", 32);
}

void close_font(TTF_Font *font)
{
    if (font != NULL) {
        TTF_CloseFont(font);
    }
}

void font_delete(void) {
    close_font(font);
    TTF_Quit();
}
