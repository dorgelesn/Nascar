#include "section.h"

Section* newSection(){
	Section* section = malloc(sizeof(Section));	
	section->voitureGauche = NULL;
	section->voitureDroite = NULL;
	return section;
}

int sortirSection(Section* section, Voiture* voiture)
{

	if(voiture == section->voitureGauche)
	{
		section->voitureGauche = NULL;	
		return 0;
	}
	if(voiture == section->voitureDroite)
	{
		section->voitureDroite = NULL;	
		return 0;
	}
	return -1;
}

int entrerSection(Section* section, Voiture* voiture)
{

	if(section->voitureDroite == NULL)
	{
		section->voitureDroite = voiture;
		return 0;
	}
	if(section->voitureGauche == NULL)
	{
		section->voitureGauche = voiture;
		return 0;
	}
	return -1;
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
