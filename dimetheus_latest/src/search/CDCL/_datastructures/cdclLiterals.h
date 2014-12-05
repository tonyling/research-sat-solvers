/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCLLITERALS_H_
#define CDCLLITERALS_H_

typedef struct cdclLiteral cdclLiteral;

struct cdclLiteral {
	cdclClause **occs; 		//The occurrence of the literal.
	uint32_t* occLitPos;	//The position in the clauses where this literal occurs.
	cdclClause **implies;	//The implications that this literal imposes if satisfied. Format: [int32_t lit | cdclClause* rea]
	cdclClause** w1;		//The list of clauses where this literal is first watcher.
	cdclClause** w2;		//The list of clauses where this literal is second watcher.
	cdclLiteral* root;		//The root literal for UNHIDING.
	cdclLiteral* prt;		//The parent literal for UNHIDING.
	uint32_t stamp;			//The literals stamp, if any.
	uint32_t upStamp;		//The unit propagation related stamp for CDCL UP.
	uint32_t numOccsAvail;	//The occurrence list number of memory positions.
	uint32_t numOccsUsed;	//The occurrence list number of memory positions in use.
	uint32_t numImpAvail;	//The number of implications available for storage in the array.
	uint32_t numImpUsed;	//The number of implications that this literal currently has.
	uint32_t numw1Avail;	//The number of w1 available for storage in the array.
	uint32_t numw1Used;		//The number of w2 that this literal currently has.
	uint32_t numw2Avail;	//The number of w2 available for storage in the array.
	uint32_t numw2Used;		//The number of w2 that this literal currently has.
	uint32_t dsc;			//The discovery time stamp for UNHIDING.
	uint32_t fin;			//The finishing time stamp for UNHIDING.
	uint32_t obs;			//The last observing time stamp for UNHIDING.
	uint32_t lSPos;			//The position of this literal in the literal stack (0 if not on stack).
	uint32_t lQScore;		//The score of the literal for the literal priority queue.
	uint32_t lQPos;			//The position of this literal in the literal priority queue.
};

#define GET_CDCL_LIT_ID_FROM_PTR( __lPtr )			((int32_t)(cdcl_literals > __lPtr ? -(cdcl_literals - __lPtr) : (__lPtr - cdcl_literals)))
#define GET_CDCL_LIT_PTR_FROM_ID( __l )				( cdcl_literals + __l )

//The following macros are used if the literal is given as a signed integer.
#define IS_CDCL_LIT_SAT( __l )						( (__l > 0 && IS_CDCL_VAR_TRUE( __l)) || (__l < 0 && IS_CDCL_VAR_FALSE(-__l)) )
#define IS_CDCL_LIT_SAT_ON_DL( __l, __dl )			( IS_CDCL_LIT_SAT(__l) && GET_CDCL_VAR_DECLEVEL(abs(__l)) <= __dl )
#define IS_CDCL_LIT_UNSAT( __l )					( (__l > 0 && IS_CDCL_VAR_FALSE( __l)) || (__l < 0 && IS_CDCL_VAR_TRUE(-__l)) )
#define IS_CDCL_LIT_UNSAT_ON_DL( __l, __dl )		( IS_CDCL_LIT_UNSAT(__l) && GET_CDCL_VAR_DECLEVEL(abs(__l)) <= __dl )
#define IS_CDCL_LIT_ASSIGNED( __l )					( (__l > 0 && !IS_CDCL_VAR_UNASSIGNED(__l)) || (__l < 0 && !IS_CDCL_VAR_UNASSIGNED(-__l)))

#define GET_CDCL_LIT_PTR_OPPOSITE( __lPtr )			( cdcl_literals - GET_CDCL_LIT_ID_FROM_PTR(__lPtr) )

#define GET_CDCL_LIT_OCCS( __l )					( cdcl_literals[__l].occs )
#define GET_CDCL_LIT_OCCNUM( __l , __n )			( cdcl_literals[__l].occs[__n] )
#define GET_CDCL_LIT_OCCLITPOSNUM( __l , __n )		( cdcl_literals[__l].occLitPos[__n] )
#define GET_CDCL_LIT_NUMOCCS_USED( __l )			( cdcl_literals[__l].numOccsUsed )
#define GET_CDCL_LIT_NUMOCCS_AVAIL( __l )			( cdcl_literals[__l].numOccsAvail )
#define GET_CDCL_LIT_OCCLITPOS( __l )				( cdcl_literals[__l].occLitPos )
#define GET_CDCL_LIT_IMPLIES( __l )					( cdcl_literals[__l].implies )
#define GET_CDCL_LIT_NUMIMPS_USED( __l )			( cdcl_literals[__l].numImpUsed )
#define GET_CDCL_LIT_NUMIMPS_AVAIL( __l )			( cdcl_literals[__l].numImpAvail )
#define GET_CDCL_LIT_W1( __l )						( cdcl_literals[__l].w1 )
#define GET_CDCL_LIT_NUMW1_USED( __l )				( cdcl_literals[__l].numw1Used )
#define GET_CDCL_LIT_NUMW1_AVAIL( __l )				( cdcl_literals[__l].numw1Avail )
#define GET_CDCL_LIT_W2( __l )						( cdcl_literals[__l].w2 )
#define GET_CDCL_LIT_NUMW2_USED( __l )				( cdcl_literals[__l].numw2Used )
#define GET_CDCL_LIT_NUMW2_AVAIL( __l )				( cdcl_literals[__l].numw2Avail )
#define GET_CDCL_LIT_STAMP( __l )					( cdcl_literals[__l].stamp )
#define GET_CDCL_LIT_UPSTAMP( __l )					( cdcl_literals[__l].upStamp )
#define GET_CDCL_LIT_W1_WATCHED_CLS( __l , __n) 	( cdcl_literals[__l].w1[__n] )
#define GET_CDCL_LIT_W2_WATCHED_CLS( __l , __n) 	( cdcl_literals[__l].w2[__n] )
#define GET_CDCL_LIT_IMPNUM( __l, __n )				( (int32_t) (addr_t) cdcl_literals[__l].implies[__n] )
#define GET_CDCL_LIT_IMPNUM_REASON( __l, __n )		( cdcl_literals[__l].implies[__n+1] )
#define GET_CDCL_LIT_DSC( __l )						( cdcl_literals[__l].dsc )
#define GET_CDCL_LIT_FIN( __l )						( cdcl_literals[__l].fin )
#define GET_CDCL_LIT_OBS( __l )						( cdcl_literals[__l].obs )
#define GET_CDCL_LIT_PRT( __l )						( cdcl_literals[__l].prt )
#define GET_CDCL_LIT_ROOT( __l )					( cdcl_literals[__l].root )
#define GET_CDCL_LIT_SPOS( __l )					( cdcl_literals[__l].lSPos )
#define GET_CDCL_LIT_QSCORE( __l )					( cdcl_literals[__l].lQScore )
#define GET_CDCL_LIT_QPOS( __l )					( cdcl_literals[__l].lQPos )
#define GET_CDCL_LIT_ELS_INDEX( __l )				( cdcl_literals[__l].dsc )
#define GET_CDCL_LIT_ELS_LOWLINK( __l )				( cdcl_literals[__l].fin )
#define GET_CDCL_LIT_ELS_ISINS( __l )				( cdcl_literals[__l].obs )
#define GET_CDCL_LIT_TRL_ISINQ( __l )				( cdcl_literals[__l].obs )

#define SET_CDCL_LIT_OCCS( __l, __o )				( cdcl_literals[__l].occs = __o )
#define SET_CDCL_LIT_OCCNUM( __l , __n , __o )		( cdcl_literals[__l].occs[__n] = __o )
#define SET_CDCL_LIT_OCCLITPOSNUM( __l , __n , __o)	( cdcl_literals[__l].occLitPos[__n] = __o )
#define SET_CDCL_LIT_NUMOCCS_USED( __l, __u )		( cdcl_literals[__l].numOccsUsed = __u )
#define SET_CDCL_LIT_NUMOCCS_AVAIL( __l, __a )		( cdcl_literals[__l].numOccsAvail = __a )
#define SET_CDCL_LIT_OCCLITPOS( __l, __olp )		( cdcl_literals[__l].occLitPos = __olp )
#define SET_CDCL_LIT_IMPLIES( __l, __imp )			( cdcl_literals[__l].implies = __imp )
#define SET_CDCL_LIT_NUMIMPS_USED( __l, __u )		( cdcl_literals[__l].numImpUsed = __u )
#define SET_CDCL_LIT_NUMIMPS_AVAIL( __l, __a )		( cdcl_literals[__l].numImpAvail = __a )
#define SET_CDCL_LIT_W1( __l , __w1l )				( cdcl_literals[__l].w1 = __w1l )
#define SET_CDCL_LIT_NUMW1_USED( __l, __u )			( cdcl_literals[__l].numw1Used = __u )
#define SET_CDCL_LIT_NUMW1_AVAIL( __l, __a )		( cdcl_literals[__l].numw1Avail = __a )
#define SET_CDCL_LIT_W2( __l , __w2l )				( cdcl_literals[__l].w2 = __w2l )
#define SET_CDCL_LIT_NUMW2_USED( __l, __u )			( cdcl_literals[__l].numw2Used = __u )
#define SET_CDCL_LIT_NUMW2_AVAIL( __l, __a )		( cdcl_literals[__l].numw2Avail = __a )
#define SET_CDCL_LIT_STAMP( __l , __s )				( cdcl_literals[__l].stamp = __s )
#define SET_CDCL_LIT_UPSTAMP( __l , __s )			( cdcl_literals[__l].upStamp = __s )
#define SET_CDCL_LIT_W1_WATCHED_CLS( __l, __n, __c) ( cdcl_literals[__l].w1[__n] = __c )
#define SET_CDCL_LIT_W2_WATCHED_CLS( __l, __n, __c) ( cdcl_literals[__l].w2[__n] = __c )
#define SET_CDCL_LIT_DSC( __l , __d )				( cdcl_literals[__l].dsc = __d )
#define SET_CDCL_LIT_FIN( __l , __f )				( cdcl_literals[__l].fin = __f )
#define SET_CDCL_LIT_OBS( __l , __o )				( cdcl_literals[__l].obs = __o )
#define SET_CDCL_LIT_PRT( __l , __p )				( cdcl_literals[__l].prt = __p )
#define SET_CDCL_LIT_ROOT( __l , __r )				( cdcl_literals[__l].root = __r )
#define SET_CDCL_LIT_SPOS( __l , __p )				( cdcl_literals[__l].lSPos = __p )
#define SET_CDCL_LIT_QSCORE( __l , __p )			( cdcl_literals[__l].lQScore = __p )
#define SET_CDCL_LIT_QPOS( __l , __p )				( cdcl_literals[__l].lQPos = __p )
#define SET_CDCL_LIT_ELS_INDEX( __l , __d )			( cdcl_literals[__l].dsc = __d )
#define SET_CDCL_LIT_ELS_LOWLINK( __l , __d )		( cdcl_literals[__l].fin = __d )
#define SET_CDCL_LIT_ELS_ISINS( __l , __d )			( cdcl_literals[__l].obs = __d )
#define SET_CDCL_LIT_TRL_ISINQ( __l , __d )			( cdcl_literals[__l].obs = __d )

