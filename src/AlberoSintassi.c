#include <AlberoSintassi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);
char Parentesi(char* esp,size_t *caratteriProcessati, struct AutomaSintassi *a);

const char* ordineOperatori=".@^/*+-";

char SoloNumeri(const char* stringa);

char SoloNumeri(const char* stringa){
	size_t conteggio=0;
	while('0' <= stringa[conteggio] && stringa[conteggio] <= '9') conteggio++;
	return conteggio == strlen(stringa);
}

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
			sprintf(stringaFunzione,"%s%c%s%c", operandoSx, n.no->op=='@' ? '(' : n.no->op, operandoDx, n.no->op=='@' ? ')' : '\0');
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
	
	ResetAutoma(a);
	
	//processamento della stringa
	while(nonFineStringa){
		carattere=*funzioneSym;
		//char statoAutomaFunzioni=StatoCorrenteAutoma();
		
		//se il carattere letto è un operatore o una parentesi per la composizione
		if(operatore(carattere) || statoAutomaFunzioni==(unsigned char)STATO_PARENTESI){
			bufferStringa[i]='\0';
			char op=statoAutomaFunzioni==(unsigned char)STATO_PARENTESI ? '@' : carattere;
			
			NodoOperatore* nuovoOp=(NodoOperatore*)malloc(sizeof(NodoOperatore));			
			Nodo funzioneOp;
			if(sottoAlberoParentesi.ni == NULL){ //se non ci sono state parentesi a separare l'operatore
				funzioneOp.nf=(NodoFunzione*)malloc(sizeof(NodoFunzione));
				funzioneOp.nf->tipo='f';
				funzioneOp.nf->genitore=NULL;
				funzioneOp.nf->f=(char*)malloc(strlen(bufferStringa)+1);
				strcpy(funzioneOp.nf->f, (const char *)bufferStringa);
			}else{
				funzioneOp=sottoAlberoParentesi;
				funzioneOp.ni->genitore=NULL;
				sottoAlberoParentesi.ni=NULL;
			}
			nuovoOp->tipo='o';
			nuovoOp->op=op;
			
			//stabilire la priorità per la costruzione dell'albero
			if(prioritaOperatori(ultimoOperatore,op)){	//salgo
				
				if(nodoCorrente.ni == NULL){	//primo passo di costruzione
					nuovoOp->n1=funzioneOp;
					funzioneOp.nf->genitore=nuovoOp;
					nodoRadice.no=nuovoOp;
				}else{	//passi successivi
					nodoCorrente.no->n2=funzioneOp;
					funzioneOp.ni->genitore=nodoCorrente.no;
										
					//cerco l'ultimo operatore per mantenere le priorità
					NodoOperatore* genitore=nodoCorrente.ni->genitore;
					while(genitore && prioritaOperatori(genitore->op,op)){
						genitore=genitore->genitore;
					}
					if(genitore){	//non sono arrivato alla radice
						nuovoOp->n1=genitore->n2;
						genitore->n2.no->genitore=nuovoOp;
						genitore->n2.no=nuovoOp;
						
						nuovoOp->genitore=genitore;
					}else{
						nuovoOp->n1=nodoRadice;
						nodoRadice.ni->genitore=nuovoOp;
						nodoRadice.no=nuovoOp;
					}
				}
			}else{	//scendo
				nodoCorrente.no->n2.no=nuovoOp;
				nuovoOp->n1=funzioneOp;
				
				funzioneOp.ni->genitore=nuovoOp;
				nuovoOp->genitore=nodoCorrente.no;
			}
			nodoCorrente.no=nuovoOp;
			ultimoOperatore=op;
			
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
				ultimaFunzione.nf->genitore=NULL;
				ultimaFunzione.nf->f=(char*)malloc(strlen(bufferStringa)+1);
				strcpy(ultimaFunzione.nf->f, (const char *)bufferStringa);
			}else{
				ultimaFunzione=sottoAlberoParentesi;
				ultimaFunzione.nf->genitore=NULL;
				sottoAlberoParentesi.no=NULL;
			}
			
			if(nodoRadice.ni){	//se non ho mai incontrato un operatore
				nodoCorrente.no->n2=ultimaFunzione;
				ultimaFunzione.ni->genitore=nodoCorrente.no;
			}else{	//completo l'ultimo operatore
				nodoRadice=ultimaFunzione;
			}
			
			*numeroCaratteri=caratteriProcessati;
			nonFineStringa=0;
		}else{
			bufferStringa[i]=carattere;
			i++;				
		}
		
		//faccio avanzare l'automa e conto tutti i caratteri processati
		if(statoAutomaFunzioni != (unsigned char)STATO_PARENTESI){	//if necessario per trattare come sottoalbero l'argomento delle funzioni
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

const char* AnalizzaAlbero(struct Tabella* tabella, Nodo radice){
	char* stringaDerivata=NULL;
	if(radice.no){
		//Se il nodo radice è un operatore devo sostituire con la regola di derivazione e risolvere ricorsivamente gli operandi
		if(radice.no->tipo=='o'){
			//risolvo gli operandi
			const char* derivataOpSx=AnalizzaAlbero(tabella,radice.no->n1);
			const char* derivataOpDx=AnalizzaAlbero(tabella,radice.no->n2);
			char numeroSx=strcmp(derivataOpSx,"0")==0, numeroDx=strcmp(derivataOpDx,"0")==0; 
			size_t dimSx=strlen(derivataOpSx);
			size_t dimDx=strlen(derivataOpDx);
			const char *OpDx, *OpSx;
			
			switch(radice.no->op){
				case '+':
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+6));
					
					if(numeroSx || numeroDx){
						sprintf(stringaDerivata,"%s",numeroSx ? derivataOpDx : derivataOpSx);
					}else{
						sprintf(stringaDerivata,"(%s)+(%s)",derivataOpSx,derivataOpDx);
					}
					break;
				case '-':
					OpSx=AlberotoStr(radice.no->n1);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+6));
					printf("%s\n",OpSx);
					
					if(numeroSx || numeroDx){
						sprintf(stringaDerivata,"%s%s", strcmp(OpSx,"")==0 ? "-" : "", numeroSx ? derivataOpDx : derivataOpSx);
					}else{
						sprintf(stringaDerivata,"(%s)-(%s)",derivataOpSx,derivataOpDx);
					}
					break;
				case '*':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+strlen(OpSx)+strlen(OpDx)+12));
					
					if(numeroSx || numeroDx){
						sprintf(stringaDerivata,"%s*(%s)", numeroSx ? OpSx : OpDx, numeroSx ? derivataOpDx : derivataOpSx);
					}else{
						sprintf(stringaDerivata,"(%s)*(%s)+(%s)*(%s)",derivataOpSx,OpDx,OpSx,derivataOpDx);
					}
					free((void*)OpSx);
					free((void*)OpDx);
					break;
				case '/':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+strlen(OpSx)+strlen(OpDx)*2+21));
					
					if(numeroSx || numeroDx){
						sprintf(stringaDerivata,"(%s%s*(%s))/((%s)^2)",numeroSx ? "-" : "", numeroSx ? OpSx : OpDx, numeroSx ? derivataOpDx : derivataOpSx, OpDx);
					}else{
						sprintf(stringaDerivata,"((%s)*(%s)-(%s)*(%s))/((%s)^2)",derivataOpSx,OpDx,OpSx,derivataOpDx,OpDx);
					}
					
					free((void*)OpSx);
					free((void*)OpDx);
					break;
				case '@':
					OpDx=AlberotoStr(radice.no->n2);
					char* DerivataComposta=(char*)malloc(sizeof(char)*(dimSx+strlen(OpDx)-1));
					sprintf(DerivataComposta,derivataOpSx,OpDx);
					
					stringaDerivata=(char*)malloc(sizeof(char)*(strlen(DerivataComposta)+dimDx+6));
					
					if(numeroDx){
						strcpy(stringaDerivata,"0");
					}else{
						sprintf(stringaDerivata,"(%s)*(%s)",DerivataComposta,derivataOpDx);
					}
					
					free((void*)OpDx);
					free((void*)DerivataComposta);
					break;
				case '^':
					OpSx=AlberotoStr(radice.no->n1);
					OpDx=AlberotoStr(radice.no->n2);
					stringaDerivata=(char*)malloc(sizeof(char)*(dimSx+dimDx+3*strlen(OpSx)+2*strlen(OpDx)+29));
					
					if(numeroSx && !numeroDx){
						sprintf(stringaDerivata,"(%s^(%s))*(%s)*ln(%s)",OpSx,OpDx,derivataOpDx,OpSx);
					}else if(numeroDx && !numeroSx){
						int esponente=atoi(OpDx);
						
						//Se esponente intero
						sprintf(stringaDerivata,"(%d*(%s)^%d)*(%s)",esponente,OpSx,esponente-1,derivataOpSx);
					}else{
						sprintf(stringaDerivata,"((%s)^(%s))*((%s)*ln(%s)+(%s)*((%s)/(%s)))",OpSx,OpDx,derivataOpDx,OpSx,OpDx,derivataOpSx,OpSx);
					}
					
					free((void*)OpSx);
					free((void*)OpDx);
					break;
			}
			
			free((void*)derivataOpSx);
			free((void*)derivataOpDx);
		}else{
			const char* df=OttieniDerivata(tabella,radice.nf->f);
			stringaDerivata=(char*)malloc(sizeof(char)*(df ? strlen(df)+1 : 2));
			strcpy(stringaDerivata,df ? df : "0");
		}
	}
	return stringaDerivata;
}

