/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SLSCLAUSES_H_
#define SLSCLAUSES_H_

typedef struct {
	uint32_t numTrue;		//The number of currently satisfied literals under the SLS assignment.
	int32_t tLwU;			//Either one of the true literals or the position of the clause in the sls_unsatClauses array.
	int32_t *lits;			//The literals of the clause, terminated by a 0.
} slsClause;

#define SET_SLS_CLS_NUMTRUE(__c, __v) 		( __c->numTrue = __v )
#define SET_SLS_CLS_TRUELIT(__c, __l) 		( __c->tLwU = __l )
#define SET_SLS_CLS_WHEREUNSAT(__c, __w) 	( __c->tLwU = __w )
#define SET_SLS_CLS_LITS(__c, __l) 			( __c->lits = __l )

#define SET_SLS_CLS_NUMTRUE_INC( __c )		( ++__c->numTrue )
#define SET_SLS_CLS_NUMTRUE_DEC( __c )		( --__c->numTrue )

#define GET_SLS_CLS_NUMTRUE(__c) 			( __c->numTrue )
#define GET_SLS_CLS_TRUELIT(__c) 			( __c->tLwU )
#define GET_SLS_CLS_WHEREUNSAT(__c)			( __c->tLwU )
#define GET_SLS_CLS_LITS(__c) 				( __c->lits )

void sls_clauses_prepareCall(); //Creates a restriction of the main formula into the clauses of the SLS.

void sls_clauses_prepareTry();

void sls_clauses_reset();
void sls_clauses_init();
void sls_clauses_dispose();

#ifdef VERBOSE_SLS
void sls_clauses_printAll();
void sls_clauses_printClause(slsClause*);
#endif
#endif /* SLSCLAUSES_H_ */
