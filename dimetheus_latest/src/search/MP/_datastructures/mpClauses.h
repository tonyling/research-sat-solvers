/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef MPCLAUSES_H_
#define MPCLAUSES_H_

#include "../mp.h"

typedef struct {
	int32_t *lits;			//The literals of the clause, terminated with 0.
	float_ty *omegas;		//The warning messages \omega(c,v) this clause sends to all contained variables.
	float_ty *weights;		//The literal occurrence weights \psi.
} mpClause;

#define GET_MP_CLS_LITS( __c )						( __c->lits )
#define GET_MP_CLS_OMEGAS( __c )					( __c->omegas )
#define GET_MP_CLS_WEIGHTS( __c )					( __c->weights )

#define SET_MP_CLS_LITS( __c, __l )					( __c->lits = __l )
#define SET_MP_CLS_OMEGAS( __c, __m )				( __c->omegas = __m )
#define SET_MP_CLS_WEIGHTS( __c, __w )				( __c->weights = __w )

#define GET_MP_CLS_LITPOSWEIGHT( __c, __i ) 		( __c->weights[ __i ] )
#define SET_MP_CLS_LITPOSWEIGHT( __c, __i, __w )	( __c->weights[ __i ] = __w )

void mp_clauses_prepareCall();  //Creates a restriction of the main formula into the clauses of the SLS.

void mp_clauses_retainMsgs();	//This method just copies over all the messages from current data-structures.

void mp_clauses_reset();
void mp_clauses_init();
void mp_clauses_dispose();

#ifdef VERBOSE_MP
void mp_clauses_printClause(mpClause*);
void mp_clauses_printAll();
#endif
#endif /* MPCLAUSES_H_ */
