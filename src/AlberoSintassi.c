#include <AlberoSintassi.h>
#include <GrafoAutoma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);
const char* ordineOperatori=".@/*+-";

void StampaAlbero(Nodo n, unsigned livello){
	if(n.nf != (struct NodoFunzione*)NULL){
		for(unsigned i=0; i < livello; i++){
			printf(i == livello-1 ? "|" : "-");
		}
		
		if(n.no->tipo=='o'){
			printf("%c\n", n.no->op);
			StampaAlbero(n.no->n1,livello+1);
			StampaAlbero(n.no->n2,livello+1);
		}else{
			printf("%s\n", n.nf->f);
		}
	}else{
		printf("Albero vuoto\n");
	}
}


Nodo CreaAlbero(struct AutomaSintassi *a, const char* funzioneSym, size_t* numeroCaratteri){
	//inizializzazione per la costruzione dell'albero
	Nodo nodoCorrente,nodoRadice,sottoAlberoParentesi;
	int livello_max=0, livello=0;
	size_t caratteriProcessati=0, caratteriSottoalbero=0;
	nodoCorrente.no=NULL;
	nodoRadice.no=NULL;
	sottoAlberoParentesi.no=NULL;
	
	char* bufferStringa=(char*)malloc(100*sizeof(char));
	char ultimoOperatore='.'; //operatore a massima priorità
	char nonFineStringa=1;
	unsigned i=0;
	char statoAutomaFunzioni='0';
	char carattere=*funzioneSym;
	
	ResetAutoma(a);
	
	//processamento della stringa
	while(nonFineStringa){
		carattere=*funzioneSym;
		//char statoAutomaFunzioni=StatoCorrenteAutoma();
		
		//se il carattere letto è un operatore o una parentesi per la composizione
		if(operatore(carattere) || statoAutomaFunzioni==STATO_COMPOSIZIONE){
			bufferStringa[i]='\0';
			char op=statoAutomaFunzioni==STATO_COMPOSIZIONE ? '@' : carattere;
			
			NodoOperatore* nuovoOp=(NodoOperatore*)malloc(sizeof(NodoOperatore));			
			Nodo funzioneOp;
			if(sottoAlberoParentesi.no == NULL){ //se non ci sono state parentesi a separare l'operatore
				funzioneOp.nf=(NodoFunzione*)malloc(sizeof(NodoFunzione));
				funzioneOp.nf->tipo='f';
				funzioneOp.nf->f=(char*)malloc(strlen(bufferStringa)+1);
				strcpy(funzioneOp.nf->f, (const char *)bufferStringa);
			}else{
				funzioneOp=sottoAlberoParentesi;
				sottoAlberoParentesi.no=NULL;
			}
			nuovoOp->tipo='o';
			nuovoOp->op=op;
			
			//stabilire la priorità per la costruzione dell'albero
			if(prioritaOperatori(ultimoOperatore,op)){	//salgo
				if(nodoCorrente.no == NULL){	//primo passo di costruzione
					nuovoOp->n1=funzioneOp;
				}else{	//passi successivi
					nodoCorrente.no->n2=funzioneOp;
					nuovoOp->n1.no=nodoCorrente.no;
				}
				
				livello++;
			}else{	//scendo
				nodoCorrente.no->n2.no=nuovoOp;
				nuovoOp->n1=funzioneOp;
				
				livello--;
			}
			nodoCorrente.no=nuovoOp;
			ultimoOperatore=op;
			
			if(livello > livello_max) {
				nodoRadice=nodoCorrente;
				livello_max=livello;
			}
			
			//torno a inizio buffer
			i=0;
		}else if(carattere == '(' && statoAutomaFunzioni == STATO_INIZIALE){
			//Il contenuto tra parentesi ha maggiore priorità e bisogna creare un albero specifico
			sottoAlberoParentesi=CreaAlbero(a,funzioneSym+1,&caratteriSottoalbero);
			ResetAutoma(a); //Per tornare allo stato 0
		}else if(carattere=='\n' || carattere==')'){ //fine del ciclo
			//lettura ultima funzione
			Nodo ultimaFunzione;
			if(sottoAlberoParentesi.no == NULL){ //se non ci sono state parentesi a separare l'operatore
				bufferStringa[i]='\0';
				ultimaFunzione.nf=(NodoFunzione*)malloc(sizeof(NodoFunzione));
				ultimaFunzione.nf->tipo='f';
				ultimaFunzione.nf->f=(char*)malloc(strlen(bufferStringa)+1);
				strcpy(ultimaFunzione.nf->f, (const char *)bufferStringa);
			}else{
				ultimaFunzione=sottoAlberoParentesi;
				sottoAlberoParentesi.no=NULL;
			}
			
			if(livello_max==0){	//se non ho mai incontrato un operatore
				nodoRadice=ultimaFunzione;
			}else{	//completo l'ultimo operatore
				nodoCorrente.no->n2=ultimaFunzione;
			}
			
			*numeroCaratteri=caratteriProcessati;
			nonFineStringa=0;
		}else{
			bufferStringa[i]=carattere;
			i++;				
		}
		
		//faccio avanzare l'automa e conto tutti i caratteri processati
		if(statoAutomaFunzioni != STATO_COMPOSIZIONE){	//if necessario per trattare come sottoalbero l'argomento delle funzioni
			funzioneSym += sottoAlberoParentesi.nf == NULL ? 1 : caratteriSottoalbero+2;
			caratteriProcessati += sottoAlberoParentesi.nf == NULL ? 1 : caratteriSottoalbero+2;
		}
		char res=InputAutoma(a,carattere);
		statoAutomaFunzioni = res >= 0 ? res : statoAutomaFunzioni;
	}
	
	return nodoRadice;
}

void DeallocaAlbero(Nodo radice){
	if(radice.no){
		if(radice.no->tipo=='o'){
			DeallocaAlbero(radice.no->n1);
			DeallocaAlbero(radice.no->n2);
		}else{	//dealloco la funzione
			free(radice.nf->f);
		}
		free((void*)radice.no);
	}
}

char* AnalizzaAlbero(Nodo radice){
	char* stringaDerivata=NULL;
	if(radice.no){
	}
	return stringaDerivata;
}

int operatore(char c){
	return strchr(ordineOperatori,c) != NULL;
}

int prioritaOperatori(char uo, char o){
	unsigned pos_uo=0,pos_o=0;
	for(long unsigned i=0; i < strlen(ordineOperatori); i++){
		if(uo==ordineOperatori[i]) pos_uo=i;
		if(o==ordineOperatori[i]) pos_o=i;
	}
	return pos_uo < pos_o;
}
