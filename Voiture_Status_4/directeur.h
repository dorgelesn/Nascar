#ifndef DIRECTEUR
#define DIRECTEUR

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "system.h"
#include "circuit.h"
#include "voiture.h"
#include "equipe.h"

typedef struct Circuit Circuit;
typedef struct Voiture Voiture;
typedef struct Equipe Equipe;
typedef struct Directeur Directeur;
typedef struct Evenement Evenement;

struct Directeur{
	Circuit* circuit;
	Equipe** equipes;
	pthread_mutex_t* directeurVerrou;
	Evenement** evenements;
	int nbEvenement;
	int isRunning;
	int isQualification;
};
struct Evenement{
	 int status;
	 Directeur* directeur;
	 pthread_t* thread_evenement;
};

Directeur* newDirecteur(Equipe** equipes);
void freeDirecteur(Directeur* directeur);

Evenement* newEvenement(Directeur* directeur);
void freeEvenement(Evenement* evenement);

void addEvenement(Directeur* directeur,Evenement* evenement);
void cleanAllDirecteur(Directeur* directeur);
void cleanDirecteur(Directeur* directeur);
void* thread_directeur(void* arg);

void accidentMineur(Directeur* directeur);

#endif 
