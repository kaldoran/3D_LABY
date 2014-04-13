/*
 * SAI project - 3D Laby
 * File : k-tree.h
 * Authors : Hivert Kevin - Reynaud Nicolas.
  */
#include "object.h"

#ifndef __KTREE
#define __KTREE

#define K         4

#define MAX(X,Y)  (((X) > (Y) ? (X) : (Y)))
#define MIN(X,Y)  (((X) < (Y) ? (X) : (Y)))

typedef struct Element
{
    Point *s1;
    Point *s2;
    Object_list *ol;
} Element;

typedef struct Ktree
{
    Element *e;
    struct Ktree *son[K];
} Ktree;

Ktree * quad_tree;

Element *element_new(Point *s1, Point *s2, Object_list *ol);
void element_free(Element *e);

Ktree *ktree_new(void);
void ktree_free(Ktree *k);

Ktree *ktree_add(Ktree *k, Element *e, ...);
Ktree *ktree_son(int ieme, Ktree *A);
Element *ktree_root(Ktree *A);

void ktree_print(Ktree *A, int depth);

Ktree *object_list_to_ktree(Object_list *ol);
Ktree *object_list_to_ktree_bis(Point *s1, Point *s2, Object_list *ol);

void ktree_display(Ktree *k);
int need_to_cut(float minx, float miny, float maxx, float maxy, Ktree *k);

#endif
