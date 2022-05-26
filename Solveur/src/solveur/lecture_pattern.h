#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include "arbreF.h"
///#include "l'arbre de nathan

noeud_t* lecture_pattern(noeud_t* noeud,char* motif);

int nbr_noeuds(noeud_t* noeud);

bool nbrgood(char* motif);

arbre_t *arbrefinal(int n);

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos);