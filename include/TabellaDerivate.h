/*! \file TabellaDerivate.h
 *  \brief File header per la definizione della tabella delle derivate delle funzioni e degli operatori
 */
#pragma once
#include <stdlib.h>
 
struct CoppiaDerivata{
	const char* chiave;
	const char* valore;
	struct CoppiaDerivata* prec;
	struct CoppiaDerivata* succ;
};

struct Tabella{
	struct CoppiaDerivata *testa;
	size_t numeroVoci;
};


/*! \fn const char* OttieniDerivata(const char* f)
*		\brief metodo per ottenere la derivata dalla tabella
*
*		\param f funzione o operatore
*		\return torna la stringa della derivata o NULL se f non è una chiave della tabella
*/

/*! \fn void CancellaTabella(struct CoppiaDerivata* tabella)
*		\brief metodo per deallocare tutta la tabella delle derivate
*
*		\param tabella tabella da cancellare
*/

/*! \fn struct Tabella* CreaTabella()
*		\brief metodo per creare la tabella delle derivate
*
*		\return torna la tabella delle derivate
*/

/*! \fn void MatchChiave(struct CoppiaDerivata** tabella, const char *f)
*		\brief metodo per selezionare le voci che combaciano parzialmente o totalmente con la funzione f
*
*		\param tabella la tabella da modificare
*		\param f funzione da cercare
*/

/*! \fn struct Tabella* CopiaTabella(struct CoppiaDerivata* tabella)
*		\brief metodo per copiare una tabella
*
*		\param tabella tabella da copiare
*		\return torna la tabella copia
*/

struct Tabella* CreaTabella();
const char* OttieniDerivata(struct Tabella* tabella, const char* f);
void CancellaTabella(struct Tabella* tabella);
void MatchChiave(struct Tabella* tabella, const char *f);
struct Tabella* CopiaTabella(struct Tabella* tabella);
void StampaTabella(struct Tabella* tabella);