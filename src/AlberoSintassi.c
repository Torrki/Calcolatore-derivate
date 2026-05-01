#include <AlberoSintassi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);
const char* ordineOperatori="./*+-";

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
		printf("Indirizzo dell'albero da stampare NULL\n");
	}
}


Nodo CreaAlbero(const char* funzioneSym, size_t* numeroCaratteri){
	//inizializzazione per la costruzione dell'albero
	Nodo nodoCorrente,nodoRadice,sottoAlberoParentesi;
	int livello_max=0, livello=0;
	size_t caratteriProcessati=0, caratteriSottoalbero=0;
	nodoCorrente.no=NULL;
	nodoRadice.no=NULL;
	sottoAlberoParentesi.no=NULL;
	char statoSintassi='f';	//variabile di stato per alternare la lettura tra funzioni e operatori
	
	char* bufferStringa=(char*)calloc(100,sizeof(char));
	char ultimoOperatore='.'; //operatore a massima priorità
	char condizioneCiclo=1; //la condizione del ciclo è se non ci sono stati errori di sintassi o fine della stringa
	unsigned i=0;
		
	//processamento della stringa
	while(condizioneCiclo){
		char carattere=*funzioneSym;
		
		//se il carattere letto è una funzione o un operatore
		if(statoSintassi == 'o' && operatore(carattere)){
			bufferStringa[i]='\0';
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
			nuovoOp->op=carattere;
			
			//stabilire la priorità per la costruzione dell'albero
			if(prioritaOperatori(ultimoOperatore,carattere)){	//salgo
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
			ultimoOperatore=carattere;
			
			if(livello > livello_max) {
				nodoRadice=nodoCorrente;
				livello_max=livello;
			}
			
			//torno a inizio buffer
			statoSintassi='f';
			i=0;
		}else if(statoSintassi == 'f' && carattere == '('){
			//Il contenuto tra parentesi ha maggiore priorità e bisogna creare un albero specifico
			sottoAlberoParentesi=CreaAlbero(funzioneSym+1,&caratteriSottoalbero);
			statoSintassi='o';
		}else if(statoSintassi == 'o' && (*funzioneSym=='\n' || *funzioneSym==')') ){ //fine del ciclo
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
			condizioneCiclo=0;
		}else if(statoSintassi == 'f' && !operatore(carattere)){
			bufferStringa[i]=carattere;
			i++;
			statoSintassi='o';			
		}else{
			printf("Errore nella sintassi della funzione al carattere %ld\n", caratteriProcessati+1);
			condizioneCiclo=0;
			nodoRadice.nf=(struct NodoFunzione*)NULL;
		}
		funzioneSym += sottoAlberoParentesi.nf == NULL ? 1 : caratteriSottoalbero+2;
		caratteriProcessati += sottoAlberoParentesi.nf == NULL ? 1 : caratteriSottoalbero+2;
	}
	
	return nodoRadice;
}

int operatore(char c){
	return (c==ordineOperatori[1]) | (c==ordineOperatori[2]) | (c==ordineOperatori[3]) | (c==ordineOperatori[4]);
}

int prioritaOperatori(char uo, char o){
	unsigned pos_uo=0,pos_o=0;
	for(long unsigned i=0; i < strlen(ordineOperatori); i++){
		if(uo==ordineOperatori[i]) pos_uo=i;
		if(o==ordineOperatori[i]) pos_o=i;
	}
	return pos_uo < pos_o;
}