//The following macros are used if the literal is given as a pointer.
#define GET_CDCL_LIT_PTR_OCCS( __l )						( __l->occs )
#define GET_CDCL_LIT_PTR_OCCNUM( __l , __n )				( __l->occs[__n] )
#define GET_CDCL_LIT_PTR_OCCLITPOSNUM( __l , __n )			( __l->occLitPos[__n] )
#define GET_CDCL_LIT_PTR_NUMOCCS_USED( __l )				( __l->numOccsUsed )
#define GET_CDCL_LIT_PTR_NUMOCCS_AVAIL( __l )				( __l->numOccsAvail )
#define GET_CDCL_LIT_PTR_OCCLITPOS( __l )					( __l->occLitPos )
#define GET_CDCL_LIT_PTR_IMPLIES( __l )						( __l->implies )
#define GET_CDCL_LIT_PTR_NUMIMPS_USED( __l )				( __l->numImpUsed )
#define GET_CDCL_LIT_PTR_NUMIMPS_AVAIL( __l )				( __l->numImpAvail )
#define GET_CDCL_LIT_PTR_W1( __l )							( __l->w1 )
#define GET_CDCL_LIT_PTR_NUMW1_USED( __l )					( __l->numw1Used )
#define GET_CDCL_LIT_PTR_NUMW1_AVAIL( __l )					( __l->numw1Avail )
#define GET_CDCL_LIT_PTR_W2( __l )							( __l->w2 )
#define GET_CDCL_LIT_PTR_NUMW2_USED( __l )					( __l->numw2Used )
#define GET_CDCL_LIT_PTR_NUMW2_AVAIL( __l )					( __l->numw2Avail )
#define GET_CDCL_LIT_PTR_STAMP( __l )						( __l->stamp )
#define GET_CDCL_LIT_PTR_UPSTAMP( __l )						( __l->upStamp )
#define GET_CDCL_LIT_PTR_W1_WATCHED_CLS( __l , __n) 		( __l->w1[__n] )
#define GET_CDCL_LIT_PTR_W2_WATCHED_CLS( __l , __n) 		( __l->w2[__n] )
#define GET_CDCL_LIT_PTR_IMPNUM( __l, __n )					( (int32_t) (addr_t) (__l->implies[__n]) )
#define GET_CDCL_LIT_PTR_IMPNUM_REASON( __l, __n )			( __l->implies[__n+1] )
#define GET_CDCL_LIT_PTR_DSC( __l )							( __l->dsc )
#define GET_CDCL_LIT_PTR_FIN( __l )							( __l->fin )
#define GET_CDCL_LIT_PTR_OBS( __l )							( __l->obs )
#define GET_CDCL_LIT_PTR_PRT( __l )							( __l->prt )
#define GET_CDCL_LIT_PTR_ROOT( __l )						( __l->root )
#define GET_CDCL_LIT_PTR_SPOS( __l )						( __l->lSPos )
#define GET_CDCL_LIT_PTR_QSCORE( __l )						( __l->lQScore )
#define GET_CDCL_LIT_PTR_QPOS( __l )						( __l->lQPos )
#define GET_CDCL_LIT_PTR_ELS_INDEX( __l )					( __l->dsc )
#define GET_CDCL_LIT_PTR_ELS_LOWLINK( __l )					( __l->fin )
#define GET_CDCL_LIT_PTR_ELS_ISINS( __l )					( __l->obs )
#define GET_CDCL_LIT_PTR_TRL_ISINQ( __l )					( __l->obs )

