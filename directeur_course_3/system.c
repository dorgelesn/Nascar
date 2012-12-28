#include "system.h"

void clear(){printf("\033c");}

void fondVert(){printf("\033[42m     \033[0m");}

void fondGris(int equipe, int voiture){
	printf("\033[30m");
	printf("\033[47m %*d:%d\033[0m", 2, equipe, voiture);
}


void fondGrisVide(){printf("\033[47m     \033[0m");}

int aleatoire(int a, int b)
{
	return rand()%(b-a) +a;
}

void erreur(const char* message, int retour)
{
	perror(message);
	exit(retour);
}
