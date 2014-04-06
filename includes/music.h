/*
 * SAI project - 3D Laby
 * File : music.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
  
#include "config.h"
#include <SDL/SDL_mixer.h>
  
#ifndef __MUSIC
#define __MUSIC

#define CHANG_VOL 5

Mix_Music  *music;

void load_song( char *filename );
void change_volume(int value);
void music_new(void);
void music_delete(void);
#endif
