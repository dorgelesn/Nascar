#ifndef VOITURE
#define VOITURE

#include "system.h"
#include "section.h"
#include <stdlib.h>
#include <stdio.h>

#define VITESSE_MAX 210

typedef struct Section Section;
typedef struct Voiture Voiture;
struct Voiture{
	int numEquipe;
	int numVoiture;
	int vitesseMax;
	int nbTourEffectue;
	int numSection;
};

Voiture* newVoiture(int numEquipe, int numVoiture);
void freeVoiture(Voiture *voiture);

#endif
