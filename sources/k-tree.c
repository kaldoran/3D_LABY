#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "k-tree.h"

/*
 * New Point allocation.
  */
Point *point_new(float x, float y, float z)
{
	Point *p = malloc(sizeof *p);
	p->x = x;
	p->y = y;
	p->z = z;
return p;
}

void point_free(Point *p)
{
	if(p != NULL)
	{
		free(p);
	}
}

/*
 * Element allocation
  */
Element *element_new(Point *s1, Point *s2, char p)
{
	Element *e = malloc(sizeof *e);
	e->s1 = s1;
	e->s2 = s2;
	e->p = p;
return e;
}

void element_free(Element *e)
{
	if (e != NULL)
	{
		point_free(e->s1);
		point_free(e->s2);
		free(e);
	}
}
/*
 * K-Tree allocation.
  */
Ktree *ktree_new()
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
	if (k->e->p == 0)
	{
		fprintf(stderr, "\n");
		for (i = 0; i < K; ++i)
		{
			ktree_print(k->son[i], depth+1);
		}
	} else {
		fprintf(stderr, "%c\n", k->e->p);
	}
}

/*
 * Intersection of two Ktree 
  */
Ktree *intersection(Ktree *k1, Ktree *k2)
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
}