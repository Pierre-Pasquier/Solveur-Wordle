#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    int len_mot = 10;
    arbre_t* test=construct_arbre(len_mot);
    print_arbre(test);
    printf("aaaaaaa\n");
    FILE* fptr;
    char link[100];
    sprintf(link,"./Mots/mot%d.txt",len_mot);
    fptr = fopen(link,"r");
    if (fptr==NULL){
        perror ("Error reading file");
    }
    char line[256];
    fgets(line, sizeof(line), fptr);
    int nb_mot;
    sscanf(line,"%d",&nb_mot);
    int** matrix=calloc(nb_mot,sizeof(int*));
    for (int i=0;i<nb_mot;i++){
        matrix[i]=calloc(nb_mot,sizeof(int));
    }
    int *num_mot_cherche = malloc(sizeof(int));
    num_mot_cherche[0] = 0;
    mot_suivant(test,test->racine,"",len_mot,num_mot_cherche,matrix,nb_mot);
    for (int k=0;k<nb_mot;k++){
        for (int j=0;j<nb_mot;j++){
          printf("%d, ",matrix[k][j]);
      }
      printf("\n");
    }

    free(num_mot_cherche);
    for (int i=0;i<nb_mot;i++){
        free(matrix[i]);
    }
    free(matrix);
    destroy_arbre(test);





    
    
    
    }