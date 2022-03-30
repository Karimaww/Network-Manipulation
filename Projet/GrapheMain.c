#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Graphe.h"

/*-----------------------------------------------------------------------------------------*/
int main(int argc, char **argv){
  if(argc != 2){ // On verifie que l'utilisateur met bien le nom du fichier lors de l'exectution
    fprintf(stderr, "==%d== Erreur, je m'attendais a avoir 2 arguments (le main + le nom du fichier.cha)\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  FILE *f = fopen(argv[1], "r"); //Ouverture du fichier a lire
  Chaines *C = lecture_chaine(f); //Lecture de ce fichier dans C

  Reseau *R = reconstitueReseauListe(C);

  int reseau = reorganiseReseau(R); //Verification 
  
  printf("Pour le fichier %s executé:\n", argv[1]);
  if(reseau == 1) printf("[VRAI] - Pour toute arete du Graphe, le nombre de chaines qui passent < gamma\n");
  else printf("[FAUX] - Pour une arete du Graphe, le nombre de chaines >= gamma\n");

  liberer_Chaines(C);

  liberer_Reseau(R);

return 0;
}
/*-----------------------------------------------------------------------------------------*/