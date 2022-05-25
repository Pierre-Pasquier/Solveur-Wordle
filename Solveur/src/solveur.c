#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include "arbre.h"



bool is_in(char a,char *mot,int *tab){
    for (int i=0 ; i<strlen(mot) ; i++){
        if (mot[i] == a && tab[i] != 1){
            return true;
        }
    }
    return false;
}

char *cat_d(char car, char *mot){     //concatène les 2 mots dans le 2ème élément
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    res[0] = car;
    for (int k=0;k<strlen(mot);k++){
        res[k+1] = mot[k];
    }
    res[strlen(mot)+1] = '\0';
    return res;
}

char *cat_f(char *mot, char car){     //concatène les 2 mots dans le 2ème élément
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    for (int k=0;k<strlen(mot);k++){
        res[k] = mot[k];
    }
    res[strlen(mot)] = car;
    res[strlen(mot)+1] = '\0';
    return res;
}

int paterne(char *mot_cherche,char *mot_donne,int len_mot){   
    int x = 0;
    int n;
    if (strlen(mot_cherche) > len_mot){
        n = len_mot;
    }
    else{
        n = strlen(mot_cherche);
    }
    int tab_c[n];
    int tab_d[n];
    for (int k=0;k<n;k++){
        tab_c[k] = 0;
        tab_d[k] = 0;
    }
    for (int i=0 ; i<n ; i++){
        if (mot_cherche[i] == mot_donne[i]){
            x += (int) 2*pow(3.0,(double) (n-1-i));
            tab_c[i] = 1;
            tab_d[i] = 1;
        }
    }
    for (int i=0 ; i<n ; i++){
        if (is_in(mot_donne[i],mot_cherche,tab_c) && tab_d[i] != 1){
            tab_c[index(mot_cherche,mot_donne[i])] = 1;
            x += (int) pow(3.0,(double) (n-1-i));
        }
        
    }
    return x;
}

int index(char *mot,char x){
    for (int k=0;k<strlen(mot);k++){
        if (mot[k] == x){
            return k;
        }
    }
    return -1;
}


void mot_suivant(arbre_t *arbre,element_t * elem, char *prefixe,int len_mot,int *num_mot_cherche,int **matrice,int nb_mot){
    if (elem->terminal){    // cas de base, si on est dans une feuille
        printf("Mot : %s\n",prefixe);
        int *l = parcours(arbre,len_mot,num_mot_cherche,matrice,elem,prefixe,nb_mot);    //on parcours tous les autres mots qui sont à droite
        int int_c = num_mot_cherche[0];
        for (int k=0;k<nb_mot;k++){
            matrice[int_c][k] = l[k];
        }
        free(l); //On libère l'espace mémoire
        num_mot_cherche[0] = num_mot_cherche[0] + 1;
    }
    for (int i=0; i<26 ; i++){      //pour tous les fils
        if ((elem->fils)[i] != NULL){   //si le fils est non-vide
            char new_prefixe[strlen(prefixe)+2];       // on commence à écrire le mot
            if (elem->value == '\0'){
                memcpy(new_prefixe,prefixe,strlen(prefixe));
                new_prefixe[0] = elem->fils[i]->value;
                new_prefixe[1] = '\0';
            }
            else{
                memcpy(new_prefixe,prefixe,strlen(prefixe));        //copie pour ajouter la valeur du noeud courant  
                new_prefixe[strlen(prefixe)] = elem->fils[i]->value;      //ajout de la valeur du noeud courant
                new_prefixe[strlen(prefixe)+1] = '\0';
            }
            mot_suivant(arbre,elem->fils[i],new_prefixe,len_mot,num_mot_cherche,matrice,nb_mot);      //on descend dans l'arbre pour trouver le suite du mot jusqu'à la feuille
        }
    }
}


int *parcours(arbre_t *arbre, int len_mot, int *num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot){
    
    element_t *tmp = position;      //copie de la position
    int *l = calloc(nb_mot,sizeof(int));
    for (int i=0 ; i<nb_mot; i++){   //pour tous les mots à droite
    
        if (i != num_mot_cherche[0]){
            bool flag = true;
            char *suffixe_mot_cherche = calloc(strlen(mot_cherche)+1,sizeof(char));     //initialisation du suffixe du mot cherche
            char *suffixe_mot_donne = calloc(strlen(mot_cherche)+1,sizeof(char));     //initialisation du suffixe du mot donne
            int profondeur = len_mot;
            tmp = position;     //on réinitialise la position
            int score;
            score = (int) pow(3.0,(double) len_mot) - 1;      //initialisation du score (= paterne en base 10)
            while (profondeur > 0 && !(tmp->char_is_in)[i]){      //tant que la lettre du noeud courant n'est pas dans le mot que l'on veut (mot_donne)
                score -= 2*(int)pow(3.0,(double) len_mot-profondeur);          //vu que la lettre n'est pas dans le mot on enleve la valeur associé du score (repésenté par un 2 dans le paterne en bas 3)
                if (flag == true){
                    flag = false;
                    suffixe_mot_cherche[0] = tmp->value;
                    suffixe_mot_cherche[1] = '\0';
                }else{
                    char *interm = cat_d(tmp->value,suffixe_mot_cherche);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                    for (int k=0;k<strlen(interm);k++){
                        suffixe_mot_cherche[k] = interm[k];
                    }
                    free(interm);
                }
                profondeur--;       //on met à jour la profondeur
                tmp = tmp->pere;    //on remonte au noeud d'au dessus
            }
            flag = true;
            while (tmp != NULL && !(tmp->terminal)){     //une fois arrivé à une lettre dans le prochain mot, on commence la descente
                for (int k=0; k<26 ; k++){      //pour tous les fils
                    if ((tmp->fils)[k] != NULL && ((tmp->fils)[k]->char_is_in)[i]){       //si la lettre est dans le prochain mot
                        tmp = (tmp->fils)[k];       //on se deplace dans ce fils
                        if (flag == true){
                            flag = false;
                            suffixe_mot_donne[0] = tmp->value;
                            suffixe_mot_donne[1] = '\0';
                        }else{
                            char *interm = cat_f(suffixe_mot_donne,tmp->value);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                            for (int k=0;k<strlen(interm);k++){
                                suffixe_mot_donne[k] = interm[k];
                            }
                            free(interm);
                        }
                        break;
                    } 
                }
            }
            score += paterne(suffixe_mot_cherche,suffixe_mot_donne,len_mot);        //  on compare les suffixe des 2 mots pour avoir le score du patterne et on met à jour le score
            l[i] = score;
            free(suffixe_mot_cherche);
            free(suffixe_mot_donne);
        }
        else {
            l[i]=(int) pow(3.0,(double) len_mot) - 1;
        }

    }
    printf("l[0] : %d\n",l[0]);
    return l;
}




void rev(char *mot, size_t len)
{
  char tmp;
  len--;
  if (len < 2){
      return;
  } 

  tmp = *mot;
  *mot = mot[len];
  mot[len] = tmp;

  rev(mot+1, len -1);
}




























