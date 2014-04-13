/*
 * SAI project - 3D Laby
 * File : music.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "music.h"
#include "config.h"

void load_song( char *filename ) {

	/* On free la musique précedent ( on sait jamais ) */
	if ( music != NULL ) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}

	/* On charge la nouvelle chanson */
	music = Mix_LoadMUS( my_strcat(conf->path,filename) );
	if (music == NULL) {
		fprintf(stderr, "Can't read the music \n");
		return;
	}

	/* On active la répétition de la musique à l'infini */
	if(Mix_PlayMusic(music, -1) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	}
}
         
void load_chunk(char *filename, int id_chunck) {
	sound[id_chunck] = Mix_LoadWAV(my_strcat(conf->path,filename));
	if(sound[id_chunck] == NULL) {
		printf("Unable to load Ogg file: %s\n", Mix_GetError());
		return;
	}
}

void chunk_new(void) {
	load_chunk("music/hurtflesh1.wav", HURT_1);
	load_chunk("music/hurtflesh2.wav", HURT_2);
	load_chunk("music/hurtflesh3.wav", HURT_3);
}

void chunk_delete(void) {
	int i;
	for(i = 0; i < NUMBER_OF_CHUNCK; i++)
		Mix_FreeChunk(sound[i]);
}

void music_new(void) {
	load_song(conf->music);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
}

void music_delete(void) {
	if ( music != NULL ) {
		Mix_FreeMusic(music);
     }
     
	Mix_CloseAudio();
	Mix_Quit();
}

void change_volume(int value) {
	int tmp_volume = conf->volume;
	tmp_volume += value;

	if ( tmp_volume <= MIX_MAX_VOLUME && tmp_volume >= 0 ) {
		conf->volume = tmp_volume;
		Mix_VolumeMusic(conf->volume);
	}
}
