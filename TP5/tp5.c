#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cellule {
    int valeur;
    struct Cellule *suivant;
} Cellule;
typedef Cellule PILE;

Cellule *creer_cellule(int valeur) {
    Cellule *NE = malloc(sizeof(Cellule));
    if (!NE) {
        printf("Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }
    NE->valeur = valeur;
    NE->suivant = NULL;
    return NE;
}

PILE *empiler(PILE *p, int valeur) {
    PILE *NE = creer_cellule(valeur);
    NE->suivant = p;
    return NE;
}

int est_vide(PILE *p) {
    return p == NULL;
}

PILE *depiler(PILE *p) {
    if (est_vide(p)) return NULL;
    PILE *temp = p;
    p = p->suivant;
    free(temp);
    return p;
}

int sommet_pile(PILE *p) {
    if (est_vide(p)) {
        printf("Pile vide\n");
        exit(EXIT_FAILURE);
    }
    return p->valeur;
}

PILE *initialiser_pile(void) {
    return NULL;
}







/* r = R', t = T', f = F' */
const char *NT = "RrTtFfP";
const char *VT = "ab()+*$";

/* Lignes : R, R', T, T', F, F', P ; colonnes : a b ( ) + * $ */
const int M[7][7] = {
    { 1, 1, 1, 0, 0, 0, 0},
    { 0, 0, 0, 3, 2, 0, 3},
    { 4, 4, 4, 0, 0, 0, 0},
    { 5, 5, 5, 6, 6, 0, 6},
    { 7, 7, 7, 0, 0, 0, 0},
    { 9, 9, 9, 9, 9, 8, 9},
    {11,12,10, 0, 0, 0, 0}
};

/* Chaine vide = epsilon */
const char *regle[13] = {
    "", "Tr", "+Tr", "", "Ft", "Ft", "",
    "Pf", "*f", "", "(R)", "a", "b" 
};

int indice(const char *s, char x) {
    const char *p = strchr(s, x);
    return p ? (int)(p - s) : -1;
}

int analyser(const char *mot) {
    PILE *pile = initialiser_pile();
    pile = empiler(pile, '$');
    pile = empiler(pile, 'R');
    int ps = 0, accepte = 0;

    /* Le nombre maximal d'etapes evite de boucler en cas d'erreur. */
    for (int etape = 0; etape < 100000 && !est_vide(pile); etape++) {
        char X = (char)sommet_pile(pile);
        char a = mot[ps];
        int i = indice(NT, X);

        if (i >= 0) { /* Non-terminal : consulter M[X,a] */
            int j = indice(VT, a);
            if (j < 0 || M[i][j] == 0) break;
            int r = M[i][j];
            pile = depiler(pile);
            int n = (int)strlen(regle[r]);
            for (int k = n - 1; k >= 0; k--)
                pile = empiler(pile, regle[r][k]);
        } else if (X == '$') { /* Fin */
            accepte = (a == '$' && mot[ps + 1] == '\0');
            break;
        } else if (X == a) { /* Terminal reconnu */
            pile = depiler(pile);
            ps++;
        } else {
            break; /* Erreur */
        }
    }

    while (!est_vide(pile)) pile = depiler(pile);
    return accepte;
}

int main(void) {
    char mot[256];
    printf("Expression : ");
    if (scanf("%254s", mot) != 1) return 1;
    strcat(mot, "$");
    printf("%s\n", analyser(mot) ? "ACCEPTE" : "REJETE");
    return 0;
}
