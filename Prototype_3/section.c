#include "section.h"

Section* newSection(int numSection){
	Section* section = malloc(sizeof(Section));	
	section->numSection = numSection;
	section->voitureGauche = NULL;
	section->voitureDroite = NULL;
	return section;
}

int sortirSection(Section* section, Voiture* voiture, pthread_mutex_t *mutex)
{
	int returnValue;
	pthread_mutex_lock(mutex);
	if(voiture == section->voitureGauche)
	{
		section->voitureGauche = NULL;	
		returnValue = 1;
	}else if(voiture == section->voitureDroite)
	{
		section->voitureDroite = NULL;	
		returnValue = 1;
	} else returnValue = 0;
	pthread_mutex_unlock(mutex);
	return returnValue;
}

int entrerSection(Section* section, Voiture* voiture,pthread_mutex_t *mutex)
{
	int returnValue;
	pthread_mutex_lock(mutex);
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
	pthread_mutex_unlock(mutex);
	return returnValue;
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
