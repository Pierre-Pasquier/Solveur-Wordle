#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include "arbre.h"



bool is_in(char a,char *mot,int *tab){
    for (int i=0 ; i<strlen(mot) ; i++){
        if (mot[i] == a && tab[i] != 1){
            return true;
        }
    }
    return false;
}

char *cat_d(char car, char *mot){     //concatène les 2 mots dans le 2ème élément
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    res[0] = car;
    for (int k=0;k<strlen(mot);k++){
        res[k+1] = mot[k];
    }
    res[strlen(mot)+1] = '\0';
    return res;
}

char *cat_f(char *mot, char car){     //concatène les 2 mots dans le 2ème élément
    char *res = malloc((strlen(mot)+2)*sizeof(char));
    for (int k=0;k<strlen(mot);k++){
        res[k] = mot[k];
    }
    res[strlen(mot)] = car;
    res[strlen(mot)+1] = '\0';
    return res;
}

int paterne(char *mot_cherche,char *mot_donne,int len_mot){   
    int x = 0;
    int n;
    if (strlen(mot_cherche) > len_mot){
        n = len_mot;
    }
    else{
        n = strlen(mot_cherche);
    }
    int tab_c[n];
    int tab_d[n];
    for (int k=0;k<n;k++){
        tab_c[k] = 0;
        tab_d[k] = 0;
    }
    for (int i=0 ; i<n ; i++){
        if (mot_cherche[i] == mot_donne[i]){
            x += (int) 2*pow(3.0,(double) (n-1-i));
            tab_c[i] = 1;
            tab_d[i] = 1;
        }
    }
    for (int i=0 ; i<n ; i++){
        if (is_in(mot_donne[i],mot_cherche,tab_c) && tab_d[i] != 1){
            tab_c[index(mot_cherche,mot_donne[i])] = 1;
            x += (int) pow(3.0,(double) (n-1-i));
        }
        
    }
    return x;
}

int index(char *mot,char x){
    for (int k=0;k<strlen(mot);k++){
        if (mot[k] == x){
            return k;
        }
    }
    return -1;
}


void mot_suivant(arbre_t *arbre,element_t * elem, char *prefixe,int len_mot,int *num_mot_cherche,int **matrice,int nb_mot){
    if (elem->terminal){    // cas de base, si on est dans une feuille
        int *l = parcours(arbre,len_mot,num_mot_cherche,matrice,elem,prefixe,nb_mot);    //on parcours tous les autres mots qui sont à droite
        int int_c = num_mot_cherche[0];
        for (int k=0;k<nb_mot;k++){
            matrice[int_c][k] = l[k];
        }
        free(l); //On libère l'espace mémoire
        num_mot_cherche[0] = num_mot_cherche[0] + 1;
    }
    for (int i=0; i<26 ; i++){      //pour tous les fils
        if ((elem->fils)[i] != NULL){   //si le fils est non-vide
            char new_prefixe[strlen(prefixe)+2];       // on commence à écrire le mot
            if (elem->value == '\0'){
                memcpy(new_prefixe,prefixe,strlen(prefixe));
                new_prefixe[0] = elem->fils[i]->value;
                new_prefixe[1] = '\0';
            }
            else{
                memcpy(new_prefixe,prefixe,strlen(prefixe));        //copie pour ajouter la valeur du noeud courant  
                new_prefixe[strlen(prefixe)] = elem->fils[i]->value;      //ajout de la valeur du noeud courant
                new_prefixe[strlen(prefixe)+1] = '\0';
            }
            mot_suivant(arbre,elem->fils[i],new_prefixe,len_mot,num_mot_cherche,matrice,nb_mot);      //on descend dans l'arbre pour trouver le suite du mot jusqu'à la feuille
        }
    }
}


