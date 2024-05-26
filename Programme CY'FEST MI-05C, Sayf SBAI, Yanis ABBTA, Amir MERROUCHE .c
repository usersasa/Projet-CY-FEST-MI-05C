#include <stdio.h> // Inclusion de la bibliothèque standard d'entrée/sortie
#include <stdlib.h> // Inclusion de la bibliothèque standard des fonctions utilitaires
#include <string.h> // Inclut la bibliothèque des fonctions de manipulation des chaînes de caractères

#define MAX_SALLES 10 // Définit le nombre maximal de salles de concert
#define MAX_PLACE_NAME 50 // Définit la longueur maximale pour le nom d'une salle
#define MAX_ARTIST_NAME 50 // Définit la longueur maximale pour le nom d'un artiste

// Définitions des couleurs pour l'affichage
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_PURPLE  "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Structure représentant une salle de concert
typedef struct {
    char nom[MAX_PLACE_NAME]; // Nom de la salle
    int nombre_rangees_A; // Nombre de rangées pour la catégorie A
    int nombre_rangees_B; // Nombre de rangées pour la catégorie B
    int nombre_rangees_total; // Nombre total de rangées
    int *sieges_par_rangee;  // Pointeur vers le nombre de sièges par rangée
    int fosse;    //  la fosse 
    int concert_attribue; // Variable qui indique si un concert est attribué
    char artiste[MAX_ARTIST_NAME]; // Nom de l'artiste
    float prix_categorie_concert[3]; // Prix des billets par catégorie
    int fin_concert; // Heure de fin du concert
    int sieges_reserves;  // Nombre de sièges réservés
} SalleConcert;

// Fonction pour vider le tampon d'entrée
void viderTampon() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fonction pour lire une chaîne de caractères
void lireChaine(char *chaine, int longueur) {
    fgets(chaine, longueur, stdin);
    chaine[strcspn(chaine, "\n")] = '\0';
}

// Fonction pour sauvegarder les salles dans un fichier
void sauvegarderSalles(SalleConcert salles[], int nombre_salles) {
    FILE *file = fopen("salles.txt", "w"); // Ouvre le fichier en écriture
    for (int i = 0; i < nombre_salles; i++) {
         // Écrit les informations de base de chaque salle
        fprintf(file, "%s %d %d %d %d %d %d %d\n", salles[i].nom, salles[i].nombre_rangees_A, salles[i].nombre_rangees_B, salles[i].nombre_rangees_total, salles[i].fosse, salles[i].concert_attribue, salles[i].fin_concert, salles[i].sieges_reserves);
        for (int j = 0; j < salles[i].nombre_rangees_total; j++) {
             // Écrit le nombre de sièges par rangée
            fprintf(file, "%d ", salles[i].sieges_par_rangee[j]);
        }
        fprintf(file, "\n");
         // Écrit les informations de l'artiste et les prix des billets
        fprintf(file, "%s %.2f %.2f %.2f\n", salles[i].artiste, salles[i].prix_categorie_concert[0], salles[i].prix_categorie_concert[1], salles[i].prix_categorie_concert[2]);
    }
    fclose(file); // Ferme le fichier
}

// Fonction pour charger les salles à partir d'un fichier
void chargerSalles(SalleConcert salles[], int *nombre_salles) {
    FILE *file = fopen("salles.txt", "r"); // Ouvre le fichier en lecture
    if (!file) return;

    // Lit les informations des salles depuis le fichier
    while (fscanf(file, "%s %d %d %d %d %d %d %d\n", salles[*nombre_salles].nom, &salles[*nombre_salles].nombre_rangees_A, &salles[*nombre_salles].nombre_rangees_B, &salles[*nombre_salles].nombre_rangees_total, &salles[*nombre_salles].fosse, &salles[*nombre_salles].concert_attribue, &salles[*nombre_salles].fin_concert, &salles[*nombre_salles].sieges_reserves) != EOF) {
        salles[*nombre_salles].sieges_par_rangee = (int *)malloc(salles[*nombre_salles].nombre_rangees_total * sizeof(int)); // Alloue la mémoire pour les sièges par rangée
        for (int j = 0; j < salles[*nombre_salles].nombre_rangees_total; j++) {
            fscanf(file, "%d", &salles[*nombre_salles].sieges_par_rangee[j]); // Lit le nombre de sièges par rangée
        }
        fscanf(file, "\n");
        fscanf(file, "%s %f %f %f\n", salles[*nombre_salles].artiste, &salles[*nombre_salles].prix_categorie_concert[0], &salles[*nombre_salles].prix_categorie_concert[1], &salles[*nombre_salles].prix_categorie_concert[2]);
        (*nombre_salles)++;  // Incrémente le compteur de salles
    }
    fclose(file); // Ferme le fichier
}

