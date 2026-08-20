#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

void ecrire_partie(partie_politic *partie) {
    vider_buffer(); // Nettoie le \n laissé par le scanf du menu

    int existe_parti = 0;//VERIFICATION DE L'EXISTE D'UN PARTI POLITIQUE
    do{
        printf("\nEntrer l'Id du parti politique : ");
        lire_chaine(partie->id_PP, sizeof(partie->id_PP));
        existe_parti = verifier_existe_parti_politic(partie->id_PP);
        if (existe_parti) {
            printf("\n>> ERREUR : Un parti politique avec l'id '%s' existe deja ! Veuillez reessayer.\n", partie->id_PP);
        }
    }while(existe_parti);

    printf("\nEntrer le nom du parti politique : ");
    lire_chaine(partie->Nom, sizeof(partie->Nom));

    printf("\nEntrer la date de creation du parti politique (JJ/MM/AAAA) : ");
    lire_chaine(partie->date_creat, sizeof(partie->date_creat));

    printf("\nEntrer la date d'inscription du parti politique (JJ/MM/AAAA) : ");
    lire_chaine(partie->date_ins, sizeof(partie->date_ins));

    //obtention de l'adresse
    char no_rue[6], rue[20], dept[30];
    printf("\nL'adresse du parti politique (No Rue, Rue, Departement) : ");
    printf("\nEntrer le No de rue (ex : 12) : ");
    lire_chaine(no_rue, sizeof(no_rue));
    printf("\nEntrer la Rue (ex : Rue Capois) : ");
    lire_chaine(rue, sizeof(rue));
    printf("\nEntrer le Departement (ex : Nord) : ");
    lire_chaine(dept, sizeof(dept));

    //assemblage de l'adresse
    sprintf(partie->Adresse, "%s, %s, %s", no_rue, rue, dept);

    printf("\nEntrer le nom du responsable du parti politique : ");
    lire_chaine(partie->Responsable, sizeof(partie->Responsable));

    printf("\nEntrer le telephone du parti politique : ");
    lire_chaine(partie->Telephone, sizeof(partie->Telephone));

    printf("\nEntrer l'email du parti politique : ");
    lire_chaine(partie->email, sizeof(partie->email));

    FILE *fichier = fopen("p_pol.dat", "ab");//enregistrement de donnees
    if (fichier == NULL) {
        printf("\nErreur d'ouverture du fichier p_pol.dat\n\n");
        return;
    }

    fwrite(partie, sizeof(partie_politic), 1, fichier);
    fclose(fichier);
    printf("\nDonnees enregistrees avec succes !\n");
}

void afficher_PP(partie_politic *parti) {
    FILE *fichier = fopen("p_pol.dat", "rb");//ouverture du fichier en mode lecture binaire
    if (fichier == NULL) {
        printf("\nAucune donnee trouvee ou erreur d'ouverture du fichier p_pol.dat\n\n");
        return;
    }

    //affichage des donnees
    printf("\n========================================================================================================================================================================\n");
    printf(" %-10s | %-20s | %-13s | %-13s | %-30s | %-20s | %-12s | %-25s\n",
           "ID_PP", "NOM", "DATE CREATION", "DATE INSCRIT", "ADRESSE", "RESPONSABLE", "TELEPHONE", "EMAIL");
    printf("========================================================================================================================================================================\n");

    while (fread(parti, sizeof(partie_politic), 1, fichier) == 1) {
        printf(" %-10s | %-20s | %-13s | %-13s | %-30s | %-20s | %-12s | %-25s\n",
               parti->id_PP, parti->Nom, parti->date_creat, parti->date_ins, parti->Adresse, parti->Responsable, parti->Telephone, parti->email);
    }
    printf("========================================================================================================================================================================\n");
    fclose(fichier);//fermeture du fichier ( liberation de l'espace memoire)
}