int *parcours(arbre_t *arbre, int len_mot, int *num_mot_cherche, int **matrice, element_t *position,char *mot_cherche,int nb_mot){
    
    element_t *tmp = position;      //copie de la position
    int *l = calloc(nb_mot,sizeof(int));
    for (int i=0 ; i<nb_mot; i++){   //pour tous les mots à droite
    
        if (i != num_mot_cherche[0]){
            bool flag = true;
            char *suffixe_mot_cherche = calloc(strlen(mot_cherche)+1,sizeof(char));     //initialisation du suffixe du mot cherche
            char *suffixe_mot_donne = calloc(strlen(mot_cherche)+1,sizeof(char));     //initialisation du suffixe du mot donne
            int profondeur = len_mot;
            tmp = position;     //on réinitialise la position
            int score;
            score = (int) pow(3.0,(double) len_mot) - 1;      //initialisation du score (= paterne en base 10)
            while (profondeur > 0 && !(tmp->char_is_in)[i]){      //tant que la lettre du noeud courant n'est pas dans le mot que l'on veut (mot_donne)
                score -= 2*(int)pow(3.0,(double) len_mot-profondeur);          //vu que la lettre n'est pas dans le mot on enleve la valeur associé du score (repésenté par un 2 dans le paterne en bas 3)
                if (flag == true){
                    flag = false;
                    suffixe_mot_cherche[0] = tmp->value;
                    suffixe_mot_cherche[1] = '\0';
                }else{
                    char *interm = cat_d(tmp->value,suffixe_mot_cherche);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                    for (int k=0;k<strlen(interm);k++){
                        suffixe_mot_cherche[k] = interm[k];
                    }
                    free(interm);
                }
                profondeur--;       //on met à jour la profondeur
                tmp = tmp->pere;    //on remonte au noeud d'au dessus
            }
            flag = true;
            while (tmp != NULL && !(tmp->terminal)){     //une fois arrivé à une lettre dans le prochain mot, on commence la descente
                for (int k=0; k<26 ; k++){      //pour tous les fils
                    if ((tmp->fils)[k] != NULL && ((tmp->fils)[k]->char_is_in)[i]){       //si la lettre est dans le prochain mot
                        tmp = (tmp->fils)[k];       //on se deplace dans ce fils
                        if (flag == true){
                            flag = false;
                            suffixe_mot_donne[0] = tmp->value;
                            suffixe_mot_donne[1] = '\0';
                        }else{
                            char *interm = cat_f(suffixe_mot_donne,tmp->value);     //on ajoute la valeur au suffixe (Attention, reconstitution du suffixe à l'envers)
                            for (int k=0;k<strlen(interm);k++){
                                suffixe_mot_donne[k] = interm[k];
                            }
                            free(interm);
                        }
                        break;
                    } 
                }
            }
            score += paterne(suffixe_mot_cherche,suffixe_mot_donne,len_mot);        //  on compare les suffixe des 2 mots pour avoir le score du patterne et on met à jour le score
            l[i] = score;
            free(suffixe_mot_cherche);
            free(suffixe_mot_donne);
        }
        else {
            l[i]=(int) pow(3.0,(double) len_mot) - 1;
        }

    }
    return l;
}




void rev(char *mot, size_t len)
{
  char tmp;
  len--;
  if (len < 2){
      return;
  } 

  tmp = *mot;
  *mot = mot[len];
  mot[len] = tmp;

  rev(mot+1, len -1);
}

