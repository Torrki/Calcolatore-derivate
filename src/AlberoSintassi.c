#include <AlberoSintassi.h>
#include <stdio.h>
#include <string.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);
const char* ordineOperatori=".@^/*+-";

void StampaAlbero(Nodo n, unsigned livello){
	if(n.ni != (struct NodoInfo*)NULL){
		for(unsigned i=0; i < livello; i++){
			printf(i == livello-1 ? "|" : "-");
		}
		
		if(n.ni->tipo=='o'){
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

const char* AlberotoStr(Nodo n){
	char* stringaFunzione=NULL;
	if(n.ni != (struct NodoInfo*)NULL){		
		if(n.ni->tipo=='o'){
			const char* operandoSx=AlberotoStr(n.no->n1);
			const char* operandoDx=AlberotoStr(n.no->n2);
			
			stringaFunzione=(char*)malloc(sizeof(char)*(strlen(operandoSx)+strlen(operandoDx+3)));
			sprintf(stringaFunzione,"%s%c%s%c",operandoSx, n.no->op=='@' ? '(' : n.no->op, operandoDx, n.no->op=='@' ? ')' : '\0');
			free((void*)operandoSx);
			free((void*)operandoDx);
		}else{
			stringaFunzione=(char*)malloc(sizeof(char)*(strlen(n.nf->f)+1));
			strcpy(stringaFunzione,n.nf->f);
		}
	}
	return stringaFunzione;		
}


Nodo CreaAlbero(struct AutomaSintassi *a, const char* funzioneSym, size_t* numeroCaratteri){
	//inizializzazione per la costruzione dell'albero
	Nodo nodoCorrente,nodoRadice,sottoAlberoParentesi;
	
	int livello_max=0, livello=0;
	size_t caratteriProcessati=0, caratteriSottoalbero=0;
	nodoCorrente.ni=NULL;
	nodoRadice.ni=NULL;
	sottoAlberoParentesi.ni=NULL;
	
	char* bufferStringa=(char*)malloc(100*sizeof(char));
	char ultimoOperatore='.'; //operatore a massima priorità
	char nonFineStringa=1;
	unsigned i=0;
	char statoAutomaFunzioni='0';
	char carattere=*funzioneSym;
	//char faseSD=0, faseSD_prec=0;	//variabile per indicare salita (0) nella costruzione o discesa (1)
	
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
			if(sottoAlberoParentesi.ni == NULL){ //se non ci sono state parentesi a separare l'operatore
				funzioneOp.nf=(NodoFunzione*)malloc(sizeof(NodoFunzione));
				funzioneOp.nf->tipo='f';
				funzioneOp.nf->f=(char*)malloc(strlen(bufferStringa)+1);
				strcpy(funzioneOp.nf->f, (const char *)bufferStringa);
			}else{
				funzioneOp=sottoAlberoParentesi;
				sottoAlberoParentesi.ni=NULL;
			}
			nuovoOp->tipo='o';
			nuovoOp->op=op;
			
			//stabilire la priorità per la costruzione dell'albero
			if(prioritaOperatori(ultimoOperatore,op)){	//salgo
				/*faseSD = 0;
				if(faseSD_prec){	//torno nella condizione iniziale dell'algoritmo per individuare le nuove radici
					livello=0;
					livello_max=0;
				}*/
				
				if(nodoCorrente.ni == NULL){	//primo passo di costruzione
					nuovoOp->n1=funzioneOp;
					funzioneOp.nf->genitore=nuovoOp;
				}else{	//passi successivi
					nodoCorrente.no->n2=funzioneOp;
					nuovoOp->n1.no=nodoCorrente.no;
					
					funzioneOp.ni->genitore=nodoCorrente.no;
					nodoCorrente.no->genitore=nuovoOp;
				}
				
				livello++;
			}else{	//scendo
				/*faseSD=1;
				if(!faseSD_prec){	//memorizzo la nuova radice
					NodoRadice* nuovoNodo=(NodoRadice*)malloc(sizeof(NodoRadice));
					nuovoNodo->radice=nodoRadice;
					nuovoNodo->successivo=NULL;
					
					if(RadiceAlbero){
						NodoRadice* tmp=RadiceAlbero;
						while(tmp->successivo) tmp=tmp->successivo;
						tmp->successivo=nuovoNodo;
					}else{
						RadiceAlbero=nuovoNodo;
					}
				}*/
				nodoCorrente.no->n2.no=nuovoOp;
				nuovoOp->n1=funzioneOp;
				
				funzioneOp.ni->genitore=nuovoOp;
				nuovoOp->genitore=nodoCorrente.no;
				
				livello--;
			}
			//faseSD_prec=faseSD;
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
				ultimaFunzione.ni->genitore=NULL;
				nodoRadice=ultimaFunzione;
			}else{	//completo l'ultimo operatore
				nodoCorrente.no->n2=ultimaFunzione;
				ultimaFunzione.ni->genitore=nodoCorrente.no;
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

const char* AnalizzaAlbero(struct CoppiaDerivata* tabella, Nodo radice){
	char* stringaDerivata=NULL;
	if(radice.no){
		//Se il nodo radice è un operatore devo sostituire con la regola di derivazione e risolvere ricorsivamente gli operandi
		if(radice.no->tipo=='o'){
			//risolvo gli operandi
			const char* derivataOpSx=AnalizzaAlbero(tabella,radice.no->n1);
			const char* derivataOpDx=AnalizzaAlbero(tabella,radice.no->n2);
			const char *OpDx, *OpSx;
			size_t dimSx=strlen(derivataOpSx);
			size_t dimDx=strlen(derivataOpDx);
			
			switch(radice.no->op){
				case '+':
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+2));
					sprintf(stringaDerivata,"%s+%s",derivataOpSx,derivataOpDx);
					break;
				case '-':
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+2));
					sprintf(stringaDerivata,"%s-%s",derivataOpSx,derivataOpDx);
					break;
				case '*':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+strlen(OpSx)+strlen(OpDx)+16));
					sprintf(stringaDerivata,"((%s)*(%s))+((%s)*(%s))",derivataOpSx,OpDx,OpSx,derivataOpDx);
					
					free((void*)OpSx);
					free((void*)OpDx);
					break;
				case '/':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+strlen(OpSx)+strlen(OpDx)*2+24));
					sprintf(stringaDerivata,"(((%s)*(%s))-((%s)*(%s)))/((%s)^2)",derivataOpSx,OpDx,OpSx,derivataOpDx,OpDx);
					
					free((void*)OpSx);
					free((void*)OpDx);
					break;
				case '@':
					OpDx=AlberotoStr(radice.no->n2);
					char* DerivataComposta=(char*)malloc(sizeof(char)*(dimSx+strlen(OpDx)-1));
					sprintf(DerivataComposta,derivataOpSx,OpDx);
					
					stringaDerivata=(char*)malloc(sizeof(char)*(strlen(DerivataComposta)+dimDx+4));
					
					sprintf(stringaDerivata,"(%s)*%s",DerivataComposta,derivataOpDx);
					free((void*)OpDx);
					free((void*)DerivataComposta);
					break;
				case '^':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+3*strlen(OpSx)+2*strlen(OpDx)+29));
					sprintf(stringaDerivata,"((%s)^(%s))*((%s)*ln(%s)+(%s)*((%s)/(%s)))",OpSx,OpDx,derivataOpDx,OpSx,OpDx,derivataOpSx,OpSx);
					
					free((void*)OpSx);
					free((void*)OpDx);
					break;
			}
			
			free((void*)derivataOpSx);
			free((void*)derivataOpDx);
		}else{
			const char* df=OttieniDerivata(tabella,radice.nf->f);
			stringaDerivata=(char*)malloc(sizeof(char)*(strlen(df)+1));
			strcpy(stringaDerivata,df);
		}
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
