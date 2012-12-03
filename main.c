#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "system.h"
#include "voiture.h"
#include "circuit.h"
#include "equipe.h"

int main(int argc, char** argv)
{
	initRandom();
	creationCircuit();
	creationEquipes();

	wait(NULL);
	return 0;
}
