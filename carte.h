#ifndef __CARTE_H__
#define __CARTE_H__

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF 1024
typedef struct Carte{//structure carte
  int largeur, longueur;
  char * data;
}Carte;
#define clear() printf("\033[H\033[J")//efface la console

#define data(carte,i,j) ((carte)->data[(j) * (carte)->longueur + (i)]) //renvoi les données situant dans carte[i][j]

Carte* init_carte(int fd);
void affiche_carte(Carte* c);

#endif