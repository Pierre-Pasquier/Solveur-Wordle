#include "ArbreFinal.h"
#include "lecture_pattern.h"

int main(int argc, char const *argv[])
{
    /// 0) paramètres :
    int n;
    FILE *fptr;
    char* motif;
    char* stat;
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

        if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10)
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

    if (n != 6 && n != 7 && n != 8 && n != 9 && n != 10){
            printf("\nErreur : longueur de mot invalide");
            return (0);
    }

    /// 2) créer arbre

    //arb = ArbreFinal_Create(n);

    /// 3) proposer racine

    printf("Longueur %d, meilleur premier mot :\n",n);
    //printf("%s\n",GetRoot(arb));

    /// 4) boucle pattern -> proposition -> etc
    //récup du pattern résultant
    printf("Entrez le motif du résultat du mot :\n (2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
    
    scanf("%s %s",&motif, &stat);
    if(motif=="-1"){printf("Arrêt du programme...\n"); return(0);}
    while(n!=strlen(motif)){
        printf("le motif ne correspond pas à un mot de cette longueur.\n");
        printf("Entrez le motif du résultat du mot :\n (2 = bien placé ; 1 = dans le mot ; 0 absent du mot ; ex 01211)\n(ajoutez -i après le motif pour avoir plus d'informations)\n");
        scanf("%s %s",&motif, &stat);
    }

    while(nbrgood(motif)!=n){
        //parcours dans l'arbre + cas si -i
        


    }

    return 0;
}
