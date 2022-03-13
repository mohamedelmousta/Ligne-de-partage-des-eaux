/* ------------------------- hachage.h ------------------------- */
/* Fichier d'entête de la bibliotèque de manipulation du hachage */
/* ------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* --------------------- */
/* STRUCTURES DE DONNEES */
/* --------------------- */

typedef struct myimage{
  unsigned hauteur; // Hauteur de l'image
  unsigned largeur; // Largeur de l'image
  unsigned char** couleur; // Tableau où on range la quantité de nuance de noir de chaque pixels
} myimage;

typedef struct coord{
  uint32_t i; // Coordonnée en i d'un pixel (i,j)
  uint32_t j; // Coordonnée en j d'un pixel (i,j)
}coord;

typedef struct maillon{
  coord value; // Valeur du maillon
  struct maillon* next; // Pointeur vers le prochain maillon
}maillon;

typedef struct liste{
  uint32_t  taille; // Taille de la liste
  maillon* tete; // Pointeur vers le premier maillon de la liste
  maillon* queue; // Pointeur vers le dernier maillon de la liste
}liste;

typedef struct tab_hachage{
 liste **tab; // Pointeur vers un tableau de pointeur
 uint32_t taille; // Taille du tableau de pointeur
} tab_hachage;

/* ------------------------------------ */
/* FONCTIONS DE MANIPULATION DU HACHAGE */
/* ------------------------------------ */

//__ Initialisation d'une liste
      /*
        Cette fonction permet de créer une liste vierge
        pré-condition : le résultat de la fonction doit être contenu dans une variable de type liste*
        post-condition : un pointeur de type liste est retourné
        post-condition : allocation mémoire d'une variable de type liste
      */
liste* new_list();

//__ Initialisation d'un maillon
      /*
        Cette fonction permet de créer un maillon avec comme valeur v
        pré-condition : le résultat de la fonction doit être contenu dans une variable de type maillon*
        pré-condition : element est les coordonnées et la valeur d'un pixel (i,j)
        post-condition : un pointeur de type maillon est retourné
        post-condition : allocation mémoire d'une variable de type maillon
        post-condition : taille est initialisé à 0
      */
maillon* maillon_new(coord element);

//__ Ajout d'un maillon à la tête d'une liste
      /*
        Cette fonction permet d'ajouter un élément en tête d'une liste
        pré-condition : liste est un pointeur de type liste*
        pré-condition : liste doit au moins avoir été appellé par la fonction new_list
        pré-condition : element est les coordonnées et la valeur d'un pixel (i,j)
        post-condition : taille de la liste augmenté de 1
        post-condition : nouveau maillon ajouté dans la liste
        post-condition : allocation mémoire d'une variable de type maillon
      */
void liste_ajoutTete(liste* liste, coord element);

//__ Retrait d'un maillon à la tête d'une liste
      /*
        Cette fonction permet de retirer un élément en tête d'une liste et de retourner la valeur de l'élément
        pré-condition : liste est un pointeur de type liste*
        pré-condition : liste doit au moins avoir été appellé par la fonction new_list
        pré-condition : la liste doit au moins contenir un élément
        post-condition : taille de la liste diminué de 1
        post-condition : une variable de type coord est retournée
        post-condition : libération de d'une allocation mémoire d'une variable de type maillon
      */
coord liste_retirerTete(liste* liste);

//__ Libération de l'allocation mémoire d'une liste
      /*
        Cette fonction permet de libérer l'allocation mémoire de la liste et de ses maillons
        pré-condition : liste est un pointeur de type liste*
        pré-condition : liste doit au moins avoir été appelé par la fonction new_list
        post-condition : libération de l'allocation mémoire des maillons composant la liste
        post-condition : libération de l'allocation mémoire de la variable liste
        post-condition : la liste est vide et sa taille est égale à zéro
      */
void free_list(liste* liste);

