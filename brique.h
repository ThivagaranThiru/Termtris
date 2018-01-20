#ifndef __BRIQUE_H__
#define __BRIQUE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define BUF 1024

typedef struct Brique{//structure brique
  int largeur, longueur;
  char* data;
}Brique;

#define databrique(brique,i,j) ((brique).data[(j) * (brique).longueur + (i)])//renvoi les données situant dans brique[i][j]

Brique* init_brique(int fd);
void affiche_brique(Brique c);

#endif