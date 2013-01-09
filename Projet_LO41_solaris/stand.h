#ifndef STAND
#define STAND

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "voiture.h"
#include "circuit.h"

struct Voiture;
struct Circuit;

typedef struct Stand{
	 struct Circuit* circuit;
	 struct Voiture* voiture1;
	 struct Voiture* voiture2;
	 struct Voiture* voitureStand;//Voiture attendu au stand
	 struct Voiture* occupation;  //Voiture actuellement au stand
	 pthread_mutex_t* standVoitureVerrou;
	 int tempCharge;
} Stand;

Stand*  newStand();
Stand** creationStands(int nbEquipe);
void freeStand(Stand* stand);
void freeStands(Stand** stands, int nbEquipe);
void entreeStand(Stand* stand);

void* standardiser(void* arg);
#endif
