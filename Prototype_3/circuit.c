#include "circuit.h"

Circuit* newCircuit()
{
	Circuit *circuit = malloc(sizeof(Circuit));
	circuit->longueur = 100;
	circuit->sections = malloc(sizeof(Section*)*circuit->longueur);
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

void getClassement(Circuit* circuit)
{
	clear();
	int i,j;
	Voiture* tmpVoiture1;
	Voiture* tmpVoiture2;
	Section* tmpSection;


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
			else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture);
		}
	}

	for(i=0; i<24; i++){
		
		tmpSection = circuit->sections[99-i];
		tmpVoiture1 = tmpSection->voitureGauche;
		tmpVoiture2 = tmpSection->voitureDroite;

		if(tmpVoiture1 == NULL) fondGrisVide();
		else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture);
		if(tmpVoiture2 == NULL) fondGrisVide();
		else fondGris(tmpVoiture2->numEquipe, tmpVoiture2->numVoiture);
		
		for(j=0; j<21; j++){
			fondVert();
		}
		
		tmpSection = circuit->sections[26+i];
		tmpVoiture1 = tmpSection->voitureGauche;
		tmpVoiture2 = tmpSection->voitureDroite;
			
		if(tmpVoiture1 == NULL) fondGrisVide();
		else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture);
		if(tmpVoiture2 == NULL) fondGrisVide();
		else fondGris(tmpVoiture2->numEquipe, tmpVoiture2->numVoiture);
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
			else fondGris(tmpVoiture1->numEquipe, tmpVoiture1->numVoiture);
		}
	}
	
	for(i=0; i<25; i++){
		fondVert();
	}
	printf("\n");

}














