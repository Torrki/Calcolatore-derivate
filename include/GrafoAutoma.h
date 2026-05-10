/*! \file GrafoAutoma.h
 *  \brief File header per l'automa delle funzioni
 */
#pragma once
#include <TabellaDerivate.h>

/*! \brief Stato per rappresentare il vertice del grafo
 */
struct Stato{
	//parte grafo
	unsigned char s;
	const char* inputs;
	struct Stato** statiSuccessivi;
	
	//parte lista circolare
	struct Stato* successivo;
};

struct AutomaSintassi{
	struct Stato* statoCorrente;
	struct Stato* statoIniziale;
	size_t numeroStati;
	int parentesi;
};

/*! \fn void CreaAutoma()
 *  \brief metodo per creare l'automa della sintassi del programma
 */
 
 /*! \fn char InputAutoma(char i)
 *  \brief metodo per fornire un input all'automa
 *
 *  \param i input da fornire
 *	\return torna lo stato successivo dell'automa o -1 se l'input non è valido
 */
 
 /*! \fn char StatoCorrenteAutoma()
 *  \brief metodo per ottenere lo stato attuale dell'automa
 *
 *	\return torna lo stato attuale dell'automa
 */
 
 /*! \fn int InputValido(char i)
 *  \brief metodo per verificare che un input sia valido nello stato attuale dell'automa
 *
 *  \param i input da verificare
 *	\return torna 1 se l'input è valido altrimenti 0
 */
 
 /*! \fn void ResetAutoma()
 *  \brief metodo per far tornare l'automa nello stato iniziale
 */
 
 /*! \fn char VerificaStatoFinale()
 *  \brief metodo per verificare che l'automa sia nello stato finale e con la sintassi corretta
 *
 *	\return torna 1 se la sintassi è corretta altrimenti 0
 */

/*! \fn void CancellaAutoma(struct AutomaSintassi* a)
 *  \brief metodo per deallocare l'automa della sintassi
 *	\param a automa da deallocare
 */

struct AutomaSintassi* CreaAutoma(struct Tabella* tabella);

unsigned char InputAutoma(struct AutomaSintassi* a, char i);
char StatoCorrenteAutoma(struct AutomaSintassi* a);
int InputValido(struct AutomaSintassi* a, char i);
void ResetAutoma(struct AutomaSintassi* a);
char VerificaStatoFinale(struct AutomaSintassi* a);
void CancellaAutoma(struct AutomaSintassi* a);
