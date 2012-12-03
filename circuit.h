#ifndef CIRCUIT
#define CIRCUIT

#include <stdio.h>
#include <stdlib.h>

typedef struct Circuit Circuit;
struct Circuit{
	int longueur;
	int ligneDePuit;
};

Circuit* creationCircuit();

#endif