char Parentesi(char* esp, size_t *caratteriProcessati, struct AutomaSintassi *a){
	char* espTmp=esp+1;
	char c=*espTmp, statoAutoma=StatoCorrenteAutoma(a);
	size_t cp=1; //la parentesi aperta d'inizio
	//printf("Carattere inizio: %c\tstato inizio: %hhd\n", c, statoAutoma);
	
	//se è una parentesi per la composizione
	char utile=statoAutoma==STATO_PARENTESI;
	statoAutoma=InputAutoma(a,'(');

	while(c != '\0' && c != ')'){
		if(!utile) utile= statoAutoma==STATO_OPERATORI || (statoAutoma==STATO_INIZIALE && c == '-');
		if(c=='('){
			size_t cpSotto=0;
			char utileSottoParentesi=Parentesi(espTmp,&cpSotto,a);
			//printf("utile: %hhd\tprocessati: %lu\n", utileSottoParentesi,cpSotto);
			espTmp += cpSotto;
			cp += cpSotto;
			c=*espTmp;
			
			//sostituisco le parentesi
			if(!utileSottoParentesi){
				*(espTmp-cpSotto)=0x01; //carattere non stampato, non ha alcun effetto su tutta la stringa
				*espTmp=0x01;
			}
		}
		statoAutoma=InputAutoma(a,c);
		c=*++espTmp;
		cp++;
		//printf("stato P: %hhd\n", statoAutoma);
	}

	// + (c==')' ? 1 : 0)
	*caratteriProcessati=cp;
	return utile;
}

void SemplificaEspressione(char* esp, struct AutomaSintassi *a){
	size_t lunghezzaEsp=strlen(esp);
	//char* espSemplificata=(char*)malloc(sizeof(char)*(lunghezzaEsp+1));
	char* espTmp=esp;
	char c=*espTmp;
	
	//elimino le parentesi in eccesso
	ResetAutoma(a);
	while(c){
		if(c=='('){
			size_t cp=0;
			char p=Parentesi(espTmp,&cp,a);
			//printf("utile: %hhd\tprocessati: %lu\n", p,cp);
			espTmp += cp;
			c=*espTmp;
			
			//sostituisco le parentesi
			if(!p){
				*(espTmp-cp)=0x01; //carattere non stampato, non ha alcun effetto su tutta la stringa
				*espTmp=0x01;
			}
		}
		//printf("carattere: %c\n", c);
		char s=InputAutoma(a,c);
		//printf("stato S: %hhd\n", s);
		c=*++espTmp;
	}
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