void modifier_PP(partie_politic *partie, char id_modif[]) {//fonction de gestion de modification de donnees pour un ID donne
    FILE *fichier_modifier = fopen("p_pol.dat", "r+b");

    if (fichier_modifier == NULL) {
        printf("\nErreur d'ouverture du fichier p_pol.dat\n");
        return;
    }

    int trouve = 0;

    //on parcours le document pour recherche l'id en question
    while (fread(partie, sizeof(partie_politic), 1, fichier_modifier) == 1) {
        if (strcasecmp(partie->id_PP, id_modif) == 0) {
            trouve = 1;
            int choix_mod = 0;

            do {//id retrouve, on demande quel champs on veut modifier
                printf("\n\n========== MODIFICATION DU PARTI POLITIQUE : %s (ID: %s) ==========\n", partie->Nom, partie->id_PP);
                printf("1. Nom\n");
                printf("2. Date de Creation\n");
                printf("3. Date d'Inscription\n");
                printf("4. Adresse\n");
                printf("5. Responsable\n");
                printf("6. Telephone\n");
                printf("7. Email\n");
                printf("8. Enregistrer les modifications et Quitter\n");
                printf("=============================================================\n");
                printf("Choix : ");

                if (scanf("%d", &choix_mod) != 1) {
                    vider_buffer();
                    continue;
                }

                vider_buffer(); // Libere le buffer avant la saisie d'un texte

                switch (choix_mod) {
                    case 1:
                        printf("Entrer le nouveau Nom : ");
                        lire_chaine(partie->Nom, sizeof(partie->Nom));
                        break;

                    case 2:
                        printf("Entrer la nouvelle Date de Creation (JJ/MM/AAAA) : ");
                        lire_chaine(partie->date_creat, sizeof(partie->date_creat));
                        break;

                    case 3:
                        printf("Entrer la nouvelle Date d'Inscription (JJ/MM/AAAA) : ");
                        lire_chaine(partie->date_ins, sizeof(partie->date_ins));
                        break;

                    case 4:
                        char no_rue[6], rue[20], dept[30];
                        printf("\nL'adresse du parti politique (No Rue, Rue, Departement) : ");
                        printf("\nEntrer le No de rue (ex : 12) : ");
                        lire_chaine(no_rue, sizeof(no_rue));
                        printf("\nEntrer la Rue (ex : Rue Capois) : ");
                        lire_chaine(rue, sizeof(rue));
                        printf("\nEntrer le Departement (ex : Nord) : ");
                        lire_chaine(dept, sizeof(dept));

                        //assemblage de l'adresse
                        sprintf(partie->Adresse, "%s, %s, %s", no_rue, rue, dept);
                        break;

                    case 5:
                        printf("Entrer le nouveau Responsable : ");
                        lire_chaine(partie->Responsable, sizeof(partie->Responsable));
                        break;

                    case 6:
                        printf("Entrer le nouveau Telephone : ");
                        lire_chaine(partie->Telephone, sizeof(partie->Telephone));
                        break;

                    case 7:
                        printf("Entrer le nouvel Email : ");
                        lire_chaine(partie->email, sizeof(partie->email));
                        break;

                    case 8:
                        printf("\nSauvegarde des modifications en cours...\n");
                        break;

                    default:
                        printf("\nChoix invalide !\n");
                        break;
                }

            } while (choix_mod != 8);

            // Re-positionner le pointeur de fichier sur le début du registre
            fseek(fichier_modifier, -((long)sizeof(partie_politic)), SEEK_CUR);//ajout des nouveaux elements de champs
            fwrite(partie, sizeof(partie_politic), 1, fichier_modifier);//ecrire et fermeture du fichier
            fflush(fichier_modifier);
            break;
        }
    }

    fclose(fichier_modifier);

    if (trouve) {
        printf("\nModification effectuee avec succes !\n\n");
    } else {
        printf("\nAucun parti politique trouve avec l'ID : %s\n\n", id_modif);
    }
}

int p_pol() {//fonction principale dans ce module qui va appeler les autres champs
    partie_politic partie;//declaration d'une nouvellle variable de ce type
    int choix = 0;
    char id_PP[10];

    printf("\n================== GESTION PARTIS POLITIQUES ==================\n");
    do {
        printf("\n1. AJOUT PARTI POLITIQUE\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER LE MENU PARTIS POLITIQUE\n\nChoix : ");//on demande ce que l'utilisateur veux faire
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }//on fait en sorte que le bufffer soit toujours vider

        switch (choix) {
            case 1:
                ecrire_partie(&partie);
                break;

            case 2:
                afficher_PP(&partie);
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id_PP);
                modifier_PP(&partie, id_PP);
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
