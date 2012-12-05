#ifndef MAIN
#define MAIN

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "semaphore.c"
#include "system.h"
#include "voiture.h"
#include "circuit.h"
#include "equipe.h"

typedef struct Equipe Equipe;
typedef struct Circuit Circuit;
typedef struct Voiture Voiture;


void freeMain();
int main(int argc, char** argv);
#endif

