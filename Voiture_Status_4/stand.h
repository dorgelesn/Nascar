#ifndef STAND
#define STAND

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "voiture.h"
#include "circuit.h"

typedef struct Circuit Circuit;
typedef struct Voiture Voiture;
typedef struct Stand Stand;
struct Stand{
	 Circuit* circuit;
	 Voiture* voiture1;
	 Voiture* voiture2;
	 Voiture* voitureStand;
	 pthread_mutex_t* standVoitureVerrou;
	 int tempCharge;
};

Stand*  newStand();
Stand** creationStands(int nbEquipe);
void freeStand(Stand* stand);
void freeStands(Stand** stands, int nbEquipe);
void entreeStand(Stand* stand);

void* standardiser(void* arg);
#endif
