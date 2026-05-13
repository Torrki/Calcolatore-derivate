#include <stdio.h>
#include <stdlib.h>
#include <AlberoSintassi.h>

int main(int argc, char* argv[]){
	char* stringaInput=NULL;
	size_t dimensioneStringa=0;
	struct Tabella* TabellaDerivate=CreaTabella();
	struct AutomaSintassi* Automa=CreaAutoma(TabellaDerivate);
	
	//lettura input stringa
	printf("funzione nella variabile x: ");
	ssize_t numeroCaratteri=getline(&stringaInput,&dimensioneStringa,stdin);
	size_t caratteriProcessati=0;
	char statoSuccessivo=0;
	
	//buffer per la stringa dell'albero
	char* stringaAlbero=(char*)malloc(sizeof(char)*(numeroCaratteri+1));
	unsigned j=0;
	
	//analisi della sintassi
	ssize_t i=0;
	for(; i<numeroCaratteri-1 && statoSuccessivo>-1; ++i){
		char carattere_i=stringaInput[i];
		if((carattere_i != '0' && Automa->statoCorrente->s==0) || carattere_i != ' '){
			statoSuccessivo=InputAutoma(Automa,carattere_i);
			stringaAlbero[j]=carattere_i;
			++j;
		}
	}
	
	//analisi della semantica e calcolo della derivata
	if(statoSuccessivo > -1 && VerificaStatoFinale(Automa)){
		stringaAlbero[j]='\n';
		stringaAlbero[j+1]='\0';
		
		//la costruzione dell'albero avviene senza errori di sintassi
		Nodo radiceAlberoSintassi=CreaAlbero(Automa,stringaAlbero,&caratteriProcessati);
		StampaAlbero(radiceAlberoSintassi,0);
		
		const char* derivataFunzione=AnalizzaAlbero(TabellaDerivate,radiceAlberoSintassi);
		SemplificaEspressione((char*)derivataFunzione,Automa);
		printf("\n\nLa derivata: %s\n", derivataFunzione);
		
		DeallocaAlbero(radiceAlberoSintassi);
	}else{
		printf("Errore nella sintassi della stringa al carattere %ld\n", i+1);
	}
	
	CancellaAutoma(Automa);
	CancellaTabella(TabellaDerivate);
	free(stringaInput);
	
	return 0;
}
