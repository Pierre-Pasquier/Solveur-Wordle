#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    arbre_t* test=create_arbre_mots(4);
    insert_arbre(test,"DBAA",0);
    insert_arbre(test,"FBCA",1);
    insert_arbre(test,"PABC",2);
    insert_arbre(test,"SACA",3);
    int** matrix=calloc(4,sizeof(int*));
    for (int i=0;i<4;i++){
        matrix[i]=calloc(4,sizeof(int));
    }
    for (int k=0;k<4;k++){
        for (int i=0;i<4;i++){
            printf("Main : matrice[%d][%d] = %d\n",k,i,matrix[k][i]);
        }
    }
    int *num_mot_cherche = malloc(sizeof(int));
    num_mot_cherche[0] = 0;
    mot_suivant(test,test->racine,"",4,num_mot_cherche,matrix,4);


    //on doit obtenir
    // 00 21 12 13
    // 19 00 05 08
    // 12 11 00 20
    // 13 08 20 00






    print_arbre(test);


    free(num_mot_cherche);
    for (int i=0;i<3;i++){
        free(matrix[i]);
    }
    free(matrix);
    destroy_arbre(test);





    
    
    
    }