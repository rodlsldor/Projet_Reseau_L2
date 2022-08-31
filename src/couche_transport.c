#include <stdio.h>
#include <stdbool.h>
#include "couche_transport.h"
#include "services_reseau.h"

/* ************************************** */
/* Fonctions utilitaires couche transport */
/* ************************************** */

/*
 * Ajouter ici vos fonctions utilitaires
 * (generer_controle, verifier_controle...)
 */
uint8_t generer_controle(paquet_t paquet)
{
    uint8_t control = paquet.type ^ paquet.num_seq ^ paquet.lg_info;

    for (int i = 0; i < paquet.lg_info; i++)
        control ^= paquet.info[i];
    return control;
}

bool verifier_controle(paquet_t paquet)
{
    return paquet.somme_ctrl == generer_controle(paquet);
}

/* ======================================================================= */
/* =================== Fenêtre d'anticipation ============================ */
/* ======================================================================= */

/*--------------------------------------*/
/* Fonction d'inclusion dans la fenetre */
/*--------------------------------------*/
int dans_fenetre(unsigned int inf, unsigned int pointeur, int taille)
{

    unsigned int sup = (inf + taille - 1) % SEQ_NUM_SIZE;

    return
        /* inf <= pointeur <= sup */
        (inf <= sup && pointeur >= inf && pointeur <= sup) ||
        /* sup < inf <= pointeur */
        (sup < inf && pointeur >= inf) ||
        /* pointeur <= sup < inf */
        (sup < inf && pointeur <= sup);
}
