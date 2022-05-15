#include"ArbreFinal.h"

// implémenter toutes les fonctions de base

noeud_t* creation_noeud_t(char* mot,int pattern){
    noeud_t* res = malloc(sizeof(noeud_t));
    res->MotDuNoeud=mot;
    res->pattern=pattern;
    res->fils=malloc(sizeof(noeud_t))
    return(res)
};


bool noeud_is_empty(arbre_t *one_tree){
    if(one_tree->père==NULL){
        return(true)
    }else{
        return(false)
    }
}

// aviser celui la car ça a l'air complexe
// faire une fonction recursive qui free le pere et s'engouffre dans chacun des fils qui devient à son tour père
void arbre_destroy(arbre_t *one_tree){
    noeud_t *current=one_tree->père;
    noeud_t *tmp=NULL;
    while(current!=NULL){
        for(i=0,i<sizeof(current->fils),i++){     // on est obligé de faire sauter tous les fils
            tmp = current; // pas sur que ça marche
            free(tmp);
            arbre_destroy(arbre_t *one_tree)  // on a besoin d'aller dans les fils en partant du pere mais comment faire retrer ça dans la fonction (onetree->fils marche pas et le type d'entré n'est pas un noeud)
        }
        current=current->père;
    }
    free(one_tree)
}


void tree_append(arbre_t *one_tree, char *MotDuNoeud, int *pattern){
    noeud_t *new_noeud = malloc(sizeof(noeud_t));
        new_noeud->MotDuNoeud=MotDuNoeud;
        new_noeud->pattern=pattern;
        new_noeud->fils=NULL;

    noeud_t *current = one_tree->père;

    if(current==NULL)){
        one_tree->père = new_noeud;
    }
    else{
        while(current->fils!=NULL){  // probleme ici car on entre dans une branche mais comment savoir que c'est la bonne et qu'on va dans les bons fils ?
            current=current->fils;
        }
        current->fils = new_noeud;
    }
}


int main() {

    FILE *f;
    char c;
    f=fopen("test.txt","r");

    while((c=fgetc(f))!=EOF){
        // ici on va rentrer les conditions sur les mots qu'on reçoit
        // ici on met un autre while en attendant que le mots soit stocker entièrement
        char* morceau;
        fscanf( f , "%s" , &morceau); // on recupere les trucs lettres par lettres et on stock dans morceau mais apres il faut se demerder pour decouper en fonction des lignes et des virgules et tout le bordel 
        printf("%c",c);
    }

    fclose(f);

    return 0;
}