#define SET_CDCL_LIT_PTR_OCCS( __l, __o )					( __l->occs = __o )
#define SET_CDCL_LIT_PTR_OCCNUM( __l , __n , __o)			( __l->occs[__n] = __o )
#define SET_CDCL_LIT_PTR_OCCLITPOSNUM( __l , __n , __o )	( __l->occLitPos[__n] = __o )
#define SET_CDCL_LIT_PTR_NUMOCCS_USED( __l, __u )			( __l->numOccsUsed = __u )
#define SET_CDCL_LIT_PTR_NUMOCCS_AVAIL( __l, __a )			( __l->numOccsAvail = __a )
#define SET_CDCL_LIT_PTR_OCCLITPOS( __l, __olp )			( __l->occLitPos = __olp )
#define SET_CDCL_LIT_PTR_IMPLIES( __l, __imp )				( __l->implies = __imp )
#define SET_CDCL_LIT_PTR_NUMIMPS_USED( __l, __u )			( __l->numImpUsed = __u )
#define SET_CDCL_LIT_PTR_NUMIMPS_AVAIL( __l, __a )			( __l->numImpAvail = __a )
#define SET_CDCL_LIT_PTR_W1( __l , __w1l )					( __l->w1 = __w1l )
#define SET_CDCL_LIT_PTR_NUMW1_USED( __l, __u )				( __l->numw1Used = __u )
#define SET_CDCL_LIT_PTR_NUMW1_AVAIL( __l, __a )			( __l->numw1Avail = __a )
#define SET_CDCL_LIT_PTR_W2( __l , __w2l )					( __l->w2 = __w2l )
#define SET_CDCL_LIT_PTR_NUMW2_USED( __l, __u )				( __l->numw2Used = __u )
#define SET_CDCL_LIT_PTR_NUMW2_AVAIL( __l, __a )			( __l->numw2Avail = __a )
#define SET_CDCL_LIT_PTR_STAMP( __l , __s )					( __l->stamp = __s )
#define SET_CDCL_LIT_PTR_UPSTAMP( __l , __s )				( __l->upStamp = __s )
#define SET_CDCL_LIT_PTR_W1_WATCHED_CLS( __l , __n, __c ) 	( __l->w1[__n] = __c )
#define SET_CDCL_LIT_PTR_W2_WATCHED_CLS( __l , __n, __c ) 	( __l->w2[__n] = __c )
#define SET_CDCL_LIT_PTR_DSC( __l , __d )					( __l->dsc = __d )
#define SET_CDCL_LIT_PTR_FIN( __l , __f )					( __l->fin = __f )
#define SET_CDCL_LIT_PTR_OBS( __l , __o )					( __l->obs = __o )
#define SET_CDCL_LIT_PTR_PRT( __l , __p )					( __l->prt = __p )
#define SET_CDCL_LIT_PTR_ROOT( __l , __r )					( __l->root = __r )
#define SET_CDCL_LIT_PTR_SPOS( __l , __p )					( __l->lSPos = __p )
#define SET_CDCL_LIT_PTR_QSCORE( __l , __p )				( __l->lQScore = __p )
#define SET_CDCL_LIT_PTR_QPOS( __l , __p )					( __l->lQPos = __p )
#define SET_CDCL_LIT_PTR_ELS_INDEX( __l , __d )				( __l->dsc = __d )
#define SET_CDCL_LIT_PTR_ELS_LOWLINK( __l , __d )			( __l->fin = __d )
#define SET_CDCL_LIT_PTR_ELS_ISINS( __l , __d )				( __l->obs = __d )
#define SET_CDCL_LIT_PTR_TRL_ISINQ( __l , __d )				( __l->obs = __d )

//The following macros are used to work with the signature of a clause.
#define GET_CDCL_LIT_HASH( __l ) ( __l > 0 ? ( __l % 32 ) : (( -__l % 32 ) + 32) )
#define	GET_CDCL_LIT_SIG( __l ) 	( 1ULL << GET_CDCL_LIT_HASH( __l ) )

//The following macro is used to add clause c to the occurrence list of l where l is at position lPos in c.
#define ADD_CDCL_CLS_TO_LIT_OCCS( __cPtr , __l, __lPos) {\
		cdcl_literals[ __l ].occs[cdcl_literals[ __l ].numOccsUsed] = __cPtr; \
		cdcl_literals[ __l ].occLitPos[cdcl_literals[ __l ].numOccsUsed] = __lPos; \
		__cPtr->occPos[ __lPos ] = cdcl_literals[ __l ].numOccsUsed++; \
		if (cdcl_literals[ __l ].numOccsAvail == cdcl_literals[ __l ].numOccsUsed){ \
			cdcl_literals[ __l ].numOccsAvail *= 2; \
			cdcl_literals[ __l ].occs = realloc(cdcl_literals[ __l ].occs, sizeof(cdclClause*)*cdcl_literals[ __l ].numOccsAvail); \
			cdcl_literals[ __l ].occLitPos = realloc(cdcl_literals[ __l ].occLitPos, \
												sizeof(uint32_t)*cdcl_literals[ __l ].numOccsAvail); \
		} \
}

