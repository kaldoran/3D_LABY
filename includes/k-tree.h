#ifndef __KTREE
#define __KTREE

#define K 8

typedef struct Point
{
	float x;
	float y;
	float z;	
} Point;

typedef struct Element
{
	Point *s1;
	Point *s2;
	char p;
} Element;

typedef struct Ktree {
	Element *e;
	struct Ktree *son[K];
} Ktree;

Point *point_new(float x, float y, float z);
void point_free(Point *point);

Element *element_new(Point *s1, Point *s2, char p);
void element_free(Element *e);

Ktree *ktree_new();
void ktree_free(Ktree *k);

Ktree *ktree_add(Ktree *k, Element *e, ...);
Ktree *ktree_son(int ieme, Ktree *A);
Element *ktree_root(Ktree *A);
Ktree *intersection(Ktree *k1, Ktree *k2);
void ktree_print(Ktree *A, int depth);

#endif
