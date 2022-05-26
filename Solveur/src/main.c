#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    char* tab[5]={"CASIER","RACINEE","REALITES","CERTAINES","CARENTIELS"};
    for (int len_mot=6;len_mot<=10;len_mot++){  
        printf("%d\n",len_mot);  
        arbre_t* test=construct_arbre(len_mot);
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
        printf("%s\n",tab[len_mot-6]);
        node* root=remplissage_arbre_rec(test,matrix,len_mot,tab[len_mot-6],777);
        print_arbre_pat(root);
        char filename[100];
        sprintf(filename,"out%d.txt",len_mot);
        FILE* out=fopen(filename,"w+");
        

        arbre_pat* res=calloc(1,sizeof(arbre_pat));
        res->root=root;
        write_fichier(out,res);
        free(num_mot_cherche);
        for (int i=0;i<nb_mot;i++){
            free(matrix[i]);
        }
        free(matrix);
        destroy_arbre(test);
        destroy_rec(root);
        free(res);
        fclose(out);
    }
}