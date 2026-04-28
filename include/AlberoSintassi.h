/*! \file AlberoSintassi.h
 *  \brief File header per l'albero della sintassi dell'espressione simbolica
 */

struct NodoFunzione;
struct NodoOperatore;
union Nodo;

/*! \brief Unione per rappresentare il tipo generico del nodo
 */
union Nodo{
	struct NodoFunzione* nf;
	struct NodoOperatore* no;
};

/*! \brief Nodo per rappresentare un operatore
 */
struct NodoOperatore{
	char tipo;
	char op;
	union Nodo n1;
	union Nodo n2;
};

/*! \brief Nodo per rappresentare una funzione
 */
struct NodoFunzione{
	char tipo;
	char* f;
};

typedef struct NodoFunzione NodoFunzione; /*!< Nodo per rappresentare una funzione*/
typedef struct NodoOperatore NodoOperatore; /*!< Nodo per rappresentare un operatore*/
typedef union Nodo Nodo; /*!< Tipo generico del nodo*/

/*! \fn void StampaAlbero(Nodo n, unsigned livello)
 *  \brief Metodo per stampare l'albero del nodo n
 *
 *  \param n nodo radice dell'albero
 *  \param livello livello associato al nodo, default è 0
 */

void StampaAlbero(Nodo n, unsigned livello);
