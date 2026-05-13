#include <GrafoAutoma.h>
#include <TabellaDerivate.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INPUT_INIZIALE_BASE " (x0123456789-"
#define INPUT_OPERATORI ") ^*+/-0123456789"

void CollegamentoVertici(struct Stato* v1, struct Stato* v2, char input);

struct AutomaSintassi* CreaAutoma(struct Tabella* tabella){
	struct AutomaSintassi* automa=(struct AutomaSintassi*)malloc(sizeof(struct AutomaSintassi));
	
	size_t numeroStati=3;
	size_t numeroInputBase=strlen(INPUT_INIZIALE_BASE), numeroInputOperatori=strlen(INPUT_OPERATORI);
	
	//Creo i vertici degli stati iniziale,fine funzione,operatore
	struct Stato* vertice0=(struct Stato*)malloc(sizeof(struct Stato));
	//struct Stato* verticeMeno=(struct Stato*)malloc(sizeof(struct Stato));
	struct Stato* verticeFinale=(struct Stato*)malloc(sizeof(struct Stato));
	struct Stato* verticeOperatori=(struct Stato*)malloc(sizeof(struct Stato));
	
	//stato iniziale
	vertice0->s=0;
	vertice0->inputs=(char*)calloc(numeroInputBase+1,sizeof(char));
	vertice0->statiSuccessivi=(struct Stato**)calloc(numeroInputBase+1,sizeof(struct Stato*));
	vertice0->successivo=verticeFinale;
	
	strcpy(vertice0->inputs,INPUT_INIZIALE_BASE);
	
	//collegamenti input base
	vertice0->statiSuccessivi[0]=vertice0;
	vertice0->statiSuccessivi[1]=vertice0;
	vertice0->statiSuccessivi[2]=verticeOperatori;
	vertice0->statiSuccessivi[3]=verticeOperatori;
	vertice0->statiSuccessivi[4]=verticeOperatori;
	vertice0->statiSuccessivi[5]=verticeOperatori;
	vertice0->statiSuccessivi[6]=verticeOperatori;
	vertice0->statiSuccessivi[7]=verticeOperatori;
	vertice0->statiSuccessivi[8]=verticeOperatori;
	vertice0->statiSuccessivi[9]=verticeOperatori;
	vertice0->statiSuccessivi[10]=verticeOperatori;
	vertice0->statiSuccessivi[11]=verticeOperatori;
	vertice0->statiSuccessivi[12]=verticeOperatori;
	vertice0->statiSuccessivi[13]=vertice0;
	
	//stato finale delle funzioni
	verticeFinale->s=1;
	verticeFinale->inputs=(char*)calloc(2,sizeof(char));
	verticeFinale->statiSuccessivi=(struct Stato**)calloc(2,sizeof(struct Stato*));
	verticeFinale->successivo=verticeOperatori;
	
	verticeFinale->inputs[0]='(';
	verticeFinale->statiSuccessivi[0]=vertice0;
		
	//stato operatori
	verticeOperatori->s=2;
	verticeOperatori->inputs=(char*)calloc(numeroInputOperatori+1,sizeof(char));
	verticeOperatori->statiSuccessivi=(struct Stato**)calloc(numeroInputOperatori+1,sizeof(struct Stato*));
	verticeOperatori->successivo=NULL;
	
	strcpy((char*)(verticeOperatori->inputs),INPUT_OPERATORI);
	
	//collegamento input operatori
	verticeOperatori->statiSuccessivi[0]=verticeOperatori;
	verticeOperatori->statiSuccessivi[1]=verticeOperatori;
	verticeOperatori->statiSuccessivi[2]=vertice0;
	verticeOperatori->statiSuccessivi[3]=vertice0;
	verticeOperatori->statiSuccessivi[4]=vertice0;
	verticeOperatori->statiSuccessivi[5]=vertice0;
	verticeOperatori->statiSuccessivi[6]=vertice0;
	verticeOperatori->statiSuccessivi[7]=verticeOperatori;
	verticeOperatori->statiSuccessivi[8]=verticeOperatori;
	verticeOperatori->statiSuccessivi[9]=verticeOperatori;
	verticeOperatori->statiSuccessivi[10]=verticeOperatori;
	verticeOperatori->statiSuccessivi[11]=verticeOperatori;
	verticeOperatori->statiSuccessivi[12]=verticeOperatori;
	verticeOperatori->statiSuccessivi[13]=verticeOperatori;
	verticeOperatori->statiSuccessivi[14]=verticeOperatori;
	verticeOperatori->statiSuccessivi[15]=verticeOperatori;
	verticeOperatori->statiSuccessivi[16]=verticeOperatori;
	
