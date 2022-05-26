#include "lecture_pattern.h"

bool nbrgood(char* motif){
    return(!strcmp(motif,"222") || !strcmp(motif,"222222") ||!strcmp(motif,"2222222") ||!strcmp(motif,"22222222") ||!strcmp(motif,"222222222") ||!strcmp(motif,"2222222222"));
}

int nbr_noeuds(noeud_t* noeud){
    if(noeud==NULL){return 0;}
    if(noeud->nbfils==0){return 1;} //on pourrait simplement return nbfils, mais thomas a des cas où nbfils=1 avec 0 fils
    else{
        int S = 0;
        for(int i=0;i<noeud->nbfils;i++){
            S= S+nbr_noeuds(noeud->fils[i]);
        }
        return S+1;
    }
}


noeud_t* lecture_pattern(noeud_t* noeud,char* motif){
    //parcourir les fils et si non NULL check le pattern et si ok, return le mot
    if(noeud==NULL){return NULL;}
    if(noeud->nbfils==0){return NULL;}//cas où mot bon, donc ton motif aurait dû être 22..2
    else{
        noeud_t* node=NULL;
        for(int i=0;i<noeud->nbfils;i++){
            if(noeud->fils[i]!=NULL){
                if(strtol(motif,(char**)NULL,3)==noeud->fils[i]->pattern)
                    {node = noeud->fils[i];}
            }
        }
        return node;
    }
}


arbre_t *arbrefinal(int n){
    arbre_t* arbre = create_arbre();
    FILE* fptr;
    char line[256];
    
    ///ouverture fichier
    char link[32] = "FichierTest";
    char str[10];
    sprintf(str, "%d", n);
    strcat(link,str);
    strcat(link,".txt");
    //test si se combine bien

    //long pos = 0;

    fptr = fopen(link,"r");

    if (fptr==NULL) 
    {
        perror ("Error reading file");
    }
    else
    {
        fgets(line, sizeof(line), fptr);
        
        int N=0;
        char *root=malloc(10);
        int nbrfils=0;
        char tmp[10]="";
        N = strtoll(line,(char**)NULL,10); ///conversion de 10 de line en 10 int
        fgets(line, sizeof(line), fptr);
        sscanf(line,"%s %s",root,tmp); ///extraction en root et tmp
        nbrfils = strtoll(tmp,(char**)NULL,10);///pour convertir séparement tmp en int 
        noeud_t *noeud_pere =create_noeud(nbrfils, N, root); ///on triche avec N afin de pouvoir le récup (pattern du père n'existant pas il n'est pas sensé être utilisé)
        ///de plus, nbrfils car il faut initialiser un tableau de nbrfils fils
        arbre->pere = noeud_pere;

        //lectfils(noeud_pere, fptr, 0, pos);
    }
    if(fptr!=NULL){fclose(fptr);} //ok ne pas si pointeur vers null(ex fichier pas trouvé)

    return arbre; 
}

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos){
    char line[256]; //attention prblm de thomas : si fils vide ";;;;;;;" alors ligne suivante n'aura pas leurs
    fseek(fptr, pos, SEEK_CUR); //fils
    fgets(line,sizeof(line),fptr);
    char* mot = "";
    int nbrpvir2 = 0;
    int nbrvir = 0;
    char* val="";
    int *pattern=0;
    int *nbrfils=0;
    for (int i = 0; i < 256; i++)
    {
        if(nbrpvir2 == nbrpvir){
            if(strcmp(&line[i],",")==0){ //vérif mot non vide 
                sscanf(mot, "%d %s %d",pattern,val,nbrfils);
                noeud_t *newnoeud = create_noeud(*nbrfils,*pattern,val);
                noeud_pere->fils[nbrvir]=newnoeud;
                mot = "";
                lectfils(newnoeud, fptr,nbrpvir2+nbrpvir,ftell(fptr)); ///+ nbrpvir?
                nbrvir++;
            }
            else if (strcmp(&line[i],";")!=0 && strcmp(&line[i],",")!=0){
                strncat(mot,&line[i],1);
            }
        }
        if(strcmp(&line[i],";")==0){
            ///vérif cas où mot fini par ; à ajouter et vérif non vide
            if(strcmp(mot,"")!=0){
                sscanf(mot, "%d %s %d",pattern,val,nbrfils);
                noeud_t *newnoeud = create_noeud(*nbrfils,*pattern,val);
                noeud_pere->fils[nbrvir]=newnoeud;
                mot="";
                lectfils(newnoeud,fptr,nbrpvir2+nbrpvir,ftell(fptr));
            }
            nbrpvir2++; 
        }
    }    
}
