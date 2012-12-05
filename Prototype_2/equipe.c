#include "equipe.h"

Equipe** newEquipes(int nbEquipe)
{
	return malloc(nbEquipe * sizeof(Equipe*));
}

Equipe* newEquipe(int numEquipe)
{
	printf("Création de l'équipe %d\n",numEquipe);
	Equipe *equipe = malloc(sizeof(Equipe));
	equipe->num = numEquipe;
	equipe->voiture1 = NULL;
	equipe->voiture2 = NULL;
	return equipe;
}

void freeEquipe(Equipe* equipe)
{
	if(equipe != NULL)
	{
		printf("Suppression de l'équipe %d\n",equipe->num);
		freeVoiture(equipe->voiture1);
		freeVoiture(equipe->voiture2);
		free(equipe);
	}
}

void freeEquipes(Equipe** equipes, int nbEquipe)
{
	if(equipes != NULL)
	{
		int i;
		for(i=0; i<nbEquipe; i++)
			freeEquipe(equipes[i]);
		free(equipes);
	}
}
