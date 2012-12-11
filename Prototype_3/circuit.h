#ifndef CIRCUIT
#define CIRCUIT

#include <stdio.h>
#include <stdlib.h>

#include "section.h"
#include "system.h"

typedef struct Section Section;
typedef struct Circuit Circuit;
struct Circuit{
	int longueur;
	Section** sections;
};

Circuit* newCircuit();
void freeCircuit(Circuit *circuit);

void getClassement(Circuit* circuit);

#endif
