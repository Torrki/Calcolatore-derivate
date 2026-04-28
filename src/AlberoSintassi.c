#include <AlberoSintassi.h>
#include <stdio.h>

void StampaAlbero(Nodo n, unsigned livello){
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
}
