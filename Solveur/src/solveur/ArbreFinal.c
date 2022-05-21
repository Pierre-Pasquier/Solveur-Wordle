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



arbre_t *arbrefinal(int n){
    arbre_t* arbre = creation_arbre_t();
    FILE* fptr;
    char line[256];
    
    ///ouverture fichier
    char* link = "FicherTestn.txt";
    link[10] = itoa(n);
    //test si se combine bien
    printf("%s",link);
    long pos = 0;

    fptr = fopen(link,"r");

    if (fptr==NULL) 
    {
        perror ("Error reading file");
    }
    else
    {
        fgets(line, sizeof(line), fptr);
        int N;
        sscanf(line,"%d",N);
        fgets(line, sizeof(line), fptr);
        char* root;
        int nbrfils;
        sscanf(line,"%s %d",root,nbrfils);

        noeud_t *noeud_pere =creation_noeud_t(root, N, nbrfils); ///on triche avec N afin de pouvoir le récup (pattern du père n'existant pas il n'est pas sensé être utilisé)
        ///de plus, nbrfils car il faut initialiser un tableau de nbrfils fils
        arbre->père = noeud_pere;

        lectfils(noeud_pere, fptr, 0, pos);
    }
    

    fclose(fptr);

    return arbre;
}

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos){
    char line[256];
    fseek(fptr, pos, SEEK_CUR);
    fgets(line,sizeof(line),fptr);
    char* mot = "";
    int nbrpvir2 = 0;
    int nbrvir = 0;
    char* val;
    int pattern;
    int nbrfils;
    for (int i = 0; i < 256; i++)
    {
        if(nbrpvir2 == nbrpvir){
            if(line[i]==","){
                sscanf(mot, "%d %s %d",pattern,val,nbrfils);
                noeud_t *newnoeud = creation_noeud_t(val,pattern,nbrfils);
                noeud_pere->tabfils[nbrvir]=newnoeud;
                mot = "";
                lectfils(newnoeud, fptr,nbrpvir2,ftell(fptr));
                nbrvir++;
            }
            else if (line[i]!=";" && line[i]!=","){
                strncat(mot,line[i],1);
            }
        }
        if(line[i]==";"){
            nbrpvir2++;
        }
    }
    
}
