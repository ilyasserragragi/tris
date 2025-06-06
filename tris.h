
typedef struct {
    char* nom;
    double temps;
} PerfTri;

// Fonctions de tri
void tri_insertion(int* tableau, int taille, PerfTri* perf);
void tri_selection(int* tableau, int taille, PerfTri* perf);
void tri_bulles(int* tableau, int taille, PerfTri* perf);
void tri_rapide(int* tableau, int debut, int fin,PerfTri* perf);
void tri_fusion(int* tableau, int gauche, int droite,PerfTri* perf);

void afficher_tableau(int* tableau, int taille);
void generer_tableau_aleatoire(int* tableau, int taille, int min, int max);
void copier_tableau(int* source, int* destination, int taille);
void echanger(int* a, int* b);
void comparer_tris(int* original, int taille);


