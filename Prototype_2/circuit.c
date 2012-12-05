#include "circuit.h"

Circuit* newCircuit()
{
	printf("Création d'un nouveau circuit\n");
	Circuit *circuit = malloc(sizeof(Circuit));
	circuit->longueur = 2000;
	//Ligne de puits de 1900 à 2000
	circuit->ligneDePuit = 100;
	return circuit;
}

void freeCircuit(Circuit *circuit)
{
	printf("Suppression du circuit\n");
	free(circuit);
}
