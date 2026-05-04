#include <TabellaDerivate.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int AggiungiDerivata(struct CoppiaDerivata** tabella, const char* f, const char* df);

int AggiungiDerivata(struct CoppiaDerivata** tabella, const char* f, const char* df){
	int ret=1;
	
	//creo la nuova coppia e copio chiave e valore
	struct CoppiaDerivata* nuovaCoppia=(struct CoppiaDerivata*)malloc(sizeof(struct CoppiaDerivata));
	nuovaCoppia->chiave=(const char*)malloc(sizeof(char)*(strlen(f)+1));
	nuovaCoppia->valore=(const char*)malloc(sizeof(char)*(strlen(df)+1));
	nuovaCoppia->succ=NULL;
	
	strcpy((char*)nuovaCoppia->chiave,f);
	strcpy((char*)nuovaCoppia->valore,df);
	
	//aggiungo in coda
	if(*tabella){
		struct CoppiaDerivata* testa_tmp=*tabella;
		
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
		*tabella=nuovaCoppia;
	}
	return ret;
}

const char* OttieniDerivata(struct CoppiaDerivata* tabella, const char* f){
	const char* ret=NULL;
	if(tabella){
		struct CoppiaDerivata* testa_tmp=tabella;
		
		while(strcmp(testa_tmp->chiave,f) && testa_tmp->succ) testa_tmp=testa_tmp->succ;
		if(testa_tmp->succ || strcmp(testa_tmp->chiave,f)==0){
			ret=testa_tmp->valore;
		}
	}
	return ret;
}

struct CoppiaDerivata* CreaTabella(){

	struct CoppiaDerivata* tabellaDerivate=NULL;
	//operatori
	AggiungiDerivata(&tabellaDerivate,"+","(%s+%s)");
	AggiungiDerivata(&tabellaDerivate,"-","(%s-%s)");
	AggiungiDerivata(&tabellaDerivate,"*","(%s*%s)+(%s*%s)");
	AggiungiDerivata(&tabellaDerivate,"/","((%s*%s)-(%s*%s))/(%s^2)");
	AggiungiDerivata(&tabellaDerivate,"@","%s(%s)*%s");
	
	//funzioni
	AggiungiDerivata(&tabellaDerivate,"x","1");
	AggiungiDerivata(&tabellaDerivate,"exp","exp");
	AggiungiDerivata(&tabellaDerivate,"ln","1/x");
	AggiungiDerivata(&tabellaDerivate,"sin","cos");
	AggiungiDerivata(&tabellaDerivate,"cos","-sin");
	return tabellaDerivate;
}

void CancellaTabella(struct CoppiaDerivata* tabella){
	while(tabella){
		struct CoppiaDerivata* successivo=tabella->succ;
		printf("Cancellata voce di %s\n", tabella->chiave);
		free((void*)tabella->chiave);
		free((void*)tabella->valore);
		free((void*)tabella);
		
		tabella=successivo;
	}
}
