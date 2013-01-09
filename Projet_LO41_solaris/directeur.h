#ifndef DIRECTEUR
#define DIRECTEUR

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "system.h"
#include "circuit.h"
#include "voiture.h"
#include "equipe.h"

struct Circuit;
struct Equipe;
struct Evenement;

typedef struct Directeur{
	struct Circuit* circuit;
	struct Equipe** equipes;
	pthread_mutex_t* directeurVerrou;
	struct Evenement** evenements;
	int nbEvenement;
	int isRunning;
	int isQualification;
	int accidentGrave;
}Directeur;

typedef struct Evenement{
	 int status;
	 struct Directeur* directeur;
	 pthread_t* thread_evenement;
}Evenement;

Directeur* newDirecteur(struct Equipe** equipes);
void freeDirecteur(struct Directeur* directeur);

Evenement* newEvenement(struct Directeur* directeur);
void freeEvenement(struct Evenement* evenement);

void addEvenement(struct Directeur* directeur,struct Evenement* evenement);
void cleanAllDirecteur(struct Directeur* directeur);
void cleanDirecteur(struct Directeur* directeur);

void* thread_directeur(void* arg);

void* thread_accidentMineur(void* arg);
void accidentMineur(struct Directeur* directeur);

void* thread_accidentGrave(void* arg);
void accidentGrave(struct Directeur* directeur);

#endif 
