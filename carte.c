#include "carte.h"


Carte* init_carte(int fd){
  char* buf = malloc(sizeof(char)*BUF);
  Carte* carte = malloc(sizeof(carte));
  read(fd,buf,1);
  carte->largeur = atoi (buf) * 6;
  
  lseek(fd,2,SEEK_SET);
  read(fd,buf,2);
  carte->longueur = atoi(buf);
  //printf("%d %d\n", carte->longueur, carte->largeur);
  carte->data = malloc(sizeof(char) * carte->longueur * carte->largeur);//creation de la map
  char c[1], cptL = 0, cptH = 0;
  do{//liberation d'espace de jeu
    data(carte, cptH, cptL) = '0';
    cptH++;
    if(cptH == carte->longueur){
      cptH = 0;
      cptL++;
    }
  } while (cptL < carte->largeur - (carte->largeur/6));

  lseek(fd, 1, SEEK_CUR);
  do{//copie de la map recupereé du fichier
    read(fd, c, 1); 
    data(carte, cptH, cptL) = c[0];
    cptH++;
    if(cptH == carte->longueur){
      lseek(fd, 1, SEEK_CUR);
      cptH = 0;
      cptL++;
    }
  } while (cptL < carte -> largeur);
  return carte;
}


void affiche_carte(Carte* c){
  write(1,"\x1b[34m",5);  // ecrire en bleu
  write(1,"\x1b[5m",4);
  clear();
  for (int i = 0; i < c -> largeur; i++){//affichage de la carte dans la console
    for (int j = 0; j < c -> longueur; j++){
      if (data(c, j, i) == '0')
      {
        printf(" ");
      }else if(i+1 == c ->largeur){printf("%c", data(c, j, i));
      }else printf("@");//printf("%c", data(c, j, i));
    }
    printf("\n");
  }
  write(1,"\x1b[0m",4);
}

