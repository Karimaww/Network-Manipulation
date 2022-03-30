#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include <stdio.h>
#include <stdlib.h>

#include "SVGwriter.h"

/*-----------------------------------------------------------------------------------------------------*/
/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;
/*-----------------------------------------------------------------------------------------------------*/

/* Fonction qui permet de creer une liste chaînée de CellPoint */
CellPoint *creer_CellPoint(double x, double y);

/* Fonction qui permet la création d'une liste chaînée de CellChaine */
CellChaine *creer_CellChaine(int numero);

/* Fonction qui compte le nombre de points de la CellChaine */
int nombre_points(CellChaine *cellChaine);

/* Fonction qui permet la création de l'ensemble des chaînes Chaines */
Chaines *creer_Chaines(int gamma, int nbChaines);

/* Insertion en tete d'un CellPoint dans la CellChaine */
void inserer_en_tete_CellPoint(CellChaine *cellChaine, double x, double y);

/* Insertion en fin d'un CellPoint dans la CellChaine */
void inserer_fin_CellPoint(CellChaine *cellChaine, double x, double y);

/* Insertion en tete d'une CellChaine dans la liste des chaines de la structure Chaines */
void inserer_en_tete_CellChaine(Chaines *chaines, CellChaine *cellChaine);

/* Insertion en fin d'une CellChaine dans la liste des chaines de la structure Chaines */
void inserer_fin_CellChaine(Chaines *chaines, CellChaine *cellChaine);

/* Affichage d'un CellPoint */
void afficher_CellPoint(CellPoint *cP);

/* Affichage d'une CellChaine */
void afficher_CellChaine(CellChaine *cellChaine);

/* Affichage de la structure Chaines */
void afficher_Chaines(Chaines *C);

/* Liberation d'un CellPoint */
void liberer_CellPoint(CellPoint *cP);

/* Liberation d'une CellChaine */
void liberer_CellChaine(CellChaine *cC);

/* Liberartion de la structure Chaines */
void liberer_Chaines(Chaines *C);

/* Fonction permettant d’allouer, de remplir et de retourner une instance de la structure Chaines a partir d’un fichier f */
Chaines *lecture_chaine(FILE *f);

/* Ecriture permettant l'ecriture de la structure Chaines C dans un fichier f */
void ecrire_Chaines(Chaines *C, FILE *f);

/* Affichage des Chaines SVG*/
void afficheChainesSVG(Chaines *C, char* nomInstance);

/* Fonction qui calcule la longueur physique d'une CellChaine */
double longueurChaine(CellChaine *c);

/* Fonction qui calcule la longueur physique de toutes les CellChaines de la structure Chianes C */
double longueurTotale(Chaines *C);

/* Fonction qui compte le nombre total de points de la structure Chaines C */
int compte_points_total(Chaines *C);

/* Fonction qui genere une structure Chaines C avec des nouvelles données aleatoires */
Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax, int ymax);

/*-----------------------------------------------------------------------------------------*/
#endif	