#define ADD_CDCL_CLS_TO_LIT_PTR_OCCS( __cPtr , __l, __lPos) {\
		__l->occs[__l->numOccsUsed] = __cPtr; \
		__l->occLitPos[__l->numOccsUsed] = __lPos; \
		__cPtr->occPos[ __lPos ] = __l->numOccsUsed++; \
		if (__l->numOccsAvail == __l->numOccsUsed){ \
			__l->numOccsAvail *= 2; \
			__l->occs = realloc(__l->occs, sizeof(cdclClause*)*__l->numOccsAvail); \
			__l->occLitPos = realloc(__l->occLitPos, \
												sizeof(uint32_t)*__l->numOccsAvail); \
		} \
}

//The following macros are used to remove clause at occPos from the occurrence list of literal l (number __l, pointer __lPtr).
#define REM_CDCL_LIT_OCC( __l, __occPos) { \
		cdcl_literals[ __l ].occs[ __occPos ] = cdcl_literals[ __l ].occs[ --cdcl_literals[ __l ].numOccsUsed ]; \
		cdcl_literals[ __l ].occLitPos[ __occPos ] = cdcl_literals[ __l ].occLitPos[ cdcl_literals[ __l ].numOccsUsed ]; \
		cdcl_literals[ __l ].occs[ __occPos ]->occPos[ cdcl_literals[ __l ].occLitPos[ __occPos ] ] = __occPos; \
}
#define REM_CDCL_LIT_PTR_OCC( __lPtr, __occPos ) {\
		__lPtr->occs[ __occPos ] = __lPtr->occs[ --__lPtr->numOccsUsed ]; \
		__lPtr->occLitPos[ __occPos ] = __lPtr->occLitPos[ __lPtr->numOccsUsed ]; \
		__lPtr->occs[ __occPos ]->occPos[ __lPtr->occLitPos[ __occPos ] ] = __occPos; \
}

//The following macro is used to add the necessary literal implications given a binary clause. The clause MUST be binary.
#define ADD_CDCL_LIT_IMPLICATIONS( __cPtr, __lit1 , __lit2 ) {\
		__lit1 = -__cPtr->lits[0]; \
		__lit2 = __cPtr->lits[1]; \
		__cPtr->whereW2 = cdcl_literals[ __lit1 ].numImpUsed; \
		cdcl_literals[ __lit1 ].implies[cdcl_literals[ __lit1 ].numImpUsed++] = (cdclClause*)(addr_t)__lit2; \
		cdcl_literals[ __lit1 ].implies[cdcl_literals[ __lit1 ].numImpUsed++] = __cPtr; \
		if (cdcl_literals[ __lit1 ].numImpAvail == cdcl_literals[ __lit1 ].numImpUsed){ \
			cdcl_literals[ __lit1 ].numImpAvail *= 2; \
			cdcl_literals[ __lit1 ].implies = realloc(cdcl_literals[ __lit1 ].implies,\
												sizeof(cdclClause*)*cdcl_literals[ __lit1 ].numImpAvail); \
		} \
		__lit1 = __cPtr->lits[0]; \
		__lit2 = -__cPtr->lits[1]; \
		__cPtr->whereW1 = cdcl_literals[ __lit2 ].numImpUsed; \
		cdcl_literals[ __lit2 ].implies[cdcl_literals[ __lit2 ].numImpUsed++] = (cdclClause*)(addr_t)__lit1; \
		cdcl_literals[ __lit2 ].implies[cdcl_literals[ __lit2 ].numImpUsed++] = __cPtr; \
		if (cdcl_literals[ __lit2 ].numImpAvail == cdcl_literals[ __lit2 ].numImpUsed){ \
			cdcl_literals[ __lit2 ].numImpAvail *= 2; \
			cdcl_literals[ __lit2 ].implies = realloc(cdcl_literals[ __lit2 ].implies,\
												sizeof(cdclClause*)*cdcl_literals[ __lit2 ].numImpAvail); \
		} \
}