// Fonction pour afficher le menu des concerts disponibles
void afficherMenuConcerts(SalleConcert salles[], int nombre_salles) {
    printf("Sélectionnez un concert :\n");
    for (int i = 0; i < nombre_salles; i++) {
        if (salles[i].concert_attribue) { // Affiche seulement les salles avec des concerts attribués
            printf("%d. Concert de %s dans la salle %s\n", i + 1, salles[i].artiste, salles[i].nom);
        }
    }
}
// Fonction pour vérifier si une salle possède des rangées de catégorie C
int possedeRangCategorieC(SalleConcert salle) {
    int nombreRangeesC = salle.nombre_rangees_total - salle.nombre_rangees_A - salle.nombre_rangees_B;
    return nombreRangeesC > 0; // Retourne vrai si le nombre de rangées de catégorie C est positif
}


// Fonction pour créer une nouvelle salle de concert
void creerSalle(SalleConcert salles[], int *nombre_salles) {
    if (*nombre_salles >= MAX_SALLES) {   // Vérifie si le nombre maximal de salles est atteint
        printf("Nombre maximal de salles atteint.\n");
        return;
    }

    SalleConcert nouvelle_salle; // Crée une nouvelle salle
    printf("\nCréation d'une nouvelle salle de concert :\n");  // Message d'information
    printf("Nom de la salle de concert : "); // Demande du nom de la salle
    lireChaine(nouvelle_salle.nom, MAX_PLACE_NAME);  // Lecture du nom de la salle

    do {
        printf("Nombre de rangées pour la catégorie A : "); // Demande du nombre de rangées pour la catégorie A
        while (scanf("%d", &nouvelle_salle.nombre_rangees_A) != 1 || nouvelle_salle.nombre_rangees_A < 0) {  // Lecture et vérification de l'entrée
            printf("Donner un nombre de rangées correct (entier positif) svp : ");
            viderTampon(); // Vidage du tampon
        }
    } while (nouvelle_salle.nombre_rangees_A < 0);  // Boucle jusqu'à obtenir une valeur correcte

    do {
        printf("Nombre de rangées pour la catégorie B : "); // Demande du nombre de rangées pour la catégorie B
        while (scanf("%d", &nouvelle_salle.nombre_rangees_B) != 1 || nouvelle_salle.nombre_rangees_B < 0) { // Lecture et vérification de l'entrée
            printf("Donner un nombre de rangées correct (entier positif) svp : ");
            viderTampon(); // Vidage du tampon
        }
    } while (nouvelle_salle.nombre_rangees_B < 0);  // Boucle jusqu'à obtenir une valeur correcte

    do {
        printf("Combien de rangées au total : "); // Demande du nombre total de rangées
        while (scanf("%d", &nouvelle_salle.nombre_rangees_total) != 1 || nouvelle_salle.nombre_rangees_total < nouvelle_salle.nombre_rangees_A + nouvelle_salle.nombre_rangees_B) { // Lecture et vérification de l'entrée
            printf("Le nombre total de rangées doit être supérieur ou égal à la somme des rangées de catégorie A et B.\n");
            viderTampon(); // Vidage du tampon
        }
    } while (nouvelle_salle.nombre_rangees_total < nouvelle_salle.nombre_rangees_A + nouvelle_salle.nombre_rangees_B); // Boucle jusqu'à obtenir une valeur correcte


    viderTampon(); // Vidage du tampon

    nouvelle_salle.sieges_par_rangee = (int *)malloc(nouvelle_salle.nombre_rangees_total * sizeof(int));  // Allocation mémoire pour les sièges par rangée

    int personnaliser;
    do {
        printf("Voulez-vous personnaliser le nombre de sièges par rangée ? (1 pour oui, 0 pour non) : "); //choisir si l'utilisateur veut personnliser le nombre de siege par rangée ou toutes les rangées ont le meme nombre de sieges
        while (scanf("%d", &personnaliser) != 1 || (personnaliser != 0 && personnaliser != 1)) {
            printf("Veuillez entrer 1 pour oui ou 0 pour non : ");
            viderTampon();
        }
    } while (personnaliser != 0 && personnaliser != 1);

    viderTampon();

    if (personnaliser == 1) {
        for (int i = 0; i < nouvelle_salle.nombre_rangees_total; i++) {  // Boucle sur les rangées
            do {
                printf("Nombre de sièges pour la rangée %d : ", i + 1); // Demande du nombre de sièges pour la rangée i
                while (scanf("%d", &nouvelle_salle.sieges_par_rangee[i]) != 1 || nouvelle_salle.sieges_par_rangee[i] <= 0) { // Lecture et vérification de l'entrée
                    printf("Donner un nombre de sièges correct (entier positif) svp  ");
                    printf("Veuillez entrer un nombre de sièges correct (entier positif) : ");
                    viderTampon();
                }
            } while (nouvelle_salle.sieges_par_rangee[i] <= 0); // Boucle jusqu'à obtenir une valeur correcte
        }
    } else { // Si l'utilisateur choisit un nombre uniforme de sièges pour toutes les rangées
        int sieges_par_rangee;
        do {
            printf("Nombre de sièges par rangée : ");
            while (scanf("%d", &sieges_par_rangee) != 1 || sieges_par_rangee <= 0) {
                printf("Veuillez entrer un nombre de sièges correct (entier positif) : ");
                viderTampon();
            }
        } while (sieges_par_rangee <= 0);

        viderTampon();

        for (int i = 0; i < nouvelle_salle.nombre_rangees_total; i++) { // Boucle sur les rangées
            nouvelle_salle.sieges_par_rangee[i] = sieges_par_rangee;   // Assigne le même nombre de sièges à chaque rangée
        }
    }

    do {
        printf("La salle a-t-elle une fosse ? (1 pour oui, 0 pour non) : "); // Demande si la salle a une fosse
        while (scanf("%d", &nouvelle_salle.fosse) != 1 || (nouvelle_salle.fosse != 0 && nouvelle_salle.fosse != 1)) { // Lecture et vérification de l'entr
            printf("Veuillez entrer 1 pour oui ou 0 pour non : ");
            viderTampon();
        }
    } while (nouvelle_salle.fosse != 0 && nouvelle_salle.fosse != 1); // Boucle jusqu'à obtenir une valeur correcte


    viderTampon();

    if (nouvelle_salle.fosse == 1) {
        for (int i = 0; i < nouvelle_salle.nombre_rangees_A; i++) {
            nouvelle_salle.sieges_par_rangee[i] *= 2;
        }
    }

    nouvelle_salle.concert_attribue = 0;  // Initialise à 0 (aucun concert attribué)
    nouvelle_salle.sieges_reserves = 0;  // Initialise à 0 (aucun siège réservé)

    salles[*nombre_salles] = nouvelle_salle;  // Ajoute la nouvelle salle à la liste des salles
    (*nombre_salles)++; // Incrémente le compteur de salles
    printf("Salle de concert créée avec succès ! ");
    sauvegarderSalles(salles, *nombre_salles); // Sauvegarde les salles dans le fichier
}

