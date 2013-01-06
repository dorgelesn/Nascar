#include "circuit.h"

Circuit* newCircuit()
{
	 Circuit *circuit = malloc(sizeof(Circuit));
	 circuit->nbTour = 5;
	 circuit->longueur = 100;
	 circuit->sections = malloc(sizeof(Section*)*circuit->longueur);
	 circuit->vitesseMax = 210;
	 int i;
	 for(i=0; i<circuit->longueur;i++)
		  circuit->sections[i]=newSection(i);	
	 return circuit;
}

void freeCircuit(Circuit *circuit)
{
	 freeSections(circuit->sections, circuit->longueur);
	 free(circuit);
}

void printCircuit(Circuit* circuit, Stand** stands)
{
	 clear();
	 int i,j;
	 Voiture* tmpVoiture1;
	 Voiture* tmpVoiture2;
	 Section* tmpSection;
	 Stand* tmpStand;


	 for(i=0; i<25; i++){
		  fondVert();
	 }
	 printf("\n");


	 for(j=0; j<2; j++){
		  for(i=0; i<25;i++)
		  {
			   tmpSection = circuit->sections[i];
			   if( j%2 == 0 )
					tmpVoiture1 = tmpSection->voitureGauche;
			   else tmpVoiture1 = tmpSection->voitureDroite;

			   if(tmpVoiture1 == NULL) fondGrisVide();
			   else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture,tmpVoiture1->status);
		  }
		  printf("\n");
	 }

	 for(i=0; i<24; i++){

		  tmpSection = circuit->sections[99-i];
		  tmpVoiture1 = tmpSection->voitureGauche;
		  tmpVoiture2 = tmpSection->voitureDroite;

		  if(tmpVoiture1 == NULL) fondGrisVide();
		  else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture,tmpVoiture1->status);
		  if(tmpVoiture2 == NULL) fondGrisVide();
		  else fondGris(tmpVoiture2->numEquipe, tmpVoiture2->numVoiture,tmpVoiture2->status);

		  for(j=0; j<5; j++){
			 fondVert();
		  }

	  	  if( i>0 && i < 22 ){
			 tmpStand = stands[i-1];
			 printf("Stand n°%d\t", i);
			
			 if( tmpStand->voitureStand == NULL )
				 printf("Aucune voiture.  ");
			 else{
				 printf("%*d:%d             ", 2, tmpStand->voiture1->numEquipe, tmpStand->voiture2->numVoiture);
			 }

			 for(j=0; j<10; j++){
				fondVert();
			 }

		  }else{
			  for(j=0; j<16; j++){
				  fondVert();
			  }
		  }
		
		  tmpSection = circuit->sections[26+i];
		  tmpVoiture1 = tmpSection->voitureGauche;
		  tmpVoiture2 = tmpSection->voitureDroite;

		  if(tmpVoiture1 == NULL) fondGrisVide();
		  else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture,tmpVoiture1->status);
		  if(tmpVoiture2 == NULL) fondGrisVide();
		  else fondGris(tmpVoiture2->numEquipe, tmpVoiture2->numVoiture,tmpVoiture2->status);
		  printf("\n");
	 }

	 for(j=0; j<2; j++){
		  for(i=75; i>50;i--)
		  {
			   tmpSection = circuit->sections[i];
			   if( j%2 == 0 )
					tmpVoiture1 = tmpSection->voitureGauche;
			   else tmpVoiture1 = tmpSection->voitureDroite;

			   if(tmpVoiture1 == NULL) fondGrisVide();
			   else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture,tmpVoiture1->status);
		  }
		  printf("\n");
	 }

	 for(i=0; i<25; i++){
		  fondVert();
	 }
	 printf("\n");

}


