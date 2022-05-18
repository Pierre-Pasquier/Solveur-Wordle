#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<assert.h>


#ifndef ARBRE_H
#define ARBRE_H

struct _element_t{
    
    char value;
    struct _element_t* pere;
    struct _element_t *fils[26];
    bool* char_is_in;
    bool terminal;
};

typedef struct _element_t element_t;

struct _arbre_t {
    int nbr_mots;
    element_t* racine;
};

typedef struct _arbre_t arbre_t;

element_t* create_element(int nbr_mots);

arbre_t* create_arbre_mots(int nbre_mots);

bool is_in(char a,char *mot);

void destroy_element(element_t* begin);

void destroy_arbre(arbre_t* abr);

bool insert_arbre(arbre_t* abr,char* signed_mot,int num_mot);

void print_rec_element(element_t* elem,unsigned char* prefix,int len);

void print_arbre(arbre_t* abr);

int paterne(char *mot_cherche,char *mot_donne);

void mot_suivant(arbre_t *arbre,element_t * elem, char *prefixe,int num_mot_cherche,int len_mot,int **matrice,int nb_mot);

void parcours(arbre_t *arbre, int len_mot, int num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot);

#endif // ARBRE_H
