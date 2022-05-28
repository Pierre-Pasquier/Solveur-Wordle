#include "lecture_pattern.h"

bool nbrgood(char* motif){
    return(!strcmp(motif,"222") || !strcmp(motif,"222222") ||!strcmp(motif,"2222222") ||!strcmp(motif,"22222222") ||!strcmp(motif,"222222222") ||!strcmp(motif,"2222222222"));
}

int nbr_noeuds(noeud_t* noeud){
    if(noeud==NULL){return 0;}
    if(noeud->nbfils==1){return 1;} 
    else{
        int S = 0;
        for(int i=0;i<noeud->nbfils;i++){
            S= S+nbr_noeuds(noeud->fils[i]);
        }
        return S;
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
    char link[32]; 
    sprintf(link, "SortiePreTT/out%d.txt", n);
    //test si se combine bien

    long pos = 0;

    fptr = fopen(link,"r");

    if (fptr==NULL) 
    {
        perror ("Error reading file");
    }
    else
    {
        fgets(line, sizeof(line), fptr);
        
        int N=0;
        char root[12]="";
        char nbr[12]="";
        int nbrfils=0;
        char tmp[10]="";
        sscanf(line,"%s %s %s",nbr,root,tmp); ///extraction en root et tmp et nbr
        N = strtoll(nbr,(char**)NULL,10); ///conversion de 10 de line en 10 int
        nbrfils = strtoll(tmp,(char**)NULL,10);///pour convertir séparement tmp en int 
        noeud_t *noeud_pere =create_noeud(nbrfils, N, root); ///on triche avec N afin de pouvoir le récup (pattern du père n'existant pas il n'est pas sensé être utilisé)
        ///de plus, nbrfils car il faut initialiser un tableau de nbrfils fils
        arbre->pere = noeud_pere;
        pos = ftell(fptr);

        lectfils(noeud_pere, fptr, 0, pos);
    }
    if(fptr!=NULL){fclose(fptr);} //ok ne pas si pointeur vers null(ex fichier pas trouvé)

    return arbre; 
}

void lectfils(noeud_t *noeud_pere,FILE* fptr,int nbrpvir,long pos){
    char *line=NULL; //attention prblm de thomas : si fils vide ";;;;;;;" alors ligne suivante n'aura pas leurs
    size_t line_size;
    size_t line_buf_size = 0;
    fseek(fptr, pos, SEEK_SET); //on se repositionne dans le fichier
    line_size = getline(&line,&line_buf_size,fptr);
    if(line_size==-1){free(line);return;}

    long pos2 = ftell(fptr);
    char *car=malloc(2);
    char* mot = malloc(20);
    strncpy(mot,"",1);
    int nbrpvir2 = 0;
    int nbrvir = 0;
    char val[12]="";
    int pattern=0;
    int nbrfils=0;
    char *tmp = line;
    int pvir = 0;

    for (int i = 0; i < line_size; i++)
    {
        strncpy(car,line,1);
        car[1]='\0';
        
        if(nbrpvir2 == nbrpvir){
            if(strcmp(car,",")==0){
                if(strcmp(mot,"")!=0){ //vérif mot non vide 
                    sscanf(mot, "%d %s %d",&pattern,val,&nbrfils);
                    noeud_t *newnoeud = create_noeud(nbrfils,pattern,val);
                    noeud_pere->fils[nbrvir]=newnoeud;
                    memset(mot,0,1);
                    
                    lectfils(newnoeud, fptr,pvir,pos2); 
                    fseek(fptr, pos2, SEEK_SET); //on se repositionne dans le fichier parce que lectfils déplace indirectement
                    nbrvir++;
                    pvir++;
                }
            }
            if(strcmp(car,";")==0){
                ///vérif cas où mot fini par ; à ajouter et vérif non vide
                if(strcmp(mot,"")!=0){
                    sscanf(mot, "%d %s %d",&pattern,val,&nbrfils);
                    noeud_t *newnoeud = create_noeud(nbrfils,pattern,val);
                    noeud_pere->fils[nbrvir]=newnoeud;
                    memset(mot,0,1);
                    lectfils(newnoeud,fptr,pvir,pos2);
                    fseek(fptr, pos2, SEEK_SET); //on se repositionne dans le fichier parce que lectfils déplace indirectement
                    pvir++;
                }
                nbrpvir2++; 
                
            } 
        }
        else if(strcmp(car,",")==0 || strcmp(car,";")==0){
            if(strcmp(mot,"")!=0){pvir++;}
            if(strcmp(car,";")==0){nbrpvir2++;}
            memset(mot,0,1);
            }
        if (strcmp(car,";")!=0 && strcmp(car,",")!=0){
            strncat(mot,car,1);
        }    
        line++;
    }

    free(tmp);
    free(mot);
    free(car);
}

