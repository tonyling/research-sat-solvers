/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SLSLITERALS_H_
#define SLSLITERALS_H_

#include "../sls.h"

typedef struct {
	slsClause **occs;		//The occurrences of this literal. Terminated with NULL.
	uint32_t numOccsUsed;	//The number of occurrences used in this clause.
	uint32_t numOccsAvail;	//The number of occurrences available in this clause.
} slsLiteral;

#define SET_SLS_LIT_OCCS( __l, __o )			( sls_literals[__l].occs = __o )
#define SET_SLS_LIT_NUMOCCS_USED( __l, __v )	( sls_literals[__l].numOccsUsed = __v )
#define SET_SLS_LIT_NUMOCCS_AVAIL( __l, __a )	( sls_literals[__l].numOccsAvail = __a )

#define GET_SLS_LIT_OCCS( __l )					( sls_literals[__l].occs )
#define GET_SLS_LIT_NUMOCCS_USED( __l )			( sls_literals[__l].numOccsUsed )
#define GET_SLS_LIT_NUMOCCS_AVAIL( __l )		( sls_literals[__l].numOccsAvail )

#define ADD_SLS_LIT_OCC( __l, __c )	{ \
	if (sls_literals[__l].numOccsUsed == sls_literals[__l].numOccsAvail-1U) { \
		sls_literals[__l].numOccsAvail += BLOCKSIZE; \
		sls_literals[__l].occs = realloc(sls_literals[__l].occs, sizeof(slsClause*)*sls_literals[__l].numOccsAvail); \
	} \
	sls_literals[ __l ].occs[ sls_literals[__l].numOccsUsed++ ] = __c; \
	sls_literals[ __l ].occs[ sls_literals[__l].numOccsUsed ] = NULL; \
	if (sls_literals[__l].numOccsUsed > sls_litMaxOcc) sls_litMaxOcc = sls_literals[__l].numOccsUsed; \
}

#define SET_SLS_LIT_ASSIGNMENT_FLIP( __theLit ) (\
		__theLit < 0 ? \
		(sls_assignments[((-__theLit) >> 6ULL)] ^= (1ULL << ((-__theLit) & 63ULL))) : \
		(sls_assignments[(( __theLit) >> 6ULL)] ^= (1ULL << (( __theLit) & 63ULL))) \
)

#define GET_SLS_LIT_ASSIGNMENT( __theLit ) ( \
	__theLit < 0 ? \
	(((sls_assignments[((-__theLit) >> 6ULL)]) >> ((-__theLit) & 63ULL) ) & 1ULL) : \
	(((sls_assignments[(( __theLit) >> 6ULL)]) >> (( __theLit) & 63ULL) ) & 1ULL) \
)

#define IS_SLS_LIT_SAT( __theLit ) (\
	((__theLit > 0 && GET_SLS_VAR_ASSIGNMENT(__theLit) == 1ULL) || (__theLit < 0 && GET_SLS_VAR_ASSIGNMENT(-__theLit) == 0ULL)) ? 1 : 0 \
)

#define IS_SLS_LIT_UNSAT( __theLit ) (\
	((__theLit > 0 && GET_SLS_VAR_ASSIGNMENT(__theLit) == 0ULL) || (__theLit < 0 && GET_SLS_VAR_ASSIGNMENT(-__theLit) == 1ULL)) ? 1 : 0 \
)

void sls_literals_prepareCall();

void sls_literals_prepareTry();

void sls_literals_reset();
void sls_literals_init();
void sls_literals_dispose();

#endif /* SLSLITERALS_H_ */
