#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    
    arbre_t* test=create_arbre_mots(4);
    insert_arbre(test,"ABC",0);
    insert_arbre(test,"ACA",1);
    insert_arbre(test,"BAA",2);
    insert_arbre(test,"BCA",3);
    int** matrix=malloc(3*sizeof(int*));
    for (int i=0;i<3;i++){
        matrix[i]=malloc(3*sizeof(int));
    }
    int num_mot_cherche=0;
    int num_mot_donne=1;


    mot_suivant(test,test->racine,"",&num_mot_cherche,&num_mot_donne,3,matrix,4);











    print_arbre(test);



    for (int i=0;i<3;i++){
        free(matrix[i]);
    }
    free(matrix);
    destroy_arbre(test);





    
    
    
    }