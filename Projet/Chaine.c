#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Chaine.h"
#include "SVGwriter.h"

#define MAX 5 
#define MIN 0 

/*-----------------------------------------------------------------------------------------*/
CellPoint *creer_CellPoint(double x, double y){
  CellPoint *cP = (CellPoint*)malloc(sizeof(CellPoint));
  if(!cP){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du CellPoint\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Declaration des attributs du CellPoint */
  cP->x = x;
  cP->y = y;
  cP->suiv = NULL;

  return cP;
}
/*-----------------------------------------------------------------------------------------*/
CellChaine *creer_CellChaine(int numero){
  CellChaine *cellChaine = (CellChaine*)malloc(sizeof(CellChaine));
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de CellChaine\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Declaration des attributs de la CellChaine */
  cellChaine->numero = numero;
  cellChaine->suiv = NULL;
  cellChaine->points = NULL;
  
  return cellChaine;
}
/*-----------------------------------------------------------------------------------------*/
int nombre_points(CellChaine *cellChaine){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, la CellChaine est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  CellPoint *tmp = cellChaine->points; //On prend une variable temporaire pour pouvoir parcourir la CellChaine
  int cpt = 0; //Compteur qui compte le nombre de points
  /* Parcours de la CellChaine et incrémentation de 1*/
  while(tmp){
    tmp = tmp->suiv;
    cpt++;
  }

  return cpt;
}
/*-----------------------------------------------------------------------------------------*/
Chaines *creer_Chaines(int gamma, int nbChaines){
  Chaines *ch = (Chaines*)malloc(sizeof(Chaines));
  if(!ch){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de Chaines\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Declaration des attributs de Chaines */
  ch->gamma = gamma;
  ch->nbChaines = nbChaines;
  ch->chaines = NULL;

  return ch;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_en_tete_CellPoint(CellChaine *cellChaine, double x, double y){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, la CellChaine est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Creation d'un point qu'on va inserer */
  CellPoint *cell = creer_CellPoint(x, y);
  /* Insertion du CellPoint a la tete de la liste chainee des chaines de points */
  cell->suiv = cellChaine->points; 
  cellChaine->points = cell; //Changement de la tete de la CellChaine

  return;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_fin_CellPoint(CellChaine *cellChaine, double x, double y){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, la CellChaine est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Creation d'un point qu'on va inserer */
  CellPoint *cell = creer_CellPoint(x, y);

  /* Cas si la liste de points de la CellChaine est vide */
  if(!cellChaine->points){
    cellChaine->points = cell; //On ajoute le CellPoint crée
    return;
  }

  CellPoint *tmp = cellChaine->points; //Variable temporaire pour parcourir tous les points de la CellChaine
  CellPoint *prec = NULL; //Variable temporaire qui va nous permettre de garder le dernier pour pouvoir inserer dans son suivant le CellPoint

  /* Parcours de la liste jusqu'a la fin */
  for(; tmp; prec = tmp, tmp = tmp->suiv); 
  /* Ajout du CellPoint a la fin de la CellChaine */
  prec->suiv = cell; 

  return;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_en_tete_CellChaine(Chaines *C, CellChaine *cellChaine){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, la CellChaine est vide - rien a ajouter\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  if(!C){
    fprintf(stderr, "==%d== Erreur, Chaines C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Insertion de la chaine des points en tete */
  cellChaine->suiv = C->chaines;
  C->chaines = cellChaine; //Changement de la tete des Chaines C

  return;
}
/*-----------------------------------------------------------------------------------------*/
void inserer_fin_CellChaine(Chaines *C, CellChaine *cellChaine){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, la CellChaine est vide - rien a ajouter\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  if(!C){
    fprintf(stderr, "==%d== Erreur, Chaines C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Cas si on a pas de CellChaines dans Chaines C */
  if(!C->chaines){
    C->chaines = cellChaine;
    return;
  }
  
  CellChaine *tmp = C->chaines; //Variable temporaire qui permet de parcourir l'ensemble de de CellChaine de Chaines C
  /* Parcours de la liste de CellChaines jusqu'a a la fin */
  while(tmp->suiv) tmp = tmp->suiv;
  /* Ajout de la CellChaine a la fin */
  tmp->suiv = cellChaine;

  return;
}
/*-----------------------------------------------------------------------------------------*/
void afficher_CellPoint(CellPoint *cP){
  if(!cP){
    fprintf(stderr, "==%d== Erreur, pas de CellPoint - rien a afficher\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Affichage du CellPoint cP */
  printf("[%.2lf, %.2lf] ", cP->x, cP->y);  

  return;
}
/*-----------------------------------------------------------------------------------------*/
void afficher_CellChaine(CellChaine *cellChaine){
  if(!cellChaine){
    fprintf(stderr, "==%d== Erreur, pas de CellChaine - rien a afficher\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Affichage du numero de la CellChaine */
  printf("%d ", cellChaine->numero);

  CellPoint *point = cellChaine->points; //Variable temporaire qui stocke les CellPoints de la CellChaine
  /* On parcours l'ensemble des CellPoints de la CellChaine */
  while(point){
    afficher_CellPoint(point); // On affiche chaque CellPoint
    point = point->suiv; // Passage au CellPoint suivant
  }
  printf("\n");
  
  return;
}
/*-----------------------------------------------------------------------------------------*/
void afficher_Chaines(Chaines *C){
  if(!C){
    fprintf(stderr, "==%d== Erreur, pas de Chaines - rien a afficher\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  printf("Gamma: %d\nNombre de chaines: %d\n", C->gamma, C->nbChaines);

  CellChaine *courant = C->chaines; //Variable temporaire qui permet de parcourir l'ensemble des CellChaines de la strucutre Chaines
  /* Parcours de l'ensemble des CellChaines de la structure Chaines */
  while(courant){
    afficher_CellChaine(courant); //Affichage d'une CellChaine
    courant = courant->suiv; //Passage a la CellChaine suivante
  }
  printf("\n");  

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_CellPoint(CellPoint *cP){
  if(!cP){
    fprintf(stderr, "==%d== Erreur, pas de CellPoint - rien a liberer\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  free(cP); //Libération du CellPoint

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_CellChaine(CellChaine *cC){
  if(!cC){
    fprintf(stderr, "==%d== Erreur, pas de CellPoint - rien a liberer\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  CellPoint *point = cC->points; //Variable temporaire qui permet de stoker l'ensemble de CellPoints de la CellChaine
  CellPoint *precedent = NULL; //On garde le CellPoint precedent pour pouvoir liberer les CellPoints
  /* Parcours de l'ensemble des CellPoints de la CellChaine */
  while(point){
    precedent = point;
    point = point->suiv; //On passe au CellPoint precedent
    liberer_CellPoint(precedent); //Liberation du CellPoint precedent
  } 

  free(cC); //Liberation de la CellChaine

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_Chaines(Chaines *C){
  if(!C){
    fprintf(stderr, "==%d== Erreur, pas de Chaines - rien a liberer\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  CellChaine *courant = C->chaines; //Variable temporaire qui permet de stocker l'ensemble des CellChaines de la structure Chaines
  CellChaine *precedent = NULL;     //On garde la CellChaine precedente pour pouvoir liberer les CellChaines 
  /* Parcours de l'ensemble des CellChaines de la structure Chaines */
  while(courant){ 
    precedent = courant; 
    courant = courant->suiv; //On passe a la prochaine CellChaine
    liberer_CellChaine(precedent); //Liberation de la CellChaine precedente
  } 

  free(C); //Liberation de la structure Chaines

  return;
 }
/*-----------------------------------------------------------------------------------------*/
Chaines* lecture_chaine(FILE *f){ //On suppose que f est est deja ouvert en lecture
  if(!f){
    fprintf(stderr, "==%d== Erreur d'ouverture\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  int nbChaines;
  int gamma;
  char buffer[100000];
  char lecture[100000];
  
  /* Récuperation de nbChaines */
  fgets(buffer,15, f);
  sscanf(buffer,"%s %d", lecture, &nbChaines); 

  /* Récuperation de gamma */
  fgets(buffer, 15, f);
  sscanf(buffer ,"%s %d", lecture, &gamma);  

  /* Création de le la structure Chaines */
  Chaines* C = creer_Chaines(gamma, nbChaines);
  
  int id; //Numero de la chaîne
  int nbPoints; //Nombre de points de la chaîne

  /* On parcours ligne par ligne le FILE f */
  while(fgets(buffer, 100000, f)){ 
    if(strcmp(buffer, "\n") == 0) continue; //Ignorer les eventuels sauts de ligne
    char coordonnes[100000]; //Tableau qui permet de stocker les coordonnees
    sscanf(buffer ,"%d %d %[^\n]", &id, &nbPoints, coordonnes);  // [^\n] permet de recuperer des phrases avec des espaces contrairement a %s
    
    CellChaine *cellC = creer_CellChaine(id);  //Creation de la CellChaine avec le numero id  
    /* On met tous les CellPoints dans la CellChaine*/
    for(int i=0; i<nbPoints; i++){
      double x, y;  // coordonnées des points
      sscanf(coordonnes, "%lf %lf %[^\n]", &x, &y, coordonnes); //%lf pour le type double 
      inserer_en_tete_CellPoint(cellC, x, y); // on ajoute chaque //dans la liste de chaines
    }
    /* Insertion de la CellChaine dans la structure Chaines */
    inserer_en_tete_CellChaine(C, cellC);
  } 
  fclose(f);
  
  return C;
}
/*-----------------------------------------------------------------------------------------*/
void ecrire_Chaines(Chaines *C, FILE *f){ 
  if(!C){
    fprintf(stderr, "==%d== Erreur, la strucure Chaines C est vide - rien a ecrire\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  if(!f){
      fprintf(stderr, "==%d== Erreur d'ouverture\n", __LINE__);
      exit(EXIT_FAILURE);
  }

  /* Recuperation des donnees gamma et nbChaines */
  int nbChaines = C->nbChaines;
  int gamma = C->gamma;
  /* Ecriture dans le fichier f des donnees gamma et nbChaines */
  fprintf(f, "NbChain: %d\nGamma: %d\n", nbChaines, gamma);

  CellChaine *tmpCellChaines = C->chaines; //Variable temporaire qui permet de parcourir toutes les CellChaines de C
  /* Parcours de toutes les CellChaines de C */
  for(; tmpCellChaines; tmpCellChaines = tmpCellChaines->suiv){
     int nbpoints= nombre_points(tmpCellChaines);//Recuperation du nombre de points de chaque CellChaine
     fprintf(f,"%d %d ", tmpCellChaines->numero, nbpoints); //Ecriture du numero et du nombre de points de chaque CellChaine
     CellPoint *tmpCellPoints = tmpCellChaines->points; //Variable temporaire qui permet de parcourir tous les CellPoints
     /* Parcours de tous les CellPoints de la CellChaine */
     for(; tmpCellPoints; tmpCellPoints = tmpCellPoints->suiv)
      fprintf(f, "[%.2lf, %.2lf] ", tmpCellPoints->x, tmpCellPoints->y); //Ecriture des coordonnees de chaque CellPoint
    fprintf(f, "\n"); //Saut de ligne pour passer a la prochiane CellChaine
  }

  fclose(f); //Fermeture du fichier

  return;
}
/*-----------------------------------------------------------------------------------------*/
void afficheChainesSVG(Chaines *C, char* nomInstance){ //Fonction recuperé
    //int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}
/*-----------------------------------------------------------------------------------------*/
double longueurChaine(CellChaine *c){
  if(!c){
    fprintf(stderr, "==%d== Erreur, la CellChaine c est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  double longueur = 0.0; //Longueur resultat 

  CellPoint *tmp = c->points; //La variable temporaire pour faire le parcours des points
  /* Parcours de l'ensemble des CellPoints de la CellChaine */
  for(;tmp->suiv; tmp = tmp->suiv){
    /* Coordonnes des points de la chaine */
    double xA = tmp->x;
    double yA = tmp->y;
    
    double xB = tmp->suiv->x;
    double yB = tmp->suiv->y;

    /* Somme des distances entre deux CellPoints */
    longueur += sqrt(pow(xB-xA, 2) + pow(yB-yA, 2));
  }

  return longueur;
}
/*-----------------------------------------------------------------------------------------*/
double longueurTotale(Chaines *C){
   if(!C){
    fprintf(stderr, "==%d== Erreur, la structure C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  double res = 0.0; //Longueur resultat totale 

  CellChaine *tmp = C->chaines; //Variable temporarie pour le parcours des CellChaines
  /* Parcours de l'ensemble de CellChaines de la structure Chaine C */
  while(tmp){
    res += longueurChaine(tmp); //Somme des distances des CellChaines
    tmp = tmp->suiv; 
  }
  
  return res;
}
/*-----------------------------------------------------------------------------------------*/
int compte_points_total(Chaines *C){
  if(!C){
    fprintf(stderr, "==%d== Erreur, la structure C est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  int nbPointsTotal = 0; //Nombre de points au debut
  
  CellChaine *tmp = C->chaines; //Variable temporaire qui va parcourir toutes les CellChaines
  
  /* On parcourt l'ensemble des CellChaines de Chaines C et on recupere le nombre de points de chaque CellChaine avec la fonction nombre_points definie plus haut */
  for(; tmp; tmp=tmp->suiv){
    nbPointsTotal+= nombre_points(tmp);
  }
  
  return nbPointsTotal;
}
/*-----------------------------------------------------------------------------------------*/
Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax, int ymax){
  int gamma = rand()%(MAX-MIN)+MIN; //On choisit gamma entre 0 et 5
  Chaines *C = creer_Chaines(gamma, nbChaines); //Creation de la structure Chaines resultat

  /* Creation de nbChaines CellChaines */
  for(int i=0; i<nbChaines; i++){
    CellChaine *cellChaine = creer_CellChaine(i); //Creation de la CellChaine numero i
    /* Creation de nbPointsChaine CellPoints pour la CellChaine*/
    for(int j = 0; j < nbPointsChaine; j++){
      double x = rand()%xmax; //x prend une valeur entre 0 et xmax
      double y = rand()%ymax; //y prend une valeur entre 0 et y max
      inserer_en_tete_CellPoint(cellChaine, x, y); //Insertion en tete du CellPoint dans la CellChaine
    }
    inserer_en_tete_CellChaine(C, cellChaine); //Insertion en tete du CellPoint dans la structure Chaines C
  }

  return C;
}
/*-----------------------------------------------------------------------------------------*/
