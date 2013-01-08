#include "stand.h"

Stand*  newStand()
{
	 Stand* stand = malloc(sizeof(Stand));
	 stand->circuit = NULL;
	 stand->voitureStand = NULL;
	 stand->voiture1 = NULL;
	 stand->voiture2 = NULL;
	 stand->occupation =NULL;
	 stand->standVoitureVerrou = malloc(sizeof(pthread_mutex_t));
	 pthread_mutex_init(stand->standVoitureVerrou,NULL);
	 stand->tempCharge = aleatoire(80000,10000);
	 return stand;
}

Stand** creationStands(int nbEquipe)
{
	 int i;
	 Stand** stands = malloc(sizeof(Stand*)*nbEquipe);
	 for(i=0; i<nbEquipe; i++)
		  stands[i] = newStand();
	 return stands;
}

void freeStands(Stand** stands, int nbEquipe)
{
	 int i;
	 for(i =0; i<nbEquipe; i++)
	 {freeStand(stands[i]);}
	 free(stands);
}

void freeStand(Stand* stand)
{
	 if(stand != NULL)
	 {
		  free(stand->standVoitureVerrou);
		  free(stand);
	 }
}

void* standardiser(void* arg)
{
	 Stand* stand = (Stand*)arg;
	 int limiteEssence = stand->voiture1->essenceTotal / 2;
	 int nbTour = stand->circuit->nbTour;
	 while(1)
	 {
		  pthread_mutex_lock(stand->standVoitureVerrou);
		  if(stand->voitureStand==NULL)
		  {
			   if(stand->voiture1 != NULL && estEnCourse(stand->voiture1))
					if(stand->voiture1->essenceActuelle<limiteEssence)
						 stand->voitureStand = stand->voiture1;
			   if(stand->voiture2 != NULL && estEnCourse(stand->voiture2))
					if(stand->voiture2->essenceActuelle<limiteEssence)
						 stand->voitureStand = stand->voiture2;
		  }
		  pthread_mutex_unlock(stand->standVoitureVerrou);
		  usleep(200000);
	 }
}

void entreeStand(Stand* stand)
{
	 pthread_mutex_lock(stand->standVoitureVerrou);
	 stand->occupation = stand->voitureStand;
	 pthread_mutex_unlock(stand->standVoitureVerrou);
	 usleep(stand->tempCharge);
	 sleep(2);
	 pthread_mutex_lock(stand->standVoitureVerrou);
	 stand->voitureStand->essenceActuelle = stand->voitureStand->essenceTotal;
	 stand->occupation = NULL;
	 stand->voitureStand = NULL;
	 pthread_mutex_unlock(stand->standVoitureVerrou);
}
