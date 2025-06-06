#include "tris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>




double temp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void echanger(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void tri_insertion(int* tableau, int taille, PerfTri* perf) {
    double debut = temp();

    for (int i = 1; i < taille; i++) {
        int cle = tableau[i];
        int j = i - 1;

        while (j >= 0 && tableau[j] > cle) {
            tableau[j + 1] = tableau[j];
            j--;
        }
        tableau[j + 1] = cle;
    }

    if (perf) perf->temps = temp() - debut;
}

void tri_selection(int* tableau, int taille, PerfTri* perf) {
    double debut = temp();

    for (int i = 0; i < taille-1; i++) {
        int indice_min = i;
        for (int j = i+1; j < taille; j++) {
            if (tableau[j] < tableau[indice_min]) {
                indice_min = j;
            }
        }
        echanger(&tableau[indice_min], &tableau[i]);
    }

    if (perf) perf->temps = temp() - debut;
}

void tri_bulles(int* tableau, int taille, PerfTri* perf) {
    double debut = temp();

    for (int i = 0; i < taille-1; i++) {
        for (int j = 0; j < taille-i-1; j++) {
            if (tableau[j] > tableau[j+1]) {
                echanger(&tableau[j], &tableau[j+1]);
            }
        }
    }

    if (perf) perf->temps = temp() - debut;
}

int partitionner(int* tableau, int debut, int fin) {
    int pivot = tableau[fin];
    int i = (debut - 1);

    for (int j = debut; j <= fin-1; j++) {
        if (tableau[j] < pivot) {
            i++;
            echanger(&tableau[i], &tableau[j]);
        }
    }
    echanger(&tableau[i + 1], &tableau[fin]);
    return (i + 1);
}

void tri_rapide(int* tableau, int debut, int fin, PerfTri* perf) {
    double start = temp();
    if (debut < fin) {
        int pi = partitionner(tableau, debut, fin);
        tri_rapide(tableau, debut, pi - 1, NULL);
        tri_rapide(tableau, pi + 1, fin, NULL);
    }
    if (perf) perf->temps = temp() - start;
}

void fusionner(int* tableau, int gauche, int milieu, int droite) {
    int i, j, k;
    int n1 = milieu - gauche + 1;
    int n2 = droite - milieu;

    int gauche_temp[n1], droite_temp[n2];

    for (i = 0; i < n1; i++) gauche_temp[i] = tableau[gauche + i];
    for (j = 0; j < n2; j++) droite_temp[j] = tableau[milieu + 1 + j];

    i = 0; j = 0; k = gauche;

    while (i < n1 && j < n2) {
        if (gauche_temp[i] <= droite_temp[j]) tableau[k++] = gauche_temp[i++];
        else tableau[k++] = droite_temp[j++];
    }

    while (i < n1) tableau[k++] = gauche_temp[i++];
    while (j < n2) tableau[k++] = droite_temp[j++];
}

void tri_fusion(int* tableau, int gauche, int droite, PerfTri* perf) {
    double start = temp();
    if (gauche < droite) {
        int milieu = gauche + (droite - gauche) / 2;
        tri_fusion(tableau, gauche, milieu, NULL);
        tri_fusion(tableau, milieu + 1, droite, NULL);
        fusionner(tableau, gauche, milieu, droite);
    }
    if (perf) perf->temps = temp() - start;
}

void afficher_tableau(int* tableau, int taille) {
    for (int i = 0; i < taille; i++) printf("%d ", tableau[i]);
    printf("\n");
}

void generer_tableau_aleatoire(int* tableau, int taille, int min, int max) {
    for (int i = 0; i < taille; i++)
        tableau[i] = rand() % (max - min + 1) + min;
}

void copier_tableau(int* source, int* destination, int taille) {
    for (int i = 0; i < taille; i++)
        destination[i] = source[i];
}

void comparer_tris(int* original, int taille) {
    int* tableau = malloc(taille * sizeof(int));
    PerfTri resultats[5] = {
        {"Insertion", 0}, {"Selection", 0},
        {"Bulles", 0}, {"Rapide", 0}, {"Fusion", 0}
    };

    // Insertion
    copier_tableau(original, tableau, taille);
    tri_insertion(tableau, taille, &resultats[0]);

    // Selection
    copier_tableau(original, tableau, taille);
    tri_selection(tableau, taille, &resultats[1]);

    // Bulles
    copier_tableau(original, tableau, taille);
    tri_bulles(tableau, taille, &resultats[2]);

    // Rapide
    copier_tableau(original, tableau, taille);
    tri_rapide(tableau, 0, taille-1, &resultats[3]);

    // Fusion
    copier_tableau(original, tableau, taille);
    tri_fusion(tableau, 0, taille-1, &resultats[4]);

    printf("\nComparaison des algorithmes (temps en ms):\n");
    printf("Algorithme\tTemps (ms)\n");
    printf("------------------------\n");
    for (int i = 0; i < 5; i++) {
        printf("%-10s\t%.10f\n", resultats[i].nom, resultats[i].temps);
    }

    free(tableau);
}

void afficher_menu_principal() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Saisir un tableau\n");
    printf("2. Generer un tableau aleatoire\n");
    printf("3. Trier le tableau\n");
    printf("4. Comparer les algorithmes\n");
    printf("5. Quitter\n");
    printf("Choix : ");
}

