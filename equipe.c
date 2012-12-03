#include "equipe.h"

Equipe* initEquipe(int numEquipe)
{
	Equipe *equipe = malloc(sizeof(Equipe));
	equipe->num = numEquipe;
	equipe->voiture1 = NULL;
	equipe->voiture2 = NULL;
	return equipe;
}

void creationEquipes()
{
	int iterEquipe;
	Equipe *equipe;
	for(iterEquipe = 1; iterEquipe < 22; iterEquipe ++)
	{
		printf("Création de l'équipe : %d\n",iterEquipe);
		switch(fork())
		{
			case -1 : erreur("problème création équipe",1);
			case 0 : 
					  equipe = initEquipe(iterEquipe);
					  creationVoitures(equipe);
					  freeEquipe(equipe);
					  exit(0);
			default :
					  break; 
		}
	}	
}

void freeEquipe(Equipe* equipe)
{
	free(equipe->voiture1);
	free(equipe->voiture2);
	free(equipe);
}
