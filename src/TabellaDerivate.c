#include <TabellaDerivate.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int AggiungiDerivata(struct Tabella* tabella, const char* f, const char* df);

int AggiungiDerivata(struct Tabella* tabella, const char* f, const char* df){
	int ret=1;
	
	//creo la nuova coppia e copio chiave e valore
	struct CoppiaDerivata* nuovaCoppia=(struct CoppiaDerivata*)malloc(sizeof(struct CoppiaDerivata));
	nuovaCoppia->chiave=(const char*)malloc(sizeof(char)*(strlen(f)+1));
	nuovaCoppia->valore=(const char*)malloc(sizeof(char)*(strlen(df)+1));
	nuovaCoppia->succ=NULL;
	
	strcpy((char*)nuovaCoppia->chiave,f);
	strcpy((char*)nuovaCoppia->valore,df);
	
	//aggiungo in coda
	if(tabella->testa){
		struct CoppiaDerivata* testa_tmp=tabella->testa;
		
		//ciclo fino a quando trovo una chiave uguale o sono arrivato all'ultima coppia
		while(strcmp(testa_tmp->chiave,f) && testa_tmp->succ) testa_tmp=testa_tmp->succ;
		
		//se c'è un successivo allora sono uscito prima e dunque esiste già una chiave uguale oppure l'ultimo elemento ha la chiave uguale
		if(testa_tmp->succ || strcmp(testa_tmp->chiave,f)==0){
			ret=0;
		}else{
			testa_tmp->succ=nuovaCoppia;
			nuovaCoppia->prec=testa_tmp;
		}
	}else{
		nuovaCoppia->prec=NULL;
		tabella->testa=nuovaCoppia;
	}
	tabella->numeroVoci++;
	return ret;
}

const char* OttieniDerivata(struct Tabella* tabella, const char* f){
	const char* ret=NULL;
	if(tabella->testa){
		struct CoppiaDerivata* testa_tmp=tabella->testa;
		
		while(strcmp(testa_tmp->chiave,f) && testa_tmp->succ) testa_tmp=testa_tmp->succ;
		if(testa_tmp->succ || strcmp(testa_tmp->chiave,f)==0){
			ret=testa_tmp->valore;
		}
	}
	return ret;
}

struct Tabella* CreaTabella(){

	struct Tabella* tabellaDerivate=(struct Tabella*)malloc(sizeof(struct Tabella));
	
	//funzioni
	AggiungiDerivata(tabellaDerivate,"x","1");
	AggiungiDerivata(tabellaDerivate,"exp","exp(%s)");
	AggiungiDerivata(tabellaDerivate,"ln","1/(%s)");
	AggiungiDerivata(tabellaDerivate,"sin","cos(%s)");
	AggiungiDerivata(tabellaDerivate,"cos","-sin(%s)");
	AggiungiDerivata(tabellaDerivate,"arccos","-1/sqrt(1-(%s)^2)");
	AggiungiDerivata(tabellaDerivate,"arcsin","1/sqrt(1-(%s)^2)");
	AggiungiDerivata(tabellaDerivate,"sqrt","1/(2*sqrt(%s))");
	return tabellaDerivate;
}

void MatchChiave(struct Tabella* tabella, const char *f){
	struct CoppiaDerivata* tabellaTmp=tabella->testa;
	while(tabellaTmp){
		char match=strstr(tabellaTmp->chiave,f)==tabellaTmp->chiave; //match parziale della funzione
		struct CoppiaDerivata* successivoTmp=tabellaTmp->succ;
		if(!match){
			struct CoppiaDerivata* precedente=tabellaTmp->prec;
			if(precedente){ //se non sono al primo elemento
				precedente->succ=tabellaTmp->succ;
			}else{
				tabella->testa=tabellaTmp->succ;
				tabella->testa->prec=NULL;
			}
				
			free((void*)tabellaTmp->chiave);
			free((void*)tabellaTmp->valore);
			free((void*)tabellaTmp);
			tabella->numeroVoci--;
		}
		tabellaTmp=successivoTmp;
	}
}

struct Tabella* CopiaTabella(struct Tabella* tabella){
	struct Tabella* tabellaDerivate=(struct Tabella*)malloc(sizeof(struct Tabella));
	struct CoppiaDerivata* tabellaTmp=tabella->testa;
	
	while(tabellaTmp){
		AggiungiDerivata(tabellaDerivate,tabellaTmp->chiave,tabellaTmp->valore);
		tabellaTmp=tabellaTmp->succ;
	}
	return tabellaDerivate;
}

void CancellaTabella(struct Tabella* tabella){
	while(tabella->testa){
		struct CoppiaDerivata* successivo=tabella->testa->succ;
		free((void*)tabella->testa->chiave);
		free((void*)tabella->testa->valore);
		free((void*)tabella->testa);
		
		tabella->testa=successivo;
	}
	free((void*)tabella);
}

void StampaTabella(struct Tabella* tabella){
	struct CoppiaDerivata* testaTmp=tabella->testa;
	for(size_t i=0;testaTmp;++i){
		printf("%lu)%12s-->%12s\n",i,testaTmp->chiave,testaTmp->valore);
		testaTmp=testaTmp->succ;
	}
}
