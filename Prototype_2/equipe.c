#include "equipe.h"

Equipe** newEquipes(int nbEquipe)
{
	return malloc(nbEquipe * sizeof(Equipe*));
}

Equipe* newEquipe(int numEquipe)
{
	Equipe *equipe = malloc(sizeof(Equipe));
	equipe->num = numEquipe;
	equipe->voiture1 = NULL;
	equipe->voiture2 = NULL;
	return equipe;
}

Equipe** creationEquipes(int nbEquipe){
	int numEquipe;
	Equipe **equipes = newEquipes(nbEquipe); 
	for(numEquipe=1; numEquipe<=nbEquipe; numEquipe++)
	{
		equipes[numEquipe-1] = newEquipe(numEquipe);
		equipes[numEquipe-1]->voiture1 = newVoiture(numEquipe,1);
		equipes[numEquipe-1]->voiture2 = newVoiture(numEquipe,2);
	}
	return equipes;
}


void freeEquipe(Equipe* equipe)
{
	if(equipe != NULL)
	{
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

