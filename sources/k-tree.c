/*
 * SAI project - 3D Laby
 * File : k-tree.c
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

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

/*
 * Intersection of two Ktree 
  */
/*Ktree *intersection(Ktree *k1, Ktree *k2)
{
	Ktree *k = ktree_new();
	Element *e;
	int i;

	if (k1 == NULL || k2 == NULL)
	{
		return NULL;
	}

	if (k1->e->p == '1')
	{
		return k2;
	} else if (k1->e->p == '0') {
		return k1;
	}

	if (k2->e->p == '1')
	{
		return k1;
	} else if (k2->e->p == '0') {
		return k2;
	}

	e = element_new(k1->e->s1, k1->e->s2, k1->e->p);
	k->e = e;
	for (i = 0; i < K; ++i)
	{
		k->son[i] = intersection(k1->son[i], k2->son[i]);
	}
return k;
}*/

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
	int midx = MIN(s1->x, s2->x) + abs(s2->x - s1->x) / 2;
	int midy = MIN(s1->x, s2->x) + abs(s2->x - s1->x) / 2;
	int minx = MIN(s1->x, s2->x);
	int miny = MIN(s1->y, s2->y);

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
	if (maxx - minx <= CELL_SIZE || ol->size < 10)
	{
		fprintf(stderr, "%d\n", ol->size);
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
	float view = 20 * CELL_SIZE;

	float minx = conf->eye->x - view;
	float miny = conf->eye->y - view;

	float maxx = conf->eye->x + view;
	float maxy = conf->eye->y + view;


	if (k == NULL)
	{
		return;
	}

	if (/*need_to_cut(minx, miny, maxx, maxy, k) && */ktree_son(1, k) == NULL)
	{
		object_list_display(k->e->ol);
	}

	if (/*need_to_cut(minx, miny, maxx, maxy, k) && */ktree_son(1, k) != NULL)
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

	if (dist(conf->eye->x, conf->eye->y, k_minx + ((k_maxx - k_minx) / 2),  k_miny + ((k_maxy - k_miny) / 2)) <= 20 * CELL_SIZE)
		return 1;
return 0;
}