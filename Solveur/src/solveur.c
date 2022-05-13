#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include "arbre.h"



bool is_in(char a,char *mot){
    for (int i=0 ; i<strlen(mot) ; i++){
        if (mot[i] == a){
            return true;
        }
    }
    return false;
}

int paterne(char *mot_cherche,char *mot_donne){
    int x = 0;
    int tab_c[strlen(mot_cherche)];
    int tab_d[strlen(mot_cherche)];
    for (int i=0 ; i<strlen(mot_cherche) ; i++){
        if (mot_cherche[i] == mot_donne[i]){
            printf("%d\n",i);
            x += 2*pow(3.0,(double) (strlen(mot_cherche)-1-i));
            tab_c[i] = 1;
            tab_d[i] = 1;
        }
    }
    for (int i=0 ; i<strlen(mot_cherche) ; i++){
        if (is_in(mot_donne[i],mot_cherche) && tab_d[i] != 1){
            printf("%d\n",i);
            tab_d[i] = 1;
            x += pow(3.0,(double) (strlen(mot_cherche)-1-i));
        }
    }
    return x;
}



void mot_suivant(arbre_t *arbre,element_t * elem, char *prefixe,int *num_mot_cherche,int *num_mot_donne,int len_mot,int **matrice,int nb_mot){
    printf("Prefixe : %s\n",prefixe);
    if (elem->terminal){    // cas de base, si on est dans une feuille
        printf("fin\n");
        printf("%d\n",*num_mot_cherche);
        *num_mot_cherche = *num_mot_cherche + 1;      //on passe au mot d'après
        *num_mot_donne = *num_mot_cherche + 1;      //on prend le mot suivant
        parcours(arbre,len_mot,num_mot_cherche,matrice,elem,prefixe,nb_mot);    //on parcours tous les autres mots qui sont à droite
    }
    printf("Avant la boucle \n");
    for (int i=0; i<26 ; i++){      //pour tous les fils
        printf("Pas dans if, i = %d \n",i);
        if ((elem->fils)[i] != NULL){   //si le fils est non-vide
            printf("Dans if, i = %d \n",i);
            printf("%s\n",prefixe);
            printf("%c\n",elem->value);
            char new_prefixe[strlen(prefixe)+2];       // on commence à écrire le mot
            printf("Créé \n");
            if (elem->value == '\0'){
                memcpy(new_prefixe,prefixe,strlen(prefixe));
                new_prefixe[0] = elem->fils[i]->value;
                new_prefixe[1] = '\0';
                printf("if \n");
            }
            else{
                memcpy(new_prefixe,prefixe,strlen(prefixe));        //copie pour ajouter la valeur du noeud courant  
                new_prefixe[strlen(prefixe)] = elem->fils[i]->value;      //ajout de la valeur du noeud courant
                new_prefixe[strlen(prefixe)+1] = '\0';
                printf("else \n");
            }
            printf("%s\n",new_prefixe);
            mot_suivant(arbre,elem->fils[i],new_prefixe,num_mot_cherche,num_mot_donne,len_mot,matrice,nb_mot);      //on descend dans l'arbre pour trouver le suite du mot jusqu'à la feuille
        }
    }
}


void parcours(arbre_t *arbre, int len_mot, int *num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot){
    element_t *tmp = position;      //copie de la position
    for (int i=*num_mot_cherche+1 ; i < nb_mot; i++){   //pour tous les mots à droite
        char *suffixe_mot_cherche = "";     //initialisation du suufixe du mot cherche
        char *suffixe_mot_donne = "";     //initialisation du suufixe du mot donne
        int profondeur = len_mot;
        tmp = position;     //on réinitialise la position
        int score = 3^len_mot - 1;      //initialisation du score (= paterne en base 10)
        while (!(tmp->char_is_in)[i]){      //tant que la lettre du noeud courant n'est pas dans le mot que l'on veut (mot_donne)
            score -= 2*3^(len_mot-profondeur);          //vu que la lettre n'est pas dans le mot on enleve la valeur associé du score (repésenté par un 2 dans le paterne en bas 3)
            char merge[2];
            merge[0]=tmp->value;
            merge[1]='\0';
            strcat(suffixe_mot_cherche,merge);     //on ajoute la valeur au suufixe (Attention, reconstitution du suffixe à l'envers)
            profondeur--;       //on met à jour la profondeur
            tmp = tmp->pere;    //on remonte au noeud d'au dessus
        }
        while (!tmp->terminal){     //une fois arrivé à une lettre dans le prochain mot, on commence la descente
            for (int k=0; k<26 ; k++){      //pour tous les fils
                if (((tmp->fils)[k]->char_is_in)[i]){       //si la lettre est dans le prochain mot
                    char merge[2];
                    merge[0]=tmp->value;
                    merge[1]='\0';
                    strcat(suffixe_mot_donne,merge);   //on ajoute cette lettre au suffixe du prochain mot (mot donne)
                    
                    tmp = (tmp->fils)[i];       //on se deplace dans ce fils
                } 
            }
        }
        score += paterne(suffixe_mot_cherche,suffixe_mot_donne);        //  on compare les suffixe des 2 mots pour avoir le score du patterne et on met à jour le score
        matrice[*num_mot_cherche][i] = score;   //on met à jour le score dans la matice
    }
}

