#ifdef SNOW_ENABLED
describe(pattern){
    it("Test de pattern : égalité de mot"){
        char* mot1="BONJOUR";
        char* mot2="BONJOUR";
        int len=7;
        assert(paterne(mot1,mot2,len)==(int)pow(3.0,len)-1,"Vérification de l'égalité de mot");
    }
    it("Test de pattern : vérification valeur"){
        char* mot1="BONJOUR";
        char* mot2="HELLOOO";
        int len=7;
        assert(paterne(mot1,mot2,len)==21,"Vérification de valeur");
        assert(paterne(mot2,mot1,len)==18+(int)pow(3.0,5.0),"Vérification de valeur");
    }
    it("Test des limites : B "){
        char* mot1="BONJOUR";
        char* mot2="BONJOU";
        int len=6;
        int a=paterne(mot1,mot2,len);
        assert(a==(int)pow(3.0,len)-1);
        len=7;
        int b=paterne(mot1,mot2,len);
        assert(b==(int)pow(3.0,7)-3,"Vérification de limite de mots n'étant pas de même longueur");
        mot2="BO";
        int c=paterne(mot1,mot2,len);
        assert(c==2*(int)pow(3.0,6)+2*pow(3.0,5),"Vérification des limites");
        //paterne attendu : 2200000
        //On renvoie le paterne avec les lettres présentes dans le mot
        int d=paterne(mot2,mot1,len);
        assert(d==8,"Vérification de limite de mots n'étant pas de même longueur");
        //paterne attendu : 0000022 <-> 8
        mot2="";
        len=0;
        int e=paterne(mot1,mot2,len);
        assert(e==0,"Vérification du mot vide");
        int f=paterne(mot2,mot1,len);
        assert(f==0,"Vérification du mot vide");
    }
    //On ne peut pas trouver un mot spécifique avec un paterne donné, même en fixant le mot cherché, il peut avoir plusieurs mots qui correspondent
    //Mais on peut au moins vérifier qu'il appartient à la classe des mots de ce paterne donné
        it("Test inverse : I"){
            arbre_t* arbre=construct_arbre(6);
            int** matrice=calloc(arbre->nbr_mots,sizeof(int*));
            for (int i=0;i<arbre->nbr_mots;i++){
                matrice[i]=calloc(arbre->nbr_mots,sizeof(int));
            }
            int *num_mot_cherche = malloc(sizeof(int));
            num_mot_cherche[0] = 0;
            mot_suivant(arbre,arbre->racine,"",6,num_mot_cherche,matrice,arbre->nbr_mots);
            int d=paterne("ABATTU","CASIER",6); // L'arbre des paternes est fait de telle sorte que le paterne d'un noeud correspond à 
            //paterne(mot_du_noeud,mot_du_père)
            int nb_mots=same_pattern(arbre,matrice,d,get_num_mot(arbre,"CASIER"));
            arbre_t* temp=create_arbre_mots(nb_mots);
            insert_same_pattern(arbre,temp,matrice,d,get_num_mot(arbre,"CASIER"));
            print_arbre(temp);
            bool t=insert_arbre(temp,"ABATTU",0);
            //On vérifie que le mot est déjà dans l'arbre
            assert(!t,"Vérification inverse");
            //Libération de la mémoire
            for (int i=0;i<arbre->nbr_mots;i++){
                free(matrice[i]);
            }
            free(matrice);
        }
        //Nous n'avons pas trouvé de façon de cross-check
        //Pour forcer l'erreur, les paramètres n'étant pas du bon type, le programme ne compile pas
        //Niveau performances, le programme met beaucoup de temps avec "peu" de mots (6000), nous forçant à rester sur notre dictionnaire actuel
    }
