/* ------------------------ watershed.c ------------------------ */
/* Fichier source de la bibliotèque de manipulation du watershed */
/* ------------------------------------------------------------- */

#include "watershed.h"

//__ CONSTANTE SYMBOLIQUE POUR LE DEBUGAGE
#define WS_DEBUG 0
#define I_DEBUG 0
#define CG_DEBUG 0
#define CP_DEBUG 0
#define CLPE_DEBUG 0
#define TLPE_DEBUG 0
#define BLPE_DEBUG 0

myimage LireImage(const char* nom_fichier)
{
  #if(WS_DEBUG || I_DEBUG)
  printf("%s(%s) in\n",__func__, nom_fichier);
  #endif

  myimage im;
  unsigned error;
  unsigned char* image;
  unsigned width, height;
  uint32_t i,j;

  error = lodepng_decode32_file(&image, &width, &height, nom_fichier);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  im=AllouerImage(height,width); // Allocation mémoire pour la variable im

  for(i=0;i<height;i++) // On range dans chaque tableau de couleur la quantité de couleur (correspondante) de l'image "image"
    for(j=0;j<width;j++)
      im.couleur[i][j]=image[4*(i*width + j)];

  // Libération de l'allocation de mémoire
  free(image);

  #if(WS_DEBUG ||I_DEBUG)
  printf("%s out\n",__func__);
  #endif

  return im;
}

void EcrireImage(myimage im, const char* nom_fichier)
{
  #if(WS_DEBUG || I_DEBUG)
  printf("%s(%p, %s) in\n",__func__, &im, nom_fichier);
  #endif

  unsigned char* image;
  uint32_t i,j;

  uint32_t largeur = im.largeur,hauteur = im.hauteur;
  image=malloc(4*largeur*hauteur*sizeof(unsigned char)); // Allocation mémoire

  for(i=0;i<hauteur;i++){ // On associe à chaque pixel les quantités de couleurs qui leur correspondent
    for(j=0;j<largeur;j++){
      image[4*(i*largeur + j)]=im.couleur[i][j];
      image[4*(i*largeur + j)+1]=im.couleur[i][j];
      image[4*(i*largeur + j)+2]=im.couleur[i][j];
      image[4*(i*largeur + j)+3]=255;
    }
  }

  unsigned error = lodepng_encode32_file(nom_fichier, image, im.largeur, im.hauteur);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  // Libération de l'allocation de mémoire
  free(image);

  #if(WS_DEBUG || I_DEBUG)
  printf("%s out\n",__func__);
  #endif
}

myimage AllouerImage(unsigned hauteur, unsigned largeur)
{
  #if(WS_DEBUG || I_DEBUG)
  printf("%s(%u, %u) in\n",__func__, hauteur, largeur);
  #endif

  myimage im;
  uint32_t i;

  // Allocation mémoire d'un tableau de pointeur de longueur "hauteur"
  im.couleur=malloc(hauteur*sizeof(unsigned char*));
  if(im.couleur == NULL){ // Si l'allocation a échoué
    perror("Allocation mémoire");
    assert(0);
  }

  // Allocation mémoire de tableau de unsigned char de longueur "largeur"
  for(i=0;i<hauteur;i++)
  {
    im.couleur[i]=calloc(largeur,sizeof(unsigned char)); // Les cases prennent la valeur de 0
    if(im.couleur == NULL){ // Si l'allocation a échoué
      while(i>0){ // Libération de l'allocation mémoire des tableaux précédents
        free(im.couleur[i-1]);
        i--;
      }
      free(im.couleur);
      perror("Allocation mémoire");
      assert(0);
    }
  }

  im.hauteur=hauteur;
  im.largeur=largeur;

  #if(WS_DEBUG || I_DEBUG)
  printf("%s out\n",__func__);
  #endif

  return im;
}

void LibererImage(myimage im)
{
  #if(WS_DEBUG || I_DEBUG)
  printf("%s(%p) in\n",__func__, &im);
  #endif

  uint32_t i;

  // Libération de l'allocation mémoire des tableaux
  for(i=0;i<im.hauteur;i++)
    free(im.couleur[i]);
  // Libération de l'allocation mémoire du tableau de pointeur
  free(im.couleur);

  #if(WS_DEBUG || I_DEBUG)
  printf("%s out\n",__func__);
  #endif
}

myimage CalculerGradient(myimage im, uint32_t r)
{

  #if(WS_DEBUG || CG_DEBUG)
  printf("%s(%p, %u) in\n",__func__, &im, r);
  #endif

  uint32_t i,j;
  myimage gradient=AllouerImage(im.hauteur,im.largeur); // Allocation mémoire pour la variable gradient
  // Calcul du gradient pour chaque pixel de l'image
  for(i=0;i<im.hauteur;i++)
    for (j=0;j<im.largeur;j++)
      gradient.couleur[i][j]=CalculerPixel(im,i,j,r);

  #if(WS_DEBUG || CG_DEBUG)
  printf("%s out\n",__func__);
  #endif

  return gradient;
}

unsigned char CalculerPixel(myimage im, uint32_t i, uint32_t j, uint32_t r)
{

  #if(WS_DEBUG || CP_DEBUG)
  printf("%s(%p, %d, %d, %u) in\n",__func__, &im, i, j, r);
  #endif

  uint32_t k,l;
  unsigned char max=0, min=255;

  k=i-r; // Première ligne de pixel à explorer par rapport au rayon
  while(k<=i+r && (max!=255 || min!=0)) // Tant qu'on n'a pas exploré toutes les lignes, on continue d'explorer
  {                                     // Si on trouve max = 255 et min = 0, il est inutile de continuer à chercher, car on ne trouvera pas mieux
    l=j-r; // Première colonne de pixel à explorer par rapport au rayon
    while(l<=j+r && (max!=255 || min!=0)) // Tant qu'on n'a pas exploré toutes les colonnes, on continue d'explorer
    {                                     // Si on trouve max = 255 et min = 0, il est inutile de continuer à chercher, car on ne trouvera pas mieux
      if(k>=0 && k<im.hauteur && l>=0 && l<im.largeur)
      {
        if(im.couleur[k][l]<min) // Si on trouve un meilleur min
          min=im.couleur[k][l];
        if(im.couleur[k][l]>max) // Si on trouve un meilleur max
          max=im.couleur[k][l];
      }
      l++;
    }
    k++;
  }

  #if(WS_DEBUG || CP_DEBUG)
  printf("%s out\n",__func__);
  #endif

  return (max-min); // On retourne la valeur du gradient du pixel (i,j)
}

void CalculerLPE(myimage im_gradiee, myimage im_marqueur)
{

  #if(WS_DEBUG || CLPE_DEBUG)
  printf("%s(%p, %p) in\n",__func__, &im_gradiee, &im_marqueur);
  #endif

  coord d,aux;
  uint32_t k,l;
  tab_hachage* L = new_tab_hachage(256);
  for(d.i=0;d.i<im_marqueur.hauteur;d.i++){ // On explore tous les pixels
    for(d.j=0;d.j<im_marqueur.largeur;d.j++){
      if (im_marqueur.couleur[d.i][d.j]!=0) // Si le pixel est sur le marqueur, on ajoute le couple dans la liste L
        add_tete_th(L,d,im_gradiee); // On ajoute dans la table de hachage le pixel (i,j)
    }
  }
  while (!th_est_vide(L)) // Tant qu'on n'a pas traité tous les pixels (x,y)
  {
    d=rechercher_min(L); // On prend le couple (x,y), tel que la valeur du gradient soit le minimum de la liste
    for(k=d.i-1;k<=d.i+1;k++){ // On explore les pixels voisins au pixel (i,j)
      for(l=d.j-1;l<=d.j+1;l++){
        if(k>=0 && k<im_marqueur.hauteur && l>=0 && l<im_marqueur.largeur) // Si ce pixel existe bien dans l'image
        {
          if(im_marqueur.couleur[k][l]==0){  // Si le pixel (k,l) n'a pas encore été considéré comme à l'intérieur ou à l'extérieur
            aux.i=k;
            aux.j=l;
            im_marqueur.couleur[k][l]=im_marqueur.couleur[d.i][d.j]; // Le pixel (k,l) prend la coloration du pixel (i,j)
            add_tete_th(L,aux,im_gradiee); // On ajoute ce pixel dans la liste (pour regarder ses voisins)
          }
        }
      }
    }
  }

  free_hachage(L);

  #if(WS_DEBUG || CLPE_DEBUG)
  printf("%s out\n",__func__);
  #endif
}

void TracerLPE(myimage im, myimage im_LPE,const char* nom_fichier)
{
  #if(WS_DEBUG || TLPE_DEBUG)
  printf("%s(%p, %p, %s) in\n",__func__, &im, &im_LPE, &nom_fichier);
  #endif

  unsigned char* image;
  uint32_t i,j;
  uint32_t largeur = im.largeur,hauteur = im.hauteur;
  image=malloc(4*largeur*hauteur*sizeof(unsigned char)); // Allocation mémoire
  for(i=0;i<hauteur;i++){ // On associe à chaque pixels les quantités de couleurs qui leurs correspondent
    for(j=0;j<largeur;j++){
      if(est_LPE(im_LPE,i,j)){ // Si le pixel (i,j) est dans le contour, on colore le pixel en rouge sinon on laisse le pixel d'origine
        image[4*(i*largeur + j)]=255;
        image[4*(i*largeur + j)+1]=0;
        image[4*(i*largeur + j)+2]=0;
      }
      else{
        image[4*(i*largeur + j)]=im.couleur[i][j];
        image[4*(i*largeur + j)+1]=im.couleur[i][j];
        image[4*(i*largeur + j)+2]=im.couleur[i][j];
      }
      image[4*(i*largeur + j)+3]=255;
    }
  }

  unsigned error = lodepng_encode32_file(nom_fichier, image, im.largeur, im.hauteur);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  // Libération de l'allocation de mémoire
  free(image);

  #if(WS_DEBUG || TLPE_DEBUG)
  printf("%s out\n",__func__);
  #endif
}

_Bool est_LPE(myimage im_LPE, uint32_t i, uint32_t j)
{
  #if(WS_DEBUG || BLPE_DEBUG)
  printf("%s(%p, %u, %u) in\n",__func__, &im_LPE, i, j);
  #endif

  uint32_t k,l;
  for(k=i-1;k<=i+1;k++)
    for(l=j-1;l<=j+1;l++)
      if(k>=0 && k<im_LPE.hauteur && l>=0 && l<im_LPE.largeur)
        if(im_LPE.couleur[k][l]!=im_LPE.couleur[i][j]){
          #if(WS_DEBUG || BLPE_DEBUG)
          printf("%s out\n",__func__);
          #endif
          return 1;
        }

  #if(WS_DEBUG || BLPE_DEBUG)
  printf("%s out\n",__func__);
  #endif

  return 0;
}

void EcrireImageCouleur(myimage im, const char* nom_fichier)
{
  #if(WS_DEBUG || I_DEBUG)
  printf("%s(%p, %s) in\n",__func__, &im, nom_fichier);
  #endif

  unsigned char* image;
  uint32_t i,j;
  _Bool valeur[256]={0};
  unsigned char r[256],v[256],b[256];

  uint32_t largeur = im.largeur,hauteur = im.hauteur;
  image=malloc(4*largeur*hauteur*sizeof(unsigned char)); // Allocation mémoire

  for(i=0;i<hauteur;i++){ // On regarde les valeurs qu'on a dans l'image
    for(j=0;j<largeur;j++){
      if(valeur[im.couleur[i][j]]==0){
        valeur[im.couleur[i][j]]=1;
      }
    }
  }
  for(i=0;i<256;i++){ // On crée des pixels de couleur pour chaque pixel qu'on a dans l'image
    if(valeur[i]){
      r[i]=rand()%256;
      v[i]=rand()%256;
      b[i]=rand()%256;
    }
  }

  for(i=0;i<hauteur;i++){ // On associe à chaque pixel les quantités de couleurs qui leur correspondent
    for(j=0;j<largeur;j++){
      image[4*(i*largeur + j)]=r[im.couleur[i][j]];
      image[4*(i*largeur + j)+1]=v[im.couleur[i][j]];
      image[4*(i*largeur + j)+2]=b[im.couleur[i][j]];
      image[4*(i*largeur + j)+3]=255;
    }
  }

  unsigned error = lodepng_encode32_file(nom_fichier, image, im.largeur, im.hauteur);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  // Libération de l'allocation de mémoire
  free(image);

  #if(WS_DEBUG || I_DEBUG)
  printf("%s out\n",__func__);
  #endif
}
