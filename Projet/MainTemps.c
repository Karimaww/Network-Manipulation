#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

#define M 200
#define NB_POINTS_CHAINE 100
#define XMAX 5000
#define YMAX 5000
#define NB_MAX_CHAINES 5000
#define PAS 500

/*--------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
  if(argc != 2){ // On verifie que l'utilisateur met bien le nom du fichier lors de l'exectution
    fprintf(stderr, "==%d== Erreur, je m'attendais a avoir 2 arguments (le main + le nom du fichier.cha)\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));

  /* On lit le fichier qui nous interesse */
  FILE *read = fopen(argv[1], "r");
  Chaines *C = lecture_chaine(read);

  /* Manipulations pour ne pas ecraser a chaque fois les fichier d'affichage (.txt et .html) */
  char cheminEtNom[100]; //Variable qui va stocker une le chemin+nom du fichier .txt et .html
  strcpy(cheminEtNom, "./Sortie_vitesses/"); //On copie premierement le chemin 
  char *p = strtok(argv[1], "."); //Variable qui va contenir le nom du fichier sans extention
  strcat(cheminEtNom, p); //On concatenes pour avoir le chemin+nom du fichier sans extention
  strcat(cheminEtNom, "_temps.txt");

  /* Creation des fichiers qui vont stocker les temps des differentes structures */
	FILE *SortieVitesse = fopen(cheminEtNom, "a");
  if(!SortieVitesse){
  	fprintf(stderr, "==%d== Probleme lors de l'ouverture du fichier SortieVitesse.\n", __LINE__);
  	return EXIT_FAILURE;
  }

  /* Variables permettant de stocker les temps de calcul */
  clock_t  temps_initial;
  clock_t  temps_final;
  double temps_algo1= 0, temps_algo2 = 0, temps_algo3 = 0;

  /* On calcul le temps en fonction de la taille M de la Table de Hachage */
  /* On fait cela pour la Table de Hachage ou les valeurs vont chnager comparé aux deux autres structures */
  for(int j=1; j<=M; j+=5){ 
    printf("M = %d\n", j); //Pour suivre l'evolution dans le terminal 
    /* Boucle permettant de faire la moyenne de temps sur 2 executions*/
	  for(int i=1; i<=2; i++){
      temps_initial = clock();
      Reseau *ReseauListeChainee = reconstitueReseauListe(C); //Cas de la Liste Chainee
      temps_final = clock();
      temps_algo1 += ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable
      printf("---%d\n", i); //Pour suivre l'evolution dans le terminal
      temps_initial = clock();
      Reseau *ReseauTableHachage = reconstitueReseauHachage(C, j); //Cas de la Table de Hachage
      temps_final = clock();
      temps_algo2 += ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable
      printf("---%d\n", i); //Pour suivre l'evolution dans le terminal
      temps_initial = clock();
      Reseau *ReseauArbreQuaternaire = reconstitueReseauArbre(C); //Cas de l'Arbre Quaternaire 
      temps_final = clock(); 
      temps_algo3 += ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable
      printf("---%d\n", i); //Pour suivre l'evolution dans le terminal
      /* Liberation des differentes structures de données */
      liberer_Reseau(ReseauListeChainee);
      liberer_Reseau(ReseauTableHachage);
      liberer_Reseau(ReseauArbreQuaternaire);
    }
    /* Ecriture des temps dans le fichier correspondant */
    fprintf(SortieVitesse,"%d %f %f %f\n", j, (double)(temps_algo1/2), (double)(temps_algo2/2), (double)(temps_algo3/2)); //On divise par 10 pour pouvoir faire la moyenne
    temps_algo1 = temps_algo2 = temps_algo3 = 0;
  }

  /* Fermeture du fichier ou l'on a stocké les temps */
  fclose(SortieVitesse);

  /* Liberation de Chaines */
  liberer_Chaines(C);
  
  
  //=================================================================================================//


  /* 6.3 */
  FILE *GraphiqueListeChaines = fopen("./Sortie_vitesses/GraphiqueListeChaines.txt", "a");
  if(!GraphiqueListeChaines){
  	fprintf(stderr, "==%d== Probleme lors de l'ouverture du fichier GraphiqueListeChaines.\n", __LINE__);
  	return EXIT_FAILURE;
  }

  FILE *GraphiqueArbreHachage = fopen("./Sortie_vitesses/GraphiqueArbreHachage.txt", "a");
  if(!GraphiqueArbreHachage){
  	fprintf(stderr, "==%d== Probleme lors de l'ouverture du fichier GraphiqueArbreHachage.\n", __LINE__);
  	return EXIT_FAILURE;
  }
  
  printf("=== Passage a la deuxieme partie ===\n\n");
  for(int i = 500; i <= NB_MAX_CHAINES; i += PAS){
    printf("i = %d\n", i); ////Pour suivre l'evolution dans le terminal
    Chaines *C = generationAleatoire(i, NB_POINTS_CHAINE, XMAX, YMAX); //On genere a chaque fois une structure C

    temps_initial = clock();
    Reseau *ReseauListeChainee = reconstitueReseauListe(C); //Cas de la Liste Chainee
    temps_final = clock();
    temps_algo1 = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable

    temps_initial = clock();
    Reseau *ReseauTableHachage = reconstitueReseauHachage(C, i/100);  //Cas de la Table de Hachage
    temps_final = clock();                       //M max =5000/100=50 pour la taille de la Table de Hachage
    temps_algo2 = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable
      
    temps_initial = clock();
    Reseau *ReseauArbreQuaternaire = reconstitueReseauArbre(C); //Cas de l'Arbre Quaternaire 
    temps_final = clock(); 
    temps_algo3 = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC; //On stocke le temps dans une variable
      
    /* Liberation des differentes structures de données */
    liberer_Reseau(ReseauListeChainee);
    liberer_Reseau(ReseauTableHachage);
    liberer_Reseau(ReseauArbreQuaternaire);
  
    fprintf(GraphiqueListeChaines, "%d %f\n", i, (double)(temps_algo1));
    fprintf(GraphiqueArbreHachage, "%d %f %f\n", i, (double)(temps_algo2), (double)(temps_algo3));
    
    temps_algo1 = temps_algo2 = temps_algo3 = 0;

    liberer_Chaines(C); //Liberation de la structure Chaines
  }

  /* Fermeture des fichiers ou l'on a stocké les temps */
  fclose(GraphiqueArbreHachage);
  fclose(GraphiqueListeChaines);

  return EXIT_SUCCESS;
}
/*--------------------------------------------------------------------------------------------------------*/
