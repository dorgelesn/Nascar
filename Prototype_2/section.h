#ifndef SECTION
#define SECTIOn

#include <stdio.h>
#include <stdlib.h>
#include "voiture.h"

typedef struct Voiture Voiture;
typedef struct Section Section;
struct Section{
	Voiture* voitureGauche;
	Voiture* voitureDroite;
};

Section* newSection();
void freeSection(Section *Section);
void freeSections(Section **Sections, int longueur);
int entrerSection(Section* section, Voiture* voiture);
int sortirSection(Section* section, Voiture* voiture);

#endif
