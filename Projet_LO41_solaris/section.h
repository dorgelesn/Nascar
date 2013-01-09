#ifndef SECTION
#define SECTION

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "voiture.h"

struct Voiture;

typedef struct Section{
	int numSection;
	struct Voiture* voitureGauche;
	struct Voiture* voitureDroite;
	pthread_mutex_t* sectionVerrou;
}Section;

Section* newSection(int numSection);
void freeSection(Section *Section);
void freeSections(Section **Sections, int longueur);
int entrerSection(Section* section, struct Voiture* voiture);
int sortirSection(Section* section, struct Voiture* voiture);

#endif
