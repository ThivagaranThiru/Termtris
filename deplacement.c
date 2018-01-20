#include "deplacement.h"

int down(Carte* carte, int i, int j, Brique brique){
//fonction occupé par la descente elle prend la carte les coordonnées vers les quelles descendre et la brique a afficher
	int a = i, b = j;
	int old_j = j;
	bool pas_de_place = false;
	int new_i = i + brique.largeur;
	
	for (int y = 0; y < brique.longueur; y++){
    	if( data(carte, j, new_i-1) == '1' ) pas_de_place = true;
    	j++;
    }
    if (new_i == carte->largeur+1)	pas_de_place = true;
	j = old_j;
	if (pas_de_place == false){
		if (i != 0){
			for (int z = 0; z < brique.largeur; z++){
			    for (int y = 0; y < brique.longueur; y++){
			    	data(carte, b, a-1) = '0';
			    	b++;
			    }
			    b = old_j;
			    a++;
			}
		}
		for (int z = 0; z < brique.largeur; z++){
		    for (int y = 0; y < brique.longueur; y++){
	    		data(carte, j, i) = databrique(brique, y, z);
	    		j++;
		    }
		    j = old_j;
		    i++;
		}
		return 0;
	}else return 1;
} 

int gauche(Carte* carte, int i, int j, Brique brique){
//fonction occupé par le mouvement gauche elle prend la carte les coordonnées vers les quelles aller et la brique a afficher
	bool pas_de_place = false;
	int old_j = j;
	int a = i, b = j;
	int new_i = i-1;
	for (int y = 0; y < brique.largeur; y++){//gestion des collisions
    	if(data(carte, j-1, new_i) == '1' ) pas_de_place = true;
    	new_i++;
    }
    j = old_j;
	if (i != 0 && j-1 > 0 && pas_de_place == false){//effacage de la brique d'avant 
		for (int z = 0; z < brique.largeur; z++){
		    for (int y = 0; y < brique.longueur; y++){
		    	data(carte, b, a-1) = '0';
		    	b++;
		    }
		    b = old_j;
		    a++;
		}
		
		for (int z = 0; z < brique.largeur; z++){//affichage de la nouvelle position de la brique
		    for (int y = 0; y < brique.longueur; y++){
	    		data(carte, j-1, i) = databrique(brique, y, z);
		    	j++;
		    }
		    j = old_j;
		    i++;
		}
		return 0;
	}else return 1;
} 
int droite(Carte* carte, int i, int j, Brique brique){
//fonction occupé par le mouvement droite elle prend la carte les coordonnées vers les quelles aller et la brique a afficher
	bool pas_de_place = false;//devient true en cas de collision
	int old_j = j;
	int a = i, b = j;
	int new_i = i-1;
	for (int y = 0; y < brique.largeur; y++){//gestion des collisions
    	if(data(carte, j+brique.largeur+1, new_i) == '1' ) pas_de_place = true;
    	new_i++;
    }
    j = old_j;
	if (i != 0 && j+1 < carte->longueur-2 && pas_de_place == false){

		for (int z = 0; z < brique.largeur; z++){//effacage de la brique d'avant 
		    for (int y = 0; y < brique.longueur; y++){
		    	data(carte, b, a-1) = '0';
		    	b++;
		    }
		    b = old_j;
		    a++;
		}
		for (int z = 0; z < brique.largeur; z++){//affichage de la nouvelle position de la brique
		    for (int y = 0; y < brique.longueur; y++){
	    		data(carte, j+1, i) = databrique(brique, y, z);
	    		j++;
		    }
		    j = old_j;
		    i++;
		}
		return 0;
	}else return 1;
}

void deplacement (Carte* carte, Brique* brique, double vitesse, int deroulement[], int niv_brique_game){
	int y = 0;
	int i = 0, j = carte->longueur / 2 /* lancement des briques au milieu de la map*/, int_touch, brique_a_affiche = deroulement[y];
	
	char* touch = malloc(sizeof(char));//la touche enfoncé par l'utilisateur
	double vitesse_map = vitesse;//la vitesse de initial de la map
	/*POUR LE MODE NON CANONIQUE*/
	//On cree deux structure de type termios
	struct termios origine;
	struct termios new;
	//On recupere la configuration initial du terminal et on la copie dans la new structure, on modifie enfin le flag
	if (tcgetattr(STDIN_FILENO,& origine) < 0) perror("Erreur tcgetattr 1 : ");
	new = origine;
	//On le met en mode non canonique (ICANON pas positioné)
	new.c_lflag = !ICANON; 
	//et on veut 1 caractere par 1  caractere
	new.c_cc[VMIN] = 1;
	//temps qu"on a pas un caractere tapé on attend lis le man de termios vers le bas c bien expliqué, mtn on a plus besoin de taper entrer a chaque fois
	new.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO,TCSANOW,&new) < 0) perror("Erreur tcsetattr 1 : ");

	struct pollfd fds[1];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	while(1){
		clear();
		affiche_carte(carte);//affichage continue de la carte
		int_touch = poll(fds, 1, vitesse);//execution de poll avec la vitesse de la map
		touch[0] = int_touch;
		if (touch[0] == 0){//si l'utilisateur ne touche aucun bouton 
		    if((down(carte, i, j, brique[brique_a_affiche -1])) == 1){
				y++;
				brique_a_affiche = deroulement[y];
		    	i = 0;
				j = carte->longueur / 2;
				vitesse = vitesse_map;
			}
		    i++;
		}
		else read(0, touch, 1);
		//sinon on recupere le mouvement à affectuer
		if (touch[0] == 68 ) if(gauche(carte, i, j, brique[brique_a_affiche-1]) == 0){
			j--;
			i++;
		}
		if (touch[0] == 67)	if(droite(carte, i, j, brique[brique_a_affiche-1]) == 0){
			j++;
			i++;
		}
		if (touch[0] == 66)	vitesse = 80;//si c'est la touche entrée on descend plus vite
		if (touch[0] == 10)	vitesse = 0;//si c'est la touche du bas on descend immediatement
		if (touch[0] == 103 || y == niv_brique_game){//si le niveau est terminée ou c'est la touche g on passe a la map suivant
			tcsetattr(STDIN_FILENO,TCSANOW,& origine);
			return;
		}
		if (touch[0] == 102){//f pour quitter
			tcsetattr(STDIN_FILENO,TCSANOW,& origine);
			printf("Merci d'avoir jouer.. Fermeture\n");	
			sleep(3);
			exit(EXIT_SUCCESS);
		}
	}
}
