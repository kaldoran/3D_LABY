/*
 * SAI project - 3D Laby
 * File : k-tree.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>


#include "k-tree.h"
#include "config.h"
#include "laby.h"

/*
 * Element allocation
  */
Element *element_new(Point *s1, Point *s2, Object_list *ol)
{
	Element *e;

	if ((e = malloc(sizeof *e)) == NULL)
	{
		return NULL;
	}
	e->s1 = s1;
	e->s2 = s2;
	e->ol = ol;
return e;
}

void element_free(Element *e)
{
	if (e != NULL)
	{
		point_free(e->s1);
		point_free(e->s2);

		object_list_free(e->ol);
		free(e);
	}
}
/*
 * K-Tree allocation.
  */
Ktree *ktree_new(void)
{
	int i;

	Ktree *k = malloc(sizeof *k);
	k->e = NULL;	
	for (i = 0; i < K; ++i)
	{
		k->son[i] = NULL;
	}
return k;
}

void ktree_free(Ktree *k)
{
	int i;
	if (k != NULL)
	{
		element_free(k->e);
		for (i = 0; i < K; ++i)
		{
			ktree_free(k->son[i]);
		}
		free(k);
	}
}

/*
 * K-Tree building function
  */
Ktree *ktree_add(Ktree *k, Element *e, ...)
{
	int i = 0;
	va_list l;
	k->e = e;

	va_start(l,e);
	for (i = 0; i < K; ++i)
	{
		k->son[i] = va_arg(l, Ktree *);
	}
	va_end(l);
return k;
}

/*
 * Catch the nth son.
  */
Ktree *ktree_son(int nth, Ktree *k)
{
	if (k == NULL || nth < 0 || nth >= K)
	{
		return NULL;
	}
return k->son[nth];
}

/*
 * Catch the root.
  */
Element *ktree_root(Ktree *k)
{
	if (k == NULL)
	{
		return NULL;
	}
return k->e;
}

/*
 * Basic print function
  */
void ktree_print(Ktree *k, int depth)
{
	int i = 0;
	for (; i < depth; ++i)
	{
		fprintf(stderr, "_");
	}
	if (k->son[1] != NULL)
	{
		fprintf(stderr, "\n");
		for (i = 0; i < K; ++i)
		{
			ktree_print(k->son[i], depth+1);
		}
	} else {
		fprintf(stderr, "%d\n", k->e->ol->size);
	}
}

Ktree *object_list_to_ktree(Object_list *ol)
{
	int n     = pow2sup((MAX(WIDTH, HEIGHT) + 10) * CELL_SIZE);
	Point *s1 = point_new(-n, -n, 0);
	Point *s2 = point_new(n, n, 0);
	Ktree *k  = object_list_to_ktree_bis(s1, s2, ol);
return k;
}

