#include "main.h"

#define CLEF 12345
#define NbEquipe 21

Circuit *circuit;
Equipe** equipes;

int lockFILS;
int lockMAIN;
int lockCIRCUIT;

void* ptr_MEM;

void run(Voiture* voiture){
	int sectionVisee;
	int sectionActuelle;
	int i=0;
	int temps = tempsDeplacement(voiture);	
	while(voiture->nbTourEffectue != 10)
	{

		sectionVisee = voiture->numSection +1;
		sectionActuelle = voiture->numSection;
		if(sectionVisee == circuit->longueur) sectionVisee =0;

		P(lockCIRCUIT);
		circuit = ((Circuit*)ptr_MEM);
		if(entrerSection(circuit->sections[sectionVisee],voiture))
		{
			sortirSection(circuit->sections[sectionActuelle],voiture);
			if(sectionVisee ==0)voiture->nbTourEffectue ++;
			*((Circuit**)ptr_MEM) = circuit;
		}
		V(lockCIRCUIT);
		if(voiture->numEquipe == 0 && voiture->numVoiture == 1)
			getClassement(circuit);
		//sleep(temps);
	}
}

int main(int argc, char** argv)
{
	int i,j;
	printf("INITIALISATION DE L'ALÉATOIRE\n");
	srand(time(NULL));

	printf("CREATION DU CIRCUIT\n");
	circuit = newCircuit();

	printf("INITIALISATION DES SEMAPHORES\n");
	lockFILS    = circuit->longueur;
	lockMAIN    = circuit->longueur+1;
	lockCIRCUIT = circuit->longueur+2;
	initSem(circuit->longueur+3,"Section de courses",0);
	for(i=0; i<circuit->longueur;i++){V(i);V(i);}
	V(lockCIRCUIT);

	int id_MEM;

	printf("CREATION DES ÉQUIPES\n");
	equipes = creationEquipes(NbEquipe);
	//clear();
	printf("INITIALISATION TERMINÉE\n");

	for(i=0; i<NbEquipe; i++)
	{
		entrerSection(circuit->sections[i],equipes[i]->voiture1);
		entrerSection(circuit->sections[i],equipes[i]->voiture2);
		for(j=0; j<2; j++)
		{
			if(forkNroll() == 0)
			{
				V(lockMAIN);
				P(lockFILS);
				if ((id_MEM = shmget(CLEF, sizeof(Circuit*), 0666)) < 0)
					erreur("shmget",1);
				if ((ptr_MEM = shmat(id_MEM, NULL, 0)) == (void*) -1) 
					erreur("shmat",1);

				P(lockCIRCUIT);
				circuit = ((Circuit*)ptr_MEM);
				V(lockCIRCUIT);

				V(lockMAIN);
				P(lockFILS);

				if(j==0)run(equipes[i]->voiture1);
				else run(equipes[i]->voiture2);
				shmdt(ptr_MEM);
				exit(0);
			}
		}
	}

	printf("INITIALISATION DE LA MÉMOIRE PARTAGÉE\n");
	if ((id_MEM = shmget(CLEF, sizeof(Circuit*), 0666 | IPC_CREAT)) < 0) 
		erreur("shmget",1);
	if ((ptr_MEM = shmat(id_MEM, NULL, 0)) == (void*) -1) 
		erreur("shmat",1);
	*((Circuit**)ptr_MEM) = circuit;

	for(i=0; i<NbEquipe*2; i++)P(lockMAIN);
	for(i=0; i<NbEquipe*2; i++)V(lockFILS);
	//LES FILS CREE LEURS MÉMOIRE PARTAGÉE ET CHARGE LE CIRCUIT
	for(i=0; i<NbEquipe*2; i++)P(lockMAIN);
	for(i=0; i<NbEquipe*2; i++)V(lockFILS);
	printf("DEPART DE LA COURSE\n");


	freeMain();
	return 0;
}

void freeMain()
{
	int i;
	wait(NULL);
	printf("SUPPRESSION DES SEMAPHORES\n");
	libereSem();
	printf("SUPPRESSION DE LA MÉMOIRE PARTAGÉE\n");
	shmdt(ptr_MEM);
	printf("SUPPRESSION DES COMPOSANTS\n");
	freeCircuit(circuit);
	freeEquipes(equipes,NbEquipe);
	printf("\nà bientôt pour une nouvelle course de Nascar\n");
}
