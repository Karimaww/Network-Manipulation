#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"

/*-----------------------------------------------------------------------------------------*/
typedef struct hachage{
  int m;           /* La taille de la table de hachage */
  CellNoeud **tab; /* Le tableau de pointeurs de CellNoeuds */
}TableHachage;
/*-----------------------------------------------------------------------------------------*/

/* Fonction qui retourne la clef de la table de hachage */
int fonctionClef(double x, double y);

/* Fonction d'hachage */
int fonctionHachage(int cle, int m);

/* Recherche du Noeud dans le Reseau a partir de la table d'hachage */
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);

/* Fonction qui libere la mémoire de la table d'hachage */
void liberer_table_hachage(TableHachage *H);

/* Reconstituation du Reseau a partir de la structure Chaines C et de la taille de la table de hachage */
Reseau* reconstitueReseauHachage(Chaines *C, int M);

/* Fonction qui affiche les Noeuds de la table d'hachage */
void afficher_tableH(TableHachage *H);

/*-----------------------------------------------------------------------------------------*/
#endif