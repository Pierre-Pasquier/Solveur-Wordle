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
    for (int i=0;i<26;i++){
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
char* get_mot_num_rec(char* prefix,element_t* elem,int num_mot,int len){
    if (elem->terminal){
        return prefix;
    }
    else{
        char newprefix[len+2];
        memcpy(newprefix,prefix,len);
        newprefix[len+1]=0;
        for (int i=0;i<26;i++){
            if (elem->fils[i]!=NULL && elem->fils[i]->char_is_in[num_mot]){
                newprefix[len]=65+i;
                 return get_mot_num_rec(newprefix,elem->fils[i],num_mot,len+1);
            }

        }

    }

}

char* get_mot_num(arbre_t* arbre,int num_mot){
    assert(arbre!=NULL);
    return get_mot_num_rec(NULL,arbre->racine,num_mot,0);

}

int get_num_mot(arbre_t* arbre, char* mot){
    assert(arbre!=NULL);
    return get_num_mot_rec(arbre->racine,mot,1,arbre->nbr_mots);

}

int get_num_mot_rec(element_t* elem, char* mot,int profondeur, int nombre_mots){
    if (profondeur==strlen(mot)){
        if (elem!=NULL){
            for (int k=0;k<nombre_mots;k++){
                if (elem->char_is_in[k]){
                    return k;
                }
            }

        }
        else {return -1;}


    }
    else{
        return get_num_mot_rec(elem->fils[mot[profondeur-1]-65],mot,profondeur+1,nombre_mots);
    }

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

void write_ligne_rec(FILE* file, int ligne, node* current, int profondeur,node* pere, int nb_fils_pere,int indice_boucle){
    if (profondeur==ligne){
        fprintf(file,"%d",current->pattern);
        fprintf(file," ");
        fprintf(file,"%s",current->mot);
        fprintf(file," ");
        fprintf(file,"%d",current->nombre_fils);
        if (indice_boucle==nb_fils_pere-1){
            fprintf(file,";");
        }
        else {
            fprintf(file,",");
        }
       

    }
    else {
        
        for (int k=0;k<current->nombre_fils;k++){
            if (current->fils[k]!=NULL){
                write_ligne_rec(file,ligne,current->fils[k],profondeur+1,current,current->nombre_fils,k);
            }
    }
}
}

void write_fichier(FILE* file, arbre_pat* arbre){
    assert(arbre!=NULL);
    for (int i=0;i<arbre->len_mots;i++){
        
        write_ligne_rec(file,i,arbre->root,0,NULL,1,0);
        fprintf(file,"%c",'\n');
    }
}

arbre_pat* remplissage_arbre_rec(node* pere, arbre_t* prev_mots, int** matrice,int len_mots,char* start_mot){ //matrice_1 est la matrice des patterns correspondant à current->mot
    if (pere==NULL){
        arbre_pat* res= cree_arbre_pat(len_mots);
        int num_mot=get_num_mot(prev_mots,start_mot); //On crée nos variables 
        int tab_check[prev_mots->nbr_mots];
        for (int i=0;i<prev_mots->nbr_mots;i++){
            tab_check[i]=0;
        }
        int current_pattern; //On va calculer le nombre de fils, ce qui correspond au nombre de patterns différent sur la colonne num_mot
        for (int i=0;i<prev_mots->nbr_mots;i++){
            current_pattern=matrice[i][num_mot];
            tab_check[i]=1;
            for (int k=i+1;k<prev_mots->nbr_mots;k++){

            }


        }

        return res;
    }
} 