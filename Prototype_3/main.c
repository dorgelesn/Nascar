#include "main.h"

#define NbEquipe 21

Circuit *circuit;
Equipe** equipes;
static pthread_mutex_t* sectionVerrou;
pthread_mutex_t classementVerrou;
pthread_t pthread_id[NbEquipe*2];
Voiture** classement;
int indexClassement;

int LOCKFILS;
int LOCKMAIN;

void* printClassement()
{
	 while(1)
	 {
		  getClassement(circuit);
		  usleep(200000);
	 }
}

void* run(void* arg){
	 Voiture* voiture = (Voiture*)arg;
	 int sectionVisee;
	 int sectionActuelle;
	 int i=0;
	 int temps = tempsDeplacement(voiture);	
	 printf("Positionnement de la voiture %d de l'équipe %d\n",voiture->numVoiture,voiture->numEquipe);
	 getClassement(circuit);
	 while(voiture->nbTourEffectue <1)
	 {
		  sectionVisee = voiture->numSection +1;
		  sectionActuelle = voiture->numSection;
		  if(sectionVisee >= circuit->longueur) sectionVisee =0;
		  if(entrerSection(circuit->sections[sectionVisee],voiture,&sectionVerrou[sectionVisee]))
		  {
			   sortirSection(circuit->sections[sectionActuelle],voiture,&sectionVerrou[sectionActuelle]);
			   if(sectionVisee ==0)voiture->nbTourEffectue ++;
		  }
		  usleep(temps);
	 }
	 printf("La voiture %d de l'équipe %d est arrivée\n",voiture->numVoiture,voiture->numEquipe);
	 pthread_mutex_lock(&classementVerrou);
	 classement[indexClassement] = voiture;
	 indexClassement ++;
	 pthread_mutex_unlock(&classementVerrou);
	 sectionActuelle = voiture->numSection;
	 sortirSection(circuit->sections[sectionActuelle],voiture,&sectionVerrou[sectionActuelle]);
	 pthread_exit(0);
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

	 printf("INITIALISATION DU CLASSEMENT");
	 classement = malloc(sizeof(Voiture*)*2*NbEquipe);
	 indexClassement = 0;


	 printf("CREATION DES ÉQUIPES\n");
	 equipes = creationEquipes(NbEquipe);

	 printf("POSITIONNEMENT DES VOITURES");
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

	 pthread_t pthread_classement;
	 if(pthread_create(&pthread_classement, NULL, printClassement,NULL))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe*2; i++)
		  if(pthread_join(*(pthread_id+i),NULL)!=0)
			   erreur("pthread_join",1);

	 if(pthread_cancel(pthread_classement))
		  erreur("pthread_cancel",1);

	 clear();
	 printf("Classement de la course\n");
	 for(i=0; i<NbEquipe*2; i+=2)
	 {
		printf("%d:%d \t",classement[i]->numEquipe,classement[i]->numVoiture);
		printf("%d:%d \n",classement[i+1]->numEquipe,classement[i+1]->numVoiture);
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
	 free(classement);
	 printf("\nà bientôt pour une nouvelle course de Nascar\n");
}
