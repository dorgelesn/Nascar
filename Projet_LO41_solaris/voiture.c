#include "voiture.h"

Voiture* newVoiture(int numEquipe,int numVoiture)
{
	Voiture *voiture = malloc(sizeof(Voiture));
	voiture->numEquipe = numEquipe;
	voiture->numVoiture = numVoiture;
	voiture->vitesseMax = aleatoire(200,VITESSE_MAX);
	voiture->nbTourEffectue = 0;
	voiture->numSection = 0;
	voiture->essenceTotal = 610;
	voiture->essenceActuelle = 0;
	voiture->deplacementTotal = 0;
	voiture->status = 0;
	voiture->voitureVerrou = malloc(sizeof(pthread_mutex_t));
	voiture->vitesseActuelle = voiture->vitesseMax;
	pthread_mutex_init(voiture->voitureVerrou,NULL);
	return voiture;
}

void freeVoiture(Voiture* voiture)
{
	if(voiture != NULL)
	{
		pthread_mutex_destroy(voiture->voitureVerrou);
		free(voiture->voitureVerrou);
		free(voiture);
	}
}

int estEnCourse(Voiture* voiture)
{
	if(voiture == NULL) return 0;
	if(voiture->status == 1) return 1;
	if(voiture->status == 2) return 1;
	if(voiture->status == -2) return 1;
	return 0;
}

int tempsDeplacement(Voiture* voiture)
{
	return (VITESSE_MAX + 1 - voiture->vitesseActuelle)*100000;
}
