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
	Voiture* tmpVoiture;
	Section* tmpSection;
	int step = 5;
	for(i=0; i<circuit->longueur;i+=step)
	{
		for(j=0; j<step; j++)
		{
			if(i+j < circuit->longueur)
			{
				tmpSection = circuit->sections[i+j];
				tmpVoiture = tmpSection->voitureGauche;
				if(tmpVoiture == NULL) printf("XXXX\t");
				else printf("%d:%d\t",tmpVoiture->numEquipe,tmpVoiture->numVoiture); 
				tmpVoiture = tmpSection->voitureDroite;
				if(tmpVoiture == NULL) printf("XXXX\t");
				else printf("%d:%d\t",tmpVoiture->numEquipe,tmpVoiture->numVoiture); 
			}
		}
		printf("\n");
	}
	printf("\n\n\n\n");
}













