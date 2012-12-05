#ifndef VOITURE
#define VOITURE

#include "equipe.h"

typedef struct Equipe Equipe;
typedef struct Voiture Voiture;
struct Voiture{
	int acceleration;
	int vitesseMax;
	int etatPneu;;
	int etatEssence;
};

void creationVoitures(Equipe *equipe);

#endif
