#include "main.h"

#define NbEquipe 21

Circuit *circuit;
Equipe** equipes;
Stand** stands;
Voiture** classement;
int indexClassement;

pthread_mutex_t  *pauseVerrou;
pthread_mutex_t  *classementVerrou;

pthread_t *pthread_classement;
pthread_t **pthread_voiture;
pthread_t **pthread_stand;

int LOCKFILS;
int LOCKMAIN;
static pid_t mainPID;

void signalSIGTSTP(int num)
{
	 int i;
	 if(num == SIGTSTP)
	 {
		  for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_lock(&pauseVerrou[i]);
		  switch(showMenu())
		  {
			   default:
					clear();	
			   case 0:
					for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_unlock(&pauseVerrou[i]);
					kill(mainPID,SIGINT);
					break;
			   case 1:
					for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_unlock(&pauseVerrou[i]);
					break;
		  }
		  signal(SIGTSTP,signalSIGTSTP);
	 }
}

void signalSIGINT(int num)
{
	 int i;
	 if(num == SIGINT)
	 {
		  printf("Vous avez demandé l'arret de la course avant sa terminaison\n\n");	
		  killThread();
		  freeMain();
		  signal(SIGINT,SIG_DFL);
		  printf("BYE\n");
		  kill(mainPID,SIGINT);
	 }
	 return;
}

void* printClassement()
{
	 while(1)
	 {
		  pthread_mutex_lock(&pauseVerrou[NbEquipe*2]);
		  getClassement(circuit);
		  pthread_mutex_unlock(&pauseVerrou[NbEquipe*2]);
		  usleep(300000);
	 }
}

void* run(void* arg){
	 Voiture* voiture = (Voiture*)arg;
	 int sectionActuelle = voiture->numSection;
	 int sectionVisee = sectionActuelle +1;
	 int i=0;
	 int temps = tempsDeplacement(voiture);	
	 int id = ((voiture->numEquipe-1)*2)+(voiture->numVoiture -1);
	 voiture->essenceActuelle = voiture->essenceTotal;
	 Stand* stand = stands[voiture->numEquipe-1];
	 printf("Positionnement de la voiture %d de l'équipe %d\n",voiture->numVoiture,voiture->numEquipe);
	 V(LOCKMAIN);
	 P(LOCKFILS);
	 while(voiture->nbTourEffectue < circuit->nbTour)
	 {
		  if(voiture->essenceActuelle)
		  {
			   pthread_mutex_lock(&pauseVerrou[id]);
			   if(sectionVisee == 0 && stand->voitureStand == voiture)
			   {
					sortirSection(circuit->sections[sectionActuelle],voiture);
					entreeStand(stand);
					sectionActuelle = -1;
			   }
			   if(entrerSection(circuit->sections[sectionVisee],voiture))
			   {
					if(sectionActuelle != -1)
					{sortirSection(circuit->sections[sectionActuelle],voiture);}
					sectionActuelle = sectionVisee;
					sectionVisee = sectionActuelle +1;
					if(sectionVisee >= circuit->longueur)
					{
						 voiture->nbTourEffectue ++;
						 sectionVisee =0;
					}

			   }
			   pthread_mutex_unlock(&pauseVerrou[id]);
		  }
		  usleep(temps);

	 }
	 printf("La voiture %d de l'équipe %d est arrivée\n",voiture->numVoiture,voiture->numEquipe);
	 pthread_mutex_lock(classementVerrou);
	 classement[indexClassement] = voiture;
	 indexClassement ++;
	 pthread_mutex_unlock(classementVerrou);
	 sectionActuelle = voiture->numSection;
	 sortirSection(circuit->sections[sectionActuelle],voiture);
	 pthread_exit(0);
}

