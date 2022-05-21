struct _noeud_t{
    char * MotDuNoeud;
    _noeud_t *fils[];
    int *pattern;
};

typedef strust _noeud_t noeud_t;

struct _arbre_t{
    noeud_t *pere;
};

typedef struct _arbre_t arbre_t;

