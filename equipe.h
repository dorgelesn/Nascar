#ifndef EQUIPE
#define EQUIPE

#include "voiture.h"

typedef struct Equipe Equipe;
struct Equipe{
	int num;
	struct Voiture *voiture1;	
	struct Voiture *voiture2;
};

void creationEquipes();
void initEquipe(int numEquipe);
#endif
