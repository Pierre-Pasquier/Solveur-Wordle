///#include "ArbreFinal.h"
///#include "lecture_pattern.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    /// 0) paramètres :
    int n;
    FILE *fptr;
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
    /// 3) proposer racine
    /// 3) boucle pattern -> proposition -> etc

    return 0;
}