//__ Booléen pour connaitre si la liste est vide ou pas
      /*
        Cette fonction retourne 1 si la liste est vide, sinon 0
        pré-condition : liste doit au moins avoir été appelé par la fonction new_list
        pré-condition : liste est un pointeur de type liste*
        post-condition : un booléen est retourné
      */
_Bool est_vide(liste* liste);

//__ Initialisation de la table de hachage
      /*
        Cette fonction permet d'initialiser une table de hachage
        pré-condition : taille est la taille de la table de hachage
        pré-condition : le résultat de la fonction doit être contenu dans une variable de type tab_hachage*
        post-condition : pointeur de type tab_hachage* retourné
        post-condition : allocation mémoire d'un pointeur de type tab_hachage*
        post-condition : allocation mémoire d'un tableau de pointeur de type liste*
        post-condition : allocation mémoire de "taille" liste
        post-condition : taille initialisé à 0
      */
tab_hachage* new_tab_hachage(uint32_t taille);

//__ Libération de l'allocation mémoire d'une table de hachage
      /*
        Cette fonction permet de libérer l'allocation mémoire de la table de hachage et de ses listes
        pré-condition : th est un pointeur de type tab_hachage*
        pré-condition : th doit au moins avoir été appelé par la fonction new_tab_hachage
        post-condition : libération de l'allocation mémoire du pointeur de type tab_hachage*
        post-condition : libération de l'allocation mémoire du tableau de pointeur de type liste*
        post-condition : libération de l'allocation mémoire des "taille" listes
        post-condition : taille remis à 0
        post-condition : th est une table de hachage vide
      */
void free_hachage(tab_hachage *th);

//__ Indice pour placer un élément dans la table de hachage
      /*
        Cette fonction permet de retourner l'indice de l'emplacement pour ranger l'élément d dans la table de hachage
        pré-condition : d est les coordonnées et la valeur d'un pixel (i,j)
        pré-condition : imagegradiee est le résultat de la fonction CalculerGradient
        post-condition : un indice entre 0 et 255 est retourné
      */
unsigned char hachage(coord d,myimage imagegradiee);

//__ Ajout d'un élément dans la table de hachage
      /*
        Cette fontion permet d'ajouter un élément dans la table de hachage
        pré-condition : th est un pointeur de type tab_hachage*
        pré-condition : th doit au moins avoir été appelé par la fonction new_tab_hachage
        pré-condition : d est les coordonnées et la valeur d'un pixel (i,j)
        pré-condition : imagegradiee est le résultat de la fonction CalculerGradient
        post-condition : taille d'une des listes de la table de hachage augmenté de 1
        post-condition : nouveau maillon ajouté dans une des listes de la table de hachage
        post-condition : allocation mémoire d'une variable de type maillon
      */
void add_tete_th(tab_hachage *th, coord d,myimage imagegradiee );

//__ Recherche du couple (x,y) tel que la valeur de son gradient est le minimum et retirer ce couple de la table de hachage
      /*
        Cette fonction permet de trouver une variable de type coord tel que sa valeur du gradient soit le minimum
        et de retirer le maillon de couple de la liste où elle est stockée
        pré-condition : th est un pointeur de type tab_hachage*
        pré-condition : th doit au moins avoir été appelé par la fonction new_tab_hachage
        pré-condition : th doit au moins contenir une liste non vide
        post-condition : taille d'une des listes de la table de hachage diminué de 1
        post-condition : maillon retiré dans une des listes de la table de hachage
        post-condition : libération de l'allocation mémoire d'une variable de type maillon
      */
coord rechercher_min(tab_hachage *th);

//__ Booléen pour connaitre si les listes de la table de hachage sont toutes vide ou pas
      /*
        Cette fonction retourne 1 si toutes les listes sont vide sinon 0
        pré-condition : th est un pointeur de type tab_hachage*
        pré-condition : th doit au moins avoir été appelé par la fonction new_tab_hachage
        post-condition : un booléen est retourné
      */
_Bool th_est_vide(tab_hachage* th);
