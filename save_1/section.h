#ifndef SECTION
#define SECTION

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "voiture.h"

typedef struct Voiture Voiture;
typedef struct Section Section;
struct Section{
	int numSection;
	Voiture* voitureGauche;
	Voiture* voitureDroite;
};

Section* newSection(int numSection);
void freeSection(Section *Section);
void freeSections(Section **Sections, int longueur);
int entrerSection(Section* section, Voiture* voiture,pthread_mutex_t *mutex);
int sortirSection(Section* section, Voiture* voiture,pthread_mutex_t *mutex);

#endif
