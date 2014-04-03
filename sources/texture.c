#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "texture.h"

int load_texture(char *filename, int width, int height) {
	FILE *file = NULL;	
	unsigned int image;
	GLuint texture;

	file = fopen(filename, "r");

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 
			0, /* now */
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_INT,
			&image);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	fclose(file);

	return texture;
}
