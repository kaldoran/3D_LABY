/*
 * SAI project - 3D Laby
 * File : portals.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
 */
#include <stdlib.h>
#include <stdio.h>
#include "portals.h"

Portal *portal_new(void) {
	Portal *portal;
	if ( (portal = malloc(sizeof *portal)) == NULL ) {
		fprintf(stderr,"Probleme de création de portal\n");
		exit(EXIT_FAILURE);
	}
	portal->portail = point_new(0, 0, 10);
	portal->actif = 0;
	return portal;
}

void portal_free(Portal *portal) {
	if ( portal != NULL) {
		point_free(portal->portail);
		free(portal);
	}
}

Portals *portals_new(void) {
	Portals *portals;

	if ( (portals = malloc(sizeof *portals)) == NULL) {
		fprintf(stderr, "Probleme allocation double portal\n");
		exit(EXIT_FAILURE);
	}

	portals->bleu = portal_new();
	portals->orange = portal_new();
	return portals;
}

void portals_free(Portals *portals) {
	if (portals != NULL ) {
		portal_free(portals->bleu);
		portal_free(portals->orange);
		free(portals);
	}
}

