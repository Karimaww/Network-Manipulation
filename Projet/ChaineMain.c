#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "SVGwriter.h"

/*------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
  if(argc != 2){ // On verifie que l'utilisateur met bien le nom du fichier lors de l'exectution
    fprintf(stderr, "==%d== Erreur, je m'attendais a avoir 2 arguments (le main + le nom du fichier.cha)\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  FILE *read = fopen(argv[1], "r"); //Ouverture du fichier a lire
  Chaines *C = lecture_chaine(read); //Lecture de ce fichier dans C

  if(strcmp(argv[1], "00014_burma.cha")==0) 
    afficher_Chaines(C); //Affichage du resultat que dans ce cas car les deux autres trop grands pour afficher
                         //Voir le fichier Affichage_Chaine.txt dans les autres cas

  /* Manipulations pour ne pas ecraser a chaque fois les fichier d'affichage (.txt et .html) */
  char cheminEtNom[100]; //Variable qui va stocker une le chemin+nom du fichier .txt et .html
  strcpy(cheminEtNom, "./Test_ChaineMain/"); //On copie premierement le chemin 
  char *p = strtok(argv[1], "."); //Variable qui va contenir le nom du fichier sans extention
  strcat(cheminEtNom, p); //On concatenes pour avoir le chemin+nom du fichier sans extention

  
  afficheChainesSVG(C, cheminEtNom); //Fichier html pour l'affichage SVG

  strcat(cheminEtNom, ".txt"); //Variable qui va contenir chemi+nom+.txt
  FILE *write = fopen(cheminEtNom, "w"); //Ouverture du fichier a ecrire

  ecrire_Chaines(C, write); //Ecriture du resultat dans un fichier .txt
  
  printf("La longueur totale de la structure Chaines du fichier %s est = %.2lf\n\n", argv[1] ,longueurTotale(C));

  printf("Le nombre total d'occurences des points du fichier %s est = %d\n\n", argv[1], compte_points_total(C));
  
  liberer_Chaines(C); //Liberation de la strucure Chaines C
  
  
  return 0;
}
/*------------------------------------------------------------------------------------------------------*/