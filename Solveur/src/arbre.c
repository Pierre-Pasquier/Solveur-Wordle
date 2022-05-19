#include "arbre.h"

element_t* create_element(int nbr_mots){
    element_t* root=malloc(sizeof(element_t));
    for (int i=0;i<26;i++){
        root->fils[i]=NULL;
    }
    root->pere=NULL;
    bool* tab=malloc(nbr_mots*sizeof(bool));
    for (int i=0;i<nbr_mots;i++){
        tab[i]=false;
    }
    root->char_is_in=tab;
    root->terminal=false;
    return root;
}


arbre_t* create_arbre_mots(int nbr_mots){
    arbre_t* res=malloc(sizeof(arbre_t));
    element_t* root=create_element(nbr_mots);
    res->nbr_mots=nbr_mots;
    res->racine=root;
    res->racine->value='\0';
    res->nbr_mots=nbr_mots;

    return res;

}

void destroy_element(element_t* begin){
    if (begin->terminal){
        free(begin->char_is_in);
        free(begin);
    }
    else{
        for (int i=0;i<26;i++){
            if (begin->fils[i]!=NULL){
                destroy_element(begin->fils[i]);
            }

        }
        free(begin->char_is_in);
        free(begin);
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
        if (tmp->fils[mot[i]-65]==NULL){
            
            element_t* content=create_element(abr->nbr_mots);
            tmp->fils[mot[i]-65]=content;
            tmp->fils[mot[i]-65]->value=mot[i];
            tmp->fils[mot[i]-65]->pere=tmp; //Le père du fils
        }

        tmp->fils[mot[i]-65]->char_is_in[num_mot]=true;
        
        tmp=tmp->fils[mot[i]-65];
        
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
        if (elem->fils[i]!=NULL){
            newprefix[len]=65+i;
            print_rec_element(elem->fils[i],newprefix,len+1);
        }
    }
}

void print_arbre(arbre_t* abr){
    assert(abr!=NULL);
    print_rec_element(abr->racine,NULL,0);

    
}

// Fonctions relatives à l'arbre à générer en prétraitement

arbre_pat* cree_arbre_pat(int len){
    arbre_pat* res= malloc(sizeof(arbre_pat));
    res->len_mots=len;
    res->root=NULL;
    return res;

}

void insert_values_rec(node** noeud, char* mot, int nombre_fils, int pattern){
    if ((*noeud)==NULL){
        node* res=malloc(sizeof(node));
        res->mot=mot;
        res->nombre_fils=nombre_fils;
        res->pattern=pattern;
        res->fils=calloc(nombre_fils,sizeof(node*));
        *noeud=res;
        return;

    }
    else{
        // On insère au dernier endroit disponible 
        for (int k=0;k<(*noeud)->nombre_fils;k++){
            insert_values_rec(&((*noeud)->fils[k]),mot,nombre_fils,pattern);
        }
    }
}


void insert_values(arbre_pat* arbre, char* mot, int nombre_fils, int pattern){
    assert(arbre!=NULL);
    if (arbre->root==NULL){
        node* res=malloc(sizeof(node));
        res->mot=mot;
        res->nombre_fils=nombre_fils;
        res->pattern=pattern;
        res->fils=calloc(nombre_fils,sizeof(node*));
        arbre->root=res;
    }
    else {
        insert_values_rec(&(arbre->root),mot,nombre_fils,pattern);
    }

}

void destroy_rec(node* current){
    if (current==NULL){
        return;
    }
    else{
        for (int k=0;k<current->nombre_fils;k++){
            destroy_rec(current->fils[k]);
        }
        free(current->fils);
        free(current);
    }
}

void destroy_arbre_pat(arbre_pat* arbre){

    destroy_rec(arbre->root);

    free(arbre);

}

void write_ligne_rec(FILE* file, int ligne, node* current, int profondeur){
    if (profondeur==ligne){
        fprintf(file,"%d",current->pattern);
        fprintf(file," ");
        fprintf(file,"%s",current->mot);
        fprintf(file," ");
        fprintf(file,"%d",current->nombre_fils);
       

    }
    else {
        fprintf(file,";1");
        for (int k=0;k<current->nombre_fils;k++){
            if (current->fils[k]!=NULL){
                write_ligne_rec(file,ligne,current->fils[k],profondeur+1);
                
            }
            if (k!=current->nombre_fils-1){fprintf(file,",");}
         
        }
        fprintf(file,";");
    }
}

void write_fichier(FILE* file, arbre_pat* arbre){
    assert(arbre!=NULL);
    for (int i=0;i<arbre->len_mots;i++){
        
        write_ligne_rec(file,i,arbre->root,0);
        fprintf(file,"%c",'\n');
    }



}