describe(motsuivant){
    //On fera majoritairement des tests sur des mots de longueur 3
    int lenmot=3;
    char* mot4="POT";
    char* mot1="BUT";
    char* mot2="FUT";
    char* mot3="MAT";
    arbre_t* arbre=create_arbre_mots(4);
    insert_arbre(arbre,mot1,0); //On insère dans l'ordre lexicographique
    insert_arbre(arbre,mot2,1);
    insert_arbre(arbre,mot3,2);
    insert_arbre(arbre,mot4,3);
    //Création de la matrice
    int **matrice=calloc(4,sizeof(int*));
    for (int i=0;i<4;i++){
        matrice[i]=calloc(4,sizeof(int));
    }
    int *num_mot_cherche = malloc(sizeof(int));
    num_mot_cherche[0] = 0;
    it("Test : Vérification des résultats, Right"){
        mot_suivant(arbre,arbre->racine,"",lenmot,num_mot_cherche,matrice,4);
        for (int i=0;i<4;i++){
            assert(matrice[i][i]==(int)pow(3.0,lenmot)-1,"Vérification de la diagonale, mot_c = mot_d");
        }
        //Quelques vérifications
        assert(matrice[1][3]=paterne(mot2,mot4,lenmot),"Vérif");
        assert(matrice[3][0]==paterne(mot4,mot1,lenmot),"Vérif");
    }
    it("Test des limites : nb mot=1 ,len mot=1000"){
        char promp[1001];
        memset(promp,'A',1001);
        promp[1000]='\0';
        int** matrice2=calloc(1,sizeof(int*));
        matrice2[0]=calloc(1,sizeof(int));
        int *num_mot_cherche=calloc(1,sizeof(int));
        num_mot_cherche[0]=0; 
            
        arbre_t* t1=create_arbre_mots(1);
        insert_arbre(t1,promp,0); 
        mot_suivant(t1,t1->racine,"",100,num_mot_cherche,matrice2,1);  
    }
    it("Test des limites B: nb mot = 1, len mot=2000"){
        char promp2[2001];
        memset(promp2,'A',2001);
        promp2[2000]='\0';
        int** matrice3=calloc(1,sizeof(int*));
        matrice3[0]=calloc(1,sizeof(int));
        arbre_t* t2=create_arbre_mots(1);
        insert_arbre(t2,promp2,0);
        int *num_mot_cherche=calloc(1,sizeof(int));
        num_mot_cherche[0]=0; 
        mot_suivant(t2,t2->racine,"",200,num_mot_cherche,matrice3,1);
        }
    it ("Test des limites B: nb_mots = 2000, len=6"){
        //On prend les mots du ditionnaire
        arbre_t* arbre=construct_arbre(6);
        int** matrice=calloc(arbre->nbr_mots,sizeof(int*));
        for (int i=0;i<arbre->nbr_mots;i++){
            matrice[i]=calloc(arbre->nbr_mots,sizeof(int));
        }
        int *num_mot_cherche = malloc(sizeof(int));
        num_mot_cherche[0] = 0;
        mot_suivant(arbre,arbre->racine,"",6,num_mot_cherche,matrice,arbre->nbr_mots);
    }
    it ("Test des limites B: nb mots=4000, len=7"){
        //On prend les mots du dictionnaire
        arbre_t* arbre=construct_arbre(7);
        int** matrice=calloc(arbre->nbr_mots,sizeof(int*));
        for (int i=0;i<arbre->nbr_mots;i++){
            matrice[i]=calloc(arbre->nbr_mots,sizeof(int));
        }
        int *num_mot_cherche = malloc(sizeof(int));
        num_mot_cherche[0] = 0;
        mot_suivant(arbre,arbre->racine,"",7,num_mot_cherche,matrice,arbre->nbr_mots);
    }
    //Il n'y a pas de relation inverse
    //Le cross checking a pu être effectué avec l'implémentation en tableau
    //Les erreurs de types ne permettent pas de compiler, par contre on provoque des erreurs de lecture dans la mémoire quand on change les valeurs en paramètre de len_mots mais aussi de nombre_mots
    //Comme montré les performances en temps ne sont pas trop dérangeantes du fait de prétraitement, mais si nous avions à effectuer ces calculs à chaque lancement du solveur, les temps de calculs seraient vraiment longs
}



describe(implemTableau){
    it("Test de performance en temps avec implémentation dans un tableau"){
        FILE* fptr;
        int len_mot=7;
        ///ouverture fichier et lecture pour mettre dans un tableau
        char link[100];
        sprintf(link,"./Mots/mot%d.txt",len_mot);
        puts(link);
        fptr = fopen(link,"r");
        if (fptr==NULL){
            perror ("Error reading file");
        }
        char line[256];
        fgets(line, sizeof(line), fptr);
        int nb_mot;
        sscanf(line,"%d",&nb_mot);
        char* tabmots[nb_mot];
        if (fptr==NULL) 
        {
            perror ("Error reading file");
        }
        else
        {
            for (int k=0 ; k<nb_mot ; k++){
                char mot[20];
                fgets(line,sizeof(line),fptr);
                sscanf(line,"%s",mot);
                tabmots[k]=mot;
            }
        } 
        fclose(fptr);
        //le tableau contient tous les mots de longueur 6
        //Création de la matrice
        int **matrice=calloc(nb_mot,sizeof(int*));
        for (int i=0;i<nb_mot;i++){
            matrice[i]=calloc(nb_mot,sizeof(int));
        }
        char* mot_cherche;
        char* mot_donne;
        //Calcul des patterns

        for (int i=0;i<nb_mot;i++){
            mot_cherche=tabmots[i];
            for (int j=0;j<nb_mot;j++){
                mot_donne=tabmots[j];
                matrice[i][j]=paterne(mot_cherche,mot_donne,len_mot);
            }
        }
        //Tests

        for (int i=0;i<nb_mot;i++){
            assert(matrice[i][i]==(int)pow(3.0,len_mot)-1,"Vérification du remplissage de la matrice");
        }
    }


    }

#endif

