int main(int argc, char** argv)
{
	 int i,j;
	 mainPID = getpid();
	 printf("INITIALISATION DE L'ALÉATOIRE\n");
	 srand(time(NULL));

	 printf("INITIALISATION DES VERROUS\n");
	 pauseVerrou = malloc(sizeof(pthread_mutex_t)*(NbEquipe*2+1));
	 for(i=0; i<(NbEquipe*2+1); i++)
		  pthread_mutex_init(&pauseVerrou[i],NULL);
	 classementVerrou = malloc(sizeof(pthread_mutex_t));
	 pthread_mutex_init(classementVerrou,NULL);



	 printf("INITIALISATION DES SEMAPHORES\n");
	 signal(SIGINT,signalSIGINT);
	 signal(SIGTSTP,signalSIGTSTP);

	 printf("INITIALISATION DES SEMAPHORES\n");
	 initSem(2,"AZERTYUIOP",NULL);
	 LOCKFILS=0;
	 LOCKMAIN=1;

	 printf("CREATION DU CIRCUIT\n");
	 circuit = newCircuit();
	 if(argc ==2)circuit->nbTour = atoi(argv[1]);

	 printf("INITIALISATION DU CLASSEMENT\n");
	 classement = malloc(sizeof(Voiture*)*2*NbEquipe);
	 indexClassement = 0;

	 printf("CREATION DES ÉQUIPES\n");
	 equipes = creationEquipes(NbEquipe);

	 printf("CREATION DES STANDS\n");
	 stands = creationStands(NbEquipe); 

	 printf("INITIALISATION DES THREADSi\n");
	 pthread_classement = malloc(sizeof(pthread_t));
	 pthread_voiture = malloc(sizeof(pthread_t*)*(NbEquipe*2));
	 for(i =0; i<NbEquipe*2; i++)
		  pthread_voiture[i] = malloc(sizeof(pthread_t));
	 pthread_stand = malloc(sizeof(pthread_t*)*NbEquipe);
	 for(i =0; i<NbEquipe; i++)
		  pthread_stand[i] = malloc(sizeof(pthread_t));

	 printf("POSITIONNEMENT DES VOITURES\n");
	 for(i=0; i<NbEquipe; i++)
	 {
		  entrerSection(circuit->sections[i],equipes[i]->voiture1);
		  if(pthread_create(pthread_voiture[2*i+0], NULL, run, (void*)equipes[i]->voiture1)<0)
			   erreur("pthread_create",1);
		  entrerSection(circuit->sections[i],equipes[i]->voiture2);
		  if(pthread_create(pthread_voiture[2*i+1], NULL, run, (void*)equipes[i]->voiture2)<0)
			   erreur("pthread_create",1);
	 }

	 printf("PRÉPARATION DES STANDS\n");
	 for(i=0; i<NbEquipe; i++)
	 {
		  stands[i]->circuit = circuit;
		  stands[i]->voiture1 = equipes[i]->voiture1;
		  stands[i]->voiture2 = equipes[i]->voiture2;
		  if(pthread_create(pthread_stand[i], NULL, standardiser, (void*) stands[i])<0)
			   erreur("pthread_create",1);
	 }

	 printf("DEPART DE LA COURSE\n");

	 for(i=0;i<NbEquipe*2; i++)P(LOCKMAIN);
	 for(i=0;i<NbEquipe*2; i++)V(LOCKFILS);

	 if(pthread_create(pthread_classement, NULL, printClassement,NULL))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe*2; i++)
		  if(pthread_join(*pthread_voiture[i],NULL)!=0)
			   erreur("pthread_join",1);

	 if(pthread_cancel(*pthread_classement))
		  erreur("pthread_cancel",1);

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_cancel(*pthread_stand[i]))
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
	 int i;
	 printf("DÉCONSTRUCTION DE LA COURSE\n");
	 freeCircuit(circuit);
	 freeEquipes(equipes,NbEquipe);
	 freeStands(stands,NbEquipe);
	 free(classement);
	 printf("SUPPRESSION DES COMPOSANTS\n");
	 libereSem();
	 pthread_mutex_destroy(classementVerrou);
	 free(classementVerrou);
	 pthread_mutex_destroy(pauseVerrou);
	 free(pauseVerrou);
	 free(pthread_classement);
	 for(i=0; i<NbEquipe*2; i++)
		  free(pthread_voiture[i]);
	 free(pthread_voiture);
	 for(i=0; i<NbEquipe; i++)
		  free(pthread_stand[i]);
	 free(pthread_stand);
	 printf("\n\nà bientôt pour une nouvelle course de Nascar\n");
}

void killThread()
{
	 int i;
	 if(pthread_cancel(*pthread_classement))
		  erreur("pthread_cancel",1);

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_cancel(*pthread_stand[i]))
			   erreur("pthread_cancel",1);

	 for(i=0; i<NbEquipe*2; i++)
		  if(pthread_cancel(*pthread_voiture[i]))
			   erreur("pthread_cancel",1);
	 printf("TOUS LES THREADS ONT ÉTÉ TUÉS\n");
}
