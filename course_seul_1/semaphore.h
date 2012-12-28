#ifndef SEMAPHORE
#define SEMAPHORE

void erreurFin(const char* msg);
void initSem(int nbSem,char* nomFich,int* pvinit);
void libereSem();
void P(int numSem);
void V(int numSem);

#endif
