/*
 * SAI project - 3D Laby
 * File : music.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
  
#include <SDL/SDL_mixer.h>
  
#ifndef __MUSIC
#define __MUSIC

Mix_Music  *music;

void load_song( char *filename );
void music_new(void);
void music_delete(void);
#endif
