#include "arbreF.h"

noeud_t* create_noeud(int nbfils, int pattern, char *MotDuNoeud){
    noeud_t* noeud=malloc(sizeof(noeud_t));
    noeud_t** fils=malloc(sizeof(noeud_t*)*nbfils);
    for (int i=0;i<nbfils;i++){
        fils[i]=NULL;
    }
    noeud->nbfils=nbfils;
    noeud->fils=fils;
    noeud->pattern=pattern;
    noeud->MotDuNoeud=MotDuNoeud;
    return noeud;
}

arbre_t* create_arbre(){
    arbre_t* res=malloc(sizeof(arbre_t));
    res->pere=NULL;
    return res;
}


void destroy_noeud(noeud_t* noeud){
    if(noeud!=NULL){
        for(int i=0;i<noeud->nbfils;i++){
            if(noeud->fils[i]!= NULL){
                destroy_noeud(noeud->fils[i]);
                free(noeud->fils[i]);
            }
        }
    free(noeud->fils);
    free(noeud);
    }
}


void destroy_arbre(arbre_t* abr){
    noeud_t* noeud=abr->pere;
    destroy_noeud(noeud);
    free(abr);
}


