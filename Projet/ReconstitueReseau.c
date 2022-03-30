#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

/*-------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv){
  if(argc != 3){ // On verifie que l'utilisateur met bien le nom du fichier et le numero de la structure lors de l'exectution
    fprintf(stderr, "Erreur - Je m'attendais a avoir 3 arguments (le main + le nom du fichier + le numero de la methode\n");
    exit(EXIT_FAILURE);
  }

  /* Ouverture du fichier pour la lecture*/
  FILE *f = fopen(argv[1], "r");
  Chaines *C = lecture_chaine(f);
	
	 /* Manipulations pour ne pas ecraser a chaque fois les fichier d'affichage (.txt et .html) */
  char cheminEtNom[100]; //Variable qui va stocker une le chemin+nom du fichier .txt et .html
  strcpy(cheminEtNom, "./Test_ReconstitueReseau/"); //On copie premierement le chemin 
  char *p = strtok(argv[1], "."); //Variable qui va contenir le nom du fichier sans extention
  strcat(cheminEtNom, p); //On concatenes pour avoir le chemin+nom du fichier sans extention
  
  /* Initialisation du Reseau et du fichier pour l'ecriture */
  Reseau *R = NULL;
  FILE *f2 = NULL;
  
  switch(*argv[2]){
    case '1': // Méthode de la liste chainee
      printf("Vous avez choisi la reconstitution du reseau a partir de la liste chainee\n");
      R = reconstitueReseauListe(C);
			
			strcat(cheminEtNom, "_liste_chainee"); //Variable qui va contenir chemi+nom+.txt
			afficheReseauSVG(R, cheminEtNom);
			strcat(cheminEtNom, ".txt"); //Variable qui va contenir chemi+nom+.txt
      f2 = fopen(cheminEtNom, "w");
      ecrireReseau(R, f2);

      break;

    case '2': //Methode de la table de hachage
      printf("Vous avez choisi la reconstitution du reseau a partir de la table de hachage avec taille 15\n");
      R = reconstitueReseauHachage(C, 15);

			strcat(cheminEtNom, "_table_hachage"); 
      afficheReseauSVG(R, cheminEtNom);
			strcat(cheminEtNom, ".txt"); //Variable qui va contenir chemi+nom+.txt
      f2 = fopen(cheminEtNom, "w");
      ecrireReseau(R, f2);

      break;

    case '3': //Methode de l'arbre
      printf("Vous avez choisi la reconstitution du reseau a partir d'un arbre quaternaire\n");
      R = reconstitueReseauArbre(C);

			strcat(cheminEtNom, "_arbre_quat");
      afficheReseauSVG(R, cheminEtNom);
			strcat(cheminEtNom, ".txt"); //Variable qui va contenir chemi+nom+.txt
      f2 = fopen(cheminEtNom, "w");
      ecrireReseau(R, f2);

      break;

    default: //Pas de Methode 
      printf("Pas de methode existante !\n");
      printf("1 -> Methode avec liste chainee\n");
      printf("2 -> Methode table de hachage\n");
      printf("3 -> Methode avec un arbre quaternairearbre quaternaire\n");
      break;
  }
  
  /* Liberation de la Chaine et du Reseau */
  liberer_Chaines(C);
  if (R) liberer_Reseau(R);

  return 0;
}
/*-------------------------------------------------------------------------------------------------*/
