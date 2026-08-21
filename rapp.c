#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
//structure local pour la gestion de statistique de vote
typedef struct {
    char id_candid[15];
    char nom[20];
    char prenom[20];
    int total_votes;
    float pourcentage;
} stat_candidat;

int ge_rapp(){
    liste_electeur();
    printf("\n\n");
    liste_candidat();
    printf("\n\n");
    liste_bureau_vote();
    printf("\n\n");
    resultat();
    return 0;
}

// Extrait la partie "Departement" (situee apres la derniere virgule)
void extraire_departement(const char *adresse_complete, char *dept_dest) {
    const char *virgule = strrchr(adresse_complete, ',');//car chaque sous champs est separe par une virgule
    if (virgule != NULL) {//on va lire les caractere jusqu'a ce qu'on retrouve une virgule
        virgule++;
        while (*virgule == ' ') virgule++;
        strcpy(dept_dest, virgule);
    } else {
        strcpy(dept_dest, adresse_complete);
    }
}

//affichage des electeurs par departement et par bureau de votes
void liste_electeur(void){
    FILE *electeur = fopen("elect.dat","rb"); //ouverture format lecture binaire
    FILE *bureau = fopen("B_votes.dat", "rb");
    if(bureau == NULL || electeur == NULL){
        printf("\n>> Ouverture de fichier elect.dat ou B_votes.dat de la base de donnees echoues\n");
        if(electeur)
            fclose(electeur);
        if(bureau)
            fclose(bureau);
        return;
    }
    bureau_vote bv;
    electeurs elec;
    char depart[30];
    printf("\n=========================================================================================================\n");
    printf("                                  LISTE DES ELECTEURS PAR DEPARTEMENT ET BV                              \n");
    printf("=========================================================================================================\n");

    while (fread(&bv, sizeof(bureau_vote), 1, bureau) == 1) {
        extraire_departement(bv.Adresse_BV, depart);//on extrait le departement dans le champs adresse

        printf("\n\n\nDEPARTEMENT : %-15s | ID-Bureau de vote : %-10s\n", depart, bv.Id_BV);
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf(" %-15s | %-20s | %-20s | %-30s\n", "NINU", "NOM", "PRENOM", "ADRESSE");
        printf("---------------------------------------------------------------------------------------------------------\n");

        rewind(electeur);//on remet au debut du fichier
        int trouve = 0;
        while (fread(&elec, sizeof(electeurs), 1, electeur) == 1) {//on parcours le fichier contenant les electeur
            if (strcasecmp(elec.BV, bv.Id_BV) == 0) {//on cherche les electeurs qui sont associer a ce bureau de vote et departement
                printf(" %-15s | %-20s | %-20s | %-30s\n", elec.NINU, elec.nom, elec.prenom, elec.Adresse);
                trouve = 1;
            }
        }
        if (!trouve) printf(" (Aucun electeur trouve pour ce bureau de vote)\n");//si on ne trouve pas des electeurs associe a ce bureau de vote
    }

    fclose(bureau);
    fclose(electeur);
    printf("\n\n=========================================================================================================\n\n");
}

void liste_candidat(void){
    FILE *file_candidat = fopen("candid.dat", "rb");
    if(file_candidat == NULL){
        printf("\n>> ERREUR : Echec d'ouverture du fichier candid.dat \n\n");
        fclose(file_candidat);
        return;
    }
    candidats candi;

    printf("\n===========================================================================================\n");
    printf("                           LISTE DES CANDIDATS A LA PRESIDENCE                         \n");
    printf("===========================================================================================\n");
    printf(" %-10s | %-20s | %-20s | %-25s\n", "ID CANDID", "NOM", "PRENOM", "TYPE / PARTI");
    printf("-------------------------------------------------------------------------------------------\n");

    while (fread(&candi, sizeof(candidats), 1, file_candidat) == 1) {
        char type_str[30];
        if (strcmp(candi.type, "0") == 0) {//on avait affecter au candidat independant la valeur 0
            strcpy(type_str, "INDEPENDANT");
        } else {
            sprintf(type_str, "PARTI ID: %s", candi.type);
        }
        printf(" %-10s | %-20s | %-20s | %-25s\n", candi.ID_candid, candi.Nom, candi.Prenom, type_str);
    }
    fclose(file_candidat);
    printf("===========================================================================================\n\n");
}

