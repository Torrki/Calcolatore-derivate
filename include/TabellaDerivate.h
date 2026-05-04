/*! \file TabellaDerivate.h
 *  \brief File header per la definizione della tabella delle derivate delle funzioni e degli operatori
 */
 
struct CoppiaDerivata{
	const char* chiave;
	const char* valore;
	struct CoppiaDerivata* prec;
	struct CoppiaDerivata* succ;
};

/*! \fn const char* OttieniDerivata(const char* f)
*  \brief metodo per ottenere la derivata dalla tabella
*
*  \param f funzione o operatore
*	 \return torna la stringa della derivata o NULL se f non è una chiave della tabella
*/

/*! \fn void CancellaTabella()
*  \brief metodo per deallocare tutta la tabella delle derivate
*/

/*! \fn void CreaTabella()
*  \brief metodo per creare la tabella delle derivate
*/

struct CoppiaDerivata* CreaTabella();
const char* OttieniDerivata(struct CoppiaDerivata* tabella, const char* f);
void CancellaTabella(struct CoppiaDerivata* tabella);