Ktree *object_list_to_ktree_bis(Point *s1, Point *s2, Object_list *ol)
{
	Doubly_linked_node *iterator;
	int maxx = MAX(s1->x, s2->x);
	int maxy = MAX(s1->y, s2->y);
	int minx = MIN(s1->x, s2->x);
	int miny = MIN(s1->y, s2->y);
	int midx = MIN(s1->x, s2->x) + (abs(maxx - minx) / 2);
	int midy = MIN(s1->y, s2->y) + (abs(maxy - miny) / 2);

	Element *e;

	Object_list *ol1;
	Object_list *ol2;
	Object_list *ol3;
	Object_list *ol4;

	Ktree *ka = ktree_new();
	Point * u1, *t1;
	Ktree *ka1;

	Point * u2, *t2;
	Ktree *ka2;

	Point * u3, *t3;
	Ktree *ka3;

	Point * u4, *t4;
	Ktree *ka4;

	/* If ol.size > 10 then cut. */
	if (maxx - minx <= 2 || ol->size <= 16)
	{
		ka->e = element_new(s1, s2, ol);
		return ka;
	}

	ol1 = object_list_new();
	ol2 = object_list_new();
	ol3 = object_list_new();
	ol4 = object_list_new();
	iterator = ol->last;

	while(1)
	{
		if (((iterator->object)->anchor)->x >= minx && ((iterator->object)->anchor)->x <= midx
			&& ((iterator->object)->anchor)->y >= miny && ((iterator->object)->anchor)->y <= midy)
		{
			ol4 = object_list_push(ol4, object_new(((iterator->object)->anchor)->x, ((iterator->object)->anchor)->y, ((iterator->object)->anchor)->z, (iterator->object)->type));
		}

		if (((iterator->object)->anchor)->x >= midx && ((iterator->object)->anchor)->x <= maxx
			&& ((iterator->object)->anchor)->y >= miny && ((iterator->object)->anchor)->y <= midy)
		{
			ol3 = object_list_push(ol3, object_new(((iterator->object)->anchor)->x, ((iterator->object)->anchor)->y, ((iterator->object)->anchor)->z, (iterator->object)->type));
		}

		if (((iterator->object)->anchor)->x >= midx && ((iterator->object)->anchor)->x <= maxx
			&& ((iterator->object)->anchor)->y >= midy && ((iterator->object)->anchor)->y <= maxy)
		{
			ol2 = object_list_push(ol2, object_new(((iterator->object)->anchor)->x, ((iterator->object)->anchor)->y, ((iterator->object)->anchor)->z, (iterator->object)->type));
		}

		if (((iterator->object)->anchor)->x >= minx && ((iterator->object)->anchor)->x <= midx
			&& ((iterator->object)->anchor)->y >= midy && ((iterator->object)->anchor)->y <= maxy)
		{
			ol1 = object_list_push(ol1, object_new(((iterator->object)->anchor)->x, ((iterator->object)->anchor)->y, ((iterator->object)->anchor)->z, (iterator->object)->type));
		}

		if (iterator->next != NULL)
		{
			iterator = iterator->next;
		} else {
			break;
		}
	}

	/* Recursion */
	u1  = point_new(minx, midy, 0);
	t1  = point_new(midx, maxy, 0);
	ka1 = object_list_to_ktree_bis(u1, t1, ol1);

	u2  = point_new(midx, midy, 0);
	t2  = point_new(maxx, maxy, 0);
	ka2 = object_list_to_ktree_bis(u2, t2, ol2);

	u3  = point_new(midx, miny, 0);
	t3  = point_new(maxx, midy, 0);
	ka3 = object_list_to_ktree_bis(u3, t3, ol3);

	u4  = point_new(minx, miny, 0);
	t4  = point_new(midx, midy, 0);
	ka4 = object_list_to_ktree_bis(u4, t4, ol4);

	e = element_new(s1, s2, ol);
	ka = ktree_add(ka, e, ka1, ka2, ka3, ka4);

return ka;
}



float dist (float x1, float y1, float x2, float y2)
{
	return sqrt(pow(2, x1 - x2) + pow(2, y1 - y2));
}

