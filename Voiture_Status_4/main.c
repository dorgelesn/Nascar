#include "main.h"

#define NbEquipe 21

Circuit* circuit;
Equipe** equipes;
Stand** stands;
Voiture** classement;
Directeur* directeur;
int indexClassement,indexInverse;

pthread_mutex_t  *pauseVerrou;
pthread_mutex_t  *classementVerrou;

pthread_t *pthread_printCircuit;
pthread_t *pthread_directeur;
pthread_t **pthread_voiture;
pthread_t **pthread_stand;

int LOCKFILS;
int LOCKMAIN;
static pid_t mainPID;
int qualification;
int printCircuit_isRunning;

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
			   case 0://quitter le programme
					for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_unlock(&pauseVerrou[i]);
					kill(mainPID,SIGINT);
					break;
			   case 1://retour a la course
					for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_unlock(&pauseVerrou[i]);
					break;
			   case 2://Accident mineur
					accidentMineur(directeur);
					for(i=0; i<NbEquipe*2+1; i++) pthread_mutex_unlock(&pauseVerrou[i]);
					break;
			   case 3://Accident grave
					accidentGrave(directeur);
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
	 while(printCircuit_isRunning == 1)
	 {
		  pthread_mutex_lock(&pauseVerrou[NbEquipe*2]);
		  printCircuit(circuit, stands);
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
	 voiture->nbTourEffectue = 0;
	 voiture->vitesseActuelle = voiture->vitesseMax;
	 voiture->status = 1;
	 Stand* stand = stands[voiture->numEquipe-1];
	 V(LOCKMAIN);
	 P(LOCKFILS);
	 while(estEnCourse(voiture) == 1)
	 {
		  pthread_mutex_lock(&pauseVerrou[id]);

		  //Régulation de la vitesse
		  if(voiture->vitesseActuelle > circuit->vitesseMax)
			   voiture->vitesseActuelle = circuit->vitesseMax;
		  else
			   voiture->vitesseActuelle = voiture->vitesseMax;
		  //Si la voiture n'est pas accidenté
		  if(voiture->status == 1 || voiture->status == 2)
		  {
			   //Entrée au stand
			   if(sectionVisee == 0 && stand->voitureStand == voiture)
			   {
					sortirSection(circuit->sections[sectionActuelle],voiture);
					entreeStand(stand);
					voiture->status = 2;
			   }
			   //Entrée dans la section
			   if(entrerSection(circuit->sections[sectionVisee],voiture))
			   {
					voiture->deplacementTotal ++;
					if(voiture->status != 2)
					{sortirSection(circuit->sections[sectionActuelle],voiture);}
					sectionActuelle = sectionVisee; sectionVisee ++;
					if(sectionVisee >= circuit->longueur)
					{
						 voiture->nbTourEffectue ++;
						 sectionVisee =0;
					}
					voiture->status = 1;
			   }
			   //La voiture a fini la course
			   if(voiture->nbTourEffectue >= circuit->nbTour) voiture->status = 3;
			   //La voiture n'a plus d'essence
			   if(voiture->essenceActuelle <= 0) voiture->status = -1;
		  }
		  pthread_mutex_unlock(&pauseVerrou[id]);
		  usleep(temps);
	 }
	 sectionActuelle = voiture->numSection;
	 sortirSection(circuit->sections[sectionActuelle],voiture);

	 pthread_mutex_lock(classementVerrou);
	 if(voiture->status == 3)
	 {
		  classement[indexClassement] = voiture;
		  indexClassement ++;
	 }else if(directeur->accidentGrave == 0 && voiture->status < 0)
	 {
		  classement[indexInverse] = voiture;
		  indexInverse --;
	 }
	 pthread_mutex_unlock(classementVerrou);
	 pthread_exit(0);
}

