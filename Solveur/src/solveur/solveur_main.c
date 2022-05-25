#include "lecture_pattern.h"

int main(int argc, char const *argv[])
{
    /// 0) paramètres :
    int n;
    FILE *fptr;
    char motif[10]="";
    char stat[10]="";
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

        if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10 && n!=3) ///LE 3 SERA A RETIRER PLZ BCZ TEST OUBLIEZ PAS LISEZ MOI ICI FAITES GAFFE AUX MAJUSCULES
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

    if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10 && n!=3){ ///ENCORE UNE FOIS PAREIL QU'AU DESSUS ME RATEZ PAS PLZ
            printf("\nErreur : longueur de mot invalide");
            return (0);
    }

    /// 2) créer arbre

    arbre_t *arb = arbrefinal(n);

    /// 3) proposer racine

    printf("Longueur %d, meilleur premier mot: %s\n\n",n,arb->pere->MotDuNoeud);

    /// 4) boucle pattern -> proposition -> etc
    //récup du pattern résultant
    printf("Entrez le motif du résultat du mot :\n(2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
    char buff[15];
    fgets(buff,15,stdin);
    sscanf(buff,"%s %s",motif, stat); 
    if(strcmp(motif,"-1")==0){printf("Arrêt du programme...\n"); return(0);}
    while(n!=strlen(motif)){
        printf("le motif ne correspond pas à un mot de cette longueur.\n");
        printf("Entrez le motif du résultat du mot :\n (2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
        fgets(buff,15,stdin);
        sscanf(buff,"%s %s",motif, stat); 
    }

    while(!nbrgood(motif)){
        //parcours dans l'arbre + cas si -i
        

        fgets(buff,15,stdin);
        sscanf(buff,"%s %s",motif, stat);
    } 

    destroy_arbre(arb); ///bcz sinon memory leak (obvious) (chiant mais c'est comme ça) (et on aime pas que le terminal nous rale dessus)
    return 0;
}