	//creazione degli stati intermedi per le funzioni
	struct CoppiaDerivata* testaTmp=tabella->testa->succ;
	struct Stato* statoPrecedente=verticeOperatori; //per il collegamento della lista
	while(testaTmp){
		//TODO: fare il ciclo per le funzioni
		struct Stato* verticeCorrente=vertice0;
		size_t dimensioneFunzione=strlen(testaTmp->chiave);
		for(size_t i=0; i < dimensioneFunzione-1; ++i){
			char carattereInput=testaTmp->chiave[i];
			char* caratterePresente=strchr(verticeCorrente->inputs,carattereInput);
			if(caratterePresente){ //Se in questo vertice è già presente un carattere prendo il suo vertice successivo
				unsigned indiceCarattere=caratterePresente-verticeCorrente->inputs;
				verticeCorrente=verticeCorrente->statiSuccessivi[indiceCarattere];
			}else{ //altrimenti creo il nuovo vertice
				struct Stato* nuovoVertice=(struct Stato*)malloc(sizeof(struct Stato));
				nuovoVertice->s=numeroStati++;
				nuovoVertice->inputs="";
				nuovoVertice->statiSuccessivi=(struct Stato **)NULL;
				
				CollegamentoVertici(verticeCorrente,nuovoVertice,carattereInput);
				
				//collegamento con la lista
				statoPrecedente->successivo=nuovoVertice;
				nuovoVertice->successivo=(struct Stato*)NULL;
				statoPrecedente=nuovoVertice;
				
				verticeCorrente=nuovoVertice;
			}
		}
		//collego l'ultimo vertice con quello finale delle funzioni
		CollegamentoVertici(verticeCorrente,verticeFinale,testaTmp->chiave[dimensioneFunzione-1]);
		testaTmp=testaTmp->succ;
	}
	
	automa->statoIniziale=vertice0;
	automa->statoCorrente=vertice0;
	automa->numeroStati=numeroStati;
	automa->parentesi=0;
	automa->meno=0;
	//printf("Numero stati: %lu\n", numeroStati);
	
	return automa;
}

void CancellaAutoma(struct AutomaSintassi* a){
	struct Stato* stato_tmp=NULL;
	free((void*)a->statoIniziale->inputs);
	while(a->statoIniziale){
		stato_tmp=a->statoIniziale->successivo;
		free((void*)a->statoIniziale->statiSuccessivi);
		free((void*)a->statoIniziale);
		a->statoIniziale=stato_tmp;
	}
}

unsigned char InputAutoma(struct AutomaSintassi* a, char i){
	unsigned char ret=-1;
	if(InputValido(a,i)){
		a->parentesi += i=='(' ? 1 : i==')' ? -1 : 0; //se la variabile diventa negativa già ci sono delle parentesi chiuse non corrisposte
		if(a->parentesi >= 0 && !(a->meno * i=='-')){ //errore se dopo un '-' dello stato iniziale metto un altro '-'
			unsigned indice=strchr(a->statoCorrente->inputs,i)-a->statoCorrente->inputs;
			struct Stato* statoSuccessivo=a->statoCorrente->statiSuccessivi[indice];
			a->statoCorrente=statoSuccessivo;
			ret=statoSuccessivo->s;
			
			a->meno= a->statoCorrente->s==STATO_INIZIALE && i=='-'; //questa condizione è 1 solamente quando attraverso il cappio dell'input meno dello stato iniziale
		}
	}
	return ret;
}

int InputValido(struct AutomaSintassi* a, char i){
	return strchr(a->statoCorrente->inputs,i) != NULL;
}

void ResetAutoma(struct AutomaSintassi* a){
	a->statoCorrente=a->statoIniziale;
}

char StatoCorrenteAutoma(struct AutomaSintassi* a){
	return a->statoCorrente->s;
}

char VerificaStatoFinale(struct AutomaSintassi* a){
	return a->statoCorrente->s==STATO_OPERATORI && a->parentesi==0;
}

void CollegamentoVertici(struct Stato* v1, struct Stato* v2, char input){
	//aggiorno il buffer degli input dello stato corrente
	size_t numeroInputs=strlen(v1->inputs);
	char* nuoviInputs=(char*)calloc(numeroInputs+2, sizeof(char));
	strcpy(nuoviInputs,v1->inputs);
	nuoviInputs[numeroInputs]=input;
	if(numeroInputs > 0) free((void*)v1->inputs);
	v1->inputs=nuoviInputs;
	
	//aggiorno la sequenza dei vertici successivi dello stato corrente
	unsigned numeroStatiSuccessivi=0;
	struct Stato** statiSuccessiviTmp=v1->statiSuccessivi;
	while(statiSuccessiviTmp && statiSuccessiviTmp[numeroStatiSuccessivi]) numeroStatiSuccessivi++;
	//printf("Numero stati successivi: %u\n", numeroStatiSuccessivi);
	struct Stato** nuoviStatiSuccessivi=(struct Stato**)malloc((numeroStatiSuccessivi+2)*sizeof(struct Stato*));
	if(numeroStatiSuccessivi){
		memcpy(nuoviStatiSuccessivi,v1->statiSuccessivi,numeroStatiSuccessivi*sizeof(struct Stato*));
		free((void*)v1->statiSuccessivi);
	}
	nuoviStatiSuccessivi[numeroStatiSuccessivi]=v2;
	nuoviStatiSuccessivi[numeroStatiSuccessivi+1]=(struct Stato*)NULL;
	v1->statiSuccessivi=nuoviStatiSuccessivi;
}
