#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arbre.h"


int main(){
    printf("Hello World\n");
    
    arbre_t* test=create_arbre_mots(3);
    insert_arbre(test,"ABCD",0);
    insert_arbre(test,"ABFG",1);
    insert_arbre(test,"BEAU",2);
    printf("%d\n",test->racine->fils[0]->fils[1]->char_is_in[1]);
    print_arbre(test);
    destroy_arbre(test);





    
    
    
    }