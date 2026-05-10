#include <GrafoAutoma.h>
#include <TabellaDerivate.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INPUT_BASE " (x0123456789"
#define INPUT_OPERATORI ") ^*+/-0123456789"

struct AutomaSintassi* CreaAutoma(struct Tabella* tabella){
	size_t numeroStati=2, dimensioneMassima=0;
	struct CoppiaDerivata* testaTmp=tabella->testa;
	struct AutomaSintassi* automa=(struct AutomaSintassi*)malloc(sizeof(struct AutomaSintassi));
	automa->statoIniziale=NULL;
	size_t numeroInputBase=strlen(INPUT_BASE), numeroInputOperatori=strlen(INPUT_OPERATORI);
	
	//Identifico il numero di stati
	for(size_t i=0; i<tabella->numeroVoci; ++i){
		size_t dimensioneStringa=strlen(testaTmp->chiave);
		dimensioneMassima = dimensioneStringa > dimensioneMassima ? dimensioneStringa : dimensioneMassima;
		numeroStati += strlen(testaTmp->chiave)-1;
		
		testaTmp=testaTmp->succ;
	}
	
	//Creo i vertici
	testaTmp=tabella->testa->succ;
	size_t contatoreStati=1,contatoreVoci=0;
	struct Stato *statoPrecedente=NULL;
	size_t dimensioneStringaInputs=(2*(numeroStati-1)+numeroInputBase+tabella->numeroVoci+1)+numeroInputOperatori+1;
	char* stringaInputs=(char*)calloc(dimensioneStringaInputs+3,sizeof(char)); //3 di span
	
	//stato 0
	struct Stato* vertice0=(struct Stato*)malloc(sizeof(struct Stato));
	vertice0->s=0;
	vertice0->inputs=stringaInputs;
	vertice0->statiSuccessivi=(struct Stato**)calloc(tabella->numeroVoci+numeroInputBase+1,sizeof(struct Stato*));
	
	//cappi per ' ' e '('
	vertice0->statiSuccessivi[0]=vertice0;
	vertice0->statiSuccessivi[1]=vertice0;
	strcpy(stringaInputs,INPUT_BASE);
	/*stringaInputs[0]=' ';
	stringaInputs[1]='(';
	stringaInputs[2]='0';
	stringaInputs[3]='1';
	stringaInputs[3]='2';
	stringaInputs[3]='3';
	stringaInputs[3]='4';
	stringaInputs[3]='5';
	stringaInputs[3]='1';
	stringaInputs[3]='1';
	stringaInputs[3]='1';
	stringaInputs[3]='1';*/
	
	//stato finale delle funzioni
	struct Stato* verticeFinale=(struct Stato*)malloc(sizeof(struct Stato));
	char* inputStatoFinale=stringaInputs+dimensioneStringaInputs-numeroInputOperatori-3;
	inputStatoFinale[0]='(';	
	verticeFinale->s=numeroStati-1;
	verticeFinale->inputs=inputStatoFinale;
	verticeFinale->statiSuccessivi=(struct Stato**)calloc(2,sizeof(struct Stato*));
	verticeFinale->statiSuccessivi[0]=vertice0;
	
	//stato operatori
	struct Stato* verticeOperatori=(struct Stato*)malloc(sizeof(struct Stato));
	verticeOperatori->s=numeroStati;
	verticeOperatori->inputs=stringaInputs+dimensioneStringaInputs-numeroInputOperatori-1;
	verticeOperatori->statiSuccessivi=(struct Stato**)calloc(numeroInputOperatori+1,sizeof(struct Stato*));
	strcpy((char*)(verticeOperatori->inputs),INPUT_OPERATORI);
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
	verticeOperatori->successivo=NULL;
	
	verticeFinale->successivo=verticeOperatori;
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
	
	statoPrecedente=vertice0;
	while(testaTmp){
		//scrivo per l'input dello stato 0
		stringaInputs[contatoreVoci+numeroInputBase]=testaTmp->chiave[0];
		
		//creo i vertici intermedi e li collego
		for(unsigned i=1;testaTmp->chiave[i];++i){
			char* inputCaratteri=stringaInputs+numeroInputBase+tabella->numeroVoci+1+(contatoreStati-1)*2;
			struct Stato* vertice=(struct Stato*)malloc(sizeof(struct Stato));
			
			inputCaratteri[0]=testaTmp->chiave[i];
			vertice->s=contatoreStati;
			vertice->inputs=inputCaratteri;
			vertice->statiSuccessivi=(struct Stato**)calloc(2,sizeof(struct Stato*));
			
			if(i==1){
				vertice0->statiSuccessivi[numeroInputBase+contatoreVoci]=vertice;
			}else{
				statoPrecedente->statiSuccessivi[0]=vertice;
			}
			
			//per la lista
			statoPrecedente->successivo=vertice;
			statoPrecedente=vertice;
			contatoreStati++;
		}
		
		//per il grafo, collegamento allo stato finale
		statoPrecedente->statiSuccessivi[0]=verticeFinale;
		contatoreVoci++;
		testaTmp=testaTmp->succ;
	}
	
	statoPrecedente->successivo=verticeFinale;	
	automa->statoIniziale=vertice0;
	automa->statoCorrente=vertice0;
	automa->numeroStati=numeroStati+1;
	
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

unsigned char InputAutoma(struct AutomaSintassi* a, char i){
	unsigned char ret=-1;
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
	return a->statoCorrente->s==a->numeroStati-1 && a->parentesi==0;
}
