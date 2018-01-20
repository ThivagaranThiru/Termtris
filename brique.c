#include "brique.h"

Brique* init_brique(int fd){
	char* buf = malloc(sizeof(char)*BUF);

	read(fd,buf,1);
	lseek(fd,1,SEEK_CUR);
	int nb_brique = atoi(buf);

	Brique * brique = malloc(sizeof(Brique) * nb_brique);//allocation memoire pour la structure brique
	for(int i = 0; i< nb_brique; i++){
		char c[1], cptL = 0, cptH = 0;
		read(fd,buf,1);
		brique[i].largeur = atoi(buf);
		lseek(fd,1,SEEK_CUR);
		read(fd,buf,1);
		brique[i].longueur = atoi(buf);
		brique[i].data = malloc(sizeof(char) * brique[i].largeur * brique[i].longueur);// creation du champs data par les données de la brique
		lseek(fd,1,SEEK_CUR);
		do{
			read(fd,c,1);//recuperation du forme de la brique 
			databrique(brique[i],cptH,cptL) = c[0];
			cptH++;
			if(cptH == brique[i].longueur){
				lseek(fd, 1, SEEK_CUR);
				cptH = 0;
				cptL++;
			}
		} while (cptL < brique[i].largeur);
	}
	return brique;
}

void affiche_brique(Brique c){
	write(1,"\x1b[94m",5);
  for (int i = 0; i < c.largeur; i++){
    for (int j = 0; j < c.longueur; j++){
    	if (databrique(c, j, i) == '0')	printf(" ");
      	else printf("@");//printf("%c",databrique(c, j, i));
    }
    printf("\n");
  }
  write(1,"\x1b[0m",4);
}