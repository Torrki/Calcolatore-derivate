#include <stdio.h>
#include <stdlib.h>
#include <AlberoSintassi.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);

int main(int argc, char* argv[]){
	char* stringaInput=NULL;
	size_t dimensioneStringa=0;
	
	//lettura input stringa
	printf("funzione nella variabile x: ");
	ssize_t numeroCaratteri=getline(&stringaInput,&dimensioneStringa,stdin);
	size_t caratteriProcessati=0;
	Nodo radiceAlberoSintassi=CreaAlbero(stringaInput,&caratteriProcessati);
	StampaAlbero(radiceAlberoSintassi,0);
	free(stringaInput);
	return 0;
}
