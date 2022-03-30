#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include<stdlib.h>
#include<stdio.h>

#include "Struct_Liste.h"
#include "Struct_File.h"

/*-----------------------------------------------------------------------------------------*/
typedef struct{
  int u,v;        /* Numeros des sommets extremité */
} Arete;


typedef struct cellule_arete{
  Arete *a;     /* pointeur sur l'arete */
  struct cellule_arete *suiv;
} Cellule_arete;


typedef struct {
  int num;                 /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete* L_voisin;  /* Liste chainee des voisins */
} Sommet;


typedef struct{
  int e1,e2;    /* Les deux extremites de la commodite */
} Commod;


typedef struct{
  int nbsom;        /* Nombre de sommets */
  Sommet **T_som;    /* Tableau de pointeurs sur sommets */
  int gamma;     
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;

/*-----------------------------------------------------------------------------------------*/

/* Creation d'une arete */
Cellule_arete *creer_Cellule_arete(Arete *a);

/* Creation d'un Sommet */
Sommet *creer_Sommet(int num, double x, double y);

/* Fonction qui cree un Graphe a partir d'un Reseau */
Graphe* creerGraphe(Reseau* r);

/* Fonction qui reorganise le Reseau */
int reorganiseReseau(Reseau* r);

/* Fonction qui cree une Arete*/
Arete *creer_Arete(int u, int v);

/* Fonction qui cree une commodite Commod */
Commod creer_Commod(int e1, int e2);

/* Fonction de creation de la liste des voisins d'un sommet */
void creer_L_voisin(Cellule_arete **tabC, Sommet *som, CellNoeud *cn);

/* Fonction qui le chemin (ListeEntier) entre les sommets s1 et s2 */
ListeEntier nb_min_arretes_chaine(Graphe *G, int s1, int s2);

/* Liberation d'une cellule arete */
void liberer_Cell_arete(Cellule_arete *cellarete);

/* Liberation du sommet */
void liberer_Sommet(Sommet *som);

/* Liberation Graphe */
void liberer_Graphe(Graphe *G);

/* Fonction qui reorganise le Reseau */
int reorganiseReseau(Reseau* r);

/* Fonction qui affiche le graphe g */
void afficher_graphe(Graphe *g);

/*-----------------------------------------------------------------------------------------*/
#endif
