#include "main.h"
char** menu(){

  int i=0;
  char buf2[1024];
  char *test[10];
  char buf[10];

  char * menu = "\t\tMenu\n";

  write(1,"\x1b[?25l",6); //On cache le curseur
  write(1,"\x1b[92m",5);  // menu en vert
  write(1,"\x1b[5m",4);
  write(1,menu,strlen(menu));
  write(1,"\x1b[0m",4);

  DIR* rep;
  struct dirent *lire;
  rep = opendir("MAP");
  if (rep == NULL){ 
    perror("Impossible d'ouvrir le flux de MAP (menu.c ): 1");
    sleep(2);
    exit(EXIT_FAILURE);
  } 

  while((lire = readdir(rep))!=NULL){
    if(strcmp(lire->d_name,".")!=0 && strcmp(lire->d_name,"..")!=0){
        if((lire->d_type)&=DT_DIR){
        //On place dans notre tableau
        test[i]=lire->d_name;
        //On affiche le numero+1
        sprintf(buf,"%d",i+1);
        write(1,buf,strlen(buf));

        write(1," - ",3);
        //et on affiche le nom du mod
        write(1,test[i],strlen(test[i]));
        write(1,"\n",2);
      }
      i++;
    }
  }

  closedir(rep);

  write(1,"Quel mods voulez vous ? (f pour quitter )\n",43);

  read(STDIN_FILENO,buf,1);
  if(strcmp(buf,"f")==0){
    menu="                 Fermeture !\n";
    write(1,"\x1b[94m",5);
    write(1,menu,strlen(menu));
    write(1,"\x1b[0m",4);
    sleep(2);
    exit(EXIT_SUCCESS);
  }

  int mod;
  mod = atoi(buf)-1;
  if (chdir("MAP") == -1){
    perror("Erreur ouverture dossier MAP (menu.c): 2");
    sleep(2);
    exit(EXIT_FAILURE);
  }
  if(chdir(test[mod]) == -1){ 
    perror("Impossible d'ouvrire ce mode (menu.c): 3");       
    sleep(2); 
    exit(EXIT_FAILURE);
  }

  int fd = open("deroulement", O_RDONLY);
  read(fd,buf2,2024);

  i=0;
  int cmpt=0,j=0;
  while(buf2[i] != '\0'){
    if(buf2[i] == '\n') cmpt++;
    i++;
  }
  cmpt--;
  char **argv = malloc(sizeof(char*)*(cmpt+2));

  argv[cmpt+1]=NULL;
  char *prec=buf2;
  char *cur=buf2;
  while (cur!=NULL){
    cur=strpbrk(prec,"\n");
    argv[j]=prec;
    j++;
    if(cur != NULL){
      *cur=0;
      prec=cur+1;
    }
  } 
  close(fd);
  if(chdir("niveaux") == -1) { 
    perror("Impossible d'ouvrir dossier niveaux (menu.c): 3");      
    sleep(2); 
    exit(EXIT_FAILURE); 
  }
  i = 0;
  char pwd[1000];
  getcwd(pwd,1000);

  write(1,test[mod],strlen(test[mod]));
  printf("\nChargement du niveau ...\n\n\n");

  sleep(1);
  return argv;

  chdir(pwd);
  i++;
}
int main(int argc, char* argv[]){
  char** argv_carte = menu();//Ouverture du dossier contenant les fichiers maps
  int i = 1;
  char* buf = malloc(sizeof(char)*1024);
  while(i != 4){
    buf = NULL;
    buf = malloc(sizeof(char)*1024);
    int fd = open(argv_carte[i], O_RDONLY);// ouverture du fichier qui contient la map
    Carte* c = init_carte(fd);//initialisation de la carte
    affiche_carte(c);
    Brique* brique = init_brique(fd);//initialisation des briques

    read(fd,buf,1);
    int p = atoi(buf);//recuperation de la vitesse
    lseek(fd,1,SEEK_CUR);
    read(fd,buf,1);
    int q = atoi(buf);
    double vitesse = (double) p / (double) q * 1000;
    lseek(fd,1,SEEK_CUR);

    read(fd,buf,2);
    int nb_brique_game = atoi(buf);//recuperation du nombres de briques du niveau

    buf = malloc(sizeof(char) * 1024);
    int deroulement[nb_brique_game+1];
    int briqueID;
    for(int k = 0; k < nb_brique_game; k++){//stockage du deroulement du niveau
      lseek(fd,1,SEEK_CUR);
      read(fd,buf,1);
      briqueID = atoi(buf);
      deroulement[k] = briqueID;
    }
    deplacement(c, brique, vitesse, deroulement, nb_brique_game);//appele de la fonction qui fait derouler le niveau
    i++;
    c = NULL; 
    brique = NULL;
    close(fd);
    if (i != 4){
    	printf("Chargement de la partie suivante\n");
   		sleep(2);
    }else{
    	printf("Merci d'avoir jouer.. Fermeture\n");	
		sleep(2);
    }
    
  }

  return 0;
}
