/* Exemple de synchronisation par semaphores                  */
/*    trajet Paris-Perros synchronise par semaphores          */
/*    Un voyageur part de Paris a destination de Perros.      */
/*    Il doit emprunter :                                     */
/*            . le TGV de Paris a Plouaret,                   */
/*            . l'autorail de Plouaret a Lannion,             */
/*            . le taxi de Lannion a Perros.                  */
/*    TGV, autorail et taxi sont 3 processus independants     */
/*    qui se synchronisent pour amener le voyageur a bon port */
/*    Execution :                                             */
/*                                                            */
/*       TGV            AUTORAIL                TAXI          */
/*                                                            */
/* depart Paris                                               */
/*                    attente train                           */
/*                                        attente autorail    */
/* arrivee Plouaret                                           */
/* depart Plouaret                                            */
/*                    depart Plouaret                         */
/*                    arrivee Lannion                         */
/*                    arret                                   */
/*                                        depart Lannion      */
/*                                        arrivee Perros      */
/* arrivee Brest                                              */
/*                                        arret               */
/* arret                                                      */


#include <stdio.h>
#include "semaphore.c"

/* affichage pour suivi du trajet */
void message(int i, char* s) {
   #define COLONNE 20
   int j, NbBlanc;

   NbBlanc=i*COLONNE;
   for (j=0; j<NbBlanc; j++) putchar(' ');
   printf("%s\n",s);
   fflush(stdout);
}

/* attente en seconde, ou aleatoire */
void attente(int n) {
   sleep(n); 	/*sleep(rand() % n);*/
}

/* creation du processus TGV */
void TGV(int i) {
   if (! fork()) {
	message(i, "depart Paris");
	attente(3);
	message(i, "arrivee Plouaret");
	attente(3);
	V(1);
	message(i, "depart Plouaret");
	attente(10);
	message(i, "arrivee Brest");
	attente(3);
	message(i, "arret");
	exit(0);
   }
}

/* creation du processus autorail */
void autorail(int i) {
   if (! fork()) {
	message(i, "attente train");
	attente(3);
	P(1);
	message(i, "depart Plouaret");
	attente(3);
	message(i, "arrivee Lannion");
	attente(3);
	V(0);
	message(i, "arret");
	exit(0);
   }
}

/* creation du processus taxi */
void taxi(int i) {
   if (! fork()) {
	message(i, "attente autorail");
	attente(3);
	P(0);
	message(i, "depart Lannion");
	attente(3);
	message(i, "arrivee Perros");
	attente(3);
	message(i, "arret");
	exit(0);
   }
}

int main(){
   int i;
   printf("%10s%20s%20s\n\n", "TGV", "AUTORAIL", "TAXI");
   initSem(2,"Vive les bananes",NULL);
   TGV(0);
   autorail(1);
   taxi(2);
   for (i=1; i<=3; i++) wait(0);
   libereSem();
   exit(0);
}



