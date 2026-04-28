#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AlberoSintassi.h>

int operatore(char c);
int prioritaOperatori(char uo, char o);
const char* ordineOperatori="./*+-";

int main(int argc, char* argv[]){
	char* stringaInput=NULL;
	size_t dimensioneStringa=0;
	
	//lettura input stringa
	ssize_t numeroCaratteri=getline(&stringaInput,&dimensioneStringa,stdin);
	printf("Numero Caratteri %ld\nStringa: %s\n", numeroCaratteri, stringaInput);
	
	//inizializzazione per la costruzione dell'albero
	Nodo nodoCorrente,nodoRadice;
	int livello_max=0, livello=0;
	nodoCorrente.no=NULL;
	nodoRadice.no=NULL;
	
	char* bufferStringa=(char*)calloc(100,sizeof(char));
	char ultimoOperatore='.'; //operatore a massima priorità
	unsigned i=0;
	
	//processamento della stringa
	char* stringaInput_tmp=stringaInput;
	while(*stringaInput){
		char carattere=*stringaInput;
		
		//se il carattere letto è una funzione o un operatore
		if(operatore(carattere)){
			bufferStringa[i]='\0';
			printf("%s\n", bufferStringa);
			NodoFunzione* funzioneOp=(NodoFunzione*)malloc(sizeof(NodoFunzione));
			NodoOperatore* nuovoOp=(NodoOperatore*)malloc(sizeof(NodoOperatore));
			
			funzioneOp->tipo='f';
			funzioneOp->f=(char*)malloc(strlen(bufferStringa)+1);
			strcpy(funzioneOp->f, (const char *)bufferStringa);
			printf("Funzione: %s\n", funzioneOp->f);
			nuovoOp->tipo='o';
			nuovoOp->op=carattere;
			
			//stabilire la priorità per la costruzione dell'albero
			if(prioritaOperatori(ultimoOperatore,carattere)){	//salgo
				if(nodoCorrente.no == NULL){	//primo passo di costruzione
					nuovoOp->n1.nf=funzioneOp;
				}else{	//passi successivi
					nodoCorrente.no->n2.nf=funzioneOp;
					nuovoOp->n1.no=nodoCorrente.no;
				}
				
				livello++;
			}else{	//scendo
				nodoCorrente.no->n2.no=nuovoOp;
				nuovoOp->n1.nf=funzioneOp;
				
				livello--;
			}
			nodoCorrente.no=nuovoOp;
			ultimoOperatore=carattere;
			
			if(livello > livello_max) {
				nodoRadice=nodoCorrente;
				livello_max=livello;
			}
			
			//torno a inizio buffer
			i=0;
		}else{
			bufferStringa[i]=carattere;
			i++;
		}
		stringaInput++;
	}
	
	//lettura ultima funzione
	bufferStringa[i]='\0';
	NodoFunzione* ultimaFunzione=(NodoFunzione*)malloc(sizeof(NodoFunzione));
	ultimaFunzione->tipo='f';
	ultimaFunzione->f=(char*)malloc(strlen(bufferStringa)+1);
	strcpy(ultimaFunzione->f, (const char *)bufferStringa);
	printf("livello: %d\n", livello);
	
	if(livello_max==0){	//se non ho mai incontrato un operatore
		nodoRadice.nf=ultimaFunzione;
	}else{	//completo l'ultimo operatore
		nodoCorrente.no->n2.nf=ultimaFunzione;
	}
	StampaAlbero(nodoRadice,0);
	
	free(stringaInput_tmp);
	return 0;
}

int operatore(char c){
	return (c=='+') | (c=='-') | (c=='*') | (c=='/');
}

int prioritaOperatori(char uo, char o){
	unsigned pos_uo=0,pos_o=0;
	for(long unsigned i=0; i < strlen(ordineOperatori); i++){
		if(uo==ordineOperatori[i]) pos_uo=i;
		if(o==ordineOperatori[i]) pos_o=i;
	}
	return pos_uo < pos_o;
}



