#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    char* tab[3]={"TESTS","TROUS","PLATE"};
    arbre_t* test=create_arbre_mots(3);
    for (int k=0;k<3;k++){
        insert_arbre(test,tab[k],k);
    }
    print_arbre(test);
    
    
    
    }