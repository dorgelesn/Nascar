#include "voiture.h"

void creationVoitures(Equipe *equipe)
{
	int iterVoiture;
	for(iterVoiture=1; 	iterVoiture<3; iterVoiture++)
	{
		Voiture *voiture;
		printf("création voiture %d, de l'équipe %d\n",iterVoiture,equipe->num);
		voiture->acceleration = aleatoire(50,100);
		voiture->vitesseMax = aleatoire(200,240);
		voiture->etatPneu = 100;
		voiture->etatPneu = 100;
		if(iterVoiture == 1) equipe->voiture1 = voiture;
		else          		 equipe->voiture2 = voiture;
	}
}
