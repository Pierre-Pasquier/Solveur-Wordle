#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct _noeud_t{
    char * MotDuNoeud;
    _noeud_t *fils[];
    int *pattern;
};

typedef strust _noeud_t noeud_t;

struct _arbre_t{
    noeud_t *pere;
};

typedef struct _arbre_t arbre_t;

noeud_t* create_noeud(int nbfils, int pattern, char MotDuNoeud);

arbre_t* create_arbre();

void destroy_noeud(noeud_t* noeud);

void destroy_arbre(arbre_t* abr);