//The following macro is used to remove literal implication number p in case a BINARY clause is disabled or deleted.
#define REM_CDCL_LIT_IMPLICATIONS( __cPtr, __l, __p) {\
		__l = -__cPtr->lits[0]; \
		__p = __cPtr->whereW2; \
		cdcl_literals[ __l ].implies[__p] = cdcl_literals[ __l ].implies[cdcl_literals[ __l ].numImpUsed-2]; \
		cdcl_literals[ __l ].implies[__p+1] = cdcl_literals[ __l ].implies[cdcl_literals[ __l ].numImpUsed-1]; \
		cdcl_literals[ __l ].numImpUsed -= 2; \
		if ((cdclClause*)(addr_t)(cdcl_literals[ __l ].implies[__p])==(cdclClause*)(addr_t)(cdcl_literals[__l].implies[__p+1]->lits[0])){\
			cdcl_literals[ __l ].implies[__p+1]->whereW1 = __p;\
		} else { \
			cdcl_literals[ __l ].implies[__p+1]->whereW2 = __p;\
		} \
		__l = -__cPtr->lits[1]; \
		__p = __cPtr->whereW1; \
		cdcl_literals[ __l ].implies[__p] = cdcl_literals[ __l ].implies[cdcl_literals[ __l ].numImpUsed-2]; \
		cdcl_literals[ __l ].implies[__p+1] = cdcl_literals[ __l ].implies[cdcl_literals[ __l ].numImpUsed-1]; \
		cdcl_literals[ __l ].numImpUsed -= 2; \
		if ((cdclClause*)(addr_t)(cdcl_literals[ __l ].implies[__p])==(cdclClause*)(addr_t)(cdcl_literals[__l].implies[__p+1]->lits[0])){\
			cdcl_literals[ __l ].implies[__p+1]->whereW1 = __p;\
		} else { \
			cdcl_literals[ __l ].implies[__p+1]->whereW2 = __p;\
		} \
}

//The following two macros are used to add a clause (given as pointer in __cPtr) to a literals (given as integer in __l) watch
//list (either first or second watcher). It will dynamically extend the size of the watcher lists.
#define ADD_CDCL_CLS_TO_LIT_W1LIST( __cPtr , __l ) {\
		cdcl_literals[ __l ].w1[cdcl_literals[ __l ].numw1Used] = __cPtr; \
		__cPtr->whereW1 = cdcl_literals[ __l ].numw1Used++; \
		if (cdcl_literals[ __l ].numw1Avail == cdcl_literals[ __l ].numw1Used){ \
			cdcl_literals[ __l ].numw1Avail *= 2; \
			cdcl_literals[ __l ].w1 = realloc(cdcl_literals[ __l ].w1, sizeof(cdclClause*)*cdcl_literals[ __l ].numw1Avail); \
		} \
}
#define ADD_CDCL_CLS_TO_LIT_W2LIST( __cPtr , __l ) {\
		cdcl_literals[ __l ].w2[cdcl_literals[ __l ].numw2Used] = __cPtr; \
		__cPtr->whereW2 = cdcl_literals[ __l ].numw2Used++; \
		if (cdcl_literals[ __l ].numw2Avail == cdcl_literals[ __l ].numw2Used){ \
			cdcl_literals[ __l ].numw2Avail *= 2; \
			cdcl_literals[ __l ].w2 = realloc(cdcl_literals[ __l ].w2, sizeof(cdclClause*)*cdcl_literals[ __l ].numw2Avail); \
		} \
}
//The following two macros are used to remove a clause from a literals (integer __l) watch lists at position __pos. The memory
//for the sizes of the watch lists is not reduced.
#define REM_CDCL_CLS_FROM_LIT_W1LIST( __l , __pos) {\
		cdcl_literals[ __l ].w1[ __pos ] = cdcl_literals[ __l ].w1[ --cdcl_literals[ __l ].numw1Used ]; \
		cdcl_literals[ __l ].w1[ __pos ]->whereW1 = __pos; \
}
#define REM_CDCL_CLS_FROM_LIT_W2LIST( __l , __pos) {\
		cdcl_literals[ __l ].w2[ __pos ] = cdcl_literals[ __l ].w2[ --cdcl_literals[ __l ].numw2Used ]; \
		cdcl_literals[ __l ].w2[ __pos ]->whereW2 = __pos; \
}

