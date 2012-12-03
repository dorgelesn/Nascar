#include "equipe.h"

void initEquipe(int numEquipe)
{
	Equipe *equipe;
	equipe->num = numEquipe;
	equipe->voiture1 = NULL;
	equipe->voiture2 = NULL;
	creationVoitures(equipe);
}

void creationEquipes()
{
	int iterEquipe;
	for(iterEquipe = 1; iterEquipe < 22; iterEquipe ++)
	{
		printf("Création de l'équipe : %d\n",iterEquipe);
		switch(fork())
		{
			case -1 : erreur("problème création équipe",1);
			case 0 : 
				initEquipe(iterEquipe);
				exit(0);
			default :
					break; 
		}
	}	
}
