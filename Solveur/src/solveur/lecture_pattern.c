#include "lecture_pattern.h"

int nbrgood(char* motif){
    int i;
    int count = 0;
    for (i = 0; i < strlen(motif); i++)
    {
        if(motif[i]=="2"){
            count++;
        }
    }
    return count;    
}


arbre_t *arbrefinal(int n){
    arbre_t* arbre = create_arbre();
    FILE* fptr;
    char line[256];
    
    ///ouverture fichier
    char* link = "FicherTestn.txt";
    link[10] = atoi(n);
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

        noeud_t *noeud_pere =create_noeud(nbrfils, N, root); ///on triche avec N afin de pouvoir le récup (pattern du père n'existant pas il n'est pas sensé être utilisé)
        ///de plus, nbrfils car il faut initialiser un tableau de nbrfils fils
        arbre->pere = noeud_pere;

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
            if(line[i]==","){ //vérif mot non vide 
                sscanf(mot, "%d %s %d",pattern,val,nbrfils);
                noeud_t *newnoeud = create_noeud(nbrfils,pattern,val);
                noeud_pere->fils[nbrvir]=newnoeud;
                mot = "";
                lectfils(newnoeud, fptr,nbrpvir2+nbrpvir,ftell(fptr)); ///+ nbrpvir?
                nbrvir++;
            }
            else if (line[i]!=";" && line[i]!=","){
                strncat(mot,line[i],1);
            }
        }
        if(line[i]==";"){
            ///vérif cas où mot fini par ; à ajouter et vérif non vide
            if(strcmp(mot,"")!=0){
                sscanf(mot, "%d %s %d",pattern,val,nbrfils);
                noeud_t *newnoeud = create_noeud(nbrfils,pattern,val);
                noeud_pere->fils[nbrvir]=newnoeud;
                mot="";
                lectfils(newnoeud,fptr,nbrpvir2+nbrpvir,ftell(fptr));
            }
            nbrpvir2++; 
        }
    }    
}
