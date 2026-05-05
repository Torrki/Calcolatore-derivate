#include <stdio.h>
#include <stdlib.h>
#include <AlberoSintassi.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);

int main(int argc, char* argv[]){
	char* stringaInput=NULL;
	size_t dimensioneStringa=0;
	struct AutomaSintassi* Automa=CreaAutoma();
	struct CoppiaDerivata* TabellaDerivate=CreaTabella();
	
	//lettura input stringa
	printf("funzione nella variabile x: ");
	ssize_t numeroCaratteri=getline(&stringaInput,&dimensioneStringa,stdin);
	size_t caratteriProcessati=0;
	char statoSuccessivo=0;
	
	//buffer per la stringa dell'albero
	char* stringaAlbero=(char*)malloc(sizeof(char)*(numeroCaratteri+1));
	unsigned j=0;
	
	//analisi della sintassi
	for(ssize_t i=0; i<numeroCaratteri-1 && statoSuccessivo>-1; ++i){
		char carattere_i=stringaInput[i];
		statoSuccessivo=InputAutoma(Automa,carattere_i);
		if(statoSuccessivo > -1 && carattere_i != ' '){
			stringaAlbero[j]=carattere_i;
			++j;
		}
		//printf("Stato succ: %c\n", statoSuccessivo);
	}
	if(VerificaStatoFinale(Automa)){
		stringaAlbero[j]='\n';
		stringaAlbero[j+1]='\0';
		printf("%s\n", stringaAlbero);
		
		//la costruzione dell'albero avviene senza errori di sintassi
		Nodo radiceAlberoSintassi=CreaAlbero(Automa,stringaAlbero,&caratteriProcessati);
		StampaAlbero(radiceAlberoSintassi,0);
		
		const char* derivataFunzione=AnalizzaAlbero(TabellaDerivate,radiceAlberoSintassi,'s');
		printf("\n\nLa derivata: %s\n", derivataFunzione);
		
		DeallocaAlbero(radiceAlberoSintassi);
	}else{
		printf("Errore nella sintassi della stringa\n");
	}
	
	CancellaAutoma(Automa);
	CancellaTabella(TabellaDerivate);
	free(stringaInput);
	
	return 0;
}
