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
    int** matrix=calloc(3,sizeof(int*));
    for (int i=0;i<3;i++){
        matrix[i]=calloc(4,sizeof(int));


    }
    printf("%s\n",get_mot_num(test,2));
    printf("%d\n",get_num_mot(test,"ACA"));

    // Partie test pour l'écriture dans le fichier texte

    const char* filename= "out.txt";

    FILE* output_file= fopen(filename,"w+");


    arbre_pat* tes= cree_arbre_pat(5);
    
    insert_values(tes,"Jules",5,54);
    insert_values(tes,"Garou",4,41);
    insert_values(tes,"Marie",7,6);
    insert_values(tes,"Punos",4,45);
    insert_values(tes,"Monos",2,5);

    write_fichier(output_file,tes);
    fclose(output_file);

    


    destroy_arbre_pat(tes);











    print_arbre(test);


    for (int i=0;i<3;i++){
        free(matrix[i]);
    }
    free(matrix);
    destroy_arbre(test);





    
    
    
    }