#include "main.h"

#define NbEquipe 21

Circuit *circuit;
Equipe** equipes;
Stand** stands;
Voiture** classement;
int indexClassement,indexInverse;

pthread_mutex_t  *pauseVerrou;
pthread_mutex_t  *classementVerrou;

pthread_t *pthread_printCircuit;
pthread_t **pthread_voiture;
pthread_t **pthread_stand;

int LOCKFILS;
int LOCKMAIN;
static pid_t mainPID;
int qualification;

void signalSIGTSTP(int num)
{
	 int i;
	 if(num == SIGTSTP)
	 {
		  clear();
		  for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_lock(&pauseVerrou[i]);
		  getClassement(NbEquipe,classement,equipes);
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

void* threadPrintCircuit()
{
	 while(1)
	 {
		  pthread_mutex_lock(&pauseVerrou[NbEquipe*2]);
		  printCircuit(circuit);
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
	 voiture->deplacementTotal = voiture->numSection;
	 Stand* stand = stands[voiture->numEquipe-1];
	 V(LOCKMAIN);
	 P(LOCKFILS);
	 while(voiture->nbTourEffectue < circuit->nbTour && voiture->essenceActuelle > 0)
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
			   voiture->deplacementTotal ++;
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
		  usleep(temps);

	 }

	 sectionActuelle = voiture->numSection;
	 sortirSection(circuit->sections[sectionActuelle],voiture);
	 pthread_mutex_lock(classementVerrou);
	 if(voiture->essenceActuelle > 0)
	 {
		  classement[indexClassement] = voiture;
		  indexClassement ++;
		  voiture->numSection = -3;
	 }else{
		  classement[indexInverse] = voiture;
		  indexInverse --;
		  voiture->numSection = -2;
	 }
	 pthread_mutex_unlock(classementVerrou);
	 pthread_exit(0);
}

int main(int argc, char** argv)
{
	 int i,j;
	 char b;
	 mainPID = getpid();
	 printf("INITIALISATION DE L'ALÉATOIRE\n");
	 srand(time(NULL));

	 printf("INITIALISATION DES VERROUS\n");
	 pauseVerrou = malloc(sizeof(pthread_mutex_t)*(NbEquipe*2+1));
	 for(i=0; i<(NbEquipe*2+1); i++)
		  pthread_mutex_init(&pauseVerrou[i],NULL);
	 classementVerrou = malloc(sizeof(pthread_mutex_t));
	 pthread_mutex_init(classementVerrou,NULL);

	 printf("INITIALISATION DES SIGNAUX\n");
	 signal(SIGINT,signalSIGINT);
	 signal(SIGTSTP,signalSIGTSTP);

	 printf("INITIALISATION DES SEMAPHORES\n");
	 initSem(2,"AZERTYUIOP",NULL);
	 LOCKFILS=0;
	 LOCKMAIN=1;

	 printf("CREATION DES ÉQUIPES\n");
	 equipes = creationEquipes(NbEquipe);

	 printf("CREATION DES STANDS\n");
	 stands = creationStands(NbEquipe); 

	 ///////////////////////////////////////////////////////////////////////////
	 qualification = 1;
	 printf("CREATION DU CIRCUIT\n");
	 circuit = newCircuit();

	 printf("INITIALISATION DU CLASSEMENT\n");
	 classement = malloc(sizeof(Voiture*)*NbEquipe*2);
	 for(i=0; i<NbEquipe*2;i++) classement[i]=NULL;
	 indexClassement = indexInverse = 0;

	 printf("INITIALISATION DES THREADSi\n");
	 pthread_printCircuit = malloc(sizeof(pthread_t));
	 pthread_voiture = malloc(sizeof(pthread_t*)*(NbEquipe));
	 for(i =0; i<NbEquipe; i++)
		  pthread_voiture[i] = malloc(sizeof(pthread_t));

	 printf("POSITIONNEMENT DES VOITURES\n");
	 for(i=0; i<NbEquipe; i++)
	 {
		  entrerSection(circuit->sections[i/2],equipes[i]->voiture1);
		  if(pthread_create(pthread_voiture[i], NULL, run, (void*)equipes[i]->voiture1)<0)
			   erreur("pthread_create",1);
		  equipes[i]->voiture2->numSection = -2;
	 }

	 clear();
	 printf("Presser sur une touche pour commencer les qualifications\n");
	 scanf("%c",&b);

	 for(i=0;i<NbEquipe; i++)P(LOCKMAIN);
	 for(i=0;i<NbEquipe; i++)V(LOCKFILS);

	 if(pthread_create(pthread_printCircuit, NULL, threadPrintCircuit,NULL))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_join(*pthread_voiture[i],NULL)!=0)
			   erreur("pthread_join",1);

	 if(pthread_cancel(*pthread_printCircuit))
		  erreur("pthread_cancel",1);

	 ///////////////////////////////////////////////////////////////////////////
	 freeCircuit(circuit); 
	 free(classement);
	 free(pthread_printCircuit);
	 for(i=0; i<NbEquipe; i++)
		  free(pthread_voiture[i]);
	 free(pthread_voiture);

	 ///////////////////////////////////////////////////////////////////////////
	 qualification = 0;
	 printf("CREATION DU CIRCUIT\n");
	 circuit = newCircuit();
	 if(argc ==2)circuit->nbTour = atoi(argv[1]);
	 else circuit->nbTour = 10;

	 printf("INITIALISATION DU CLASSEMENT\n");
	 classement = malloc(sizeof(Voiture*)*2*NbEquipe);
	 for(i=0; i<2*NbEquipe;i++) classement[i]=NULL;
	 indexClassement = indexInverse = 0;

	 printf("INITIALISATION DES THREADSi\n");
	 pthread_printCircuit = malloc(sizeof(pthread_t));
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

	 clear();
	 printf("Presser sur une touche pour commencer la course\n");
	 scanf("%c",&b);

	 for(i=0;i<NbEquipe*2; i++)P(LOCKMAIN);
	 for(i=0;i<NbEquipe*2; i++)V(LOCKFILS);

	 if(pthread_create(pthread_printCircuit, NULL, threadPrintCircuit,NULL))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe*2; i++)
		  if(pthread_join(*pthread_voiture[i],NULL)!=0)
			   erreur("pthread_join",1);

	 if(pthread_cancel(*pthread_printCircuit))
		  erreur("pthread_cancel",1);

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_cancel(*pthread_stand[i]))
			   erreur("pthread_cancel",1);

	 clear();
	 printf("Classement de la course\n");
	 getClassement(NbEquipe,classement,equipes);
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
	 if(classementVerrou)
	 {
		  pthread_mutex_destroy(classementVerrou);
		  free(classementVerrou);
	 }
	 if(pauseVerrou)
	 {
		  pthread_mutex_destroy(pauseVerrou);
		  free(pauseVerrou);
	 }
	 if(pthread_printCircuit)
	 {
		  free(pthread_printCircuit);
	 }

	 int max =NbEquipe;
	 if(qualification =0) max *= 2;
	 if(pthread_voiture)
	 {
		  for(i=0; i<max; i++)
			   if(pthread_voiture[i])
					free(pthread_voiture[i]);
		  free(pthread_voiture);
	 }

	 if(pthread_stand)
	 {
		  for(i=0; i<NbEquipe; i++)
			   if(pthread_stand[i])
					free(pthread_stand[i]);
		  free(pthread_stand);
	 }
	 printf("\n\nà bientôt pour une nouvelle course de Nascar\n");
}

void killThread()
{
	 int i;
	 if(pthread_cancel(*pthread_printCircuit))
		  erreur("pthread_cancel",1);

	 if(pthread_stand)
	 {
	 for(i=0; i<NbEquipe; i++)
		  if(pthread_cancel(*pthread_stand[i]))
			   erreur("pthread_cancel",1);
	 }
	 int max =NbEquipe;
	 if(qualification =0) max *= 2;
	 for(i=0; i<max; i++)
		  if(pthread_cancel(*pthread_voiture[i]))
			   erreur("pthread_cancel",1);
	 printf("TOUS LES THREADS ONT ÉTÉ TUÉS\n");
}