void afficher_menu_tri() {
    printf("\n=== CHOIX DU TRI ===\n");
    printf("1. Tri par insertion\n");
    printf("2. Tri par selection\n");
    printf("3. Tri a bulles\n");
    printf("4. Tri rapide\n");
    printf("5. Tri fusion\n");
    printf("6. Retour\n");
    printf("Choix : ");
}

int main() {
    srand(time(NULL));
    int choix, taille = 0;
    int *tableau = NULL;
    int *copie = NULL;
    PerfTri perf = {"", 0};

    do {
        afficher_menu_principal();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Taille du tableau : ");
                scanf("%d", &taille);
                free(tableau);
                tableau = malloc(taille * sizeof(int));
                printf("Saisir les elements :\n");
                for (int i = 0; i < taille; i++) {
                    printf("Element %d : ", i+1);
                    scanf("%d", &tableau[i]);
                }
                break;

            case 2:
                printf("Taille du tableau : ");
                scanf("%d", &taille);
                free(tableau);
                tableau = malloc(taille * sizeof(int));
                generer_tableau_aleatoire(tableau, taille, 1, 100);
                printf("Tableau genere : ");
                afficher_tableau(tableau, taille);
                break;

            case 3:
                if (!tableau) {
                    printf("Veuillez d'abord creer un tableau!\n");
                    break;
                }
                free(copie);
                copie = malloc(taille * sizeof(int));
                copier_tableau(tableau, copie, taille);

                int choix_tri;
                do {
                    afficher_menu_tri();
                    scanf("%d", &choix_tri);

                    if (choix_tri >= 1 && choix_tri <= 5) {
                        copier_tableau(tableau, copie, taille);

                        switch (choix_tri) {
                            case 1:
                                tri_insertion(copie, taille, &perf);
                                perf.nom = "Insertion";
                                break;
                            case 2:
                                tri_selection(copie, taille, &perf);
                                perf.nom = "Selection";
                                break;
                            case 3:
                                tri_bulles(copie, taille, &perf);
                                perf.nom = "Bulles";
                                break;
                            case 4:
                                tri_rapide(copie, 0, taille-1, &perf);
                                perf.nom = "Rapide";
                                break;
                            case 5:
                                tri_fusion(copie, 0, taille-1, &perf);
                                perf.nom = "Fusion";
                                break;
                        }
                        printf("\nTableau trié (%s): ", perf.nom);
                        afficher_tableau(copie, taille);
                        printf("Temps : %.6f ms\n", perf.temps);
                    }
                } while (choix_tri != 6);
                break;

            case 4:
                if (!tableau) {
                    printf("Veuillez d'abord creer un tableau!\n");
                    break;
                }
                comparer_tris(tableau, taille);
                break;

            case 5:
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 5);

    free(tableau);
    free(copie);
    return 0;
}
