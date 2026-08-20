#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include <string.h>

void ecrire_bureau(bureau_vote *e) {
    vider_buffer();

    int existe_bureau = 0;

    // --- 1. ID BUREAU DE VOTE ---
    do {
        printf("\nEntrer l'Id du bureau de vote : ");
        lire_chaine(e->Id_BV, sizeof(e->Id_BV));

        existe_bureau = verifier_existe_BV(e->Id_BV);
        if (existe_bureau) {
            printf("\n>> ERREUR : Un bureau de vote avec l'id '%s' existe deja ! Veuillez reessayer.\n", e->Id_BV);
        }
    } while (existe_bureau);

    // --- 2. ADRESSE ---
    char no_rue[6], rue[20], dept[30];
    printf("\nL'adresse du bureau de vote (No Rue, Rue, Departement) : ");
    printf("\nEntrer le No de rue (ex : 12) : ");
    lire_chaine(no_rue, sizeof(no_rue));
    printf("\nEntrer la Rue (ex : Rue Capois) : ");
    lire_chaine(rue, sizeof(rue));
    printf("\nEntrer le Departement (ex : Nord) : ");
    lire_chaine(dept, sizeof(dept));

    //assemblage de l'adresse
    sprintf(e->Adresse_BV, "%s, %s, %s", no_rue, rue, dept);

    // --- 3. PRESIDENT ---
    do {
        printf("\nEntrer l'Id du president du bureau de vote : ");
        lire_chaine(e->Id_President, sizeof(e->Id_President));

        existe_bureau = verifier_IDP(e->Id_President);
        if (existe_bureau) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e->Id_President);
        }
    } while (existe_bureau);

    // --- 4. VICE PRESIDENT ---
    do {
        printf("\nEntrer l'Id du Vice president du bureau de vote : ");
        lire_chaine(e->Id_VPresident, sizeof(e->Id_VPresident));

        // Verification dans le fichier
        existe_bureau = verifier_IDP(e->Id_VPresident);

        // Verification avec la saisie en cours
        if (strcasecmp(e->Id_VPresident, e->Id_President) == 0) {
            printf("\n>> ERREUR : Le Vice-President ne peut pas avoir le meme ID que le President !\n");
            existe_bureau = 1;
        } else if (existe_bureau) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e->Id_VPresident);
        }
    } while (existe_bureau);

    // --- 5. SECRETAIRE ---
    do {
        printf("\nEntrer l'Id du Secretaire du bureau de vote : ");
         lire_chaine(e->Id_Secret, sizeof(e->Id_Secret));

        // Verification dans le fichier
        existe_bureau = verifier_IDP(e->Id_Secret);

        // Verification avec la saisie en cours
        if (strcasecmp(e->Id_Secret, e->Id_President) == 0 || strcasecmp(e->Id_Secret, e->Id_VPresident) == 0) {
            printf("\n>> ERREUR : Le Secretaire ne peut pas etre la meme personne que le President ou le Vice-President !\n");
            existe_bureau = 1;
        } else if (existe_bureau) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e->Id_Secret);
        }
    } while (existe_bureau);

    // --- SAUVEGARDE ---
    FILE *fichier = fopen("B_votes.dat", "ab");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier\n\n");
        return;
    }
    fwrite(e, sizeof(bureau_vote), 1, fichier);
    fclose(fichier);
    printf("\nDonnees enregistrees avec succes !\n");
}

void afficher_BV(bureau_vote *e){
    FILE *fichier = fopen("B_votes.dat", "rb");//ouverture en mode lecture binaire
    if(fichier == NULL){
        printf("\nOuverture du fichier B_votes.dat echouer\n\n");
        return;
    }
    printf("\n=================================================================================================================\n");
    printf(" %-15s | %-35s | %-15s | %-15s | %-15s\n", "ID BUREAU", "ADRESSE", "ID PRESIDENT", "ID V-PRESIDENT", "ID SECRETAIRE");
    printf("\n=================================================================================================================\n");

    while(fread(e, sizeof(bureau_vote), 1, fichier)==1){
        printf(" %-15s | %-35s | %-15s | %-15s | %-15s\n", e->Id_BV, e->Adresse_BV, e->Id_President, e->Id_VPresident, e->Id_Secret);
    }
    printf("\n=================================================================================================================\n");
    fclose(fichier);

}


