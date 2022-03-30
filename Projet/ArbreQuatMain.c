#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Chaine.h"
#include "Reseau.h"
#include "ArbreQuat.h"

/*-------------------------------------------------------------------------------------------*/
int main(int argc, char **argv){
  if(argc != 2){ // On verifie que l'utilisateur met bien le nom du fichier lors de l'exectution
    fprintf(stderr, "==%d== Erreur, je m'attendais a avoir 2 arguments (le main + le nom du fichier.cha)\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* On lit le fichier qui nous interesse */
  FILE *read = fopen(argv[1], "r"); //Ouverture du fichier a lire
  Chaines *C = lecture_chaine(read); //Lecture de ce fichier dans C

  /* Manipulations pour ne pas ecraser a chaque fois les fichier d'affichage (.txt et .html) */
  char cheminEtNom[100]; //Variable qui va stocker une le chemin+nom du fichier .txt et .html
  strcpy(cheminEtNom, "./Test_ArbreQuatMain/"); //On copie premierement le chemin 
  char *p = strtok(argv[1], "."); //Variable qui va contenir le nom du fichier sans extention
  strcat(cheminEtNom, p); //On concatenes pour avoir le chemin+nom du fichier sans extention

  Reseau *R = reconstitueReseauArbre(C); //On reconstitue le Reseau a partir de C

  afficheReseauSVG(R, cheminEtNom); //Affichage SVG du Reseau

  strcat(cheminEtNom, ".txt"); //Variable qui va contenir chemi+nom+.txt
  FILE *f2 = fopen(cheminEtNom, "w"); //Ouverture du fichier a ecrire
  ecrireReseau(R, f2);//Eciture du Reseau dans un fichier .txt

  liberer_Chaines(C); //Liberation de la structure Chaines C

  liberer_Reseau(R); //Liberation du Reseau R

  return 0;
}
/*-------------------------------------------------------------------------------------------*/