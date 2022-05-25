#include "arbreF.h"

noeud_t* create_noeud(int nbfils, int pattern, char MotDuNoeud){
    noeud_t* noeud=malloc(sizeof(noeud_t));
    for (int i=0;i<nbfils;i++){
        noeud->fils[i]=NULL;
    }
    noeud->pattern=pattern;
    noeud->MotDuNoeud=MotDuNoeud;
    printf(noeud->fils,noeud->pattern,noeud->MotDuNoeud);
    return noeud;
}

arbre_t* create_arbre(){
    arbre_t* res=malloc(sizeof(arbre_t));
    res->pere=NULL;
    return res;
}


void destroy_noeud(noeud_t* noeud){
    int a = 0;
    for(int i=0;i<sizeof(noeud->fils);i++){
        if(noeud->fils[i]!= NULL){
            destroy_noeud(noeud->fils[i]);
        }else{
            free(noeud->fils[i]);
        }
    };
    free(noeud->pattern);
    free(noeud->MotDuNoeud);
    free(noeud);
}



void destroy_arbre(arbre_t* abr){
    noeud_t* noeud=abr->pere;
    destroy_noeud(noeud);
    free(abr);
}


