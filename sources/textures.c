/*
 * SAI project - 3D Laby
 * File : texture.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */
#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <math.h>

#include "texture.h"
#include "config.h"
#include "display.h"
#include "font.h"

GLuint load_texture(const char* file)
{
	SDL_PixelFormat *format;
	SDL_Surface* surface = IMG_Load(file);
	GLuint texture;

	if(!surface)
	{
		fprintf(stderr, "Error during the image loading.\n");
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	format = surface->format;
	if (format->Amask)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
			surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
	} else {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
		
	SDL_FreeSurface(surface);
	return texture;
}


void create_texture_from_text(char *string, char *font_name) {
	int w, h;
	TTF_Font *font = font_load(font_name, 32);
	SDL_Color textColor;
	SDL_Surface * temp, *temp2;	
	
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	
	temp = TTF_RenderText_Blended(font, string, textColor);

	glGenTextures(1, &(conf->text));
	glBindTexture(GL_TEXTURE_2D,conf->text);

	if(temp != NULL) {
		w = pow2sup(temp->w);
		h = pow2sup(temp->h);

		temp2 = SDL_CreateRGBSurface(SDL_HWSURFACE,
									w,
									h,
									32,
									temp->format->Rmask,
									temp->format->Gmask,
									temp->format->Bmask,
									temp->format->Amask);

		if(temp2 == NULL) {
			SDL_FreeSurface(temp);
			return;
		}

		conf->width_text = temp->w;
		conf->height_text = temp->h;
		
		SDL_BlitSurface(temp, NULL, temp2, NULL);

		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, temp2->w, temp2->h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, temp2->pixels);

		SDL_FreeSurface(temp);
		SDL_FreeSurface(temp2);
	}
}
