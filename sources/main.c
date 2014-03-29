/*
 * SAI project - 3D Laby
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "laby.h"
#include "stack.h"
#include "k-tree.h"
#include "config.h"

Config *conf;

void keyboard(unsigned char touche, int x , int y);
void mouse_motion(int x, int z);
void display();
void change_center();

int main(int argc, char *argv[])
{
	Stack *stack = stack_new();
	Laby *laby = laby_new();
	conf = config_new();

	laby = maze_generation(laby);

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); 
	/*glutFullScreen();*/
	glutInitWindowPosition(SCREEN_POSITION_X, SCREEN_POSITION_Y);

	glutCreateWindow("Laby");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(SCREEN_MID_HEIGHT, SCREEN_MID_HEIGHT);

	glutDisplayFunc(display);
	glutIdleFunc(display);
		
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	stack_free(stack);
	laby_free(laby);
return 0;
}

void change_center()
{
	conf->center->x = conf->eye->x + conf->eye_direction->x;
	conf->center->y = conf->eye->y + conf->eye_direction->y;
	conf->center->z = conf->eye->z + conf->eye_direction->z;
}

void keyboard(unsigned char key, int x , int y) {
	int modifiers = glutGetModifiers();
	Point *save_eye = point_new(conf->eye->x, conf->eye->y, conf->eye->z);
	float speed = 1.5;

	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
	{
		speed = 3.1337;	
	}

	if ( key == 's' || key == 'S')
	{
		save_eye->x -= speed * conf->body_direction->x;
		save_eye->y -= speed * conf->body_direction->y;
	}
	else if ( key == 'z' || key == 'Z') {
		save_eye->x += speed * conf->body_direction->x;
		save_eye->y += speed * conf->body_direction->y;
	}
	else if ( key == 'q' || key == 'Q') { 
		save_eye->x += -(speed * conf->body_direction->y);
		save_eye->y += speed * conf->body_direction->x;
	}	
	else if ( key == 'd' || key == 'D') {
		save_eye->x += speed * conf->body_direction->y;
		save_eye->y += -(speed * conf->body_direction->x);
	}
	else if ( key == '2') {
		save_eye->z--;
	}
	else if ( key == '8' ) {
		save_eye->z++;
	}
	else if ( (int)key == 27) {
		/*glutLeaveMainLoop();*/
		exit(EXIT_SUCCESS);
	}
	
	if ( save_eye->x > 2 && save_eye->y > 2
		&& save_eye->x < (CELL_SIZE * WIDTH) - 2
		&& save_eye->y < (CELL_SIZE * HEIGHT) - 2
		&& save_eye->z > 5
	) {
		conf->eye->x = save_eye->x;
		conf->eye->y = save_eye->y;
		conf->eye->z = save_eye->z;
	} 
change_center();
free(save_eye);
}

void mouse_motion(int x, int z) {
	float tmp;

	conf->theta -= (x - SCREEN_MID_WIDTH) * SENSITIVITY;
	conf->phi -= (z - SCREEN_MID_HEIGHT) * SENSITIVITY;

	if (conf->theta > 360 )
	{
		conf->theta = 0;
	} else if (conf->theta < -360 ) {
		conf->theta = 0;
	}

	if (conf->phi > 89 )
	{
		conf->phi = 89;
	} else if (conf->phi < -89) {
		conf->phi = -89;
	}

	tmp = cos(conf->phi * M_PI / 180);

	conf->body_direction->x = cos(conf->theta * M_PI / 180);
	conf->body_direction->y = sin(conf->theta * M_PI / 180);

	conf->eye_direction->x = tmp * conf->body_direction->x;
	conf->eye_direction->y = tmp * conf->body_direction->y;
	conf->eye_direction->z = sin(conf->phi * M_PI / 180);
	
	change_center();
	return;
}

void display() {
	int i, j;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR, FAR);
	/*glFrustum(-5, 5, -5, 5, 5, 500);*/
	gluLookAt(conf->eye->x, conf->eye->y, conf->eye->z, conf->center->x, conf->center->y, conf->center->z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*tree(-30, 10, 10);
	tree(-50, 40, 10);
	tree(-50, 20, 10);
	tree(15, -100, 20);
	tree(-60, 40, 20);*/

	/* LE SOLEILLLLLLL */
	glPushMatrix();
		glTranslatef(CELL_SIZE * WIDTH / 2, CELL_SIZE * HEIGHT / 2, 300); 
		glColor3f(1, 1, 0);
		glutWireSphere(30., 15, 15);
	glPopMatrix();

	/* 1250 : LARGEUR laby = HAUTEUR laby ici*/ 
	/* Génération du sol */
	glColor3f(0, 1, 1);
	for ( i = 0; i < WIDTH * CELL_SIZE; i += CELL_SIZE ) {
		for ( j = 0; j < HEIGHT * CELL_SIZE; j += CELL_SIZE) { 
			glBegin(GL_LINE_LOOP);
				glVertex3f(i, j, 0);
				glVertex3f(i + CELL_SIZE, j, 0);
				glVertex3f(i + CELL_SIZE, j + CELL_SIZE, 0); 
				glVertex3f(i, j + CELL_SIZE, 0);
			glEnd();
		}
	}
	
	/* Mur confinement */
	glColor4f(1, 0, 0,0.1);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, 0, 10);

		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);

		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);

		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(0, 0, 5);
		glVertex3f(0, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
	glEnd();

	glColor3f(1, 0, 0);
	/* boiboite */
	glBegin(GL_QUADS);
		/* Boiboite de la face gauche au spawn */
		glVertex3f(0, 0, 4.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, 0, 5);

		glVertex3f(0, 0, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);
		glVertex3f(0, 0, 10.5);

		/* Autre boibote */
		glVertex3f(0, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 5);

		glVertex3f(0, HEIGHT * CELL_SIZE, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);
		glVertex3f(0, HEIGHT * CELL_SIZE, 10.5);

		/* autre boiboite */
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 5);

		glVertex3f(WIDTH * CELL_SIZE, HEIGHT, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
		glVertex3f(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 10.5);

		/* autre boiboite */
		glVertex3f(WIDTH * CELL_SIZE, 0, 4.5);
		glVertex3f(0, 0, 4.5);
		glVertex3f(0, 0, 5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 5);

		glVertex3f(WIDTH * CELL_SIZE, 0, 10);
		glVertex3f(0, 0, 10);
		glVertex3f(0, 0, 10.5);
		glVertex3f(WIDTH * CELL_SIZE, 0, 10.5);
	glEnd();


	glutWarpPointer(SCREEN_MID_WIDTH, SCREEN_MID_HEIGHT);
	
	glutPostRedisplay();
	glutSwapBuffers();
}