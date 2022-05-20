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


struct node{
    int nombre_fils;
    int pattern;
    char* mot;
    struct node** fils;
    
};

typedef struct node node;

struct arbre_pat{
    int len_mots;
    node* root;
};

typedef struct arbre_pat arbre_pat;

element_t* create_element(int nbr_mots);

arbre_t* create_arbre_mots(int nbre_mots);

bool is_in(char a,char *mot,int *tab);

void destroy_element(element_t* begin);

void destroy_arbre(arbre_t* abr);

bool insert_arbre(arbre_t* abr,char* signed_mot,int num_mot);

void print_rec_element(element_t* elem,unsigned char* prefix,int len);

void print_arbre(arbre_t* abr);

int index(char *mot,char x);

int paterne(char *mot_cherche,char *mot_donne);

void mot_suivant(arbre_t *arbre,element_t * elem, char *prefixe,int len_mot,int *num_mot_cherche,int **matrice,int nb_mot);

void parcours(arbre_t *arbre, int len_mot, int* num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot);

void rev(char *mot, size_t len);

char *cat(char * mot1, char *mot2);

void write_ligne_rec(FILE* file, int ligne, node* current, int profondeur,node* pere, int nb_fils_pere,int indice_boucle);

void write_fichier(FILE* file, arbre_pat* arbre);

arbre_pat* cree_arbre_pat(int len);

void insert_values(arbre_pat* arbre, char* mot, int nombre_fils, int pattern);

void insert_values_rec(node** noeud, char* mot, int nombre_fils, int pattern);

void destroy_arbre_pat(arbre_pat* arbre);

void destroy_rec(node* current);

#endif // ARBRE_H
