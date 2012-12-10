#include "main.h"

#define NbEquipe 21

Circuit *circuit;
Equipe** equipes;
static pthread_mutex_t* sectionVerrou;

int LOCKFILS;
int LOCKMAIN;

void* run(void* arg){
	Voiture* voiture = (Voiture*)arg;
	int sectionVisee;
	int sectionActuelle;
	int i=0;
	int temps = tempsDeplacement(voiture);	
	printf("Positionnement de la voiture %d de l'équipe %d\n",voiture->numVoiture,voiture->numEquipe);
	getClassement(circuit);
	while(voiture->nbTourEffectue <3)
	{
		sectionVisee = voiture->numSection +1;
		sectionActuelle = voiture->numSection;
		if(sectionVisee >= circuit->longueur) sectionVisee =0;
		if(entrerSection(circuit->sections[sectionVisee],voiture,&sectionVerrou[sectionVisee]))
		{
			sortirSection(circuit->sections[sectionActuelle],voiture,&sectionVerrou[sectionActuelle]);
			if(sectionVisee ==0)voiture->nbTourEffectue ++;
		}
		if(voiture->numEquipe == 1 && voiture->numVoiture == 1) getClassement(circuit);
		usleep(temps);
	}
	printf("La voiture %d de l'équipe %s est arrivée",voiture->numVoiture,voiture->numEquipe);
	return 0;
}

int main(int argc, char** argv)
{
	int i,j;
	printf("INITIALISATION DE L'ALÉATOIRE\n");
	srand(time(NULL));

	printf("INITIALISATION DES SEMAPHORES");
	initSem(2,"AZERTYUIOP",NULL);
	LOCKFILS=0;
	LOCKFILS=1;

	printf("CREATION DU CIRCUIT\n");
	circuit = newCircuit();

	printf("INITIALISATION DES VERROUS");
	sectionVerrou = malloc(sizeof(pthread_mutex_t)*circuit->longueur);

	printf("CREATION DES ÉQUIPES\n");
	equipes = creationEquipes(NbEquipe);

	printf("POSITIONNEMENT DES VOITURES");
	pthread_t pthread_id[NbEquipe*2];
	for(i=0; i<NbEquipe; i++)
	{
		entrerSection(circuit->sections[i],equipes[i]->voiture1,&sectionVerrou[i]);
		if(pthread_create(pthread_id+(2*i+0), NULL, run, (void*)equipes[i]->voiture1)<0)
			erreur("pthread_create",1);
		entrerSection(circuit->sections[i],equipes[i]->voiture2,&sectionVerrou[i]);
		if(pthread_create(pthread_id+(2*i+1), NULL, run, (void*)equipes[i]->voiture2)<0)
			erreur("pthread_create",1);
	}
	printf("DEPART DE LA COURSE\n");

	sleep(10);
	void * returnThread;
	for(i=0; i<NbEquipe; i++)
	{
		for(j=0; j<2;j++)
		{
			printf("Attente de %d\n",(2*i+j));
			if(pthread_join((pthread_t)pthread_id+(2*i+j),NULL)!=0)
				erreur("pthread_join",1);
		}
	}

	freeMain();
	return 0;
}

void freeMain()
{
	printf("SUPPRESSION DES COMPOSANTS\n");
	libereSem();
	freeCircuit(circuit);
	freeEquipes(equipes,NbEquipe);
	free(sectionVerrou);
	printf("\nà bientôt pour une nouvelle course de Nascar\n");
}