void modifier_BV(bureau_vote *e, char id_modif[]){
    FILE *fichier_modifier = fopen("B_votes.dat", "r+b");//en mode lecture, ecriture en mode binaire

    if(fichier_modifier == NULL){
        printf("Erreur d'ouverture du fichier data\n");
        return;
    }
    int trouve = 0;

    while(fread(e,sizeof(bureau_vote),1, fichier_modifier) == 1){//parcourir la base de donnees
       if (strcasecmp(e->Id_BV, id_modif) == 0){//fonction qui compare l'id entree et ce qu'on a dans la base de donnees
             trouve = 1;
             int choix_mod = 0;
           do {//nous pourrons modifier seulement l'adresse du bureau de vote
                printf("\n\n========== MODIFICATION DU BUREAU DE VOTE : (ID: %s) ==========\n", e->Id_BV);
                printf("1. Adresse du bureau de vote\n");
                printf("2. Enregistrer les modifications et Quitter\n");
                printf("=============================================================\n");
                printf("Choix : ");

                if (scanf("%d", &choix_mod) != 1) {
                    vider_buffer();
                    continue;
                }

                vider_buffer();//on libere le buffer avant d'ecrire afin de de pas sauvegarder des donnees que l'on ne desire pas

                switch (choix_mod) {//modification en fonction du donnees entrer par l'utilisateur
                    case 1:
                        char no_rue[6], rue[20], dept[30];
                        printf("\nL'adresse du bureau de vote (No Rue, Rue, Departement) : ");
                        printf("\nEntrer le No de rue (ex : 12) : ");
                        lire_chaine(no_rue, sizeof(no_rue));
                        printf("\nEntrer la Rue (ex : Rue Capois) : ");
                        lire_chaine(rue, sizeof(rue));
                        printf("\nEntrer le Departement (ex : Nord) : ");
                        lire_chaine(dept, sizeof(dept));

                        //assemblage de l'adresse
                        sprintf(e->Adresse_BV, "%s, %s, %s", no_rue, rue, dept);
                        break;

                    case 2:
                        printf("\nSauvegarde des modifications en cours...\n");
                        break;

                    default:
                        printf("\nChoix invalide !\n");
                        break;
                }

            } while (choix_mod != 2);

            fseek(fichier_modifier, -((long)sizeof(bureau_vote)), SEEK_CUR);

            fwrite(e, sizeof(bureau_vote), 1, fichier_modifier);

            fflush(fichier_modifier);
            break;
       }

    }
    fclose(fichier_modifier);

    if (trouve) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\nAucun bureau de vote trouve avec l'ID : %s\n", id_modif);
    }
}

int ge_BV() {
    bureau_vote e;
    int choix = 0;
    char id[10];

    printf("\n\n================== GESTION BUREAU VOTE ==================\n\n");
    do {
        printf("\n1. AJOUT BUREAU VOTE\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU BUREAU DE VOTE\n\nChoix : ");

        // Si la saisie n'est pas un nombre correct
        if (scanf("%d", &choix) != 1) {
            vider_buffer(); // Nettoie le buffer si l'utilisateur a tapé du texte ou des caractères parasites
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_bureau(&e);
                break;

            case 2:
                afficher_BV(&e);
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id);
                vider_buffer(); // Toujours vider le \n laissé par le scanf de l'ID
                modifier_BV(&e, id);
                break;

            case 4:
                printf("\nRetour au menu principal...\n");
                break;

            default:
                printf("\nOption invalide !\n");
                break;
        }
    } while (choix != 4);

    return 0;
}
