#ifndef SYSTEM
#define SYSTEM

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int aleatoire(int a, int b);
void erreur(const char* message, int retour);
void clear();

void fondVert();
void fondGris(int equipe,int voiture);
void fondGrisVide();

#endif