int main(int argc, char** argv)
{
	 int i,j;
	 char b;
	 mainPID = getpid();
	 pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
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

	 printf("CREATION DU DIRECTEUR DE COURSE\n");
	 directeur = newDirecteur(equipes);
	 pthread_directeur = NULL;

	 ///////////////////////////////////////////////////////////////////////////
	 ////Tour de qualification
	 ///////////////////////////////////////////////////////////////////////////
	 qualification = 1;
	 printf("CREATION DU CIRCUIT\n");
	 circuit = newCircuit();
	 circuit->nbTour =1;
	 directeur->circuit = circuit;
	 directeur->isQualification = 1;

	 printf("INITIALISATION DU CLASSEMENT\n");
	 classement = malloc(sizeof(Voiture*)*NbEquipe*2);
	 for(i=0; i<NbEquipe*2;i++) classement[i]=NULL;
	 indexClassement = 0;
	 indexInverse = 2*NbEquipe -1;

	 printf("INITIALISATION DES THREADS\n");
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
		  equipes[i]->voiture2->status = 0;
	 }

	 clear();
	 printf("Presser sur une touche pour commencer les qualifications\n");
	 scanf("%c",&b);

	 for(i=0;i<NbEquipe; i++)P(LOCKMAIN);
	 for(i=0;i<NbEquipe; i++)V(LOCKFILS);

	 printCircuit_isRunning = 1;
	 if(pthread_create(pthread_printCircuit, NULL, threadPrintCircuit,NULL))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_join(*pthread_voiture[i],NULL)!=0)
			   erreur("pthread_join",1);

	 printCircuit_isRunning = 0;

	 ///////////////////////////////////////////////////////////////////////////
	 ////Entre deux tours
	 ///////////////////////////////////////////////////////////////////////////

	 clear();
	 if(directeur->accidentGrave == 1)
	 {
		printf("\nLa course a pris fin à cause d'un accident grave\n\n");
		freeMain();
		return 0;
	 }
	 getClassement(NbEquipe,classement,equipes);

	 int* ordre = malloc(sizeof(int)*NbEquipe);
	 for(i=0; i<NbEquipe; i++)ordre[NbEquipe-1-i] = classement[i]->numEquipe - 1;

	 freeCircuit(circuit); 
	 free(classement);
	 free(pthread_printCircuit);
	 for(i=0; i<NbEquipe; i++)
		  free(pthread_voiture[i]);
	 free(pthread_voiture);

	 ///////////////////////////////////////////////////////////////////////////
	 /////Course
	 ///////////////////////////////////////////////////////////////////////////
	 qualification = 0;
	 printf("CREATION DU CIRCUIT\n");
	 circuit = newCircuit();
	 directeur->circuit = circuit;
	 directeur->isQualification = 0;
	 if(argc ==2)circuit->nbTour = atoi(argv[1]);
	 else circuit->nbTour = 10;

	 printf("INITIALISATION DU CLASSEMENT\n");
	 classement = malloc(sizeof(Voiture*)*2*NbEquipe);
	 for(i=0; i<2*NbEquipe;i++) classement[i]=NULL;
	 indexClassement = 0;
	 indexInverse = 2*NbEquipe -1;

	 printf("INITIALISATION DES THREADS\n");
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
		  entrerSection(circuit->sections[i],equipes[ordre[i]]->voiture1);
		  if(pthread_create(pthread_voiture[2*i+0], NULL, run, (void*)equipes[ordre[i]]->voiture1)<0)
			   erreur("pthread_create",1);
		  entrerSection(circuit->sections[i],equipes[ordre[i]]->voiture2);
		  if(pthread_create(pthread_voiture[2*i+1], NULL, run, (void*)equipes[ordre[i]]->voiture2)<0)
			   erreur("pthread_create",1);
	 }
	 free(ordre);

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
	 fflush(stdout);fflush(stdin);
	 scanf("%c",&b);

	 for(i=0;i<NbEquipe*2; i++)P(LOCKMAIN);
	 for(i=0;i<NbEquipe*2; i++)V(LOCKFILS);

	 printCircuit_isRunning = 1;
	 if(pthread_create(pthread_printCircuit, NULL, threadPrintCircuit,NULL))
		  erreur("pthread_create",1);

	 directeur->isRunning = 1;
	 pthread_directeur = malloc(sizeof(pthread_t));
	 if(pthread_create(pthread_directeur, NULL, thread_directeur,(void*)directeur))
		  erreur("pthread_create",1);

	 for(i=0; i<NbEquipe*2; i++)
		  if(pthread_join(*pthread_voiture[i],NULL)!=0)
			   erreur("pthread_join",1);

	 printCircuit_isRunning = 0;
	 directeur->isRunning =0;

	 for(i=0; i<NbEquipe; i++)
		  if(pthread_cancel(*pthread_stand[i]))
			   erreur("pthread_cancel",1);

	 if(directeur->accidentGrave == 1)
	 {
		 printf("La course a été annulé a cause d'un accident grave\n");
	 }else{
		  printf("Classement de la course\n");
		  getClassement(NbEquipe,classement,equipes);
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
	 freeDirecteur(directeur);
	 free(classement);

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

	 if(pthread_printCircuit)  free(pthread_printCircuit);

	 if(pthread_directeur)     free(pthread_directeur);

	 int max = NbEquipe;
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
	 libereSem();
	 printCircuit_isRunning =0;

	 if(pthread_stand)
	 {
		  for(i=0; i<NbEquipe; i++)
			   if(pthread_stand[i] && pthread_cancel(*pthread_stand[i]))
					erreur("pthread_cancel",1);
	 }

	 int max =NbEquipe;
	 if(qualification =0) max *= 2;
	 for(i=0; i<max; i++)
		  if(pthread_voiture[i] && pthread_cancel(*pthread_voiture[i]))
			   erreur("pthread_cancel",1);

	 printf("TOUS LES THREADS ONT ÉTÉ TUÉS\n");
}
