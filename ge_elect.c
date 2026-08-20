#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

// Fonction qui verifie si un NINU existe deja dans elect.dat

void ecrire_electeur(electeurs *elec) { // Fonction qui gere l'inscription de l'electeur
    vider_buffer();

    // --- BOUCLE DE VERIFICATION DU NINU (UNICITE) ---
    int existe_ninu = 0;
    do {
        printf("\nEntrer le NINU de l'electeur : ");
        lire_chaine(elec->NINU, sizeof(elec->NINU));

        existe_ninu = verifier_existe_NINU(elec->NINU, NULL);
        if (existe_ninu) {
            printf("\n>> ERREUR : Un electeur avec le NINU '%s' existe deja ! Veuillez reessayer.\n", elec->NINU);
        }
    } while (existe_ninu);

    printf("\nEntrer le nom de l'electeur : ");
    lire_chaine(elec->nom, sizeof(elec->nom));

    printf("\nEntrer le prenom de l'electeur : ");
    lire_chaine(elec->prenom, sizeof(elec->prenom));

    char no_rue[6], rue[20], dept[30];
    printf("\nL'adresse de l'electeur (No Rue, Rue, Departement) : ");
    printf("\nEntrer le No de rue (ex : 12) : ");
    lire_chaine(no_rue, sizeof(no_rue));
    printf("\nEntrer la Rue (ex : Rue Capois) : ");
    lire_chaine(rue, sizeof(rue));
    printf("\nEntrer le Departement (ex : Nord) : ");
    lire_chaine(dept, sizeof(dept));

    sprintf(elec->Adresse, "%s, %s, %s", no_rue, rue, dept);
    // --- BOUCLE DE VERIFICATION DE L'ELECTEUR ---
    int existe_bv = 0;
    do {
        printf("\nEntrer l'Id du bureau de votes assigne a l'electeur : ");
        lire_chaine(elec->BV, sizeof(elec->BV));

        existe_bv = verifier_existe_BV(elec->BV); // Verification de l'ID entre
        if (!existe_bv) {
            printf(">> ERREUR : Le bureau de vote '%s' n'existe pas ! Veuillez entrer un ID valide.\n", elec->BV);
        }
    } while (!existe_bv);

    FILE *fichier = fopen("elect.dat", "ab");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier elect.dat\n\n");
        return;
    }

    fwrite(elec, sizeof(electeurs), 1, fichier); // Ecriture dans le fichier electeurs
    fclose(fichier); // Fermeture du fichier
    printf("\nDonnees enregistrees avec succes !\n");
}

void affichage_electeur(electeurs *elect) { // Affichage des electeurs
    FILE *fichier = fopen("elect.dat", "rb");
    if (fichier == NULL) {
        printf("\nOuverture du fichier elect.dat echouee\n\n");
        return;
    }

    printf("\n==========================================================================================================\n");
    printf(" %-10s | %-20s | %-20s | %-30s | %-10s\n", "NINU", "NOM", "PRENOM", "ADRESSE", "ID BUREAU");
    printf("\n==========================================================================================================\n");

    while (fread(elect, sizeof(electeurs), 1, fichier) == 1) {
        printf(" %-10s | %-20s | %-20s | %-30s | %-10s\n", elect->NINU, elect->nom, elect->prenom, elect->Adresse, elect->BV);
    }
    printf("\n==========================================================================================================\n");
    fclose(fichier);
}

void modifier_electeur(electeurs *elect, char id_modif[]) { // Mise a jour d'electeur
    FILE *fichier_modifier = fopen("elect.dat", "r+b");

    if (fichier_modifier == NULL) {
        printf("\nErreur d'ouverture du fichier\n");
        return;
    }
    int trouve = 0;

    while (fread(elect, sizeof(electeurs), 1, fichier_modifier) == 1) {
        if (strcasecmp(elect->NINU, id_modif) == 0) {
            trouve = 1;
            int choix_mod = 0;
            do {
                printf("\n\n========== MODIFICATION D'INFORMATION DE L'ELECTEUR : (ID: %s) ==========\n", elect->NINU);
                printf("1. Nom de l'electeur\n");
                printf("2. Prenom de l'electeur\n");
                printf("3. Adresse de l'electeur\n");
                printf("4. Id du Bureau de vote assigne a l'electeur\n");
                printf("5. Enregistrer les modifications et Quitter\n");
                printf("=============================================================\n");
                printf("Choix : ");

                if (scanf("%d", &choix_mod) != 1) {
                    vider_buffer();
                    continue;
                }

                vider_buffer();

                switch (choix_mod) {
                    case 1:
                        printf("Entrer le nouveau nom de l'electeur : ");
                        lire_chaine(elect->nom, sizeof(elect->nom));

                        break;

                    case 2:
                        printf("Entrer le nouveau prenom de l'electeur : ");
                        lire_chaine(elect->prenom, sizeof(elect->prenom));

                        break;

                    case 3:
                       char no_rue[6], rue[20], dept[30];
                        printf("\nL'adresse de l'electeur (No Rue, Rue, Departement) : ");
                        printf("\nEntrer le No de rue (ex : 12) : ");
                        lire_chaine(no_rue, sizeof(no_rue));
                        printf("\nEntrer la Rue (ex : Rue Capois) : ");
                        lire_chaine(rue, sizeof(rue));
                        printf("\nEntrer le Departement (ex : Nord) : ");
                        lire_chaine(dept, sizeof(dept));

                        sprintf(elect->Adresse, "%s, %s, %s", no_rue, rue, dept);
                        break;

                    case 4: {
                        int existe = 0;
                        do {
                            printf("Entrer le nouveau Id assigne a l'electeur : ");
                            lire_chaine(elect->BV, sizeof(elect->BV));

                            existe = verifier_existe_BV(elect->BV);
                            if (!existe) {
                                printf(">> ERREUR : Le bureau de vote '%s' n'existe pas ! Veuillez entrer un ID valide.\n", elect->BV);
                            }
                        } while (!existe);
                        break;
                    }

                    case 5:
                        printf("\nSauvegarde des modifications en cours...\n");
                        break;

                    default:
                        printf("\nChoix invalide !\n");
                        break;
                }

            } while (choix_mod != 5);

            fseek(fichier_modifier, -((long)sizeof(electeurs)), SEEK_CUR); // Positionnement sur l'enregistrement courant
            fwrite(elect, sizeof(electeurs), 1, fichier_modifier); // Ecriture de la mise a jour
            fflush(fichier_modifier);
            break;
        }
    }
    fclose(fichier_modifier);

    if (trouve) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\nAucun electeur trouve avec l'ID : %s\n", id_modif);
    }
}

int ge_elect() {
    electeurs elect;
    int choix = 0;
    char id[15];

    printf("\n\n================== GESTION ELECTEUR ==================\n\n");

    do {
        printf("\n1. AJOUT ELECTEUR\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU ELECTEURS\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_electeur(&elect);
                break;

            case 2:
                affichage_electeur(&elect);
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%14s", id);
                vider_buffer();
                modifier_electeur(&elect, id);
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
