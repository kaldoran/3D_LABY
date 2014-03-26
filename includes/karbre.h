//----------------------------------------------------------
// AUTEUR : REYNAUD Nicolas                                 |
// FICHIER : karbre.c                         		     |
// DATE : 25/03/14					                    |
//                                                          |
// - Prototypes des fonctions de karbre.c                   |
//                                                          |
//----------------------------------------------------------

#ifndef K_ARBRE
#define K_ARBRE

#define K 4

/* structure d'un élement */
typedef struct struct_element {
	int valeur;
}struct_element;

typedef struct_element * element;

/* Structure de l'arbre */
typedef struct struct_karbre {
	struct struct_element *e;    /* Element e */
	struct struct_karbre **fils; /* Tableau de fils */
}struct_karbre;

typedef struct_karbre * karbre;

/* Structure de point */
typedef struct Point {
	int x;
	int y;
	int z;
}Point;

/* Crée et retourne un kArbre vide */
karbre kArbreVide();

/* Libere un karbre */
void freekArbre (karbre A);

/* crée un élement avec comme valeur " valeur " */
element kCreeElement(int valeur);

/* Fonction a nombre de parametre varaible 
 * Crée un arbre ayant de 1 a K élement dedans 
 */
karbre kConsArbre(element e, ... );

/* Retourne le kieme fils de l'arbre A */
karbre kFils(int ieme, karbre A);

/* Retourne la racine de l'arbre A */
element kRacine(karbre A);

/* Retourne 1 si l'arbre est vide
 * 0 Sinon 
 */
int kEstVide(karbre A);

/* Affiche un Karbre */
void kAfficher(karbre A);

/* Crée un point avec comme valeur , x , y , z */
Point kCreePoint(int x, int y, int z);

/* Fonction non utile cependant je les laisse */
int intersetionCubeSphere(Point A, Point B, Point S, int R);

karbre boule2arbre( int x, int y, int z, int r);

karbre boule2arbrebis( Point S, Point A, Point B, int r);

karbre intersetion(karbre A1, karbre A2);

#endif

