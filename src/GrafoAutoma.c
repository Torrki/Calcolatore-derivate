#include <GrafoAutoma.h>
#include <TabellaDerivate.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct AutomaSintassi* CreaAutoma(struct Tabella* tabella){
	size_t numeroStati=2, dimensioneMassima=0;
	struct CoppiaDerivata* testaTmp=tabella->testa;
	struct AutomaSintassi* automa=(struct AutomaSintassi*)malloc(sizeof(struct AutomaSintassi));
	automa->statoIniziale=NULL;
	
	//Identifico il numero di stati
	for(size_t i=0; i<tabella->numeroVoci; ++i){
		size_t dimensioneStringa=strlen(testaTmp->chiave);
		dimensioneMassima = dimensioneStringa > dimensioneMassima ? dimensioneStringa : dimensioneMassima;
		numeroStati += strlen(testaTmp->chiave)-1;
		
		testaTmp=testaTmp->succ;
	}
	
	//Creo i vertici
	testaTmp=tabella->testa;
	size_t contatoreStati=1,contatoreVoci=0;
	struct Stato *statoPrecedente=NULL;
	char* stringaInputs=(char*)calloc(2*numeroStati+tabella->numeroVoci-2+3,sizeof(char)); //3 di span
	
	//stato 0
	struct Stato* vertice0=(struct Stato*)malloc(sizeof(struct Stato));
	vertice0->s=0;
	vertice0->inputs=stringaInputs;
	vertice0->statiSuccessivi=(struct Stato**)calloc(tabella->numeroVoci+1,sizeof(struct Stato*));
	
	//stato finale
	struct Stato* verticeFinale=(struct Stato*)malloc(sizeof(struct Stato));
	char* inputStatoFinale=stringaInputs+tabella->numeroVoci+1+(numeroStati-1)*2;
	inputStatoFinale[0]='(';
	
	verticeFinale->s=numeroStati-1;
	verticeFinale->inputs=inputStatoFinale;
	verticeFinale->statiSuccessivi=(struct Stato**)calloc(2,sizeof(struct Stato*));
	verticeFinale->statiSuccessivi[0]=vertice0;
	verticeFinale->successivo=NULL;
	
	statoPrecedente=vertice0;
	while(testaTmp){
		//scrivo per l'input dello stato 0
		stringaInputs[contatoreVoci]=testaTmp->chiave[0];
		
		//creo i vertici intermedi e li collego
		for(unsigned i=1;testaTmp->chiave[i];++i){
			char* inputCaratteri=stringaInputs+tabella->numeroVoci+1+(contatoreStati-1)*2;
			struct Stato* vertice=(struct Stato*)malloc(sizeof(struct Stato));
			
			inputCaratteri[0]=testaTmp->chiave[i];
			vertice->s=contatoreStati;
			vertice->inputs=inputCaratteri;
			vertice->statiSuccessivi=(struct Stato**)calloc(2,sizeof(struct Stato*));
			
			if(i==1){
				vertice0->statiSuccessivi[contatoreVoci]=vertice;
			}else{
				statoPrecedente->statiSuccessivi[0]=vertice;
			}
			
			//per la lista
			statoPrecedente->successivo=vertice;
			statoPrecedente=vertice;
			contatoreStati++;
		}
		
		//per il grafo, collegamento allo stato finale
		statoPrecedente->statiSuccessivi[0]=testaTmp->chiave[0] == 'x' ? vertice0 : verticeFinale;
		contatoreVoci++;
		testaTmp=testaTmp->succ;
	}
	
	statoPrecedente->successivo=verticeFinale;	
	automa->statoIniziale=vertice0;
	
	return automa;
}

void CancellaAutoma(struct AutomaSintassi* a){
	struct Stato* stato_tmp=NULL;
	free((void*)a->statoIniziale->inputs);
	while(a->statoIniziale){
		stato_tmp=a->statoIniziale->successivo;
		//free((void*)a->statoIniziale->inputs);
		free((void*)a->statoIniziale->statiSuccessivi);
		free((void*)a->statoIniziale);
		a->statoIniziale=stato_tmp;
	}
}

char InputAutoma(struct AutomaSintassi* a, char i){
	char ret=-1;
	if(InputValido(a,i)){
		a->parentesi += i=='(' ? 1 : i==')' ? -1 : 0; //se la variabile diventa negativa già ci sono delle parentesi chiuse non corrisposte
		if(a->parentesi >= 0){
			unsigned indice=strchr(a->statoCorrente->inputs,i)-a->statoCorrente->inputs;
			//printf("indice:\t%u\n",indice);
			struct Stato* statoSuccessivo=a->statoCorrente->statiSuccessivi[indice];
			a->statoCorrente=statoSuccessivo;
			ret=statoSuccessivo->s;
		}
	}
	return ret;
}

int InputValido(struct AutomaSintassi* a, char i){
	return strchr(a->statoCorrente->inputs,i) != NULL;
}

void ResetAutoma(struct AutomaSintassi* a){
	//printf("%p\n", statiAutoma[0]);
	a->statoCorrente=a->statoIniziale;
}

char StatoCorrenteAutoma(struct AutomaSintassi* a){
	return a->statoCorrente->s;
}

char VerificaStatoFinale(struct AutomaSintassi* a){
	return a->statoCorrente->s==STATO_FINALE && a->parentesi==0;
}
