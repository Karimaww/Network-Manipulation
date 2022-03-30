#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "ArbreQuat.h"

/*-----------------------------------------------------------------------------------------*/
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
  /* On verifie si l'ensemble des chaines C n'est pas vide */
  if(!C){
    fprintf(stderr, "==%d== Erreur la structure Chaines C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation des pointeurs min et max */
  /* On prend les coordonnees du premier Point de la premiere Chaine du Reseau */
  *xmin = C->chaines->points->x;
  *ymin = C->chaines->points->x;
  *xmax = C->chaines->points->y;
  *ymax = C->chaines->points->y;

  CellChaine *chaine = C->chaines; //Variable qui permet de parcourir la liste de CellChaines de C
  /* Parcours de la liste de CellChaines de C */
  for(; chaine; chaine = chaine->suiv){
    CellPoint *p = chaine->points; //Variable qui permet de parcourir la liste de CellPoints de chaine
    /* Parcours de tous les CellPoints de la CellChiane */
    for(; p; p = p->suiv){
      if(p->x > *xmax) *xmax = p->x; //Si la coordonnee courante est sup a xmax, on la remplace
      if(p->x < *xmin) *xmin = p->x; //Si la coordonnee courante est inf a xmin, on la remplace
      
      if(p->y > *ymax) *ymax = p->y;  //Si la coordonnee courante est sup a ymax, on la remplace
      if(p->y < *ymin) *ymin = p->y;  //Si la coordonnee courante est inf a ymin, on la remplace
    }
  }

  return; 
}
/*-----------------------------------------------------------------------------------------*/
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
  /* Allocation de l'arbre quaternaire */
  ArbreQuat *aq = (ArbreQuat *)malloc(sizeof(ArbreQuat));
  if(!aq){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de l'Arbre Quaternaire\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Affectation des attributs */
  aq->xc = xc;
  aq->yc = yc;
  aq->coteX = coteX;
  aq->coteY = coteY;

  /* Les valeurs initialisees a vides */
  aq->noeud = NULL;
  aq->so = NULL;
  aq->se = NULL;
  aq->no = NULL;
  aq->ne = NULL;

  return aq;
}
/*-----------------------------------------------------------------------------------------*/
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
  /* Cas Arbre Vide */
  if(!*a){
    /* Initialisation des cotes du nouveau rectangle */
    double newCoteX = parent->coteX/2.0;
    double newCoteY = parent->coteY/2.0;

    /* Determination des nouvelles coordonées du centre du nouveau rectangle */
    double newXC, newYC;
    if(n->x < parent->xc)
      /* Le quadrant de gauche */
      newXC = parent->xc - parent->coteX/4.0;
    else
      /* Le quadrant de droit */  
      newXC = parent->xc + parent->coteX/4.0;
    
    if(n->y < parent->yc)
      /* Le quadrant en bas */
      newYC = parent->yc - parent->coteY/4.0;
    else
      /* Le quadrant en haut */
      newYC = parent->yc + parent->coteY/4.0;

    /* Creation d'un Arbre Quaternaire avec les nouvelles coordonees du centre et nouvelles cotes */
    ArbreQuat *arbre = creerArbreQuat(newXC, newYC, newCoteX, newCoteY);
    arbre->noeud = n; //On insere le Noeud dans l'Arbre 
    *a = arbre; //Pour eviter les warnings
    return;

  } else{
    /* Cas Feuille */
    if((*a)->noeud != NULL){//Cas ou il y a le Noeud dans l'Arbre *
      Noeud *an = (*a)->noeud; //Creation d'un Noeud temporaire pour pouvoir l'ajouter apres
      (*a)->noeud = NULL; //On le met a NULL car il va etre tansferé
      /* On determine la direction d'insertion */
      /* En comparant les coordonnées du Noeud avec les coordonnes du centre de l'Arbre *a */
      if(an->x < (*a)->xc){
        if(an->y >= (*a)->yc){
          /* Insertion Nord-ouest */
          insererNoeudArbre(an, &((*a)->no), (*a));
        } 
        else{
          /* Insertion Sud-ouest */
          insererNoeudArbre(an, &((*a)->so), (*a));
        }
      }else{
        if(an->y >= (*a)->yc){
          /* Insertion Nord-est */
          insererNoeudArbre(an, &((*a)->ne), (*a));
        }
        else{
          /* Insertion Sud-ouest */
          insererNoeudArbre(an, &((*a)->se), (*a));
        }
      }
    /* Apres l'ajout de Noeud interne de l'Arbre, on rentre dans le cas de la Cellule Interne */
    }
    
    /* Cas Cellule Interne */
    /* On determine la direction d'insertion */
    /* En comparant les coordonnées du Noeud avec les coordonnes du centre de l'Arbre *a */
    if(n->x < (*a)->xc){
      if(n->y >= (*a)->yc){
        /* Insertion Nord-ouest */
        insererNoeudArbre(n, &((*a)->no), (*a)); 
      }
      else{
        /* Insertion Sud-ouest */
        insererNoeudArbre(n, &((*a)->so), (*a));
      }
    }else{//Sinon
      if(n->y >= (*a)->yc){
        /* Insertion Nord-est */
        insererNoeudArbre(n, &((*a)->ne), (*a));
      } else{
        /* Insertion Sud-ouest */
        insererNoeudArbre(n, &((*a)->se), (*a));
      }
    } 
  }
  
  return;
}
/*-----------------------------------------------------------------------------------------*/
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
  /* Cas Arbre Vide */
  if(*a == NULL){
    R->nbNoeuds++; //Incrementation du nombre de Noeuds du Reseau
    /* Creation du Noeud */
    Noeud *n = creer_Noeud(R->nbNoeuds, x, y);
        
    /*Allocation du CellNoeud correspondant au Noeud n */
    CellNoeud *courant = creer_CellNoeud(n);
  
    /* On ajoute le CellNoeud en tete de la liste des Noeuds du Reseau R */
    courant->suiv = R->noeuds;
    R->noeuds = courant;

    /* Ajout du Noeud dans l'Arbre Quaternaire */
    insererNoeudArbre(n, a, parent);

    return n;

  } else{
    /* Cas Feuille */
    if((*a)->noeud){
      /* Verification si le Noeud n'est pas deja present, on le retourne si tel est le cas */
      if((*a)->noeud->x == x && (*a)->noeud->y == y) return (*a)->noeud;
      
      /* Sinon on le cree et on l'ajoute dans le Reseau et dans l'Arbre Quaternaire */
      R->nbNoeuds++; //Incrementation du nombre de Noeuds du Reseau
      /* Creation du Noeud */
      Noeud *n = creer_Noeud(R->nbNoeuds, x, y);
    
      /*Allocation du CellNoeud correspondant au Noeud n */
      CellNoeud *courant = creer_CellNoeud(n);
  
      /* On ajoute le CellNoeud en tete de la liste des Noeuds du Reseau R */
      courant->suiv = R->noeuds;
      R->noeuds = courant;
    
      /* Ajout du Noeud dans l'Arbre Quaternaire */
      insererNoeudArbre(n, a, (*a));
      return n;
    }

    /* Cas Cellule Interne */
    else{
      double xc = (*a)->xc, yc = (*a)->yc; //Coordonees du centre de l'Arbre Quaternaire 
      /* Determination du quadrant dans lequel on doit chercher le Noeud */
      /* On compare les coordonnees du Noeud n avec les coordonnees du centre de l'Arbre *a */
      if(x < xc){
        if(y >= yc){
          return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y); //Nord-ouest
        }
        if(y < yc){
          return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y); //Sud-ouest
        }
          
      }
      if(x >= xc){
        if(y >= yc){
          return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y); //Nord-est 
        }
          
        if(y < yc){
          return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y); //Sud-est
        }
          
      } 
    }
  }
  
  return NULL; //Pour eviter les warnings
 }
 /*-----------------------------------------------------------------------------------------*/
 void liberer_arbre_quat(ArbreQuat *A){
  if(A){
    /* Liberation recursive des fils de l'arbre quaternaire */
    liberer_arbre_quat(A->no);
    liberer_arbre_quat(A->ne);
    liberer_arbre_quat(A->so);
    liberer_arbre_quat(A->se);

    /* Liberation de la structure Arbre Quaternaire*/
    free(A);
  }

  return;
}
/*-----------------------------------------------------------------------------------------*/
Reseau* reconstitueReseauArbre(Chaines* C){
  if(!C){
    fprintf(stderr, "==%d== Erreur la structure Chaines C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Creation dun Reseau */
  Reseau *R = creer_Reseau(0, C->gamma); 
  
  double xmin, ymin, xmax, ymax; //Variables qui vont servir pour delimiter le rectangle de la racine de l'Arbre
  chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax); //Determination des limites du rectangle

  /* Creation de l'Arbre avec les coordonnes du centre et des cotes */
  ArbreQuat *parent = creerArbreQuat(xmin+(xmax-xmin)/2.0, ymin+(ymax-ymin)/2.0, (xmax-xmin), (ymax-ymin));
  
  CellChaine *tmp = C->chaines; //Variable permettant de parcourir toutes les CellChaines de C
  /* Parcours de toutes les CellChaines de C*/
  for(; tmp; tmp = tmp->suiv){
    CellPoint *p = tmp->points; //Variable permettant de parcourir tous les CellPoints de tmp
    Noeud *A = rechercheCreeNoeudArbre(R, &parent, parent, p->x, p->y); //Recuperation de l'extremite A de la CellChaine pour pouvoir creer la commodite correspondante

    Noeud *prec = A; //Le Noeud precedent
    Noeud *courant = NULL; //Le Noeud courant

    Noeud *B = NULL; //Extremite B de la CellChaine pour pouvoir creer la commodite correspondante
    /* Parcorus des CellPoints de la CellChaine */
    for(p = p->suiv; p; p = p->suiv){
      /* Recherche du Noeud de coordonees x et y */
      courant = rechercheCreeNoeudArbre(R, &parent, parent, p->x, p->y);

      /* Insertion dans la liste des voisins de courant et precedent */
      inserer_voisins(&(courant->voisins), prec);
      inserer_voisins(&(prec->voisins), courant);
        
      prec = courant; //Mise a jour de prec 
    }

    B = courant; //Extremite B correspond au dernier CellPoint de la CellChaine

    /* Ajout de la commodite */
    inserer_commodite_fin(R, creer_Commodites(A, B));
  }

  liberer_arbre_quat(parent); //Liberation de l'Arbre une fois le Reseau reconstitué

  return R;
}
/*-----------------------------------------------------------------------------------------*/



