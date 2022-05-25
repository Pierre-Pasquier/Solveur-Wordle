#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include "arbreF.h"
///#include "l'arbre de nathan

arbre_t *lecture_pattern(char* pattern);

bool nbrgood(char* motif);

arbre_t *arbrefinal(int n);

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos);