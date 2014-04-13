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


void create_texture_from_text(char *string, int r, int g, int b) {
	SDL_Color textColor;
	SDL_Surface * temp;	
	
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
	
	temp = TTF_RenderText_Blended(font, string, textColor);

	glGenTextures(1, &(conf->text));
	glBindTexture(GL_TEXTURE_2D,conf->text);

	if(temp != NULL) {
		conf->width_text = temp->w;
		conf->height_text = temp->h;
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);

		SDL_FreeSurface(temp);

	}
}
