#ifndef VOITURE
#define VOITURE

#include "system.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum ETAT ETAT;
enum ETAT{arret, acceleration, max, stand};
typedef struct Voiture Voiture;
struct Voiture{
	int numEquipe;
	int acceleration;
	int vitesseMax;
	int etatPneu;;
	int etatEssence;
	ETAT etat;
};

Voiture* newVoiture(int numEquipe);
void freeVoiture(Voiture *voiture);
Voiture* meilleureVoiture(Voiture* v1, Voiture* v2);

#endif
