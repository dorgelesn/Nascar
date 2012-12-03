#include "main.h"

int main(int argc, char** argv)
{
	printf("INITIALISATION DE L'ALEATOIRE\n");
	initRandom();
	printf("CREATION DU CIRCUIT\n");
	Circuit *circuit = creationCircuit();
	printf("CREATION DES EQUIPES\n");
	creationEquipes();

	wait(NULL);
	free(circuit);
	return 0;
}