// Fonction pour attribuer un concert à une salle
void attribuerConcert(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) { // Vérifie s'il n'y a plus de salles disponibles
        printf("Aucune salle de concert disponible.\n");
        return; // Sort de la fonction si aucune salle n'est disponible
    }

    char nom_salle[MAX_PLACE_NAME]; // Déclare une variable pour le nom de la salle
    printf("\nAttribution d'un concert :\n");
    printf("Nom de la salle de concert : "); 
    lireChaine(nom_salle, MAX_PLACE_NAME); // Lit le nom de la salle de concert renseigné par l'utilisateur

    int index_salle = -1; // Initialise l'index de la salle à -1 (non trouvé)
    for (int i = 0; i < nombre_salles; i++) { // Boucle à travers toutes les salles disponibles
        if (strcmp(nom_salle, salles[i].nom) == 0) { // Compare le nom saisi avec les noms des salles existantes
            index_salle = i; // Si trouvé, stocke l'index de la salle
            break; // Sort de la boucle une fois la salle trouvée
        }
    }

    if (index_salle == -1) { // Vérifie si la salle n'a pas été trouvée
        printf("La salle de concert spécifiée n'existe pas.\n");
        return; // Sort de la fonction si la salle n'existe pas
    }

    printf("Nom de l'artiste : ");
    lireChaine(salles[index_salle].artiste, MAX_ARTIST_NAME); // Lit le nom de l'artiste et le stocke dans la structure de la salle
    printf("Prix catégorie A : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[0]); // Lit et stocke le prix pour la catégorie A
    printf("Prix catégorie B : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[1]); // Lit et stocke le prix pour la catégorie B
    printf("Prix catégorie C : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[2]); // Lit et stocke le prix pour la catégorie C
    do {
        printf("Heure de fin du concert (format 24h) : ");
        while (scanf("%d", &salles[index_salle].fin_concert) != 1 || salles[index_salle].fin_concert < 0 || salles[index_salle].fin_concert > 24) {
            printf("Donner une heure correcte (entier positif entre 0 et 24) : "); // Demande une heure de fin valide
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (salles[index_salle].fin_concert < 0 || salles[index_salle].fin_concert > 24); // Boucle jusqu'à ce qu'une heure valide soit saisie

    viderTampon(); // Vide à nouveau le tampon d'entrée

    salles[index_salle].concert_attribue = 1; // Marque le concert comme attribué à cette salle

    sauvegarderSalles(salles, nombre_salles); // Sauvegarde les informations des salles avec les nouvelles données

    printf("Concert attribué à la salle %s avec succès.\n\n", salles[index_salle].nom); // Affiche un message de succès
}

// Fonction pour modifier les informations d'une salle de concert
void modifierSalle(SalleConcert salles[], int nombre_salles) {
    char nom_salle[MAX_PLACE_NAME]; // Déclare une variable pour le nom de la salle
    printf("\nModification de la salle de concert :\n");
    printf("Nom de la salle de concert : ");
    lireChaine(nom_salle, MAX_PLACE_NAME); // Lit le nom de la salle de concert renseigné par l'utilisateur

    int index_salle = -1; // Initialise l'index de la salle à -1 (non trouvé)
    for (int i = 0; i < nombre_salles; i++) { // Boucle à travers toutes les salles disponibles
        if (strcmp(nom_salle, salles[i].nom) == 0) { // Compare le nom saisi avec les noms des salles existantes
            index_salle = i; // Si trouvé, stocke l'index de la salle
            break; // Sort de la boucle une fois la salle trouvée
        }
    }

    if (index_salle == -1) { // Vérifie si la salle n'a pas été trouvée
        printf("La salle de concert spécifiée n'existe pas.\n");
        return; // Sort de la fonction si la salle n'existe pas
    }

    do {
        printf("Nombre de rangées pour la catégorie A : ");
        while (scanf("%d", &salles[index_salle].nombre_rangees_A) != 1 || salles[index_salle].nombre_rangees_A < 0) {
            printf("Donner un nombre de rangées correct (entier positif) svp : "); // Demande un nombre valide de rangées pour la catégorie A
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (salles[index_salle].nombre_rangees_A < 0); // Boucle jusqu'à ce qu'un nombre valide soit saisi

    do {
        printf("Nombre de rangées pour la catégorie B : ");
        while (scanf("%d", &salles[index_salle].nombre_rangees_B) != 1 || salles[index_salle].nombre_rangees_B < 0) {
            printf("Donner un nombre de rangées correct (entier positif) svp : "); // Demande un nombre valide de rangées pour la catégorie B
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (salles[index_salle].nombre_rangees_B < 0); // Boucle jusqu'à ce qu'un nombre valide soit saisi

    do {
        printf("Combien de rangées au total : ");
        while (scanf("%d", &salles[index_salle].nombre_rangees_total) != 1 || salles[index_salle].nombre_rangees_total < salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B) {
            printf("Le nombre total de rangées doit être supérieur ou égal à la somme des rangées de catégorie A et B.\n"); // Demande un nombre total valide de rangées
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (salles[index_salle].nombre_rangees_total < salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B); // Boucle jusqu'à ce qu'un nombre valide soit saisi

    viderTampon(); // Vide à nouveau le tampon d'entrée

    // Libérer l'ancienne mémoire
    free(salles[index_salle].sieges_par_rangee); // Libère la mémoire allouée précédemment pour les sièges par rangée

    salles[index_salle].sieges_par_rangee = (int *)malloc(salles[index_salle].nombre_rangees_total * sizeof(int)); // Alloue de la mémoire pour le nouveau nombre de rangées

    int personnaliser;
    do {
        printf("Voulez-vous personnaliser le nombre de sièges par rangée ? (1 pour oui, 0 pour non) : ");
        while (scanf("%d", &personnaliser) != 1 || (personnaliser != 0 && personnaliser != 1)) {
            printf("Veuillez entrer 1 pour oui ou 0 pour non : "); // Demande si l'utilisateur veut personnaliser le nombre de sièges par rangée
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (personnaliser != 0 && personnaliser != 1); // Boucle jusqu'à ce qu'une réponse valide soit saisie

    viderTampon(); // Vide à nouveau le tampon d'entrée

    if (personnaliser == 1) { // Si l'utilisateur veut personnaliser le nombre de sièges par rangée
        for (int i = 0; i < salles[index_salle].nombre_rangees_total; i++) { // Boucle à travers toutes les rangées
            do {
                printf("Nombre de sièges pour la rangée %d : ", i + 1); // Demande le nombre de sièges pour chaque rangée
                while (scanf("%d", &salles[index_salle].sieges_par_rangee[i]) != 1 || salles[index_salle].sieges_par_rangee[i] <= 0) {
                    printf("Veuillez entrer un nombre de sièges correct (entier positif) : "); // Demande un nombre valide de sièges
                    viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
                }
            } while (salles[index_salle].sieges_par_rangee[i] <= 0); // Boucle jusqu'à ce qu'un nombre valide soit saisi
        }
    } else { // Si l'utilisateur ne veut pas personnaliser le nombre de sièges par rangée
        int sieges_par_rangee;
        do {
            printf("Nombre de sièges par rangée : ");
            while (scanf("%d", &sieges_par_rangee) != 1 || sieges_par_rangee <= 0) {
                printf("Veuillez entrer un nombre de sièges correct (entier positif) : "); // Demande un nombre valide de sièges par rangée
                viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
            }
        } while (sieges_par_rangee <= 0); // Boucle jusqu'à ce qu'un nombre valide soit saisi

        viderTampon(); // Vide à nouveau le tampon d'entrée

        for (int i = 0; i < salles[index_salle].nombre_rangees_total; i++) {
            salles[index_salle].sieges_par_rangee[i] = sieges_par_rangee; // Attribue le même nombre de sièges à toutes les rangées
        }
    }

    do {
        printf("La salle a-t-elle une fosse ? (1 pour oui, 0 pour non) : ");
        while (scanf("%d", &salles[index_salle].fosse) != 1 || (salles[index_salle].fosse != 0 && salles[index_salle].fosse != 1)) {
            printf("Veuillez entrer 1 pour oui ou 0 pour non : "); // Demande si la salle a une fosse
            viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture
        }
    } while (salles[index_salle].fosse != 0 && salles[index_salle].fosse != 1); // Boucle jusqu'à ce qu'une réponse valide soit saisie

    viderTampon(); // Vide à nouveau le tampon d'entrée

    if (salles[index_salle].fosse == 1) { // Si la salle a une fosse
        for (int i = 0; i < salles[index_salle].nombre_rangees_A; i++) { // Boucle à travers les rangées de catégorie A
            salles[index_salle].sieges_par_rangee[i] *= 2; // Double le nombre de sièges par rangée pour la catégorie A
        }
    }

    sauvegarderSalles(salles, nombre_salles); // Sauvegarde les informations des salles avec les nouvelles données

    printf("Configuration de la salle %s modifiée avec succès.\n", salles[index_salle].nom); // Affiche un message de succès
}

// Fonction pour afficher l'état des salles de concert
void afficherEtatSalles(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) { // Vérifie s'il n'y a aucune salle de concert
        printf("Aucune salle de concert disponible.\n");
        return; // Sort de la fonction s'il n'y a pas de salle de concert
    }

    printf("\nSalles de concerts :\n");
    for (int i = 0; i < nombre_salles; i++) { // Boucle à travers toutes les salles de concert
        printf("Nom : %s\n", salles[i].nom); // Affiche le nom de la salle
        printf("Nombre de rangées : %d\n", salles[i].nombre_rangees_total); // Affiche le nombre total de rangées
        printf("Nombre de sièges réservés : %d\n", salles[i].sieges_reserves); // Affiche le nombre de sièges réservés

        int nombre_sieges_total = 0;
        for (int j = 0; j < salles[i].nombre_rangees_total; j++) { // Calcule le nombre total de sièges dans la salle
            nombre_sieges_total += salles[i].sieges_par_rangee[j];
        }
        float ratio_sieges_reserves = (float)salles[i].sieges_reserves / nombre_sieges_total * 100;
        printf("Ratio sièges réservés / nombre total de sièges : %.2f%%\n", ratio_sieges_reserves); // Calcule et affiche le ratio de sièges réservés

        if (salles[i].fosse) { // Vérifie si la salle a une fosse
            printf("La salle a une fosse. Les festivaliers seront debout.\n");
        } else {
            printf("La salle n'a pas de fosse.\n");
        }
        if (salles[i].concert_attribue) { // Vérifie si un concert est attribué à la salle
            printf("Concert attribué : Oui\n");
            printf("Artiste : %s\n", salles[i].artiste); // Affiche le nom de l'artiste
            printf("Heure de fin du concert : %d\n", salles[i].fin_concert); // Affiche l'heure de fin du concert
            printf("Prix catégorie A : %.2f euros\n", salles[i].prix_categorie_concert[0]); // Affiche le prix de la catégorie A
            printf("Prix catégorie B : %.2f euros\n", salles[i].prix_categorie_concert[1]); // Affiche le prix de la catégorie B
            if (possedeRangCategorieC(salles[i])) { // Vérifie si la salle possède des rangées de catégorie C
                printf("Prix catégorie C : %.2f euros\n", salles[i].prix_categorie_concert[2]); // Affiche le prix de la catégorie C
            }
        } else {
            printf("Concert attribué : Non\n");
        }

        printf("\n🪑 Plan de la salle :\n\n   🎵 SCÈNE 🥳\n");
        printf("______________________________\n");

        for (int j = 0; j < salles[i].nombre_rangees_total; j++) { // Parcourt chaque rangée de la salle
            printf("R%-2d ", j + 1); // Affiche le numéro de la rangée
            for (int k = 0; k < salles[i].sieges_par_rangee[j]; k++) { // Parcourt chaque siège de la rangée
                char seat_type = ' '; // Initialise le type de siège à vide par défaut
                if (salles[i].fosse && j < salles[i].nombre_rangees_A) { // Vérifie si la salle a une fosse et si la rangée est dans la catégorie A
                    seat_type = 'F'; // Si oui, le type de siège est F (Fosse)
                    printf(ANSI_COLOR_PURPLE "%c  " ANSI_COLOR_RESET, seat_type); // Affiche le siège en violet pour la fosse
                } else if (j < salles[i].nombre_rangees_A) { // Sinon, si la rangée est dans la catégorie A
                    seat_type = 'A'; // Le type de siège est A (Catégorie A)
                    printf(ANSI_COLOR_BLUE "%c  " ANSI_COLOR_RESET, seat_type); // Affiche le siège en bleu pour la catégorie A
                } else if (j < salles[i].nombre_rangees_A + salles[i].nombre_rangees_B) { // Sinon, si la rangée est dans la catégorie B
                    seat_type = 'B'; // Le type de siège est B (Catégorie B)
                    printf(ANSI_COLOR_GREEN "%c  " ANSI_COLOR_RESET, seat_type); // Affiche le siège en vert pour la catégorie B
                } else { // Sinon, la rangée est dans la catégorie C
                    seat_type = 'C'; // Le type de siège est C (Catégorie C)
                    printf(ANSI_COLOR_YELLOW "%c  " ANSI_COLOR_RESET, seat_type); // Affiche le siège en jaune pour la catégorie C
                }
            }
            printf("\n");
        }

        printf("\nLégende :\n");
        printf("A - Catégorie A\n");
        printf("B - Catégorie B\n");
        if (possedeRangCategorieC(salles[i])) { // Vérifie si la salle possède des rangées de catégorie C
            printf("C - Catégorie C\n");
        }
        printf("F - Fosse\n");
        printf("X - Siège réservé\n");
        printf("\n");
    }
}




// Fonction pour réserver des sièges dans une salle de concert
void reserverSiege(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) { // Vérifie s'il n'y a aucune salle de concert
        printf("Aucune salle de concert disponible.\n");
        return; // Sort de la fonction s'il n'y a pas de salle de concert
    }

    char nom_salle[MAX_PLACE_NAME];
    printf("\nRéservation de sièges :\n");
    afficherMenuConcerts(salles, nombre_salles); // Affiche le menu des concerts disponibles

    printf("Nom de la salle de concert : ");
    lireChaine(nom_salle, MAX_PLACE_NAME); // Lit le nom de la salle de concert choisie par l'utilisateur

    int index_salle = -1;
    for (int i = 0; i < nombre_salles; i++) { // Boucle à travers toutes les salles de concert
        if (strcmp(nom_salle, salles[i].nom) == 0) { // Compare le nom saisi avec les noms des salles existantes
            index_salle = i; // Si trouvé, stocke l'index de la salle
            break; // Sort de la boucle une fois la salle trouvée
        }
    }

    if (index_salle == -1 || !salles[index_salle].concert_attribue) { // Vérifie si la salle n'existe pas ou si aucun concert n'est attribué à cette salle
        printf("La salle de concert spécifiée n'existe pas ou aucun concert n'est attribué à cette salle.\n");
        return; // Sort de la fonction si la salle n'existe pas ou aucun concert n'est attribué à cette salle
    }

    int nombre_reservations;
    printf("Nombre de sièges à réserver : ");
    scanf("%d", &nombre_reservations); // Lit le nombre de sièges à réserver
    viderTampon(); // Vide le tampon d'entrée

    int verif[999][999]; // Tableau pour vérifier si un siège est déjà réservé

    int total_prix = 0;
    for (int i = 0; i < nombre_reservations; i++) { // Boucle pour chaque siège à réserver
        int rang, siege;
        printf("Rangée (1 à %d) : ", salles[index_salle].nombre_rangees_total);
        scanf("%d", &rang); // Lit le numéro de rangée choisi par l'utilisateur
        viderTampon(); // Vide le tampon d'entrée
        printf("Siège : ");
        scanf("%d", &siege); // Lit le numéro de siège choisi par l'utilisateur
        viderTampon(); // Vide le tampon d'entrée

        if (rang < 1 || rang > salles[index_salle].nombre_rangees_total || siege < 1 || siege > salles[index_salle].sieges_par_rangee[rang - 1] || verif[rang -1][siege -1]==1) {
            printf("Emplacement invalide/ou déjà réservé. Réessayez.\n"); // Vérifie si l'emplacement est invalide ou si le siège est déjà réservé
            i--;
            continue; // Recommence la boucle pour saisir un autre siège
        }
        verif[rang - 1][siege - 1] = 1; // Marque le siège comme réservé dans le tableau de vérification
        // Vérifiez et réservez le siège
        // Incrémenter le nombre de sièges réservés
        salles[index_salle].sieges_reserves++; // Incrémente le nombre de sièges réservés

        if (salles[index_salle].fosse && rang <= salles[index_salle].nombre_rangees_A) {
            total_prix += salles[index_salle].prix_categorie_concert[0]; // Ajoute le prix de la catégorie A à total_prix si la salle a une fosse et que le siège est dans la catégorie A
        } else if (rang <= salles[index_salle].nombre_rangees_A) {
            total_prix += salles[index_salle].prix_categorie_concert[0]; // Ajoute le prix de la catégorie A à total_prix si le siège est dans la catégorie A
        } else if (rang <= salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B) {
            total_prix += salles[index_salle].prix_categorie_concert[1]; // Ajoute le prix de la catégorie B à total_prix si le siège est dans la catégorie B
        } else {
            total_prix += salles[index_salle].prix_categorie_concert[2]; // Ajoute le prix de la catégorie C à total_prix si le siège est dans la catégorie C
        }
    }

    printf("Total à payer : %d euros\n", total_prix); // Affiche le total à payer

    // Sauvegarder les salles avec le nombre de sièges réservés mis à jour
    sauvegarderSalles(salles, nombre_salles);
}

// Fonction pour le mode Festivalier
void modeFestivalier(SalleConcert salles[], int nombre_salles) {
    int choix;
    do {
        printf("\nMenu du Festivalier 🎉:\n"); // Affiche le menu du mode Festivalier
        printf("1 - Réserver des sièges\n"); // Option pour réserver des sièges
        printf("2 - Quitter\n"); // Option pour quitter le mode Festivalier
        printf("Choix (choisissez entre 1 et 2) : ");
        scanf("%d", &choix); // Lit le choix de l'utilisateur
        viderTampon(); // Vide le tampon d'entrée

        switch (choix) {
            case 1:
                reserverSiege(salles, nombre_salles); // Appelle la fonction pour réserver des sièges
                break;
            case 2:
                printf("Au revoir !\n"); // Affiche un message de sortie
                break;
            default:
                printf("Choix invalide.\n"); // Affiche un message si le choix est invalide
        }
    } while (choix != 2); // Continue le menu jusqu'à ce que l'utilisateur choisisse de quitter
}

// Fonction pour le mode Manager
void modeManager(SalleConcert salles[], int *nombre_salles) {
    int choix;
    do {
        printf("\nMenu du Manager 🤓:\n"); // Affiche le menu du mode Manager
        printf("1 - Créer une nouvelle salle de concert\n"); // Option pour créer une nouvelle salle de concert
        printf("2 - Attribuer un concert à une salle existante\n"); // Option pour attribuer un concert à une salle existante
        printf("3 - Afficher l'état des salles de concerts\n"); // Option pour afficher l'état des salles de concerts
        printf("4 - Modifier une salle déjà existante\n"); // Option pour modifier une salle déjà existante
        printf("5 - Quitter\n"); // Option pour quitter le mode Manager
        printf("Choix (choisissez entre 1 et 5) : ");
        scanf("%d", &choix); // Lit le choix de l'utilisateur
        viderTampon(); // Vide le tampon d'entrée

        switch (choix) {
            case 1:
                creerSalle(salles, nombre_salles); // Appelle la fonction pour créer une nouvelle salle de concert
                break;
            case 2:
                attribuerConcert(salles, *nombre_salles); // Appelle la fonction pour attribuer un concert à une salle existante
                break;
            case 3:
                afficherEtatSalles(salles, *nombre_salles); // Appelle la fonction pour afficher l'état des salles de concerts
                break;
            case 4:
                modifierSalle(salles, *nombre_salles); // Appelle la fonction pour modifier une salle déjà existante
                break;
            case 5:
                printf("Au revoir !\n"); // Affiche un message de sortie
                break;
            default:
                printf("Choix invalide.\n"); // Affiche un message si le choix est invalide
        }
    } while (choix != 5); // Continue le menu jusqu'à ce que l'utilisateur choisisse de quitter
}

int main() {
    SalleConcert salles[MAX_SALLES]; // Déclaration d'un tableau de structures SalleConcert pour stocker les informations sur les salles de concert
    int nombre_salles = 0; // Variable pour stocker le nombre actuel de salles de concert chargées, initialisée à 0

    chargerSalles(salles, &nombre_salles); // Appel de la fonction pour charger les données des salles de concert à partir d'une source de stockage

    // Boucle principale pour interagir avec l'utilisateur
    do {
        printf("Êtes-vous un manager (saisir M/m) ou un festivalier (saisir F/f) ? (Q/q pour quitter)\n");
        char mode = getchar(); // Lire le mode saisi par l'utilisateur
        viderTampon(); // Vide le tampon d'entrée pour éviter les problèmes de lecture

        if (mode == 'M' || mode == 'm') { // Si l'utilisateur choisit le mode Manager
            printf("Mode manager activé.\n");
            modeManager(salles, &nombre_salles); // Appel de la fonction pour activer le mode Manager
        } else if (mode == 'F' || mode == 'f') { // Si l'utilisateur choisit le mode Festivalier
            printf("Mode festivalier activé.\n");
            modeFestivalier(salles, nombre_salles); // Appel de la fonction pour activer le mode Festivalier
        } else if (mode == 'Q' || mode == 'q') { // Si l'utilisateur choisit de quitter
            printf("Au revoir 👋 !\n");
            break; // Sort de la boucle principale
        } else { // Si l'utilisateur saisit une option invalide
            printf("Mode invalide.\n");
        }
    } while (1); // Boucle infinie pour continuer à afficher le menu tant que l'utilisateur ne choisit pas de quitter

    sauvegarderSalles(salles, nombre_salles); // Appel de la fonction pour sauvegarder les données des salles de concert dans une source de stockage

    return 0; // Retourne 0 pour indiquer une sortie normale du programme
}