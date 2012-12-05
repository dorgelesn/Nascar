#include "circuit.h"

Circuit* creationCircuit()
{
	Circuit *circuit = malloc(sizeof(Circuit));
	circuit->longueur = 2000;
	//Ligne de puits de 1900 à 2000
	circuit->ligneDePuit = 100;
	return circuit;
}