void liste_bureau_vote(void){
    FILE *fichier_bureau = fopen("B_votes.dat", "rb");
    if(fichier_bureau == NULL){
        printf("\n>> ERREUR : fichier B_votes.dat introuvable\n");
        fclose(fichier_bureau);
        return;
    }
    bureau_vote donnees_bureau;
    printf("\n===========================================================================================================================\n");
    printf("                                                LISTE DES BUREAUX DE VOTE                                                  \n");
    printf("===========================================================================================================================\n");
    printf(" %-10s | %-35s | %-20s | %-20s | %-20s\n", "ID BV", "ADRESSE BV", "PRESIDENT (ID)", "VICE-PRES. (ID)", "SECRETAIRE (ID)");
    printf("---------------------------------------------------------------------------------------------------------------------------\n");

    while (fread(&donnees_bureau, sizeof(bureau_vote), 1, fichier_bureau) == 1) {
        printf(" %-10s | %-35s | %-20s | %-20s | %-20s\n",
               donnees_bureau.Id_BV, donnees_bureau.Adresse_BV, donnees_bureau.Id_President, donnees_bureau.Id_VPresident, donnees_bureau.Id_Secret);
    }

    fclose(fichier_bureau);
    printf("===========================================================================================================================\n\n");
}

void resultat(void){
    FILE *fichier_vote = fopen("votes.dat", "rb");
    FILE *fichier_candidat = fopen("candid.dat", "rb");
    FILE *fichier_bureau_vote = fopen("B_votes.dat", "rb");

    if (fichier_bureau_vote == NULL || fichier_candidat == NULL || fichier_vote == NULL) {
        printf(">> ERREUR : Echec d'ouverture des fichiers\n");
        if (fichier_bureau_vote) fclose(fichier_bureau_vote);
        if (fichier_candidat) fclose(fichier_candidat);
        if (fichier_vote) fclose(fichier_vote);
        return;
    }

    candidats liste_candidats[100];
    int nombre_candidat = 0;
    while (fread(&liste_candidats[nombre_candidat], sizeof(candidats), 1, fichier_candidat) == 1) {
        nombre_candidat++;
    }

    votes liste_vote[1000];
    int nombre_vote = 0;
    while (fread(&liste_vote[nombre_vote], sizeof(votes), 1, fichier_vote) == 1) {
        nombre_vote++;
    }

    // --- 1. Initialisation de la structure pour le Cumul National ---
    stat_candidat stats_nationales[100];
    for (int i = 0; i < nombre_candidat; i++) {
        strcpy(stats_nationales[i].id_candid, liste_candidats[i].ID_candid);
        strcpy(stats_nationales[i].nom, liste_candidats[i].Nom);
        strcpy(stats_nationales[i].prenom, liste_candidats[i].Prenom);
        stats_nationales[i].total_votes = 0;
        stats_nationales[i].pourcentage = 0.0;
    }

    printf("\n===============================================================================================\n");
    printf("                                    RESULTATS DU SCRUTIN                                       \n");
    printf("===============================================================================================\n");

    int total_global_non_blancs = 0;
    int total_global_blancs = 0;

    bureau_vote bureau;
    char departement_actuel[30];

    while (fread(&bureau, sizeof(bureau_vote), 1, fichier_bureau_vote) == 1) {
        extraire_departement(bureau.Adresse_BV, departement_actuel);

        int votes_blancs_dept = 0;
        int votes_non_blancs_dept = 0;

        stat_candidat stats_dept[100];
        for (int i = 0; i < nombre_candidat; i++) {
            strcpy(stats_dept[i].id_candid, liste_candidats[i].ID_candid);
            strcpy(stats_dept[i].nom, liste_candidats[i].Nom);
            strcpy(stats_dept[i].prenom, liste_candidats[i].Prenom);
            stats_dept[i].total_votes = 0;
            stats_dept[i].pourcentage = 0.0;
        }

        // Compter les votes du bureau courant
        for (int i = 0; i < nombre_vote; i++) {
            if (strcasecmp(liste_vote[i].BV, bureau.Id_BV) == 0) {
                if (strcasecmp(liste_vote[i].id_candid, "0") == 0) {
                    votes_blancs_dept++;
                } else {
                    votes_non_blancs_dept++;
                    for (int j = 0; j < nombre_candidat; j++) {
                        if (strcasecmp(liste_vote[i].id_candid, stats_dept[j].id_candid) == 0) {
                            stats_dept[j].total_votes++;
                            // --- Cumul egalement sur le tableau National ---
                            stats_nationales[j].total_votes++;
                            break;
                        }
                    }
                }
            }
        }

        int total_dept = votes_non_blancs_dept + votes_blancs_dept;
        total_global_blancs += votes_blancs_dept;
        total_global_non_blancs += votes_non_blancs_dept;

        // Pourcentages au niveau du Département / BV
        for (int i = 0; i < nombre_candidat; i++) {
            if (total_dept > 0) {
                stats_dept[i].pourcentage = ((float)stats_dept[i].total_votes / total_dept) * 100.0;
            }
        }

        // Tri local par BV / Département
        for (int i = 0; i < nombre_candidat - 1; i++) {
            for (int j = 0; j < nombre_candidat - i - 1; j++) {
                if (stats_dept[j].pourcentage < stats_dept[j + 1].pourcentage) {
                    stat_candidat temp = stats_dept[j];
                    stats_dept[j] = stats_dept[j + 1];
                    stats_dept[j + 1] = temp;
                }
            }
        }

        // Affichage par Département / BV
        printf("\n\nDEPARTEMENT : %-15s (BV: %s)\n", departement_actuel, bureau.Id_BV);
        printf("-----------------------------------------------------------------------------------------------\n");
        printf(" %-20s | %-20s | %-12s | %-12s\n", "NOM CANDIDAT", "PRENOM", "VOTES OBTENUS", "POURCENTAGE");
        printf("-----------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < nombre_candidat; i++) {
            printf(" %-20s | %-20s | %-13d | %-6.2f %%\n",
                   stats_dept[i].nom, stats_dept[i].prenom, stats_dept[i].total_votes, stats_dept[i].pourcentage);
        }
        printf("-----------------------------------------------------------------------------------------------\n");
        printf(" Total votes non-blancs : %d | Total votes blancs : %d | Total General : %d\n",
               votes_non_blancs_dept, votes_blancs_dept, total_dept);
        printf("-----------------------------------------------------------------------------------------------\n");
    }

    // --- 2. Calcul des pourcentages et Tri pour le Résumé National ---
    int total_national_general = total_global_non_blancs + total_global_blancs;

    for (int i = 0; i < nombre_candidat; i++) {
        if (total_national_general > 0) {
            stats_nationales[i].pourcentage = ((float)stats_nationales[i].total_votes / total_national_general) * 100.0;
        }
    }

    for (int i = 0; i < nombre_candidat - 1; i++) {
        for (int j = 0; j < nombre_candidat - i - 1; j++) {
            if (stats_nationales[j].pourcentage < stats_nationales[j + 1].pourcentage) {
                stat_candidat temp = stats_nationales[j];
                stats_nationales[j] = stats_nationales[j + 1];
                stats_nationales[j + 1] = temp;
            }
        }
    }

    // --- 3. Affichage du Résumé Global National ---
    printf("\n===============================================================================================\n");
    printf("                               RESUME GLOBAL POUR TOUT LE PAYS                                 \n");
    printf("===============================================================================================\n");
    printf(" %-20s | %-20s | %-12s | %-12s\n", "NOM CANDIDAT", "PRENOM", "VOTES NATIONAUX", "POURCENTAGE");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < nombre_candidat; i++) {
        printf(" %-20s | %-20s | %-15d | %-6.2f %%\n",
               stats_nationales[i].nom, stats_nationales[i].prenom, stats_nationales[i].total_votes, stats_nationales[i].pourcentage);
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    printf(" Total National Votes Non Blancs : %d\n", total_global_non_blancs);
    printf(" Total National Votes Blancs     : %d\n", total_global_blancs);
    printf(" Total National General          : %d\n", total_national_general);
    printf("===============================================================================================\n\n");

    fclose(fichier_bureau_vote);
    fclose(fichier_candidat);
    fclose(fichier_vote);
}
