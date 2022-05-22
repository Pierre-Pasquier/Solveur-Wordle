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
    printf("Len mot = %lu\n",strlen(mot));
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    res[0] = car;
    for (int k=0;k<strlen(mot);k++){
        printf("k = %d, mot[k] = %c, =0? : %d\n",k,mot[k],mot[k] == '\0');
        res[k+1] = mot[k];
    }
    res[strlen(mot)+1] = '\0';
    return res;
}

char *cat_f(char *mot, char car){     //concatène les 2 mots dans le 2ème élément
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    for (int k=0;k<strlen(mot);k++){
        printf("k = %d, mot[k] = %c, =0? : %d\n",k,mot[k],mot[k] == '\0');
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
    printf("Longueur : %d\n",n);
    printf("egal ? : %d\n",mot_cherche[4]);
    int tab_c[n];
    int tab_d[n];
    for (int k=0;k<n;k++){
        tab_c[k] = 0;
        tab_d[k] = 0;
    }
    for (int i=0 ; i<n ; i++){
        printf("Mot cherche i : %c, boucle 1 n°%d\n",mot_cherche[i],i);
        printf("Mot donne i : %c, boucle 1 n°%d\n",mot_donne[i],i);
        if (mot_cherche[i] == mot_donne[i]){
            printf("i,2 = %d,x = %d\n",i,x);
            x += (int) 2*pow(3.0,(double) (n-1-i));
            printf("x2 = %d\n",x);
            tab_c[i] = 1;
            tab_d[i] = 1;
        }
    }
    for (int i=0 ; i<n ; i++){
        printf("Mot donne i : %c, boucle 1 n°%d, tab_d[i] = %d\n",mot_donne[i],i,tab_d[i]);
        if (is_in(mot_donne[i],mot_cherche,tab_c) && tab_d[i] != 1){
            tab_c[index(mot_cherche,mot_donne[i])] = 1;
            printf("i = %d,x = %d\n",i,x);
            x += (int) pow(3.0,(double) (n-1-i));
            printf("x = %d\n",x);
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
    printf("Prefixe : %s\n",prefixe);
    for (int k=0;k<nb_mot;k++){
        for (int i=0;i<nb_mot;i++){
            printf("Debut fonction : matrice[%d][%d] = %d\n",k,i,matrice[k][i]);
        }
    }
    if (elem->terminal){    // cas de base, si on est dans une feuille
        printf("fin\n");
        printf("%d\n",*num_mot_cherche);
        printf("--------------------Num mot cherche : %d\n",num_mot_cherche[0]);
        int *l = parcours(arbre,len_mot,num_mot_cherche,matrice,elem,prefixe,nb_mot);    //on parcours tous les autres mots qui sont à droite
        printf("Retour dans mot_suivant, len(l) = %lu\n",sizeof(l));
        int int_c = num_mot_cherche[0];
        for (int k=0;k<sizeof(l)/2;k++){
            matrice[int_c][k] = l[k];
            printf("k = %d\n",k);
        }
        for (int k=0;k<nb_mot;k++){
            for (int i=0;i<nb_mot;i++){
                printf("Dans fonction : matrice[%d][%d] = %d\n",k,i,matrice[k][i]);
            }
        }
        free(l); //On libère l'espace mémoire
        num_mot_cherche[0] = num_mot_cherche[0] + 1;
        printf("Matrice à jour\n");
        printf("Après le parcours\n");
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
            mot_suivant(arbre,elem->fils[i],new_prefixe,len_mot,num_mot_cherche,matrice,nb_mot);      //on descend dans l'arbre pour trouver le suite du mot jusqu'à la feuille
        }
    }
}


int *parcours(arbre_t *arbre, int len_mot, int *num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot){
    printf("Numéro mot cherche %d\n",num_mot_cherche[0]);
    printf("Entrée dans la fonction parcours\n");
    element_t *tmp = position;      //copie de la position
    int *l = calloc(4,sizeof(int));
    printf("Avant la boucle\n");
    for (int i=0 ; i<nb_mot; i++){   //pour tous les mots à droite
        if (i != num_mot_cherche[0]){
            bool flag = true;
            printf("Tour de boucle parcours n°%d---------------------------------------\n",i);
            char suffixe_mot_cherche[strlen(mot_cherche)*2*sizeof(char)];     //initialisation du suffixe du mot cherche
            char suffixe_mot_donne[strlen(mot_cherche)*2*sizeof(char)];     //initialisation du suffixe du mot donne
            //printf("Matrice : %d\n",matrice[0][0]);
            if (i != 0){
                memset(suffixe_mot_cherche,'\0',sizeof(suffixe_mot_cherche));
                printf("aaaaaaaaaaa\n");
                memset(suffixe_mot_donne,'\0',sizeof(suffixe_mot_donne));
                printf("Suffixe mot cherche : %s\n",suffixe_mot_cherche);
            }
            int profondeur = len_mot;
            tmp = position;     //on réinitialise la position
            int score;
            score = (int) pow(3.0,(double) len_mot) - 1;      //initialisation du score (= paterne en base 10)
            printf("Score : %d\n",score);
            printf("Avant le while\n");
            printf("Profondeur : %d, valeur noeud : %c\n",profondeur,tmp->value);
            while (profondeur > 0 && !(tmp->char_is_in)[i]){      //tant que la lettre du noeud courant n'est pas dans le mot que l'on veut (mot_donne)
                printf("Dans le while, profondeur = %d\n",profondeur);
                printf("Valeur du noeud : %c \n",tmp->value);
                printf("Valeur du tableau : %d \n",tmp->char_is_in[i]);
                
                score -= 2*(int)pow(3.0,(double) len_mot-profondeur);          //vu que la lettre n'est pas dans le mot on enleve la valeur associé du score (repésenté par un 2 dans le paterne en bas 3)
                printf("Score : %d\n",score);
                printf("%c\n",tmp->value);
                //printf("Suffixe mot cherche : %s\n",suffixe_mot_cherche);
                if (flag == true){
                    flag = false;
                    suffixe_mot_cherche[0] = tmp->value;
                    suffixe_mot_cherche[1] = '\0';
                }else{
                    char *interm = cat_d(tmp->value,suffixe_mot_cherche);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                    printf("Interm : %s\n",interm);
                    for (int k=0;k<strlen(interm);k++){
                        printf("k = %d\n",k);
                        suffixe_mot_cherche[k] = interm[k];
                        printf("k = %d\n",k);
                    }
                    free(interm);
                }
                profondeur--;       //on met à jour la profondeur
                tmp = tmp->pere;    //on remonte au noeud d'au dessus
                //printf("Suffixe mot cherche : %s\n",suffixe_mot_cherche);
                printf("Fin du while, profondeur = %d\n",profondeur);
            }
            flag = true;
            printf("Après le while : %d\n",tmp->terminal);
            while (tmp != NULL && !(tmp->terminal)){     //une fois arrivé à une lettre dans le prochain mot, on commence la descente
                printf("Dans le while 2\n");
                for (int k=0; k<26 ; k++){      //pour tous les fils
                    printf("Tour de boucle n°%d\n",k);
                    printf("Null ? : %d\n",(tmp->fils)[k] == NULL);
                    if ((tmp->fils)[k] != NULL && ((tmp->fils)[k]->char_is_in)[i]){       //si la lettre est dans le prochain mot
                        tmp = (tmp->fils)[k];       //on se deplace dans ce fils
                        printf("Dans le if \n");
                        printf("Valeur du noeud : %c\n",tmp->value);
                        printf("k = %d, flag = %d\n",k,flag);
                        if (flag == true){
                            flag = false;
                            suffixe_mot_donne[0] = tmp->value;
                            suffixe_mot_donne[1] = '\0';
                        }else{
                            char *interm = cat_f(suffixe_mot_donne,tmp->value);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                            printf("Interm : %s\n",interm);
                            for (int k=0;k<strlen(interm);k++){
                                printf("k = %d\n",k);
                                suffixe_mot_donne[k] = interm[k];
                                printf("k = %d\n",k);
                            }
                            free(interm);
                        }
                        printf("Suffixe mot donne : %s\n",suffixe_mot_donne);
                        break;
                    } 
                }
            }
            printf("Inv = %s\n",suffixe_mot_cherche);
            
            printf("Score : %d\n",score);
            printf("Paterne : %d\n",paterne(suffixe_mot_cherche,suffixe_mot_donne,len_mot));
            
            score += paterne(suffixe_mot_cherche,suffixe_mot_donne,len_mot);        //  on compare les suffixe des 2 mots pour avoir le score du patterne et on met à jour le score
            printf("Score : %d\n",score);
            printf("Num mot cherche : %d, i = %d\n",num_mot_cherche[0],i);
            l[i] = score;
            printf("Liste mise à jour \n");
            //matrice[num_mot_cherche[0]-1][i-1] = score;   //on met à jour le score dans la matrice
            printf("Matrice mise à jour\n");
        }
        else {
            l[i]=(int) pow(3.0,(double) len_mot) - 1;
        }

    }
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




























