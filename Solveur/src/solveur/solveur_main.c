#include "lecture_pattern.h"

int main(int argc, char const *argv[])
{
    /// 0) paramètres :
    int n;
    FILE *fptr;
    char motif[10]="";
    char stat[10]="";
    bool erreur = false;
    /// 1) ouvrir wsofl et récup n
    // cas où n entré en terminal
    if (argc != 1)
    {
        if (argc != 2)
        {
            printf("\nErreur : arg invalide");
            return (0);
        }
        n = atoi(argv[1]);

        if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10 && n!=3) 
        {
            printf("\nErreur : longueur de mot invalide");
            return (0);
        }

        fptr = fopen("wsolf.txt","w");

        fprintf(fptr, "%d", n);
        fclose(fptr);
    }
    //cas où n non entré et que n dans wsolf (ou non)
    fptr = fopen("wsolf.txt","r");
    fscanf(fptr,"%d", &n);
    fclose(fptr);

    if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10 && n!=3){ 
            printf("\nErreur : longueur de mot invalide");
            return (0);
    }

    /// 2) créer arbre

    arbre_t *arb = arbrefinal(n);

    /* //partie test où je rentre manuellement les valeurs
    char* mot1 = malloc(12);
    strncpy(mot1,"CIL",12);
    char* mot2 = malloc(12);
    strncpy(mot2,"BAR",12);
    noeud_t *new1 = create_noeud(0,0,mot1); ///tester de malloc avant de fils 
    noeud_t *new2 = create_noeud(0,8,mot2);
    free(mot1);
    free(mot2);
    arb->pere->fils[0]=new1;
    arb->pere->fils[1]=new2; */

    ///print de l' arbre pour test
    //arb_print(arb->pere);




    int N = arb->pere->pattern; //triche afin d'accéder au nombre de mot total
    //permet de calculer le % de mots en moins pour les stats

    /// 3) proposer racine

    printf("Longueur %d, meilleur premier mot: %s\n\n",n,arb->pere->MotDuNoeud);

    /// 4) boucle pattern -> proposition -> etc
    //récup du pattern résultant
    printf("Entrez le motif du résultat du mot :\n(2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
    char buff[15];
    fgets(buff,15,stdin);
    sscanf(buff,"%s %s",motif, stat); 
    if(strcmp(motif,"-1")==0){destroy_arbre(arb); printf("Arrêt du programme...\n"); return(0);}
    while(n!=strlen(motif)){
        printf("le motif ne correspond pas à un mot de cette longueur.\n");
        printf("Entrez le motif du résultat du mot :\n (2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
        fgets(buff,15,stdin);
        sscanf(buff,"%s %s",motif, stat); 
    }
    //on lit le pattern pour obtenir le mot suivant
    char mot_lu[10]="";
    noeud_t* noeud = arb->pere;
    //check manuellement la suite ici (déplacer puis et/ounon donner le -i)
    if(!nbrgood(motif)&&!erreur){
        //parcourir les fils et si non NULL check le pattern et si ok, return le mot
        noeud = lecture_pattern(noeud,motif);
        if(noeud!=NULL){
            strcpy(mot_lu ,noeud->MotDuNoeud);
            if(strcmp(stat,"-i")==0){
                printf("Avec ce mot, il reste %d / %d mots dans le dictionnaire !\n",nbr_noeuds(noeud),N);
            }
            printf("\nLe mot à rentrer est: %s\n",mot_lu);
        }
    }
    if(noeud==NULL){
        erreur=true;
    }

    while(!nbrgood(motif)&&!erreur){
        //parcours dans l'arbre + cas si -i
        //partie demande du motif
        printf("Entrez le motif suivant :\n");
        fgets(buff,15,stdin);
        sscanf(buff,"%s %s",motif, stat);
        while(n!=strlen(motif)){
            if(strcmp(motif,"-1")==0){destroy_arbre(arb); printf("Arrêt du programme...\n"); return(0);}
            printf("le motif ne correspond pas à un mot de cette longueur.\n");
            printf("Entrez le motif suivant :\n");
            fgets(buff,15,stdin);
            sscanf(buff,"%s %s",motif, stat); 
        }

        noeud = lecture_pattern(noeud,motif);

        ///se déplacer avant de check ça
        if(noeud!=NULL){
            strcpy(mot_lu ,noeud->MotDuNoeud);
            if(strcmp(stat,"-i")==0){
                printf("Avec ce mot, il reste %d / %d mots dans le dictionnaire !\n",nbr_noeuds(noeud),N);
            }
            printf("\nLe mot à rentrer est: %s\n",mot_lu);
        }   //check motif et erreur et check -i
        if(noeud==NULL){
        erreur=true;
    }

    } 

    //check si sortie par 222 ou par erreur(genre 222 qui prend prio)
    if(nbrgood(motif)){
        printf("\nBravo !\nSi vous avez utilisé le bon dictionnaire sans avoir menti au solveur,\nvous devriez avoir gagné\nMerci d'avoir utilisé notre solveur !\n");
    }
    else if(erreur){
        printf("Mhm vous avez dû faire une erreur quelque part,\nréessayez avec le bon dictionnaire\nou sans vous tromper dans vos réponses au solveur.\n");
    }

    destroy_arbre(arb); ///faut free bcz sinon memory leak

    ///pour restart, une fois tout clean,
    ///potentiellement factoriser tout en fct
    ///puis dans le main une boucle sur la fonction si l'utilisateur souhaite.
    return 0;
}
