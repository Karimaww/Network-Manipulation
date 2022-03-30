#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Struct_Liste.h"
#include "Graphe.h"
#include "Struct_File.h"
  
/*-----------------------------------------------------------------------------------------*/
Arete *creer_Arete(int u, int v){
  /* Allocation d'une arete */
  Arete *ar = (Arete *)malloc(sizeof(Arete));
  if(!ar){
    fprintf(stderr, "===%d== Erreur lors de l'allocation de l'Arete\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  /* Declaration des attributs */
  ar->u = u;
  ar->v = v;

  return ar;
}
/*-----------------------------------------------------------------------------------------*/
Cellule_arete *creer_Cellule_arete(Arete *a){
  /* Allocation d'une cellule arete */
  Cellule_arete *cella = (Cellule_arete*)malloc(sizeof(Cellule_arete));
  if(!cella){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de la Cellule arete\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Declaration des attributs */
  cella->a = a;
  cella->suiv = NULL; 

  return cella;
}
/*-----------------------------------------------------------------------------------------*/
Sommet *creer_Sommet(int num, double x, double y){
  /*Allocation d'un sommet */
  Sommet *som = (Sommet*)malloc(sizeof(Sommet));
  if(!som){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du sommet\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Declaration des attributs */
  som->num = num;
  som->x = x;
  som->y = y;
  som->L_voisin = NULL; // La liste des voisins est mise a vide

  return som;
}
/*-----------------------------------------------------------------------------------------*/
Commod creer_Commod(int e1, int e2){
  /* Creation d'une commodite */
  Commod com;

  /* Declaration des attributs */
  com.e1 = e1;
  com.e2 = e2;

  return com;
}
/*-----------------------------------------------------------------------------------------*/
void creer_L_voisin(Cellule_arete **tabC, Sommet *som, CellNoeud *cn){
  if(!som){
    fprintf(stderr, "==%d== Erreur, le sommet est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  if(!cn){
    fprintf(stderr, "==%d== Erreur, le CellNoeud est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  int num = cn->nd->num; //On prend le numero du CellNoeud passé en argument
  /* Parcours de la liste des voisins du CellNoeud passé en argument */
  for(CellNoeud *cnVoisins = cn->nd->voisins; cnVoisins; cnVoisins = cnVoisins->suiv){
    Arete *ar = NULL; //Variable qui va stocker l'arrete existante ou va la créer
    Cellule_arete *cella = NULL; //Cellule_arete qui va etre crée a partir de ar

    /* On verifie s'il existe une arete de valeurs num et voisins->nd->num dans le tableau de listes d'aretes tabC */
    if(tabC[num]){ // S'il existe la liste de Cellule_arete pour le sommet/noeud 'num'
      /* Parcours des listes Cellule_arete de la case tabC[num] */
      for(Cellule_arete *tmp = tabC[num]; tmp; tmp = tmp->suiv){
        /* Si on trouve l'arete pour le sommet et son voisin */
        if(tmp->a->u == num && tmp->a->v == cnVoisins->nd->num)
          /* On prend le pointeur vers cette arete */
          ar = tmp->a; 
      }
    }
    /* Si on trouve pas d'arete dans le tableau */
    if(!ar){
      ar = creer_Arete(num, cnVoisins->nd->num); /* Creation d'une arete */
      
      /* Creation de Cellule_arete le tableau de pointeurs sur Cellule_arete */
      cella = creer_Cellule_arete(ar);
      
      /* Ajout de la Cellule_arete en tete de la liste tabC[num] */
      cella->suiv = tabC[num];
      tabC[num] = cella;
    }
    
    Cellule_arete *cellaVoisin = creer_Cellule_arete(ar); //Creation d'une Cellule_arete pour le sommet

    /*Ajout de la Cellule_arete en tete de la liste des voisins L_voisin du sommet */
    cellaVoisin->suiv = som->L_voisin;
    som->L_voisin = cellaVoisin;
  }
  
  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_Cell_arete(Cellule_arete *cellarete){
  if(cellarete){  
    Cellule_arete *tmp = NULL; //Variable qui va permettre de parcourir les Cellule_aretes
    /* Parcours de la liste des Cellule_aretes */
    while(cellarete){
      tmp = cellarete;
      cellarete = cellarete->suiv; //On passe a la suivante
      free(tmp->a); //Liberation de l'Arete correspondante a la Cellule_arete
      free(tmp); //Liberation de chaque Cellule_arete
    }
  }

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_Sommet(Sommet *som){
  if(som){
    liberer_Cell_arete(som->L_voisin); //Liberation de la liste des voisins du sommet som
    free(som); //Liberation de la structure Sommet
  }

  return;
}
/*-----------------------------------------------------------------------------------------*/
void liberer_Graphe(Graphe *G){
  if(G){
    /* Liberation de tous les sommets du Graphe G */
    for(int i = 0; i <= G->nbsom; i++)
      liberer_Sommet(G->T_som[i]);
  
    //Liberation du tableau de Sommmets 
    free(G->T_som); 

    //Liberation de la structure Commod
    free(G->T_commod);

    //Liberation de la structure Graphe
    free(G);
  }

  return;
}
/*-----------------------------------------------------------------------------------------*/
Graphe* creerGraphe(Reseau* r){
  if(!r){
    fprintf(stderr, "==%d== Erreur, le Reseau r est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  /* Allocation memoire du Graphe */
  Graphe *g = (Graphe *)malloc(sizeof(Graphe));
  if(!g){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire pour le Grahpe g\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  /* Attribution des caracteristiques du Graphe */
  g->nbsom = r->nbNoeuds;
  g->gamma = r->gamma;
  g->nbcommod = nombre_Commodites(r);
  

  /* Initialisaion du tableau de commodites */
  g->T_commod = (Commod*)malloc(sizeof(Commod)*g->nbcommod);
  if(!g->T_commod){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire pour le tableau de commodites du Graphe\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  CellCommodite *cellcom = r->commodites; //Variable qui va permettre de parcourir les commodites de r
  /* Parcorus des commodites du Reseau r */
  for(int i = 0; cellcom; cellcom = cellcom->suiv, i++)
    /* Creation d'une commodite pour le graphe */
    g->T_commod[i] = creer_Commod(cellcom->extrA->num, cellcom->extrB->num);
  
  /* Allocation memoire d'un tableau de listes permettant de verifier si une arete a deja été allouée */
  Cellule_arete **tabC = (Cellule_arete**)malloc(sizeof(Cellule_arete*)*(g->nbsom+1));
  if(!tabC){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire pour le tableau de liste de Cellule_aretes\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  /* Initialisation de toutes les cases a NULL */
  for(int i=0; i<=g->nbsom; i++) 
    tabC[i] = NULL; 

  /* Initialisaion du tableau de pointeurs sur les sommets */
  g->T_som = (Sommet**)malloc(sizeof(Sommet*)*(g->nbsom+1)); //(g->nbsom+1) pour prendre le dernier sommet aussi
  if(!g->T_som){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire pour le tableau de sommets du Graphe \n", __LINE__);
    exit(EXIT_FAILURE);
  }
  /* Initialisation de toutes les cases a NULL */
  for(int i=0; i<=g->nbsom; i++)
    g->T_som[i] = NULL; 
  
  Sommet *som = NULL; //Variable qui va permettre de stocker les sommets
  CellNoeud *cn = r->noeuds; //Variable qui va permettre de parcourir les CellNoeuds du Reseau
  /* Parcours de tous les CellNoeuds du Reseau r */
  for(; cn; cn = cn->suiv){
    int num = cn->nd->num; //On garde le numero du Noeud courant
    /* Creation du sommet */
    som = creer_Sommet(num, cn->nd->x, cn->nd->y); 

    /* Creation d'une liste des voisins pour le sommet som */
    creer_L_voisin(tabC, som, cn);
    
    /* Ajout du sommet dans le tableau des sommets du graphe a la position du numero du sommet/noeud */
    g->T_som[num] = som;
  }
  
  /* Liberation du tableau des listes des cellules aretes */
  for(int i = 0; i <= g->nbsom; i++){  
    Cellule_arete *tmp = tabC[i]; //Variable qui va permettre de parcourir les listes
    Cellule_arete *t = NULL; //Variable qui va permettre de liberer la liste
  
    /* Parcours de toutes les Cellule_aretes de la case i */
    while(tmp){
      t = tmp;
      tmp = tmp->suiv; //On passe a la prochaine
      free(t); //On libere la precedente
    }
  }

  /* Liberation de la structure */
  free(tabC);
  
  return g;
}
/*-----------------------------------------------------------------------------------------*/
ListeEntier nb_min_arretes_chaine(Graphe *G, int s1, int s2){
  /* Si pas de Graphe on sort */
  if(!G){
    fprintf(stderr, "==%d== Erreur, le graphe G est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Creation du tableau de ListeEntier pour avoir l'arborescence a partir de s1 (Question 7.3) */
  ListeEntier *arborescence = (ListeEntier *)malloc(sizeof(ListeEntier)*(G->nbsom+1));
  if(!arborescence){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de memoire de l'arborescence\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation de toutes les cases de l'arborescence*/
  for(int i=0; i<=G->nbsom ; i++){
    Init_Liste(&arborescence[i]);  
    /* Chaque case de ListeEntier doit contenir son numero du Sommet */
    ajoute_en_tete(&arborescence[i], i); 
  }

  /* Allocation du tableau des sommets visités */
  int *visit =(int*)malloc(sizeof(int)*(G->nbsom+1)); //(G->nbsom+1) pour prendre en compte le dernier Sommet aussi
  if(!visit){
    fprintf(stderr, "==%d== Erreur lors de l'allocation du tableau 'visit' d'entiers\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  /* Initialisaion du tableau a 0 car on a rien visité pour l'instant */
  for(int i=0; i<=G->nbsom; i++)
    visit[i]=0;
  
  /* Le s1 est visité */
  visit[s1] = 1;

  /* Creation d'une FILE (Premier Entré Premier Sorti) */
  S_file *F = (S_file*)malloc(sizeof(S_file));
  if(!F){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire de la S_file F\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  /* Inititalisation de la FILE*/
  Init_file(F); 
  /* Insersion du sommet dans la FILE*/
  enfile(F, s1);

  /* Creation du tableau avec les longueurs entre le Sommet s1 et tous les autres Sommets du Grhape G */
  int *D = (int*)malloc(sizeof(int)*(G->nbsom+1)); //(G->nbsom+1) pour prendre en compte le dernier Sommet aussi
  /* Au debut la distance est mise a zero */
  for(int i=0; i<=G->nbsom; i++)
    D[i]=0;

  ListeEntier *arete = (ListeEntier *)malloc(sizeof(ListeEntier)*(G->nbsom+1)); //(G->nbsom+1) pour prendre en compte le dernier Sommet aussi
  if(!arete){
    fprintf(stderr, "==%d== Erreur lors de l'allocation memoire du tableau de ListeEntier des aretes\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Initialisation de chaque case du tableau de ListeEntier*/
  for(int i=0; i<=G->nbsom ; i++){
    Init_Liste(&arete[i]);
    /* Chaque case de ListeEntier doit contenir son numero du Sommet */
    ajoute_en_tete(&arete[i], i);
  }

  while(!(estFileVide(F))){
    int u = defile(F); //Variable qui stocke le Sommet 
    
    /* Variable qui prend les voisins du Sommet u */
    Cellule_arete *courant =  G->T_som[u]->L_voisin;
    /* Parcours des tous les voisins du Sommet u*/
    while(courant){ 
      int v = courant->a->v; //Numero du Sommet voisin du Sommet u
      /* Si on a jamais visité ce Soment on rentre */
      if(visit[v]==0){ 
        visit[v]=1;  //Le sommet avec le numero v devient visité   
        enfile(F, v); //Il est enfile dans la file pour parcourir ses voisins aussi
        D[v] = D[u]+1; //On incremente la distance entre le sommet v et le sommet u
        /* Creation de l'arete entre le Sommet u et son voisin le Sommet v */
        ajoute_en_tete(&arete[v], u); // k->i
     }
     courant=courant->suiv; //Passage au voisin suviant de u
    }
  }

  /* Parcours de tous les Sommets du Grahpe*/
  for(int i=1; i<=G->nbsom; i++){
    /* Si c'est la racine ou un voisin de la racine, on a deja le chemin sinon on le fait */
    while((i != s1) && (arborescence[i]->u != s1))
      /* Creation de l'arborescence entre le Sommet s1 et i */
      ajoute_en_tete(&arborescence[i], arete[arborescence[i]->u]->u);
  }
  
  ListeEntier s1s2, tmp; //s1s2 pour la ListeEntier resulat et tmp pour parcourir l'arborescence
  Init_Liste(&s1s2);
  Init_Liste(&tmp);
  tmp = arborescence[s2]; //Chemin de s1 a s2
  
  /* On recopie arborescence[s2] dans s1s2 (mais l'ordre est inversé => chemin de s2 vers s1) ce qui ne change rien*/
  for(; tmp; tmp = tmp->suiv)
    ajoute_en_tete(&s1s2, tmp->u);

  /* Liberation de l'arborescence */
  for(int i=0; i<=G->nbsom; i++)
      desalloue(&arborescence[i]);
  free(arborescence);
  
  /* Liberation du tableau des sommets visités */
  free(visit);
  
  /* Liberation du tableau avec les longueurs entre le sommet s1 et tous les autres sommets du Grhape G */
  //int longueur = D[s2]; //Variable contenant la distance entre s1 et s2 (Question 7.2)
  free(D);

  /* Liberation de la FILE */
  free(F);

  /* Liberation du tableau de listes de aretes */
  for(int i=0; i<=G->nbsom ; i++) 
    desalloue(&arete[i]);
  free(arete);
  
  return s1s2; //On retourne le chemin qui nous interesse
}
/*-------------------------------------------------------------------------------------------*/
int reorganiseReseau(Reseau* r){
if(!r){
    fprintf(stderr, "==%d== Erreur, le Reseau est vide\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* Creation d'un graphe */
  Graphe *G = creerGraphe(r);

  /* Allocation de la liste qui va stocker les chaines pour toutes les commodites */
  ListeEntier *tab = (ListeEntier*)malloc(sizeof(ListeEntier)*G->nbcommod);
  if(!tab){
    fprintf(stderr, "==%d== Erreur lors de l'allocation de liste\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  Commod commod; //Variable qui sert pour stocker les Commod
  /* Parcours de toutes les Commod du Graphe G pour les ajouter dans *tab */
  for(int i = 0; i < G->nbcommod; i++){
    commod = G->T_commod[i];  //Variable qui va stocker un Commod
    /* Stockage de la chaine entre les sommets de numero e1 et e2 */
    tab[i] = nb_min_arretes_chaine(G, commod.e1, commod.e2);
    printf("==%d== %d\n", __LINE__, i); //Pour voir l'evolution dans terminal
  }

  /* Initialisation de la matrice som-som qui stocke le nb de chaines passant par chaque arete */
  int **matriceSommetSommet = (int**)malloc(sizeof(int*)*(G->nbsom+1));
  if(!matriceSommetSommet){
    fprintf(stderr, "==%d== Erreur d'allocation de la matriceSommetSommet\n", __LINE__); 
    exit(EXIT_FAILURE);
  }

  /* Allocation des colonnes de la matrie som-som*/
  for(int i=0; i<=G->nbsom; i++)
    matriceSommetSommet[i] = (int*)malloc(sizeof(int)*(G->nbsom+1)); //(G->nbsom+1) pour prendre en compte le dernier Sommet aussi

  int res = 1; //Resultat de la fonction, supposé vrai au debut
  /* Initialisation de toutes les valeurs de la matrice à 0*/
  for(int i=0; i<=G->nbsom; i++)
    for(int j=0; j<=G->nbsom;j++)  
      matriceSommetSommet[i][j] = 0;
  
  /* Parcours de toutes les chaines de toutes les commodites du Graphe G */
  for(int i=0; i<G->nbcommod; i++){ 
    ListeEntier tmpTab = tab[i]; //Variable qui permet de parcourir les chaies des Commod du Graphe G
    /* Parcours de chaque Cellule_arete qui existe dans la chaine */
    for(; tmpTab->suiv; tmpTab = tmpTab->suiv){
      matriceSommetSommet[tmpTab->u][tmpTab->suiv->u]++; //Incrémentation d'une chaine dans la matrice pour l'arete tab[i]->u et tab[i]->suiv->u
      matriceSommetSommet[tmpTab->suiv->u][tmpTab->u]++; //Meme incrémentation pour tab[i]->suiv->u et tab[i]->u car il s'agit d'un Graphe non orienté
      if(matriceSommetSommet[tmpTab->u][tmpTab->suiv->u] >= G->gamma) 
        res = 0; //Le resultat de la fonction change 
    }    
  }
  
  /* Liberation d'un tableau qui stocke les chaines */
  for(int i = 0; i < G->nbcommod; i++)
    desalloue(&tab[i]);
  free(tab);

  /* Liberation de la matrice som som */ 
  for(int i = 0; i<=G->nbsom; i++)
    free(matriceSommetSommet[i]);
  free(matriceSommetSommet);

  //afficher_graphe(G);  //Enlever le commentaire que si on utilise "00014_burma.cha" car nb Noeuds petit
  /* Liberation d'un graphe */
  liberer_Graphe(G);

  return res;
}
/*-------------------------------------------------------------------------------------------*/
void afficher_graphe(Graphe *g){ //Fonction qui nous a permis de verifier notre code
  if(!g){
    fprintf(stderr, "Pas de Grahpe, rien a afficher\n");
    exit(EXIT_FAILURE);
  }
  printf("\n| nbsom : %d | gamma : %d | Nbcomm : %d |\n",g->nbsom, g->gamma, g->nbcommod);
  
  for(int i=1; i<=g->nbsom; i++){ // On commence a 1 parce que pas de sommet 0
    Sommet *som = g->T_som[i];
    printf("Sommet: %d\n", som->num);
    Cellule_arete *tmpA = g->T_som[i]->L_voisin;

    printf("Les voisins: ");
    for(; tmpA; tmpA = tmpA->suiv)
     printf("[%d %d] ", tmpA->a->u, tmpA->a->v);
    printf("\n");
    
  }

  printf("Commodites: \n");
  for(int i = 0; i<g->nbcommod; i++){
    printf("%d %d\n", g->T_commod[i].e1, g->T_commod[i].e2);
  }
}
/*-------------------------------------------------------------------------------------------*/
/*  
*/

    
      /*
      arete[k]=k,    ok arborescence[k]=k,
      arete[i]=k->i, ok arborescence[i]=k->i,
      arete[p]=k->p, ok arborescence[p]=k->p,
      arete[a]=k->a, ok arborescence[a]=k->a,
      arete[h]=k->h, ok arborescence[h]=k->h,
      
      A traiter :
      arete[z]=i->z, => arborescence[z]=k->i->z, ok  

      arete[d]=z->d, => arborescence[d]=k->i->z->d, ok
    
      arete[m]=z->m => arborescence[m]=k->i->z->m,  ok
    
      arete[s]=m->s => arborescence[s]=k->i->z->m->s, ok
      */
      
      
      //     p
      //     |
      //  a--k---i---z---d visit[k]=1, visit[i]=1 D[i]=1, visit[z]=1 D[z]=2, visit[d]=1 D[d]=3 
      //     |       |
      //     h       m
      //             |
      //             s
  
