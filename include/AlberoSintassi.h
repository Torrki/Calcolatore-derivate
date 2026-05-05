/*! \file AlberoSintassi.h
 *  \brief File header per l'albero della sintassi dell'espressione simbolica
 */
 
#include <GrafoAutoma.h>
#include <TabellaDerivate.h>
#include <stdlib.h>

struct NodoFunzione;
struct NodoOperatore;
struct NodoInfo;
union Nodo;

/*! \brief Unione per rappresentare il tipo generico del nodo
 */
union Nodo{
	struct NodoFunzione* nf;
	struct NodoOperatore* no;
	struct NodoInfo* ni;
};

struct NodoInfo{
	char tipo;
	struct NodoOperatore* genitori[2];
};

/*! \brief Nodo per rappresentare un operatore
 */
struct NodoOperatore{
	char tipo;
	struct NodoOperatore* genitori[2];
	char op;
	union Nodo n1;
	union Nodo n2;
};

/*! \brief Nodo per rappresentare una funzione
 */
struct NodoFunzione{
	char tipo;
	struct NodoOperatore* genitori[2];
	char* f;
};

typedef struct NodoFunzione NodoFunzione; /*!< Nodo per rappresentare una funzione*/
typedef struct NodoOperatore NodoOperatore; /*!< Nodo per rappresentare un operatore*/
typedef struct NodoInfo NodoInfo;
typedef union Nodo Nodo; /*!< Tipo generico del nodo*/

/*! \fn void StampaAlbero(Nodo n, unsigned livello)
 *  \brief Metodo per stampare l'albero del nodo n
 *
 *  \param n nodo radice dell'albero
 *  \param livello livello associato al nodo, default è 0
 */
 
/*! \fn Nodo CreaAlbero(const char* funzioneSym)
*  \brief metodo per creare l'albero dalla stringa simbolica 
*
*  \param funzioneSym stringa della funzione simbolica
*	 \return torna il nodo radice dell'albero della sintassi
*/

/*! \fn void DeallocaAlbero(Nodo radice)
*  \brief metodo per deallocare l'albero della stringa simbolica 
*
*  \param radice nodo radice dell'albero
*/

/*! \fn char* AnalizzaAlbero(Nodo radice)
*  \brief metodo per analizzare l'albero della sintassi
*
*  \param radice nodo radice dell'albero
*	 \return torna una stringa che contiene il risultato dell'operazione di derivazione
*/

void StampaAlbero(Nodo n, unsigned livello);
Nodo CreaAlbero(struct AutomaSintassi *a, const char* funzioneSym, size_t* numeroCaratteri);
void DeallocaAlbero(Nodo radice);
const char* AnalizzaAlbero(struct CoppiaDerivata* tabella, Nodo radice, char d);
const char* AlberotoStr(Nodo n);
