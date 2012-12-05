#include "system.h"

void clear(){printf("\033c");}
int aleatoire(int a, int b)
{
	return rand()%(b-a) +a;
}

void initRandom()
{
	srand(time(NULL));
}

void erreur(const char* message, int retour)
{
	perror(message);
	exit(retour);
}