void ktree_display(Ktree *k)
{
	float view = sqrt(pow(2, CELL_SIZE) + pow(2, 0.5 * CELL_SIZE));

	float x1 = conf->eye->x + view * cos(M_PI * (conf->theta + FOVY / 2) / 180);
	float y1 = conf->eye->y + view * sin(M_PI * (conf->theta + FOVY / 2) / 180);

	float x2 = conf->eye->x + view * cos(M_PI * (conf->theta - FOVY / 2) / 180);
	float y2 = conf->eye->y + view * sin(M_PI * (conf->theta - FOVY / 2) / 180);


	float minx = MIN(conf->eye->x, MIN(x1, x2));
	float miny = MIN(conf->eye->y, MIN(y1, y2));

	float maxx = MAX(conf->eye->x, MAX(x1, x2));
	float maxy = MAX(conf->eye->y, MAX(y1, y2));

	if (conf->quadTreeView)
	{
		glDisable(GL_FOG);
		glBegin(GL_LINE_LOOP);
			glColor3ub(255,255,255);
			glVertex3f(conf->eye->x, conf->eye->y, 32);
			glVertex3f(x1, y1, 32);
			glVertex3f(x2, y2, 32);
		glEnd();
		glEnable(GL_FOG);

		glDisable(GL_FOG);
		glBegin(GL_LINE_LOOP);
			glColor3ub(255,255,0);
			glVertex3f(minx, miny, 32);
			glVertex3f(minx, maxy, 32);
			glVertex3f(maxx, maxy, 32);
			glVertex3f(maxx, miny, 32);
		glEnd();
		glEnable(GL_FOG);
	}

	if (need_to_cut(minx, miny, maxx, maxy, k) && ktree_son(1, k) == NULL)
	{
		if (conf->quadTreeView)
		{
			glDisable(GL_FOG);
			glColor3ub(255,0,0);
			glBegin(GL_LINE_LOOP);
				glVertex3f(k->e->s1->x, k->e->s1->y, 31);
				glVertex3f(k->e->s1->x, k->e->s2->y, 31);
				glVertex3f(k->e->s2->x, k->e->s2->y, 31);
				glVertex3f(k->e->s2->x, k->e->s1->y, 31);
			glEnd();
			glEnable(GL_FOG);
		}

		object_list_display(k->e->ol);
	} else if (!need_to_cut(minx, miny, maxx, maxy, k) && ktree_son(1, k) == NULL) {
		if (conf->quadTreeView)
		{
			glEnable(GL_BLEND);
			glDisable(GL_FOG);
			glColor4ub(255,255,255,100);
			glBegin(GL_LINE_LOOP);
				glVertex3f(k->e->s1->x, k->e->s1->y, 30);
				glVertex3f(k->e->s1->x, k->e->s2->y, 30);
				glVertex3f(k->e->s2->x, k->e->s2->y, 30);
				glVertex3f(k->e->s2->x, k->e->s1->y, 30);
			glEnd();
			glEnable(GL_FOG);
			glDisable(GL_BLEND);
		}
	}

	if (ktree_son(1, k) != NULL)
	{
		ktree_display(k->son[0]);
		ktree_display(k->son[1]);
		ktree_display(k->son[2]);
		ktree_display(k->son[3]);
	}
}

int need_to_cut(float minx, float miny, float maxx, float maxy, Ktree *k)
{
	int k_maxx = MAX(k->e->s1->x, k->e->s2->x);
	int k_maxy = MAX(k->e->s1->y, k->e->s2->y);

	int k_minx = MIN(k->e->s1->x, k->e->s2->x);
	int k_miny = MIN(k->e->s1->y, k->e->s2->y);

	if (minx <= k_minx && k_minx <= maxx
		&& miny <= k_miny && k_miny <= maxy)
	{
		return 1;
	}

	if (minx <= k_minx && k_minx <= maxx
		&& miny <= k_maxy && k_maxy <= maxy)
	{
		return 1;
	}

	if (minx <= k_maxx && k_maxx <= maxx
		&& miny <= k_maxy && k_maxy <= maxy)
	{
		return 1;
	}

	if (minx <= k_maxx && k_maxx <= maxx
		&& miny <= k_miny && k_miny <= maxy)
	{
		return 1;
	}

	if (k_minx <= minx && minx <= k_maxx
		&& k_miny <= miny && miny <= k_maxy)
	{
		return 1;
	}

	if (k_minx <= minx && minx <= k_maxx
		&& k_miny <= maxy && maxy <= k_maxy)
	{
		return 1;
	}

	if (k_minx <= maxx && maxx <= k_maxx
		&& k_miny <= maxy && maxy <= k_maxy)
	{
		return 1;
	}

	if (k_minx <= maxx && maxx <= k_maxx
		&& k_miny <= miny && miny <= k_maxy)
	{
		return 1;
	}
return 0;
}