/* ------------------------ hachage.c ------------------------ */
/* Fichier source de la bibliotèque de manipulation du hachage */
/* ----------------------------------------------------------- */

#ifndef HACHAGE_H
#define HACHAGE_H
#include "hachage.h"

liste* new_list(){
  liste* l=malloc(sizeof(liste)); // Allocation mémoire
  assert(l!=NULL); // On s'assure du bon déroulement de l'allocation
  // Initialisation des données de la liste(taille,tête,queue)
  l->taille=0;
  l->tete=NULL;
  l->queue=NULL;
  return l; // Renvoie un pointeur vers la liste initialisée
}

maillon* new_maillon(coord element){
  maillon* r = malloc(sizeof(maillon)); // Allocation de la mémoire
  assert(r!=NULL); // On s'assure du bon déroulement de l'allocation de mémoire
  // Initialisation des données du maillon(valeur,prochain)
  r->value = element;
  r->next=NULL;
  return r; // Renvoie un pointeur vers le maillon initialisé
}

void liste_ajoutTete(liste* liste, coord element){
  maillon* maillon = new_maillon(element);// on crée un maillon
  assert(liste!=NULL); // On s'assure que le pointeur donné en argument pointe bien vers une liste non NULL
  assert(maillon!=NULL); // On s'assure que la création du maillon s'est bien déroulée

  maillon->next=liste->tete; // Le nouveau maillon pointe vers la tête de la liste
  liste->tete=maillon; // Le nouveau maillon devient la tête de liste
  if(liste->queue==NULL)
    liste->queue=liste->tete;
  liste->taille++; // On incrémente la taille de la liste
}

coord liste_retirerTete(liste* liste){
  coord element;
  maillon* maillon;
  assert(liste!=NULL && liste->taille>0); // On s'assure qu'on a bien une liste non vide
  // On délie le maillon à la liste
  maillon = liste->tete;
  liste->tete = liste->tete->next;

  if(liste->tete == NULL)
    liste->queue = NULL;
  liste->taille -=1; // Décrémentation de la taille de la liste

  element = maillon->value;
  free(maillon); // Libération de l'allocation mémoire du maillon

  return element; // Renvoie la donnée du maillon retiré
}

void free_list(liste* liste){
  while(!est_vide(liste)) // Tant que la liste n'est pas vide
    liste_retirerTete(liste); // On retire les maillons
  free(liste); // On libère l'espace alloué à ces maillons
}

_Bool est_vide(liste* liste){
  return liste->taille == 0; // Si la liste est vide, on retourne 1 sinon 0
}

tab_hachage* new_tab_hachage(uint32_t taille){
 uint32_t i;

 tab_hachage *th = malloc(sizeof(tab_hachage)); // Allocation mémoire
 assert(th != NULL); // Si l'allocation n'a pas fonctionné

 th->tab = malloc(taille*sizeof(liste*)); // Allocation mémoire
 assert(th->tab != NULL); // Si l'allocation n'a pas fonctionné

 for(i=0;i<taille;i++) // Assignation d'une liste vide dans chaque case du tableau de pointeur de liste
  th->tab[i] = new_list();

 th->taille = taille; // Taille de la table de hachage
 return th;
}

void free_hachage(tab_hachage *th){
  uint32_t i;
  for (i=0;i<th->taille;i++) // Libération de l'allocation mémoire de chaque liste
     free_list(th->tab[i]);
  free(th->tab); // Libération de l'allocation mémoire du pointeur du tableau de pointeur de liste
  free(th); // Libération de l'allocation mémoire du pointeur du pointeur du tableau de pointeur de liste
}

unsigned char hachage(coord d,myimage imagegradiee){
  return  imagegradiee.couleur[d.i][d.j]; // Retourne l'indice de la liste concernée
}

void add_tete_th( tab_hachage *th, coord d, myimage imagegradiee){
  liste_ajoutTete(th->tab[hachage(d,imagegradiee)],d); // Ajout dans la liste concernée de l'élément d
}

coord rechercher_min( tab_hachage *th){
  uint32_t i=0;
  while(est_vide(th->tab[i])) // Tant qu'on ne trouve pas une liste non vide
    i++;
  return liste_retirerTete(th->tab[i]); // Retourne un couple (i,j) et sa valeur et on retire de la liste concernée
}

_Bool th_est_vide(tab_hachage* th){
  uint32_t i=0;
  while(i<th->taille){ // Tant qu'on n'a pas regardé toutes les listes
    if(!est_vide(th->tab[i])) // Si la i-ème liste n'est pas vide, on retourne 0
      return 0;
    i++;
  }
  return 1; // Toutes les listes sont vides, on retourne 1
}

#endif