//Methods for handling the literal priority queue.
void cdcl_literals_Q_moveUp(cdclLiteral*);			//To move an element up in the literal priority queue.
void cdcl_literals_Q_moveDown(cdclLiteral*);		//To move an element down in the literal priority queue.
cdclLiteral* cdcl_literals_Q_dequeueBest();			//To dequeue the best element in the literal priority queue.
void cdcl_literals_Q_delete(cdclLiteral*);			//To delete an element from the literal priority queue.
void cdcl_literals_Q_enqueue(cdclLiteral*);			//To enqueue a literal.
void cdcl_literals_Q_touchLiteral_inprocessing_fld(cdclLiteral*);//To touch a literal in FLD mode.
void cdcl_literals_Q_touchLiteral_inprocessing_nhbr(cdclLiteral*);//To touch a literal in NHBR mode.
void cdcl_literals_Q_touchLiteral_inprocessing_treelook(cdclLiteral*);//To touch a literal in TREELOOK mode.
void cdcl_literals_Q_touchLiteral_inprocessing_niver(cdclLiteral*);//To touch a literal in NIVER mode.
void cdcl_literals_Q_touchLiteral_inprocessing_ge(cdclLiteral*);//To touch a literal in NIVER mode.
void cdcl_literals_Q_touchLiteral_null(cdclLiteral*);//To do nothing.
void (*cdcl_literals_Q_touchLiteral)(cdclLiteral*);	//This method points to one of the touch methods above.
void cdcl_literals_Q_flush();						//To remove all elements in the literal queue.

//Methods for handling the literal stack.
void cdcl_literals_S_delete(cdclLiteral*);	//To delete a literal from the literal stack.
void cdcl_literals_S_push(cdclLiteral*);	//To push something on the literal stack.
void cdcl_literals_S_touchLiteral_null(cdclLiteral*);//To do nothing.
void cdcl_literals_S_touchLiteral_inprocessing_ternary(cdclLiteral*);//To do nothing.
void (*cdcl_literals_S_touchLiteral)(cdclLiteral*);	//This method points to one of the touch methods above.
cdclLiteral* cdcl_literals_S_pop();			//To pop the topmost element from the stack.
void cdcl_literals_S_flush();				//To remove all elements from the literal stack.

void cdcl_literals_prepareCall();

void cdcl_literals_reset();
void cdcl_literals_init();
void cdcl_literals_dispose();

#ifdef VERBOSE_CDCL
void cdcl_literals_printLiteralImplications(cdclLiteral*);
void cdcl_literals_printLiteralOccs(cdclLiteral*);
void cdcl_literals_printLiteralOccLitPos(cdclLiteral*);
void cdcl_literals_printLiteralW1List(cdclLiteral*);
void cdcl_literals_printLiteralW2List(cdclLiteral*);
void cdcl_literals_printLiteral(cdclLiteral*);
void cdcl_literals_printAll();
#endif

#endif /* CDCLLITERALS_H_ */
