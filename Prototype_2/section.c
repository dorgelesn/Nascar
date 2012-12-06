#include "section.h"

Section* newSection(int numSection){
	Section* section = malloc(sizeof(Section));	
	section->numSection = numSection;
	section->voitureGauche = NULL;
	section->voitureDroite = NULL;
	return section;
}

int sortirSection(Section* section, Voiture* voiture)
{
	V(section->numSection);
	if(voiture == section->voitureGauche)
	{
		section->voitureGauche = NULL;	
		return 1;
	}
	if(voiture == section->voitureDroite)
	{
		section->voitureDroite = NULL;	
		return 1;
	}
	return 0;
}

int entrerSection(Section* section, Voiture* voiture)
{

	if(section->voitureDroite == NULL)
	{
		P(section->numSection);
		voiture->numSection = section->numSection;
		section->voitureDroite = voiture;
		return 1;
	}
	if(section->voitureGauche == NULL)
	{
		P(section->numSection);
		voiture->numSection = section->numSection;
		section->voitureGauche = voiture;
		return 1;
	}
	return 0;
}

void freeSection(Section* section)
{
	free(section);
}

void freeSections(Section** sections, int longueur)
{
	int i;
	for(i=0; i<longueur; i++)
		freeSection(sections[i]);
	free(sections);
}
