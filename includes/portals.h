/*
 * SAI project - 3D Laby
 * File : portals.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */
#include "object.h"

#ifndef __PORTALS
#define __PORTALS

#define TRIGGER_DISTANCE 5
#define PUSH_DISTANCE    7
#define WIDTH_PORTAL     CELL_SIZE / 2.5

typedef struct Portal {
	char actif;
	int rotation;
	Point *portail;
}Portal;

typedef struct Portals {
	Portal *bleu;
	Portal *orange;
}Portals;

Portals *portals;

Portal *portal_new(void);
void portal_free(Portal *portal);


Portals *portals_new(void);
void portals_free(Portals *portals);

#endif
