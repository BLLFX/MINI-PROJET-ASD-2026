#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

void ecrire_candidat(candidats *candi) {
    vider_buffer();

     // 1. Saisie de l'ID Candidat
    int existe_candidat = 0;
    do{
        printf("\nEntrer l'ID du candidat : ");
        lire_chaine(candi->ID_candid, sizeof(candi->ID_candid));

    existe_candidat = verifier_existe_candidat(candi->ID_candid);
    if (existe_candidat) {
            printf("\n>> ERREUR : Un candidat avec l'id '%s' est deja enregistre ! Veuillez reessayer.\n", candi->ID_candid);
        }
    }while(existe_candidat);

    // 2. Nom et Prenom
    printf("\nEntrer le nom du candidat : ");
    lire_chaine(candi->Nom, sizeof(candi->Nom));

    printf("\nEntrer le prenom du candidat : ");
    lire_chaine(candi->Prenom, sizeof(candi->Prenom));

    // 3. Date de Naissance
    printf("\nEntrer la date de naissance (JJ/MM/AAAA) : ");
    lire_chaine(candi->Dat_Nais, sizeof(candi->Dat_Nais));

    // 4. Adresse, Email, Telephone

    //obtention de l'adresse
    char no_rue[6], rue[20], dept[30];
    printf("\nL'adresse du candidat (No Rue, Rue, Departement) : ");
    printf("\nEntrer le No de rue (ex : 12) : ");
    lire_chaine(no_rue, sizeof(no_rue));
    printf("\nEntrer la Rue (ex : Rue Capois) : ");
    lire_chaine(rue, sizeof(rue));
    printf("\nEntrer le Departement (ex : Nord) : ");
    lire_chaine(dept, sizeof(dept));

    //assemblage de l'adresse
    sprintf(candi->Adresse, "%s, %s, %s", no_rue, rue, dept);


    printf("\nEntrer l'email : ");
    lire_chaine(candi->email, sizeof(candi->email));

    printf("\nEntrer le telephone : ");
    lire_chaine(candi->tel, sizeof(candi->tel));

    int choix = 0;
    do {
        printf("\nEntrer le type de candidat :\n\n1. Independant\n2. Sous banniere d'un parti politique\nChoix : ");
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        if (choix == 1) {
            // CORRECTION ICI : Utiliser strcpy au lieu de '='
            strcpy(candi->type, "0");
            printf("\n>> Candidature enregistree sous le statut : INDEPENDANT (Type 0)\n");
        }
        else if (choix == 2) {
            int valide = 0;
            do {
                printf("\nEntrer l'ID du Parti Politique : ");
                lire_chaine(candi->type, sizeof(candi->type));

                valide = verifier_existe_parti_politic(candi->type);
                if (!valide) {
                    printf(">> ERREUR : Le parti avec l'ID '%s' n'existe pas ! Veuillez reessayer.\n", candi->type);
                }
            } while (!valide);
        }
        else {
            printf(">> Choix invalide ! Entrez 1 pour Independant ou 2 pour Parti.\n");
        }
    } while (choix != 1 && choix != 2);

    // 6. Date d'inscription
    printf("\nEntrer la date d'inscription (JJ/MM/AAAA) : ");
    lire_chaine(candi->Date_ins, sizeof(candi->Date_ins));

    // 7. Enregistrement dans candid.dat
    FILE *fichier = fopen("candid.dat", "ab");
    if (fichier == NULL) {
        printf("\nErreur d'ouverture du fichier candid.dat\n");
        return;
    }

    fwrite(candi, sizeof(candidats), 1, fichier);
    fclose(fichier);
    printf("\nCandidat enregistre avec succes !\n");
}

void afficher_candidat(candidats *candi){
    FILE *fichier = fopen("candid.dat", "rb");//ouverture en mode lecture binaire
    if(fichier == NULL){
        printf("\nOuverture du fichier candid.dat echouer\n\n");
        return;
    }
    printf("\n========================================================================================================================================================================\n");
    printf(" %-7s | %-17s | %-20s | %-13s | %-30s | %-23s | %-12s | %-7s | %-13s\n", "ID", "NOM", "PRENOM", "DATE_NAIS", "ADRESSE", "EMAIL", "TELEPHONE", "TYPE", "DATE_INSC");
    printf("\n========================================================================================================================================================================\n");

    while(fread(candi, sizeof(candidats), 1, fichier)==1){
        printf(" %-7s | %-17s | %-20s | %-13s | %-30s | %-23s | %-12s | %-7s | %-13s\n",candi->ID_candid, candi->Nom, candi->Prenom, candi->Dat_Nais, candi->Adresse, candi->email, candi->tel, candi->type, candi->Date_ins);
    }
    printf("\n========================================================================================================================================================================\n");
    fclose(fichier);

}