void printClassement(Voiture** classement, int nbVoiture)
{
	 int i,num,equipe,voiture,essence,total,tour;
	 for(i=0; i<nbVoiture; i++)
	 {
		 num = (nbVoiture)-i;
		 if(classement[i] != NULL)
		 {
			  equipe  = classement[i]->numEquipe;
			  voiture = classement[i]->numVoiture;
			  tour = classement[i]->nbTourEffectue;
			  essence = classement[i]->essenceActuelle;
			  total   = classement[i]->essenceTotal;

			  if(classement[i]->status == 3)
				  printf("%*d) Équipe %*d, voiture %d , voiture arrivé",2,num,2,equipe,voiture);	
			  else if(classement[i]->status == 2)
				  printf("%*d) Équipe %*d, voiture %d , tour:%d, la voiture est au stand",2,num,2,equipe,voiture,tour);	
			  else if(classement[i]->status == 1)
				  printf("%*d) Équipe %*d, voiture %d , tour:%d, essence %d/%d",2,num,2,equipe,voiture,tour,essence,total);	
			  else if(classement[i]->status == 0)
				  printf("%*d) Équipe %*d, voiture %d , voiture hors course",2,num,2,equipe,voiture);	
			  else if(classement[i]->status == -1)
				  printf("%*d) Équipe %*d, voiture %d , voiture sans essence",2,num,2,equipe,voiture);	
			  else if(classement[i]->status == -2)
				  printf("%*d) Équipe %*d, voiture %d , accident légé",2,num,2,equipe,voiture);	
			  else if(classement[i]->status == -3)
				  printf("%*d) Équipe %*d, voiture %d , accient grave",2,num,2,equipe,voiture);	

			  if(i%2 == 0)printf("\n");else printf("\t\t");
		 }
	 }
	 printf("\n\n");
}


void getClassement(int nbEquipe, Voiture** classement, Equipe** equipes)
{
	 fflush(stdin);
	 fflush(stdout);

	 int i,j;
	 int indexClassement =0;
	 int accidentClassement = 0;
	 Voiture** tmpClassement = malloc(sizeof(Voiture*)*2*nbEquipe);		
	 Voiture** listNonClassee = malloc(sizeof(Voiture*)*2*nbEquipe);		
	 for(i=0; i<2*nbEquipe; i++)listNonClassee[i] = NULL;

	 int indexNonClassee=0;
	 for(i =0; i<2*nbEquipe; i++)
	 {
		  if(classement[i]!=NULL && classement[i]->status == 3)
		  {
			   tmpClassement[i] = classement[i];
			   indexClassement++;
		  }
	 }

	 Voiture* tmpVoiture;
	 if(indexClassement < nbEquipe*2 )
	 {
		  for(i=0; i<nbEquipe; i++)
		  {
			    tmpVoiture = equipes[i]->voiture1;
			    if(estEnCourse(tmpVoiture) && tmpVoiture->deplacementTotal >=0)
				{
					 listNonClassee[indexNonClassee] = tmpVoiture;
					 indexNonClassee++;	
				}
			    tmpVoiture = equipes[i]->voiture2;
			    if(estEnCourse(tmpVoiture) && tmpVoiture->deplacementTotal >=0)
				{
					 listNonClassee[indexNonClassee] = tmpVoiture;
					 indexNonClassee++;	
				}
		  }
		  quickSort(listNonClassee, 0, indexNonClassee - 1);
		  for(i=0; i<indexNonClassee; i++)
		  {
			   tmpClassement[indexClassement] = listNonClassee[i];
			   indexClassement++;
		  }
	 }
	 printClassement(tmpClassement,indexClassement);
	 free(listNonClassee);
	 free(tmpClassement);
}

void echanger(Voiture** tableau, int a, int b)
{
    Voiture* temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;
}

void quickSort(Voiture** tableau, int debut, int fin)
{
    int gauche = debut-1;
    int droite = fin+1;
    const Voiture* pivot = tableau[debut];

    if(debut >= fin) return;
    while(1)
    {
        do droite--; while(tableau[droite]->deplacementTotal > pivot->deplacementTotal);
        do gauche++; while(tableau[gauche]->deplacementTotal < pivot->deplacementTotal);

        if(gauche < droite)
            echanger(tableau, gauche, droite);
        else break;
    }

    quickSort(tableau, debut, droite);
    quickSort(tableau, droite+1, fin);
}
