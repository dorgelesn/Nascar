#include "main.h"

#define NbEquipe 21

static Circuit *circuit;
static Equipe **equipes;

void creationEquipes(){
	int numEquipe;
	equipes = newEquipes(NbEquipe); 
	for(numEquipe=1; numEquipe<=NbEquipe; numEquipe++)
	{
		equipes[numEquipe-1] = newEquipe(numEquipe);
		equipes[numEquipe-1]->voiture1 = newVoiture(numEquipe);
		equipes[numEquipe-1]->voiture2 = newVoiture(numEquipe);
	}
}

int main(int argc, char** argv)
{
	printf("INITIALISATION DE L'ALÉATOIRE\n");
	initRandom();
	printf("CREATION DU CIRCUIT\n");
	circuit = newCircuit();
	printf("CREATION DES ÉQUIPES\n");
	creationEquipes();

	freeMain();
	return 0;
}

void freeMain()
{
	freeCircuit(circuit);
	freeEquipes(equipes,NbEquipe);
}
