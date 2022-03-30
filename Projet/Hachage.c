#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"

/*-----------------------------------------------------------------------------------------*/
int fonctionClef(double x, double y){
  return (y+(x+y)*(x+y+1)/2);
}
/*-----------------------------------------------------------------------------------------*/
int fonctionHachage(int cle, int m){
  double A = (sqrt(5)-1)/2; //Constante 
  // floor() est la fonction qui prend la partie entiere inferieure et rend un double
  return floor(m*(cle*A - floor(cle*A)));
}
/*-----------------------------------------------------------------------------------------*/
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage *H, double x, double y){
  int cle = fonctionClef(x, y); //Recuperation de la clef
  int pos = fonctionHachage(cle, H->m); //Determination de la position du potentiel Noeud

  /* Parcous de la liste des noeuds de la table d'hachage a la position pos */
  if(H->tab[pos]){
    CellNoeud *tmp = H->tab[pos]; //Variable permettant de parcourir la liste de CellNoeuds a la position pos
    /* Parcours de la liste de CellNoeuds a la position pos */
    for(;tmp; tmp = tmp->suiv){
      if(tmp->nd->x == x && tmp->nd->y == y){
        return tmp->nd; //Si le noeud est present on le renvoie
      }
    }
  }
  
  /* S'il n'existe pas de Noeud dans la table d'hachage, on le crée et on l'ajoute dans le Reseau et dans la table de hachage */
  R->nbNoeuds++; //Incrementation de la taille du Reseau 
  Noeud *n = creer_Noeud(R->nbNoeuds, x, y); //Creation du Noeud
  CellNoeud *cnR = creer_CellNoeud(n); //Creation du CellNoeud correspondant au Noeud pour le Reseau
  
  /* Ajout en tete de la liste des CellNoeuds du Reseau */
  cnR->suiv = R->noeuds;
  R->noeuds = cnR;

  /* Ajout en tete de la liste des CellNoeuds de la table de hachage a la position pos */
  CellNoeud *cnH = creer_CellNoeud(n); //Creation du CellNoeud correspondant au Noeud pour la table de hachage
  cnH->suiv = H->tab[pos];
  H->tab[pos] = cnH;

  return n;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_table_hachage(TableHachage *H){
  if(!H){
    fprintf(stderr, "==%d== Erreur la table de hachage est deja vide - rien a libere\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  /* Liberation de tous les CellNoeuds de la table d'Hachage */
  for(int i=0; i<H->m ; i++){
    //Parcours de la liste des cellnoeuds de chaque case de tableau
    CellNoeud *tmpH = H->tab[i]; //Variable permettant de parcourir les CellNoeuds
    CellNoeud *tmp = NULL; //Variable permettant de liberer les CellNoeuds
    /* Parcours de la liste de CellNoeuds a la position i */
    while(tmpH){
      tmp = tmpH;
      tmpH = tmpH->suiv; //On passe au suivant
      free(tmp); //Liberation du precedent
    }
  }
  
  /*Liberation de tableau */
  free(H->tab);

  /*Liberation de la structure*/
  free(H);

  return;
}
/*-----------------------------------------------------------------------------------------*/
Reseau* reconstitueReseauHachage(Chaines *C, int M){
  if(!C){ // Si C est vide
    fprintf(stderr, "==%d== Erreur la structure Chaine C est vie - rien a reconstituer\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  Reseau *R = creer_Reseau(0, C->gamma); //Creation d'un Reseau
  
  /* Allocation de la Table de Hachage */
  TableHachage *H = (TableHachage *)malloc(sizeof(TableHachage));
  if(!H){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de la Table de Hachage\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Affectation des attributs */
  H->m = M; 
  H->tab = (CellNoeud **)malloc(sizeof(CellNoeud*)*M);
  if(!H->tab){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du tableu de listes de CellNoeuds\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation de toutes les valeurs du tableau a NULL */
  for(int i=0; i<H->m; i++)
    H->tab[i] = NULL;
  
  CellChaine *tmp = C->chaines; //Variable permettant de parcourir la liste de CellChaines de C
  for(; tmp; tmp = tmp->suiv){//Parcours des chaines
    CellPoint *p = tmp->points; //Variable permattant de parcourir la liste de CellPoints de tmp
    
    Noeud *A = rechercheCreeNoeudHachage(R, H, p->x, p->y); //Recuperation de l'extremite A de la CellChaine
    Noeud *prec = A; //Noeud precedent
    Noeud *courant = NULL; //Noeud courant

    Noeud *B = NULL; //Extremite B de la CellChaine
    /* Parcours de tous les CellPoints de la CellChaine tmp */
    for(p = p->suiv; p; p = p->suiv){
      courant = rechercheCreeNoeudHachage(R, H, p->x, p->y);  //Recherche de noeud de coordonees x et y
        
      /* Insertion dans la liste des voisins de courant et precedent */
      inserer_voisins(&(courant->voisins), prec);
      inserer_voisins(&(prec->voisins), courant);
        
      prec = courant; //La mise a jour de prec
    }

    B = courant; //Extremite B correspondant au dernier CellPoint de la CellChaine

    /* Ajout de la commodite */
    inserer_commodite_fin(R, creer_Commodites(A, B));
  }
  liberer_table_hachage(H); //Liberation de la Table de Hachage car on a reussi a reconstituer le Reseau 
  
  return R;
}
/*-----------------------------------------------------------------------------------------*/
void afficher_tableH(TableHachage *H){ //Fonction utilisé pour tester notre code  
  for(int i = 0; i<H->m; i++){
    printf("\nCase numero %d : ", i);
    for(CellNoeud *cn = H->tab[i]; cn; printf("%d -> ", cn->nd->num), cn = cn->suiv);
  }
  printf("\n");
}
/*-----------------------------------------------------------------------------------------*/
