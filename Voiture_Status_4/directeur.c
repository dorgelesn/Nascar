#include "directeur.h"

Directeur* newDirecteur(Equipe** equipes)
{
	 Directeur* directeur = malloc(sizeof(Directeur));
	 directeur->circuit = NULL;
	 directeur->equipes = equipes;
	 directeur->directeurVerrou = malloc(sizeof(pthread_mutex_t));
	 pthread_mutex_init(directeur->directeurVerrou,NULL);
	 directeur->evenements  = NULL;
	 directeur->nbEvenement = 0;
	 directeur->isRunning = 0;
	 return directeur;
}

void freeDirecteur(Directeur* directeur)
{
	 if(directeur)
	 {
		  cleanAllDirecteur(directeur);
		  if(directeur->evenements) free(directeur->evenements);
		  pthread_mutex_destroy(directeur->directeurVerrou);
		  free(directeur->directeurVerrou);
		  free(directeur);
	 }
}

Evenement* newEvenement(Directeur* directeur)
{
	 Evenement* evenement = malloc(sizeof(Evenement));
	 evenement->status =0;
	 evenement->directeur= directeur;
	 evenement->thread_evenement = malloc(sizeof(pthread_t));
	 return evenement;
}

void freeEvenement(Evenement* evenement)
{
	 if(evenement)
	 {
		  free(evenement->thread_evenement);
		  free(evenement);
	 }
}

void addEvenement(Directeur* directeur,Evenement* evenement)
{
	 directeur->nbEvenement ++;
	 Evenement** tmp = malloc(sizeof(Evenement)*directeur->nbEvenement);
	 int i;
	 for(i=0; i<directeur->nbEvenement-1; i++)
		  tmp[i]=directeur->evenements[i];
	 tmp[directeur->nbEvenement-1] = evenement;
	 if(directeur->evenements != NULL)free(directeur->evenements);
	 directeur->evenements = tmp;	
}

void cleanAllDirecteur(Directeur* directeur)
{
	 int i;
	 pthread_mutex_lock(directeur->directeurVerrou);
	 for(i=0; i<directeur->nbEvenement; i++)
		  directeur->evenements[i]->status =0;
	 pthread_mutex_unlock(directeur->directeurVerrou);
	 cleanDirecteur(directeur);
}

void cleanDirecteur(Directeur* directeur)
{
	 pthread_mutex_lock(directeur->directeurVerrou);
	 Evenement** evenements = directeur->evenements;
	 int i;
	 int evenementActif=0;
	 for(i=0; i<directeur->nbEvenement; i++)
	 {
		  if(evenements[i] != NULL)
		  {
			   if(evenements[i]->status == 1)	evenementActif++;
			   else
			   {
					freeEvenement(evenements[i]);
					evenements[i] = NULL;
			   }
		  }
	 }
	 if(evenementActif != directeur->nbEvenement && directeur->nbEvenement != 0)
	 {
		  int indexTmp= 0;
		  Evenement** tmp = NULL;
		  if(evenementActif !=0)
		  {
			   tmp = malloc(sizeof(Evenement*)*evenementActif);
			   for(i=0;i<directeur->nbEvenement; i++)
			   {
					if(evenements[i] != NULL)
					{
						 tmp[indexTmp] = directeur->evenements[i];
						 indexTmp++;
					}
			   }
		  }
		  if(directeur->evenements) free(directeur->evenements);
		  directeur->evenements = tmp;
		  directeur->nbEvenement = evenementActif;
	 }
	 pthread_mutex_unlock(directeur->directeurVerrou);
}

void* thread_directeur(void* arg)
{
	 Directeur* directeur = (Directeur*) arg;
	 while(directeur->isRunning ==1)
	 {
		  cleanDirecteur(directeur);
		  //accident aléatoire
		  usleep(800);
	 }
}

void* thread_accidentMineur(void* arg)
{
	 Evenement* evenement = (Evenement*) arg;
	 evenement->status = 1;
	 evenement->directeur->circuit->vitesseMax = 200;
	 sleep(aleatoire(10,20));
	 evenement->directeur->circuit->vitesseMax = 210;
	 evenement->status = 0;
}

void accidentMineur(Directeur* directeur)
{
	 pthread_mutex_lock(directeur->directeurVerrou);
	 Evenement* evenement = newEvenement(directeur);
	 if(pthread_create(evenement->thread_evenement, NULL,thread_accidentMineur,(void*)evenement)<0)
		  erreur("pthread_create",1);
	 addEvenement(directeur,evenement);
	 pthread_mutex_unlock(directeur->directeurVerrou);
}
