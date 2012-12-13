#include "section.h"

Section* newSection(int numSection){
	Section* section = malloc(sizeof(Section));	
	section->numSection = numSection;
	section->voitureGauche = NULL;
	section->voitureDroite = NULL;
	section->sectionVerrou = malloc(sizeof(pthread_mutex_t));
	return section;
}

int sortirSection(Section* section, Voiture* voiture)
{
	int returnValue;

	pthread_mutex_lock(&voiture->voitureVerrou);
	voiture->essenceActuelle--;
	pthread_mutex_unlock(&voiture->voitureVerrou);
	
	pthread_mutex_lock(section->sectionVerrou);
	if(voiture == section->voitureGauche)
	{
		section->voitureGauche = NULL;	
		returnValue = 1;
	}else if(voiture == section->voitureDroite)
	{
		section->voitureDroite = NULL;	
		returnValue = 1;
	} else returnValue = 0;
	pthread_mutex_unlock(section->sectionVerrou);
	return returnValue;
}

int entrerSection(Section* section, Voiture* voiture)
{
	int returnValue;
	pthread_mutex_lock(section->sectionVerrou);
	if(section->voitureDroite == NULL)
	{
		voiture->numSection = section->numSection;
		section->voitureDroite = voiture;
		returnValue = 1;
	}else if(section->voitureGauche == NULL)
	{
		voiture->numSection = section->numSection;
		section->voitureGauche = voiture;
		returnValue = 1;
	}else returnValue =0;
	pthread_mutex_unlock(section->sectionVerrou);
	return returnValue;
}

void freeSection(Section* section)
{
	free(section->sectionVerrou);
	free(section);
}

void freeSections(Section** sections, int longueur)
{
	int i;
	for(i=0; i<longueur; i++)
		freeSection(sections[i]);
	free(sections);
}
