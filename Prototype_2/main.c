#include "main.h"

#define NbEquipe 21

static Circuit *circuit;
static Equipe** equipes;

int main(int argc, char** argv)
{
	printf("INITIALISATION DE L'ALÉATOIRE\n");
	initRandom();
	printf("CREATION DU CIRCUIT\n");
	circuit = newCircuit();
	printf("CREATION DES ÉQUIPES\n");
	equipes = creationEquipes(NbEquipe);
	printf("CREATION DES ÉQUIPES\n");
	clear();
	printf("INITIALISATION TERMINÉE\n");
	printf("DEBUT DE LA PHASE DE QUALIFICATION \n");
	
	printf("FIN DE LA PHASE DE QUALIFICATION \n");

	freeMain();
	return 0;
}

void freeMain()
{
	printf("SUPPRESSION DES COMPOSANTS\n");
	freeCircuit(circuit);
	freeEquipes(equipes,NbEquipe);
	clear();
	printf("SUPPRESSION DES COMPOSANTS TERMINÉE\n");
	printf("\nà bientôt pour une nouvelle course de Nascar\n");
}
