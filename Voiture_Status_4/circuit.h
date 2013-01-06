#ifndef CIRCUIT
#define CIRCUIT

#include <stdio.h>
#include <stdlib.h>

#include "voiture.h"
#include "equipe.h"
#include "section.h"
#include "system.h"
#include "stand.h"

typedef struct Section Section;
typedef struct Voiture Voiture;
typedef struct Equipe Equipe;
typedef struct Circuit Circuit;
struct Circuit{
	int longueur;
	int nbTour;
	Section** sections;
	int vitesseMax;
};

Circuit* newCircuit();
void freeCircuit(Circuit *circuit);

void printCircuit(Circuit* circuit, Stand** stands);
void printClassement(Voiture** classement, int nbEquipe);
void getClassement(int nbEquipe, Voiture** classement, Equipe** equipes);
void echanger(Voiture** tableau, int a, int b);
void quickSort(Voiture** tableau, int debut, int fin);

#endif
