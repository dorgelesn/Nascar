#include "voiture.h"

Voiture* newVoiture(int numEquipe)
{
	printf("Création de la voiture de l'équipe %d\n",numEquipe);
	Voiture *voiture = malloc(sizeof(Voiture));
	voiture->numEquipe = numEquipe;
	voiture->acceleration = aleatoire(50,100);
	voiture->vitesseMax = aleatoire(200,240);
	voiture->etatPneu = 100;
	voiture->etatPneu = 100;
	voiture->etat = arret;
	return voiture;
}

void freeVoiture(Voiture* voiture)
{
	if(voiture != NULL)
	{
		printf("Suppression de la voiture l'équipe %d\n",voiture->numEquipe);
		free(voiture);
	}
}
