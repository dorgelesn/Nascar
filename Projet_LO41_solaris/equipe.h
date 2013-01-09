#ifndef EQUIPE
#define EQUIPE

#include "voiture.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct Voiture;

typedef struct Equipe{
	int num;
	Voiture *voiture1;	
	Voiture *voiture2;
}Equipe;

Equipe** newEquipes(int nbEquipe);
Equipe* newEquipe(int numEquipe);
Equipe** creationEquipes(int nbEquipe);
void freeEquipe(Equipe* equipe); 
void freeEquipes(Equipe** equipes, int nbEquipe); 
#endif
