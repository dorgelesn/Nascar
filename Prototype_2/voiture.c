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

Voiture* meilleureVoiture(Voiture* v1, Voiture* v2)
{
	if(!(v1 && v2)) return NULL;
	//La meilleure voiture est la plus rapide
	if(v1->vitesseMax > v2->vitesseMax)
		return v1;
	else if(v2->vitesseMax > v1->vitesseMax)
		return v2;
	//Les vitesses sont les mêmes on compare les accelerations
	if(v1->acceleration > v2->acceleration)
		return v1;
	else if(v2->acceleration > v1->acceleration)
		return v2;
	//Si les accelerations sont égales on choisit par defaut v1;
	return v1;





}
