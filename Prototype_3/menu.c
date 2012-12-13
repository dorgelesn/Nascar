#include "menu.h"

int showMenu()
{
	clear();
	char re = ' ';
	while(1){
		clear();
		if(re == 'q') return 0;
		if(re == 'r') return 1;
		showOption();
		printf("\tvotre choix :\n");
		scanf("%c",&re);
	}
}

void showOption(){
	printf("\nPresser sur 'q' pour quitter\n");
	printf("Presser sur 'r' pour retourner à la course\n\n");
}
