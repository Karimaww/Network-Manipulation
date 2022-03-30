#ifndef __RESEAU_H__
#define __RESEAU_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"

/*-----------------------------------------------------------------------------------------*/
typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;
/*-----------------------------------------------------------------------------------------*/

/* Fonction qui cree un Noeud */
Noeud *creer_Noeud(int num, double x, double y);

/* Fonction qui cree un CellNoeud */
CellNoeud *creer_CellNoeud(Noeud *nd);

/* Fonction qui cree un CellCommodite */
CellCommodite *creer_Commodites(Noeud *A, Noeud *B);

/* Fonction qui cree un Reseau */
Reseau *creer_Reseau(int nbNoeuds, int gamma);

/* Insertion du Noeud n dans la liste des voisins du CellNoeud CN */
void inserer_voisins(CellNoeud **CN, Noeud *n);

/* Fonction qui insere une CellCommodite a la fin du Reseau R */
void inserer_commodite_fin(Reseau *R, CellCommodite *com);

/* Fonction qui retourne un noeud du reseau correspondant au point (x,y) */
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);

/* Liberation Reseau */
void liberer_Reseau(Reseau *R);

/* Fonction qui retourne le nombre de commodites du Reseau */
int nombre_Commodites(Reseau *R);

/* Fonction qui retourne le nombre de liaisons/cables du Reseau */
int nombre_Liaisons(Reseau *R);

/* Fonction qui retourne le Reseau a partir d'un ensemble Chaines */
Reseau* reconstitueReseauListe(Chaines *C);

/* Affichage du Reseau dans le fichier */
void ecrireReseau(Reseau *R, FILE *f);

/* Affichage SVG */
void afficheReseauSVG(Reseau *R, char* nomInstance);

/*-----------------------------------------------------------------------------------------*/
#endif

