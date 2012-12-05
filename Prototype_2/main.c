#include "main.h"

#define NbEquipe 21

static Circuit *circuit;
static Equipe** equipes;

int forkNumEquipe;
int pere;
int i;

void creationFils(){
	int iterEquipe;
	for(iterEquipe =0;iterEquipe < NbEquipe; iterEquipe++) 
	{
		forkNumEquipe = iterEquipe +1;
		pere = forkNroll();
		if(!pere)return;
	}
}

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
	initSem(1,"Vive les courses de nascar",NULL);
	printf("INITIALISATION DES PROCESSUS FILS\n");
	creationFils();
	if(pere){for(i=0; i<21; i++)V(0);}
	else{P(0);}
	
	if(pere)printf("DEBUT DE LA PHASE DE QUALIFICATION \n");
	if(pere)printf("LES ÉQUIPES CHOISISSENT LEUR REPRÉSENTANT \n");
	if(pere)printf("FIN DE LA PHASE DE QUALIFICATION \n");

	if(pere)freeMain();
	else exit(0);
	return 0;
}

void freeMain()
{
	for(i=0; i<21; i++)	wait(NULL);
	libereSem();
	printf("SUPPRESSION DES COMPOSANTS\n");
	freeCircuit(circuit);
	freeEquipes(equipes,NbEquipe);
	clear();
	printf("SUPPRESSION DES COMPOSANTS TERMINÉE\n");
	printf("\nà bientôt pour une nouvelle course de Nascar\n");
}
