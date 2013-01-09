#ifndef CIRCUIT
#define CIRCUIT

#include <stdio.h>
#include <stdlib.h>

#include "voiture.h"
#include "equipe.h"
#include "section.h"
#include "system.h"
#include "stand.h"

struct Section;
struct Stand;
struct Voiture;
struct Equipe;

typedef struct Circuit{
	int longueur;
	int nbTour;
	struct Section** sections;
	int vitesseMax;
}Circuit;

Circuit* newCircuit();
void freeCircuit(Circuit *circuit);

void printCircuit(Circuit* circuit, struct Stand** stands);
void printClassement(struct Voiture** classement, int nbEquipe);
void getClassement(int nbEquipe, struct Voiture** classement, struct Equipe** equipes);
void echanger(struct Voiture** tableau, int a, int b);
void quickSort(struct Voiture** tableau, int debut, int fin);

#endif
