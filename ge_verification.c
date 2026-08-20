#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //gestion de date
#include "election.h"

// Fonction qui verifie si un NINU existe deja dans elect.dat
int verifier_existe_NINU(const char *ninu_recherche, char *bv_verifier) {
    FILE *fichier = fopen("elect.dat", "rb");
    if (fichier == NULL) {
        return 0; // Le fichier n'existe pas encore
    }

    electeurs temp;
    int trouve = 0;

    while (fread(&temp, sizeof(electeurs), 1, fichier) == 1) {
        if (strcasecmp(temp.NINU, ninu_recherche) == 0) {
            trouve = 1;
            if(bv_verifier!=NULL){
                strcpy(bv_verifier, temp.BV);
            }
            break;
        }
    }

    fclose(fichier);
    return trouve;
}
//recuperation de bureau vote
//=======================GESTION VERIFICATION DOUBLONS ID PRESIDENT BUREAU VOTE==
// Vérifie si un ID est déjà membre (Président, VP ou Secrétaire) dans B_votes.dat
int verifier_IDP(const char *ID_recherche) {
    FILE *fichier = fopen("B_votes.dat", "rb");
    if (fichier == NULL) {
        return 0; // Fichier inexistant, donc aucun doublon
    }

    bureau_vote temp;
    int trouve = 0;

    while (fread(&temp, sizeof(bureau_vote), 1, fichier) == 1) {
        // On vérifie si l'ID est déjà utilisé dans l'un des 3 postes
        if (strcasecmp(temp.Id_President, ID_recherche) == 0 ||
            strcasecmp(temp.Id_VPresident, ID_recherche) == 0 ||
            strcasecmp(temp.Id_Secret, ID_recherche) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(fichier);
    return trouve;
}


int verifier_existe_parti_politic(const char *id_parti) {
    FILE *fichier = fopen("p_pol.dat", "rb");
    if (fichier == NULL) {
        return 0; // Le fichier n'existe pas encore
    }

    partie_politic temp;
    int trouve = 0;

    while (fread(&temp, sizeof(partie_politic), 1, fichier) == 1) {
        if (strcasecmp(temp.id_PP, id_parti) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(fichier);
    return trouve;
}
//VERIFICATION DE CANDIDAT
int verifier_existe_candidat(const char *Id_candidat) {
    FILE *fichier = fopen("candid.dat", "rb");
    if (fichier == NULL) {
        return 0; // Le fichier n'existe pas encore
    }

    candidats temp;
    int trouve = 0;

    while (fread(&temp, sizeof(candidats), 1, fichier) == 1) {
        if (strcasecmp(temp.ID_candid, Id_candidat) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(fichier);
    return trouve;
}

//VERIFICATION DE CANDIDAT
int verifier_existe_vote(const char *id_vote) {
    FILE *fichier = fopen("votes.dat", "rb");
    if (fichier == NULL) {
        return 0; // Le fichier n'existe pas encore
    }

    votes temp;
    int trouve = 0;

    while (fread(&temp, sizeof(votes), 1, fichier) == 1) {
        if (strcasecmp(temp.Id_vote, id_vote) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(fichier);
    return trouve;
}

int verifier_ninu_a_deja_vote(const char *ninu_recherche) {
    FILE *fichier = fopen("votes.dat", "rb");
    if (fichier == NULL) {
        return 0; // Le fichier n'existe pas encore, donc personne n'a vote
    }

    votes v;
    while (fread(&v, sizeof(votes), 1, fichier) == 1) {
        if (strcasecmp(v.NINU, ninu_recherche) == 0) {
            fclose(fichier);
            return 1; // Le NINU a deja vote !
        }
    }

    fclose(fichier);
    return 0; // Le NINU n'a pas encore vote
}


// Fonction de recherche dans B_votes.dat
int verifier_existe_BV(const char *id_bv) {
    FILE *fichier = fopen("B_votes.dat", "rb");
    if (fichier == NULL) {
        return 0;
    }

    bureau_vote bv;
    int trouve = 0;

    while (fread(&bv, sizeof(bureau_vote), 1, fichier) == 1) {
        if (strcasecmp(bv.Id_BV, id_bv) == 0) {
            trouve = 1;
            break;
        }
    }

    fclose(fichier);
    return trouve;
}

//gestion de date
void obtenir_date(char *date){
    time_t temps = time(NULL);
    struct tm tm_info = *localtime(&temps);

    sprintf(date, "%02d/%02d/%04d", tm_info.tm_mday,tm_info.tm_mon+1 , tm_info.tm_year +1900);
}

// Fonction de lecture securisee pour chaines de caracteres
void lire_chaine(char *chaine, int taille) {
    if (fgets(chaine, taille, stdin) != NULL) {
        size_t len = strlen(chaine);
        if (len > 0 && chaine[len - 1] == '\n') {
            chaine[len - 1] = '\0'; // Supprime le saut de ligne
        } else {
            // Saisie trop longue : vider le surplus restant dans le flux stdin
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}
