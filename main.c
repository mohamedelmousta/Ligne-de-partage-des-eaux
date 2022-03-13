#include "watershed.h"
#include <time.h>

int main(int argc, const char *argv[])
{
  if(argc<3) // S'il manque un ou des paramètres
  {
    printf("Executer le programme de cette façon :\n ./watershed [nom_image] [nom_image_marqueur] [nom_image_sortie]\n");
    return EXIT_FAILURE;
  }

  srand(time(NULL));

  double time_spent = 0.0;
  clock_t begin = clock();
  clock_t end;

  myimage image;
  myimage im_gradiee;
  myimage im_marqueur;

  image=LireImage(argv[1]); // Lecture de l'image en noir et blanche
  im_marqueur=LireImage(argv[2]); // Lecture de l'image où il y a les marqueurs
  im_gradiee =CalculerGradient(image,1); // Calcul du gradient de l'image
  CalculerLPE(im_gradiee,im_marqueur); // Calcul de la Ligne de Partage des Eaux de l'image
  EcrireImage(im_marqueur,argv[3]); // Ecriture de l'image où il y a la Ligne de Partage des Eaux
  //TracerLPE(image,im_marqueur,argv[3]);
  //EcrireImageCouleur(im_marqueur,argv[3]);

  // Libération de l'allocation mémoire
  LibererImage(image);
  LibererImage(im_gradiee);
  LibererImage(im_marqueur);

  end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Le programme a pris %f ms\n",time_spent*1000);

  return EXIT_SUCCESS;
}
