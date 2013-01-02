#include "menu.h"

int showMenu()
{
	char re = ' ';
	while(1){
		printf("\n\n\n\n");
		if(re == 'q') return 0;
		if(re == 'r') return 1;
		if(re == '1') return 2;
		showOption();
		printf("\tvotre choix :\n");
		fflush(stdin);
		scanf("%c",&re);
	}
}

void showOption(){
	fflush(stdin);
	fflush(stdout);
	printf("Presser sur '1' pour déclancher un accident mineur\n");

	printf("\nPresser sur 'q' pour quitter\n");
	printf("Presser sur 'r' pour retourner à la course\n\n");
}