void modifier_candidat(candidats *candi, char id_modif[]){
    FILE *fichier = fopen("candid.dat", "r+b");
    if(fichier == NULL){
        printf("\nErreur ouverture du fichier candid.dat\n\n");
        return;
    }
    int trouve = 0;
    while(fread(candi,sizeof(candidats),1, fichier) == 1){//parcourir la base de donnees
       if (strcasecmp(candi->ID_candid, id_modif) == 0){//fonction qui compare l'id entree et ce qu'on a dans la base de donnees
             trouve = 1;
             int choix_mod = 0;
           do {//nous pourrons modifier seulement l'adresse du bureau de vote
                printf("\n\n========== MODIFICATION DES DONNEES DU CANDIDAT : (ID: %s) ==========\n", candi->ID_candid);
                printf("1. Date de naissance du candidat\n");
                printf("2. Adresse du candidat\n");
                printf("3. Email du candidat\n");
                printf("4. Telephone du candidat\n");
                printf("5. Type du candidat\n");
                printf("6. Date d'inscription du candidat\n");
                printf("7. Enregistrer les modifications et Quitter\n");
                printf("=============================================================\n");
                printf("Choix : ");

                if (scanf("%d", &choix_mod) != 1) {
                    vider_buffer();
                    continue;
                }

                vider_buffer();//on libere le buffer avant d'ecrire afin de de pas sauvegarder des donnees que l'on ne desire pas

                switch (choix_mod) {//modification en fonction du donnees entrer par l'utilisateur
                    case 1:
                        printf("\nEntrer la nouvelle date de naissance du candidat (JJ/MM/AAAA) : ");
                        lire_chaine(candi->Dat_Nais, sizeof(candi->Dat_Nais));
                        break;

                    case 2:
                        printf("\nEntrer la nouvelle adresse du candidat (No Rue, Rue, Departement) : ");
                        fgets(candi->Adresse, sizeof(candi->Adresse), stdin);
                        candi->Adresse[strcspn(candi->Adresse, "\r\n")] = '\0';
                        break;

                    case 3:
                        char no_rue[6], rue[20], dept[30];
                        printf("\nL'adresse du candidat (No Rue, Rue, Departement) : ");
                        printf("\nEntrer le No de rue (ex : 12) : ");
                        lire_chaine(no_rue, sizeof(no_rue));
                        printf("\nEntrer la Rue (ex : Rue Capois) : ");
                        lire_chaine(rue, sizeof(rue));
                        printf("\nEntrer le Departement (ex : Nord) : ");
                        lire_chaine(dept, sizeof(dept));

                        sprintf(candi->Adresse, "%s, %s, %s", no_rue, rue, dept);
                        break;

                    case 4:
                        printf("\nEntrer le nouveau numero de telephone du candidat : ");
                        lire_chaine(candi->tel, sizeof(candi->tel));

                        break;

                    case 5:
                        int choix = 0;
                        do {
                            printf("\nEntrer le type de candidat :\n\n1. Independant\n2. Sous banniere d'un parti politique\nChoix : ");
                            if (scanf("%d", &choix) != 1) {
                                vider_buffer();
                                continue;
                            }
                            vider_buffer();

                            if (choix == 1) {
                            // CORRECTION ICI : Utiliser strcpy au lieu de '='
                                strcpy(candi->type, "0");
                                printf("\n>> Candidature enregistree sous le statut : INDEPENDANT (Type 0)\n");
                            }
                            else if (choix == 2) {
                                int valide = 0;
                                do {
                                    printf("\nEntrer l'ID du Parti Politique : ");
                                    lire_chaine(candi->type, sizeof(candi->type));

                                    valide = verifier_existe_parti_politic(candi->type);
                                    if (!valide) {
                                        printf(">> ERREUR : Le parti avec l'ID '%s' n'existe pas ! Veuillez reessayer.\n", candi->type);
                                    }
                                } while (!valide);
                            }
                            else {
                                printf(">> Choix invalide ! Entrez 1 pour Independant ou 2 pour Parti.\n");
                            }
                        } while (choix != 1 && choix != 2);
                        break;

                    case 6:
                        printf("\nEntrer la nouvelle date d'inscription du candidat : ");
                        lire_chaine(candi->Date_ins, sizeof(candi->Date_ins));

                        break;
                    case 7:
                        printf("\nSauvegarde des modifications en cours...\n");
                        break;

                    default:
                        printf("\nChoix invalide !\n");
                        break;
                }

            } while (choix_mod != 7);

            fseek(fichier, -((long)sizeof(candidats)), SEEK_CUR);

            fwrite(candi, sizeof(candidats), 1, fichier);

            fflush(fichier);
            break;
       }

    }
    fclose(fichier);

    if (trouve) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\nAucun candidat trouve avec l'ID : %s\n", id_modif);
    }
}

int ge_candi(){
    candidats candi;
    int choix = 0;
    char id[10];

     printf("\n\n================== GESTION CANDIDATS ==================\n\n");
     do {
        printf("\n1. AJOUT CANDIDATS\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU CANDIDAT\n\nChoix : ");

        // Si la saisie n'est pas un nombre correct
        if (scanf("%d", &choix) != 1) {
            vider_buffer(); // Nettoie le buffer si l'utilisateur a tapé du texte ou des caractères parasites
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_candidat(&candi);
                break;

            case 2:
                afficher_candidat(&candi);
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id);
                vider_buffer(); // Toujours vider le \n laissé par le scanf de l'ID
                modifier_candidat(&candi, id);

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
