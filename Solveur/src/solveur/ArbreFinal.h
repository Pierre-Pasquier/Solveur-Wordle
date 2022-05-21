
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//ici on part d'un fichier texte et on le lis dans le but de créer un arbre.

// faire une structure pour la liste des fils car on sait pas combien y'en a 

struct _noeud_t{
    char* MotDuNoeud;
    struct _noeud_t *fils;
    int pattern;
};

typedef struct _noeud_t noeud_t;

struct _arbre_t{
    noeud_t *père;
};

typedef struct _arbre_t arbre_t;

noeud_t* creation_noeud_t(char* mot,int pattern);

bool arbre_is_empty(arbre_t *one_tree);

void arbre_destroy(arbre_t *one_tree);

void tree_append(arbre_t *one_tree, char *MotDuNoeud);



arbre_t *arbrefinal(int n);

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos);