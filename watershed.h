/* ------------------------- watershed.h ------------------------- */
/* Fichier d'entête de la bibliotèque de manipulation du watershed */
/* --------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include "lodepng.h"

#ifndef HACHAGE_H
#define HACHAGE_H
#include "hachage.h"

/* -------------------------------------- */
/* FONCTIONS DE MANIPULATION DU WATERSHED */
/* -------------------------------------- */

//__ Lire une image
      /*
        Cette fonction permet de convertir une image format PNG dans une variable de type myimage
        pré-condition : nom_fichier est un pointeur vers une chaîne de caractère
        pré-condition : le résultat de la fonction doit être contenu dans une variable de type myimage
        post-condition : une variable de type myimage est retournée
        post-condition : allocation de mémoire d'un tableau de pointeur de longueur "hauteur"
        post-condition : allocation de mémoire de "hauteur" tableaux de type unsigned char de longueur "largeur"
      */
myimage LireImage(const char* nom_fichier);

//__ Ecrire une image
      /*
        Cette fonction permet de convertir une variable de type myimage en une image format PNG
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        pré-condition : nom_fichier est un pointeur vers une chaîne de caractère
        post-condition : une image format PNG est créée et porte le nom de la chaîne de caractère pointé par nom_fichier
      */
void EcrireImage(myimage im, const char* nom_fichier);

//__ Allouer la mémoire pour une image
      /*
        Cette fonction permet d'initialiser une variable myimage en allouant de la mémoire d'un tableau de pointeur de longueur
         "hauteur" et de "hauteur" tableau de type unsigned char de longueur "largeur". Celle-ci retourne la variable myimage.
         pré-condition : le résultat de la fonction doit être contenu dans une variable de type myimage
         pré-condition : hauteur est une variable positive correspondant à la hauteur d'une image
         pré-condition : largeur est une variable positive correspondant à la largeur d'une image
         post-condition : on retourne une variable de type myimage
         post-condition : allocation de mémoire d'un tableau de pointeur de longueur "hauteur"
         post-condition : allocation de mémoire de "hauteur" tableaux de type unsigned char de longueur "largeur"
         post-condition : les cases des "hauteur" tableaux sont toutes initialisées à 0
      */
myimage AllouerImage(unsigned hauteur, unsigned largeur);

//__ Libération de la mémoire d'une variable myimage
      /*
        Cette fonction permet de libérer la mémoire allouée par la variable im
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        post-condition : libération de l'allocation de mémoire d'un tableau de pointeur de longueur "hauteur"
        post-condition : libération de l'allocation de mémoire de "hauteur" tableaux de type unsigned char de longueur "largeur"
      */
void LibererImage(myimage im);

//__ Calculer le Gradient d'une image
      /*
        Cette fonction permet de faire une image représentant le gradient de l'image prise en argument
        pré-condition : le résultat de la fonction doit être contenu dans une variable de type myimage
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        pré-condition : r est le rayon du calcul du gradient (r>0)
        post-condition : on retourne une variable myimage où est stocké le résultat du gradient
        post-condition : allocation de mémoire d'un tableau de pointeur de longueur "hauteur"
        post-condition : allocation de mémoire de "hauteur" tableaux de type unsigned char de longueur "largeur"
      */
myimage CalculerGradient(myimage im, uint32_t r);

//__ Calculer le Gradient d'un pixel
      /*
        Cette fonction permet de calculer le gradient d'un pixel avec un rayon r
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        pré-condition : i est le numéro de ligne de l'emplacement du pixel
        pré-condition : j est le numéro de colonne de l'emplacement du pixel
        pré-condition : r est le rayon du calcul du gradient (r>0)
        post-condition : on retourne la valeur du gradient du pixel
      */
unsigned char CalculerPixel(myimage im, uint32_t i, uint32_t j, uint32_t r);

//__ Calculer la Ligne de Partage des Eaux
      /*
        Cette fonction permet de faire une image représentant la ligne de partage des eaux de l'image
        pré-condition : im_gradient est le gradient de l'image
        pré-condition : im_gradient doit être le résultat d'un appel à la fonction CalculerGradient
        pré-condition : im_marqueur est l'image marqueur
        pré-condition : im_marqueur doit être le résultat d'un appel à la fonction LireImage
        post-condition : im_marqueur devient le résultat de la fonction
      */
void CalculerLPE(myimage im_gradient, myimage im_marqueur);

//__ Tracer le contour d'un objet
      /*
        Cette fonction permet de convertir une variable de type myimage en une image format PNG en faisant apparaître la ligne de contour des objets
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        pré-condition : im_LPE doit être le résultat de la fonction CalculerLPE
        pré-condition : nom_fichier est un pointeur vers une chaîne de caractère
        post-condition : une image format PNG est créée et porte le nom de la chaîne de caractère pointé par nom_fichier
      */
void TracerLPE(myimage im, myimage im_LPE,const char* nom_fichier);

//__ Booléen pour connaître si le pixel (i,j) a des voisins de valeurs différentes ou non
      /*
        Cette fonction retourne 1 s'il y a au moins un pixel de valeur différente de (i,j), sinon 0
        pré-condition : im_LPE doit être le résultat de la fonction CalculerLPE
        pré-condition : i est le numéro de ligne de l'emplacement du pixel
        pré-condition : j est le numéro de colonne de l'emplacement du pixel
        post-condition : un booléen est retourné
      */
_Bool est_LPE(myimage im_LPE,uint32_t i, uint32_t j);

//__ Ecrire une image en couleur
      /*
        Cette fonction permet de convertir une variable de type myimage en une image format PNG avec des pixels colorés
        pré-condition : im est au moins initialisé avec la fonction AllouerImage
        pré-condition : nom_fichier est un pointeur vers une chaîne de caractère
        post-condition : une image format PNG est créée et porte le nom de la chaîne de caractère pointé par nom_fichier
      */
void EcrireImageCouleur(myimage im, const char* nom_fichier);
#endif
