#include "arbre.h"




arbre_t* create_arbre_mots(int nbr_mots){
    arbre_t* res=calloc(1,sizeof(arbre_t));
    res->racine=malloc(sizeof(element_t*));
    res->racine->value='\0';
    res->racine->terminal=false;
    res->racine->char_is_in=calloc(nbr_mots,sizeof(bool));
    res->racine->pere=NULL;
    for (int i=0;i<25;i++){
        (res->racine->fils)[i]=NULL;
    }
    return res;

}

void destroy_element(element_t* begin){
    assert(begin!=NULL);
    element_t* top = begin;
    if (top->terminal){
        free(top);
    }
    else{
        for (int i=0;i<25;i++){
            if ((top->fils)[i]!=NULL){
                destroy_element((top->fils)[i]);
                
            }
        }
        free(top);
    }

}

void destroy_arbre(arbre_t* abr){
    element_t* root=abr->racine;
    destroy_element(root);
    free(abr);

}

bool insert_arbre(arbre_t* abr,char* signed_mot,int num_mot){
    assert(abr!=NULL);
    unsigned char* mot=(unsigned char*) signed_mot;
    element_t* tmp=abr->racine;
    int len=strlen(signed_mot);

    for (int i=0;i<len;i++){
        if ((tmp->fils)[mot[i]-65]==NULL){
            (tmp->fils)[mot[i]-65]=calloc(1,sizeof(element_t));

        }
        (tmp->fils)[mot[i]-65]->value=mot[i];
        ((tmp->fils)[mot[i]-65]->char_is_in)[num_mot]=true;
        (tmp->fils)[mot[i]-65]->pere=tmp; //Le père du fils
        tmp=(tmp->fils)[mot[i]-65];
        
    }
    if (tmp->terminal){
        return false;
    }
    else{
        tmp->terminal=true;
        return true;
    }

}

void print_rec_element(element_t* elem,unsigned char* prefix,int len){
    unsigned char newprefix[len+2];
    memcpy(newprefix,prefix,len);
    newprefix[len+1]=0;

    if (elem->terminal){
        printf("MOT : %s\n",prefix);
    }
    for (int i=0;i<25;i++){
        if ((elem->fils)[i]!=NULL){
            newprefix[len]=1;
            print_rec_element((elem->fils)[i],newprefix,len+1);
        }
    }
}

void print_arbre(arbre_t* abr){
    assert(abr!=NULL);
    print_rec_element(abr->racine,NULL,0);

    
}




