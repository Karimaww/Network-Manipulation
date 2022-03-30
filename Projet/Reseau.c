#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Chaine.h"
#include "Reseau.h"

/*-----------------------------------------------------------------------------------------*/
Noeud *creer_Noeud(int num, double x, double y){
    /* Allocation d'un Noeud */
    Noeud *nd = (Noeud*)malloc(sizeof(Noeud));
    if(!nd){
      fprintf(stderr, "==%d== Erreur lors de l'allocation du Noeud\n", __LINE__);
      exit(EXIT_FAILURE);
    }

    /* Initialisation des données */
    nd->num=num;
    nd->x=x;
    nd->y=y;
    nd->voisins = NULL; //Pas de voisins
    
    return nd;
}
/*-----------------------------------------------------------------------------------------*/
CellNoeud *creer_CellNoeud(Noeud *nd){
  /* Allocation d'un CellNoeud */
  CellNoeud *n = (CellNoeud *)malloc(sizeof(CellNoeud));
  if(!n){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du CellNoeud\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation des données */
  n->nd = nd;
  n->suiv = NULL;

  return n;
}

/*-----------------------------------------------------------------------------------------*/
CellCommodite *creer_Commodites(Noeud *A, Noeud *B){
  /* Allocation d'une commodite */
  CellCommodite *c = (CellCommodite *)malloc(sizeof(CellCommodite));
  if(!c){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de la CellCommodite\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation des données */
  c->extrA = A; //Extremite A
  c->extrB = B; //Extremite B
  c->suiv = NULL;

  return c;
}
/*-----------------------------------------------------------------------------------------*/
Reseau *creer_Reseau(int nbNoeuds, int gamma){
  /* Allocation du Reseau */
  Reseau *R = (Reseau*)malloc(sizeof(Reseau));
  if(!R){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du Reseau\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation des données */
  R->nbNoeuds = nbNoeuds;
  R->gamma = gamma;
  R->noeuds = NULL;
  R->commodites = NULL;

  return R;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_voisins(CellNoeud **CN, Noeud *n){
  if(!n){
    fprintf(stderr, "==%d== Erreur le Noeud n est vide - rien a ajouter\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Si la liste de voisins est vide alors on ajoute le noeud en tête */
  if(!*CN){
    *CN = creer_CellNoeud(n);
    return;
  }

  CellNoeud *V = *CN; //Variable temporaire qui permet de parcourir tous les voisins
  CellNoeud *prec = NULL; //Variable temporaire qui va servir pour inserer a la fin
  /* Parcours de tous les voisins */
  for(; V; prec = V, V = V->suiv){
    if(V->nd == n) return; //Si le Noeud n est deja present, pas la peine de l'ajouter a nouveau
  } 
  prec->suiv = creer_CellNoeud(n); //On ajoute le Noeud n a la fin si il est pas deja present dans les voisins
  
  return;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_commodite_fin(Reseau *R, CellCommodite *com){
  if(!R){ //Si le Reseau est vide
    fprintf(stderr, "==%d== Erreur le Reseau R est vide - rien a ajouter\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  if(!com){//Si la commodite est vide
    fprintf(stderr, "==%d== Erreur la CellCommodite est vide - rien a ajouter\n", __LINE__);
    exit(EXIT_FAILURE); 
  }

  /* Si le reseau n'a pas de commodités */
  if(!R->commodites){
    R->commodites = com;
    return;
  }

  CellCommodite *tmp = R->commodites; //Variable temporaire qui permet de parcourir les CellCommodites de R
  /* On parcours jusqu'a arriver a la derniere commodité */
  for(; tmp->suiv; tmp = tmp->suiv);

  tmp->suiv = com; //Ajout a la fin

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_Reseau(Reseau *R){ 
  CellCommodite *tmp3 = R->commodites; //Variable temporaire qui va servir pour liberer
  CellCommodite *tmp4 = R->commodites; //Variable temporaire qui permet de parcourir toutes les CellCommodites du Reseau R

  /*Liberation des commodites */
  while(tmp3){ 
    tmp4 = tmp3;
    tmp3 = tmp3->suiv; //On passe au suivant
    free(tmp4); //On libere le precedent
  }
  
  CellNoeud *tmp1 = R->noeuds; //Variable temporaire qui va servir pour parcourir tous les CellNoeuds de R
  CellNoeud *tmp2 = R->noeuds; //Variable temporaire qui va servir pour liberer
  /* Liberation des Noeuds et CellNoeuds */
  while(tmp1){
    tmp2 = tmp1;
    tmp1 = tmp1->suiv; //On passe au prochain
    /* Liberation des voisins */
    while(tmp2->nd->voisins){ 
      CellNoeud *lib = tmp2->nd->voisins; //Variable temporaire qui sert pour liberer le voisins precedent
      tmp2->nd->voisins = tmp2->nd->voisins->suiv; //On passe au voisin suivant
      free(lib);//On liberre le voisin precedent
    }
    free(tmp2->nd); //Liberation des Noeuds associé au CellNoeud
    free(tmp2); //Liberation du CellNoeud 
  }

  free(R); //Liberation du Reseau R
}
/*-----------------------------------------------------------------------------------------*/
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
  /* Cas s'il n'y a pas de noeuds dans le Reseau */
  if(R->noeuds == NULL){
    R->nbNoeuds++; //Incrementation de 1 le nombre de Noeuds du Reseau R
    Noeud *n = creer_Noeud(R->nbNoeuds, x, y); //On cree le Noeud correspondant aux coordonnées
    R->noeuds = creer_CellNoeud(n); //On cree le CellNoeud associé et on le met dans la liste de noeuds du Reseau qui est vide
    return n; //On retourne le Noeud en question
  }

  CellNoeud *tmp = R->noeuds; //Variable temporaire qui permet de parcourir tous les CellNoeuds de R
  CellNoeud *prec = NULL; //Variable temporaire pour avoir le precedent
  
  /* On parcours le Reseau jusqu'a trouver le Noeud */
  for(; tmp; prec = tmp, tmp = tmp->suiv){
    /* Si un Noeud existe avec ces coordonnées on le retourne */
    if(tmp->nd->x == x && tmp->nd->y == y){
      return tmp->nd;
    }  
  }
  
  /* Si le Noeud n'est pas present dans le Reseau on le crée  */
  R->nbNoeuds++; //Incrementation de 1 le nombre de Noeuds du Reseau R
  Noeud *n = creer_Noeud(R->nbNoeuds, x, y); //On cree le Noeud correspondant aux coordonnées
  //Ajout du noeud a la fin de la liste des CellNoeuds
  prec->suiv = creer_CellNoeud(n); //On cree le CellNoeud associé et on le met a la fin de la liste des CellNoeuds du Reseau R
  
  return n;
}
/*-----------------------------------------------------------------------------------------*/
Reseau* reconstitueReseauListe(Chaines *C){
  if(!C){ // Si C est vide
    fprintf(stderr, "==%d== Erreur la structure Chaines C est vide - rien a reconstituer\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  Reseau *R = creer_Reseau(0, C->gamma); //Creation du Reseau resultat 
  CellChaine *tmp = C->chaines; //Variable temporaire qui permet de parcourir toutes les CellChaine de C
  /* Parcours de toutes les CellChaines de la structure Chaines C*/ 
  for(; tmp; tmp = tmp->suiv){
    CellPoint *p = tmp->points; //Variable temporaire qui permet de parcourir tous les CellPoints de tmp
    Noeud *A = rechercheCreeNoeudListe(R, p->x, p->y); //Recuperation de l'extremite A de la chaine
    Noeud *prec = A; //Variable temporaire qui stocke le Noeud precedent
    Noeud *courant = NULL; //Variable temporaire qui stocke le Noeud courant 

    Noeud *B = NULL; //Variable temporaire qui va permettre d'avoir l'extremite B de la CellChaine
    /* Parcorus de tous les CellPoints du Reseau */
    for(p = p->suiv; p; p = p->suiv){
      /* Recherche du Noeud de coordonees x et y */
      courant = rechercheCreeNoeudListe(R, p->x, p->y);
        
      /* Insertion dans la liste des voisins de courant et precedent */
      inserer_voisins(&(courant->voisins), prec);
      inserer_voisins(&(prec->voisins), courant);
        
       prec = courant; //La mise a jour de prec
    }

    B = courant; //A la fin du parcours des CellPoints on a le dernier CellPoint

    /* Ajout de la commodite */
    inserer_commodite_fin(R, creer_Commodites(A, B));
  }

  return R;
}
/*-----------------------------------------------------------------------------------------*/
int nombre_Commodites(Reseau *R){
  int nbCom=0; //Variable qui va stocker le nombre de commodites a retourner 
  if(!R){
    fprintf(stderr, "==%d== Le reseau R est vide - pas de commodites\n", __LINE__);
    return nbCom; //Pas de commodites
  }

  CellCommodite *tmp = R->commodites; //Variable temporaire qui permet de parcourir toutes les CellCommodites du Reseau R
  /* On parcours la liste de CellCommodite et on compte le nombre de commodites */
  while(tmp){
    nbCom++;
    tmp = tmp->suiv;
  }

  return nbCom;
}
/*-----------------------------------------------------------------------------------------*/
int nombre_Liaisons(Reseau *R){
   int nbLiasons = 0; //Variable qui va stocker le nombre de liaisons a retourner 
  if(!R){
    fprintf(stderr, "==%d== Le reseau R est vide - pas de liaisons\n", __LINE__);
    return nbLiasons; //Pas de liaisons
  }

  CellNoeud *cn = R->noeuds; //Variabel temporaire qui permet de parcourir tous les CellNoeuds de R
  /* Parcours de la liste de CellNoeuds du Reseau */
  for(; cn; cn=cn->suiv){
    Noeud *n = cn->nd; //Variable qui stocke le Noeud n correspondant au CellNoeud
    CellNoeud *tmpVoisin = n->voisins;//Variable temporaire qui permet de parcourir tous les voisins de n
    /* Parcours des voisins de chaque Noeud n et on meme temps on incremente nbLiaisons */
    for(; tmpVoisin; tmpVoisin = tmpVoisin->suiv, nbLiasons++);
  }
  
  /* On divise par deux car on compte deux fois les liaisons lors du parcours des voisins */ 
  return nbLiasons/2; 
}
/*-----------------------------------------------------------------------------------------*/
void ecrireReseau(Reseau *R, FILE *f){
  /* Si le Reseau R est vide */
  if(!R){
    fprintf(stderr, "==%d== Le Reseau R est vide - rien a ecrire\n", __LINE__);
    exit(EXIT_FAILURE);  
  }

  /* Si le fichier n'a pas été ouvert */
  if(!f){
    fprintf(stderr, "==%d== Erreur d'ouverture du fichier\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Recuperation des donnees et ecriture de ces données */
  int nbNoeuds = R->nbNoeuds;
  int nbLiaisons = nombre_Liaisons(R);
  int nbCommodites = nombre_Commodites(R);
  int gamma = R->gamma;
  fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n", nbNoeuds, nbLiaisons, nbCommodites, gamma);

  /* Ecriture des noeuds du Reseau R */
  CellNoeud* cn = R->noeuds; //Variable temporaire qui permet de parcourir tous les CellNoeuds de R
  /* Parcours de tous les CellNoeuds du Reseau R */
  for(; cn; cn = cn->suiv){
    Noeud *n = cn->nd; //Noeud contenu dans CellNoeud
    fprintf(f, "v %d %f %f\n", n->num, n->x, n->y); //Ecriture du Noeud n dans le fichier f
  }

  fprintf(f,"\n"); //Saut de ligne pour passer aux liaisons

  /* Allocation de la matrice qui permet de verifier si les liaisons sont deja affichees dans le fichier */
  int **tab = (int**)malloc(R->nbNoeuds*sizeof(int*));
  if(!tab){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de la matrice de liaisons \n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Allocation des colonnes de la matrice */
  for(int j=0; j<R->nbNoeuds; j++){
    tab[j] = (int*)malloc((R->nbNoeuds)*sizeof(int)); 
  }

  /* On met toutes les cases à 0 */
  for(int i=0; i<R->nbNoeuds; i++)
    for(int j=0; j<R->nbNoeuds; j++)
      tab[i][j] = 0;


  CellNoeud *c = R->noeuds; //Variable temporaire qui permet de parcourir tous les CellNoeuds de R
  /* Parcours de tous les CellNoeuds du Reseau R */
  for(; c; c=c->suiv){ 
    Noeud *n = c->nd; //Noeud contenu dans CellNoeud
    CellNoeud *tmpVoisin = n->voisins; //Variable contenant les voisins du Noeud n
    /* Parcours des voisins de chaque Noeud */
    for(; tmpVoisin; tmpVoisin = tmpVoisin->suiv){
      /* On verifie s'il n'existe pas de memes liaisons affichees 
         -1 car on commence a 0 dans la matrice */
      if(tab[tmpVoisin->nd->num-1][n->num-1] == 0 && tab[n->num-1][tmpVoisin->nd->num-1] == 0){
        fprintf(f, "l %d %d\n", n->num, tmpVoisin->nd->num); //Ecriture de la liaison si elle n'est pas deja affichée
        /* On met les cases de la matrice a 1 car on viens d'afficher */
        tab[tmpVoisin->nd->num-1][n->num-1] = 1;
        tab[n->num-1][tmpVoisin->nd->num-1] = 1;
      }
      
    }
  }
  /* Liberation de la matrice */
  for(int i=0; i<R->nbNoeuds; i++)
    free(tab[i]);
  free(tab);

  fprintf(f,"\n"); //Saut de ligne pour passer aux CellCommodites
  
  CellCommodite *ext = R->commodites; //Variable qui permet de parcourir tous les CellCommodites de R
  /* Parcours de toutes les CellCommodites du Reseau R*/
  for(; ext; ext = ext->suiv)
    fprintf(f,"k %d %d\n", ext->extrA->num, ext->extrB->num); //Ecriture de la CellCommodite
  
  fclose(f);
}
/*-----------------------------------------------------------------------------------------*/
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}
/*-----------------------------------------------------------------------------------------*/
