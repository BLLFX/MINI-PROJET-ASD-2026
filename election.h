#ifndef ELECTION_H
#define ELECTION_H

#include <stdio.h>

// --- STRUCTURES COMMUNES ---
typedef struct {
    char Id_BV[15];
    char Id_President[30];
    char Id_VPresident[30];
    char Id_Secret[30];
    char Adresse_BV[100];
} bureau_vote;

typedef struct {
    char NINU[15];
    char nom[25];
    char prenom[25];
    char Adresse[100];
    char BV[15];
} electeurs;

typedef struct {
    char id_PP[10];
    char Nom[20];
    char date_creat[14];
    char date_ins[14];
    char Adresse[70];
    char Responsable[20];
    char Telephone[13];
    char email[40];
} partie_politic;

typedef struct{
    char ID_candid[10];
    char Nom[20];
    char Prenom[20];
    char Dat_Nais[14];
    char Adresse[70];
    char email[40];
    char tel[13];
    char type[20];
    char Date_ins[14];
}candidats;

typedef struct{
    char Id_vote[10];
    char Date_vote[14];
    char BV[10];
    char NINU[15];
    char id_candid[10];
}votes;

//---- PROTOTYPE DE FONCTIONS ----
int ge_BV();
int p_pol();
void vider_buffer();
int ge_elect();
int ge_vote();
int verifier_existe_NINU(const char *ninu_recherche, char *bv_verifier);
int verifier_existe_BV(const char *id_bv);
int verifier_existe_parti_politic(const char *id_parti);
int verifier_IDP(const char *ID_recherche);
int verifier_existe_candidat(const char *Id_candidat);
void vider_buffer(void);
int ge_candi();
int verifier_existe_vote(const char *id_vote);
void obtenir_date(char *date);
void lire_chaine(char *chaine, int taille);
void liste_electeur(void);
void liste_candidat(void);
int ge_rapp();
void liste_bureau_vote(void);
int verifier_ninu_a_deja_vote(const char *ninu_recherche);
void resultat(void);

#endif
