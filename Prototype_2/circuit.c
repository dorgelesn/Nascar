#include "circuit.h"

Circuit* newCircuit()
{
	printf("Création d'un nouveau circuit\n");
	Circuit *circuit = malloc(sizeof(Circuit));
	circuit->longueur = 100;
	circuit->sections = malloc(sizeof(Section*)*circuit->longueur);
	int i;
	for(i=0; i<100;i++)
		circuit->sections[i]=newSection();	
	return circuit;
}

void freeCircuit(Circuit *circuit)
{
	printf("Suppression du circuit\n");
	freeSections(circuit->sections, circuit->longueur);
	free(circuit);
}
