#ifndef EQUIPE
#define EQUIPE

#include "voiture.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct Voiture Voiture;
typedef struct Equipe Equipe;
struct Equipe{
	int num;
	Voiture *voiture1;	
	Voiture *voiture2;
};

void creationEquipes();
Equipe* initEquipe(int numEquipe);
void freeEquipe(Equipe* equipe); 
#endif
