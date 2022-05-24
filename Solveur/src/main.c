#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    arbre_t* test=create_arbre_mots(12);
    insert_arbre(test,"AAAA",0);
    insert_arbre(test,"AAAB",1);
    insert_arbre(test,"AABA",2);
    insert_arbre(test,"AABB",3);
    insert_arbre(test,"ABAA",4);
    insert_arbre(test,"ABAB",5);
    insert_arbre(test,"ABBA",6);
    insert_arbre(test,"ABBB",7);
    insert_arbre(test,"BAAA",8);
    insert_arbre(test,"BAAB",9);
    insert_arbre(test,"BBBB",10);
    insert_arbre(test,"YYYY",11);
    
    int** matrix=calloc(12,sizeof(int*));
    for (int i=0;i<12;i++){
        matrix[i]=calloc(12,sizeof(int));
    }
    for (int k=0;k<12;k++){
        for (int i=0;i<12;i++){
            printf("Main : matrice[%d][%d] = %d\n",k,i,matrix[k][i]);
        }
    }
    int *num_mot_cherche = malloc(sizeof(int));
    num_mot_cherche[0] = 0;
    mot_suivant(test,test->racine,"",4,num_mot_cherche,matrix,12);

    //on doit obtenir
    // 00 21 12 13
    // 19 00 05 08
    // 12 11 00 20
    // 13 08 20 00

    printf("%lu\n",sizeof("B"));
    getchar();
    arbre_pat* tes=cree_arbre_pat(4);
    node* res=remplissage_arbre_rec(test,matrix,4,"AAAA",777);
    tes->root=res;
    const char* filename = "out.txt";
    FILE* out=fopen(filename,"w+");
    write_fichier(out,tes);
    print_arbre_pat(tes->root);
    destroy_arbre_pat(tes);






    printf("\n");
    print_arbre(test);


    free(num_mot_cherche);
    for (int i=0;i<12;i++){
        free(matrix[i]);
    }
    free(matrix);
    destroy_arbre(test);





    
    
    
    }