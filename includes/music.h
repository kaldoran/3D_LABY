/*
 * SAI project - 3D Laby
 * File : music.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
  
#include "config.h"
#include <SDL/SDL_mixer.h>
  
#ifndef __MUSIC
#define __MUSIC

#define CHANG_VOL        3

#define HURT_1           0
#define HURT_2           1
#define HURT_3           2
#define SOUND_PORTAL	3

#define NUMBER_OF_CHUNCK 4

Mix_Chunk  *sound[NUMBER_OF_CHUNCK];
Mix_Music  *music;

void load_song( char *filename );
void change_volume(int value);
void music_new(void);
void music_delete(void);

void load_chunk(char *filename, int id_chunck);
void chunk_new(void);
void chunk_delete(void);

#endif
