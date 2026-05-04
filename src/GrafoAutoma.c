#include <GrafoAutoma.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct AutomaSintassi* CreaAutoma(){

	struct AutomaSintassi* automa=(struct AutomaSintassi*)malloc(sizeof(struct AutomaSintassi));
	const char* stati="012345678";
	const char* inputsStati[]={" ()xlesc","n","(","x","p",") ^/*+-","i","o","s"};
	const char* statiSuccessiviStr[]={"00551367","2","0","4","2","5500000","1","8","2"};
	const size_t numeroStati=strlen(stati);
	
	//creo i vertici del grafo
	struct Stato *stato_0=NULL, *stato_prec=NULL;
	for(size_t i=0; i<numeroStati; ++i){
		struct Stato* vertice=(struct Stato*)malloc(sizeof(struct Stato));
		vertice->s=stati[i];
		vertice->inputs=(const char*)malloc(sizeof(char)*(strlen(inputsStati[i])+1));
		vertice->successivo=NULL;
		strcpy((char*)vertice->inputs,inputsStati[i]);
		
		//collego i vertici nella lista circolare
		if(i==0){
			stato_0=vertice;
			stato_prec=vertice;
		}else{
			stato_prec->successivo=vertice;
			stato_prec=vertice;
		}
	}
	//stato_prec->successivo=stato_0;	//Chiudo il cerchio
	automa->statoIniziale=stato_0;
	
	//collegamento tra vertici
	stato_prec=stato_0;
	for(size_t i=0; i<numeroStati; ++i){
		const char* ss=statiSuccessiviStr[i];
		size_t numeroStatiSuccessivi=strlen(ss);
		stato_prec->statiSuccessivi=(struct Stato**)malloc(sizeof(struct Stato)*numeroStatiSuccessivi);
		for(size_t j=0; j<numeroStatiSuccessivi; ++j){
			unsigned indiceStato=ss[j]-'0';
			struct Stato* stato_tmp=stato_0;
			while(indiceStato--) stato_tmp=stato_tmp->successivo;
			stato_prec->statiSuccessivi[j]=stato_tmp;
		}
		stato_prec=stato_prec->successivo;
	}
	automa->statoCorrente=stato_0;
	automa->parentesi=0;
	return automa;
}

void CancellaAutoma(struct AutomaSintassi* a){
	struct Stato* stato_tmp=NULL;
	while(a->statoIniziale){
		stato_tmp=a->statoIniziale->successivo;
		free((void*)a->statoIniziale->inputs);
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
