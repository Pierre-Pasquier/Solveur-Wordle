#include "arbre.h"

double safe_log2(double x){
    if (x>0){
        return log2(x);
    }
    else {
        return 0.0;
    }
}





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

void insert_values_node(node** noeud, char* mot, int nombre_fils, int pattern){
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
        // On insère au premier fils disponible de noeud
        for (int k=0;k<(*noeud)->nombre_fils;k++){
            if ((*noeud)->fils[k]==NULL){
                node* res=malloc(sizeof(node));
                res->mot=mot;
                res->nombre_fils=nombre_fils;
                res->pattern=pattern;
                res->fils=calloc(nombre_fils,sizeof(node*));
                (*noeud)->fils[k]=res;
            }
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
        insert_values_node(&(arbre->root),mot,nombre_fils,pattern);
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

int profondeur(node* noeud){
    if (noeud==NULL){
        return 0;
    }
    else{
        int temp_max=0;
        for (int k=0;k<noeud->nombre_fils;k++){
            if (profondeur(noeud->fils[k])>temp_max){
                temp_max=profondeur(noeud->fils[k]);
            }
        }
        return 1 + temp_max;
            
    }
}

void write_ligne_rec(FILE* file, int ligne, node* current, int profondeur,node* pere, int nb_fils_pere,int indice_boucle){
    if (profondeur==ligne){
        if (current!=NULL){
            fprintf(file,"%d",current->pattern);
            fprintf(file," ");
            fprintf(file,"%s",current->mot);
            fprintf(file," ");
            fprintf(file,"%d",current->nombre_fils);
        }
        if (indice_boucle==nb_fils_pere-1){
            fprintf(file,";");
        }
        else {
            fprintf(file,",");
        }
       

    }
    else {
        
        for (int k=0;k<current->nombre_fils;k++){
            write_ligne_rec(file,ligne,current->fils[k],profondeur+1,current,current->nombre_fils,k);
            }
    }
}


void write_fichier(FILE* file, arbre_pat* arbre){
    assert(arbre!=NULL);
    for (int i=0;i<profondeur(arbre->root);i++){
        
        write_ligne_rec(file,i,arbre->root,0,NULL,1,0);
        fprintf(file,"%c",'\n');
    }
}

int same_pattern(arbre_t* arbre,int** matrice, int pattern, int num_motd){
    int res=1;
    for (int i=0;i<arbre->nbr_mots;i++){
        if (matrice[i][num_motd]==pattern){
            res++;
        }
    }
    return res;
}


void insert_same_pattern(arbre_t* prev,arbre_t* new, int** matrice, int pattern, int num_motd){
    int cpt=0;
    for (int i=0;i<prev->nbr_mots;i++){
        if (matrice[i][num_motd]==pattern){
            insert_arbre(new,get_mot_num(prev,i),cpt);
            cpt++;
        }
    }
}

char* best_mot(arbre_t* arbre,int **matrice,int lenmot){
    //Calcul du max de l'entropie pour tous les mots d'un arbre donné en paramètre
    double temp_sum;
    double entropie_max=0.0;
    int indice_entropie_max=0;
    int tab_check[arbre->nbr_mots];
    int nb_patterns=(int)pow(3.0,(double)lenmot);
    int tab_pattern[nb_patterns];
    for (int pattern=0;pattern<nb_patterns;pattern++){
            tab_pattern[pattern]=0;
    } 
    for (int j=0;j<arbre->nbr_mots;j++){ //Les indices des mots donnés correspondent aux indices de colonnes
        printf("Boucle  j : %d\n",j);
        temp_sum=0;
        for (int i=0;i<arbre->nbr_mots;i++){
            
            //On compte combien de mots sont associés à chaque pattern
            tab_pattern[matrice[i][j]]+=1;
            
        }
        //On calcule l'entropie
        for (int pattern=0;pattern<nb_patterns;pattern++){
            temp_sum+=-1*((double)tab_pattern[pattern]/(double)arbre->nbr_mots)*safe_log2(((double)tab_pattern[pattern]/(double)arbre->nbr_mots));
        }
        printf("Entropie du mot : %f\n",temp_sum);
        //On met à jour le max
        if (temp_sum>entropie_max){
            entropie_max=temp_sum;
            indice_entropie_max=j;
        }
    }
    //On renvoie le mot d'indice entropie_max dans l'arbre
    return get_mot_num(arbre,indice_entropie_max);

    
    
    }





arbre_pat* remplissage_arbre_rec(node* pere, arbre_t* prev_mots, int** matrice_1,int len_mots,char* start_mot,arbre_pat* arbre){ //matrice_1 est la matrice des patterns correspondant à current->mot
    //Calcul du nombre de fils;
    char* temp_best_mot;
    arbre_t* temp;
    int nb_mots;
    int num_motd=get_num_mot(prev_mots,start_mot);
    int nombre_fils=0;
    int current_pattern;
    int tab_check[prev_mots->nbr_mots];
    for (int i=0;i<prev_mots->nbr_mots;i++){
        current_pattern=matrice_1[i][num_motd];
        for (int k=i+1;k<prev_mots->nbr_mots;k++){
            if (tab_check[k]!=1 && matrice_1[i][k]==current_pattern){
                tab_check[k]=1;

            }
        }
        if (tab_check[i]!=1){   //On met à jour les patterns déjà rencontrés
            nombre_fils++;
            tab_check[i]=1;
        }


    }

    //Si le nombre de fils vaut 0 on s'arrête

    //Sinon on calcule les matrices des meilleurs fils et on insère dans l'arbre récursivement

    if (nombre_fils!=0){
        if (pere==NULL){
            //On ajoute le mot à la racine, c'est le début de l'appel récursif
            node* res=malloc(sizeof(node));
            res->mot=start_mot;
            res->nombre_fils=nombre_fils;
            res->pattern=777; // Arbitraire, pour remplir l'arbre au niveau de la racine
            res->fils=calloc(nombre_fils,sizeof(node*));
            arbre->root=res;
        }      
        int *num_mot_cherche = malloc(sizeof(int));
        
        int** matrice_2;
        for (int fils=0;fils<nombre_fils;fils++){
            current_pattern=matrice_1[fils][num_motd];
            nb_mots=same_pattern(prev_mots,matrice_1,current_pattern,num_motd);
            temp=create_arbre_mots(nb_mots);
            insert_same_pattern(prev_mots,temp,matrice_1,current_pattern,num_motd);
            //print_arbre(temp); //pour le test
            //Création de la matrice pour les fils de même pattern

            matrice_2=calloc(temp->nbr_mots,sizeof(int*));
            for (int i=0;i<temp->nbr_mots;i++){
                matrice_2[i]=calloc(temp->nbr_mots,sizeof(int));
            }
            //On rempli la matrice
            num_mot_cherche[0] = 0;
            mot_suivant(temp,temp->racine,"",len_mots,num_mot_cherche,matrice_2,temp->nbr_mots);
            //On calcule le meilleur fils
            temp_best_mot=best_mot(temp,matrice_2,len_mots);






            //On libère l'espace mémoire
            for (int i=0;i<temp->nbr_mots;i++){
                free(matrice_2[i]);
            }
            free(matrice_2);
            destroy_arbre(temp);
            free(num_mot_cherche);

        }

    }
    
    
    
    
    

} 