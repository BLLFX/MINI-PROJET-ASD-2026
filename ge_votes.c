#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>//pour obtenir la date
#include "election.h"

void ecrire_votes(votes *vote_candi){

    vider_buffer();
    // Saisie de l'ID du vote
    int existe_vote = 0;
    do {
        printf("\nEntrer l'ID du vote : ");
        fgets(vote_candi->Id_vote, sizeof(vote_candi->Id_vote), stdin);
        vote_candi->Id_vote[strcspn(vote_candi->Id_vote, "\r\n")] = '\0';

        existe_vote = verifier_existe_vote(vote_candi->Id_vote);
        if (existe_vote) {
            printf("\n>> ERREUR : Un vote avec l'id '%s' est deja enregistre ! Veuillez reessayer.\n", vote_candi->Id_vote);
        }
    } while(existe_vote);

    obtenir_date(vote_candi->Date_vote);
    printf("\nLa date du vote generee automatiquement : %s\n", vote_candi->Date_vote);

    // Saisie du NINU avec verifications
    int existe_ninu = 0;
    int ninu_deja_present = 0;
    do {
        printf("\nEntrer le NINU de l'electeur : ");
        fgets(vote_candi->NINU, sizeof(vote_candi->NINU), stdin);
        vote_candi->NINU[strcspn(vote_candi->NINU, "\r\n")] = '\0';

        // 1. Verifie si l'electeur existe dans elect.dat et recupere son BV
        existe_ninu = verifier_existe_NINU(vote_candi->NINU, vote_candi->BV);

        // 2. Verifie si le NINU est deja present dans votes.dat
        ninu_deja_present = verifier_ninu_a_deja_vote(vote_candi->NINU);

        if (!existe_ninu) {
            printf("\n>> ERREUR : L'electeur avec le NINU '%s' n'existe pas ! Veuillez reessayer.\n", vote_candi->NINU);
        }
        else if (ninu_deja_present) {
            printf("\n>> ERREUR : L'electeur avec le NINU '%s' a DEJA VOTE ! Veuillez reessayer.\n", vote_candi->NINU);
        }
        else {
            printf("\n>> Bureau de vote associe : %s\n", vote_candi->BV);
        }
    // Repeter TANT QUE le NINU n'existe pas OU qu'il a DEJA vote
    } while (!existe_ninu || ninu_deja_present);

    // Saisie de l'ID du candidat (avec gestion du vote blanc "0")
    int existe_candidat = 0;
    do {
        printf("\nEntrer l'ID du candidat (ou '0' pour vote blanc) : ");
        fgets(vote_candi->id_candid, sizeof(vote_candi->id_candid), stdin);
        vote_candi->id_candid[strcspn(vote_candi->id_candid, "\r\n")] = '\0';

        if (strcmp(vote_candi->id_candid, "0") == 0) {
            existe_candidat = 1; // Vote blanc valide
        } else {
            existe_candidat = verifier_existe_candidat(vote_candi->id_candid);
            if (!existe_candidat) {
                printf("\n>> ERREUR : Le candidat avec l'id '%s' n'existe pas ! Veuillez reessayer.\n", vote_candi->id_candid);
            }
        }
    } while (!existe_candidat);

    // Enregistrement
    FILE *fichier = fopen("votes.dat", "ab");
    if (fichier == NULL) {
        printf("\n>> ERREUR : Impossible d'ouvrir le fichier votes.dat\n");
        return;
    }

    fwrite(vote_candi, sizeof(votes), 1, fichier);
    fclose(fichier);
    printf("\n>> Vote enregistre avec succes !\n");
}
void afficher_votes(votes *vote_candi){
    FILE *fichier = fopen("votes.dat", "rb");//ouverture en mode lecture binaire
    if(fichier == NULL){
        printf("\nOuverture du fichier votes.dat echouer\n\n");
        return;
    }
    printf("\n===============================================================\n");
    printf(" %-7s | %-13s | %-7s | %-11s | %-7s\n", "ID-VOTE", "DATE VOTE", "B-V", "NINU", "ID-CANDID");
    printf("\n===============================================================\n");

    while(fread(vote_candi, sizeof(votes), 1, fichier)==1){
        printf(" %-7s | %-13s | %-7s | %-11s | %-7s\n",vote_candi->Id_vote, vote_candi->Date_vote, vote_candi->BV, vote_candi->NINU, vote_candi->id_candid);
    }
    printf("\n===============================================================\n");
    fclose(fichier);
}

int ge_vote(){
    votes vote_candi;
    int choix = 0;

     printf("\n\n================== GESTION VOTES ==================\n\n");
     do {
        printf("\n1. AJOUT VOTES\n2. AFFICHAGE\n3. QUITTER MENU VOTES\n\nChoix : ");

        // Si la saisie n'est pas un nombre correct
        if (scanf("%d", &choix) != 1) {
            vider_buffer(); // Nettoie le buffer si l'utilisateur a tapé du texte ou des caractères parasites
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_votes(&vote_candi);
                break;

            case 2:
                afficher_votes(&vote_candi);
                break;

            case 3:
                printf("\nRetour au menu principal...\n");
                break;

            default:
                printf("\nOption invalide !\n");
                break;
        }
    } while (choix != 3);
    return 0;
}
