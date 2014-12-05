/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef LITERALS_H_
#define LITERALS_H_

#include "../dimetheus.h"

typedef struct literal literal;

struct literal {
	clause** occs;			//The clauses in which this literal occurs in.
	uint32_t* occLitPos;	//The position in the clauses where this literal occurs.
	clause** w1;			//The list of clauses where this literal is first watcher.
	clause** w2;			//The list of clauses where this literal is second watcher.
	clause** implies;		//The implications that this literal imposes if satisfied. Format: [int32_t lit | clause* reason]
	literal* prt;			//Parent in the discovery process during advanced stamping.
	literal* root;			//Root in the discovery process during advanced stamping.
	uint32_t stamp;			//The literals stamp, if any.
	uint32_t upStamp;		//The unit propagation related stamp.
	uint32_t numOccsAvail;	//The number of occurrences available for storage in the array.
	uint32_t numOccsUsed;	//The number of occurrences that this literal currently has.
	uint32_t numw1Avail;	//The number of w1 available for storage in the array.
	uint32_t numw1Used;		//The number of w2 that this literal currently has.
	uint32_t numw2Avail;	//The number of w2 available for storage in the array.
	uint32_t numw2Used;		//The number of w2 that this literal currently has.
	uint32_t numImpAvail;	//The number of implications available for storage in the array.
	uint32_t numImpUsed;	//The number of implications that this literal currently has.
	uint32_t lQPos;			//The position of the literal in the literal priority queue.
	int32_t lQScore;		//The score of the literal in the queue. The smaller, the better.
	uint32_t lSPos;			//The position of the literal in the literal stack.
	uint32_t dsc;			//Discovery time for advanced stamping.
	uint32_t fin;			//Finish time for advanced stamping.
	uint32_t obs;			//Last observed time for advanced stamping.
};

//The following macros are used to work with the signature of a clause.
#define GET_LIT_HASH( __l ) ( __l > 0 ? ( __l % 32 ) : (( -__l % 32 ) + 32) )
#define	GET_LIT_SIG( __l ) 	( 1ULL << GET_LIT_HASH( __l ) )

//The following macro determines if a given literal is satisfied when only its number is provided.
#define IS_LIT_SAT( __l )	( (__l > 0 && IS_VAR_TRUE((main_varData + __l))) || (__l < 0 && IS_VAR_FALSE((main_varData -__l))) )
#define IS_LIT_UNSAT( __l )	( (__l > 0 && IS_VAR_FALSE((main_varData + __l))) || (__l < 0 && IS_VAR_TRUE((main_varData -__l))) )
#define IS_LIT_SLS_SAT( __l ) ( (__l > 0 && IS_VAR_SLS_TRUE((main_varData + __l))) || (__l < 0 && IS_VAR_SLS_FALSE((main_varData -__l))) )
#define IS_LIT_ASSIGNED( __l )	( (__l > 0 && !IS_VAR_UNASSIGNED((main_varData + __l))) || (__l < 0 && !IS_VAR_UNASSIGNED((main_varData -__l))))


//The following macro is used to compute the number of the literal given the literal pointer.
#define GET_LIT_NUM_FROM_PTR( __lPtr )	((int32_t)(main_litData > __lPtr ? -(main_litData - __lPtr) : (__lPtr - main_litData)))
#define GET_LIT_PTR_FROM_NUM( __l )		( main_litData + __l )

//The following macro is used to get the opposite literal.
#define GET_LIT_OPPOSITE( __lPtr )		( main_litData - GET_LIT_NUM_FROM_PTR(__lPtr) )

//Literal POINTER.
#define SET_LIT_Q_POS( __lPtr , __value ) 		( __lPtr->lQPos = __value )
#define SET_LIT_Q_SCORE( __lPtr , __value )		( __lPtr->lQScore = __value )
#define GET_LIT_Q_POS( __lPtr ) 				( __lPtr->lQPos )
#define GET_LIT_Q_SCORE( __lPtr )				( __lPtr->lQScore )
#define SET_LIT_S_POS( __lPtr , __value ) 		( __lPtr->lSPos = __value )
#define GET_LIT_S_POS( __lPtr ) 				( __lPtr->lSPos )

#define SET_LIT_STAMP( __lPtr , __value )		( __lPtr->stamp = __value )
#define GET_LIT_STAMP( __lPtr )					( __lPtr->stamp )

#define SET_LIT_UPSTAMP( __lPtr , __value )		( __lPtr->upStamp = __value )
#define GET_LIT_UPSTAMP( __lPtr )				( __lPtr->upStamp )

#define SET_LIT_DSC( __lPtr , __d )				( __lPtr->dsc = __d )
#define SET_LIT_FIN( __lPtr , __d )				( __lPtr->fin = __d )
#define SET_LIT_OBS( __lPtr , __d )				( __lPtr->obs = __d )
#define SET_LIT_PRT( __lPtr , __llPtr )			( __lPtr->prt = __llPtr )
#define SET_LIT_ROOT( __lPtr, __llPtr )			( __lPtr->root = __llPtr )

#define GET_LIT_DSC( __lPtr )					( __lPtr->dsc )
#define GET_LIT_FIN( __lPtr )					( __lPtr->fin )
#define GET_LIT_OBS( __lPtr )					( __lPtr->obs )
#define GET_LIT_PRT( __lPtr )					( __lPtr->prt )
#define GET_LIT_ROOT( __lPtr )					( __lPtr->root )

#define SET_LIT_ELS_INDEX( __lPtr , __d )		( __lPtr->dsc = __d )
#define SET_LIT_ELS_LOWLINK( __lPtr , __d )		( __lPtr->fin = __d )
#define SET_LIT_ELS_ISINS( __lPtr , __d )		( __lPtr->obs = __d )

#define GET_LIT_ELS_INDEX( __lPtr )				( __lPtr->dsc )
#define GET_LIT_ELS_LOWLINK( __lPtr )			( __lPtr->fin )
#define GET_LIT_ELS_ISINS( __lPtr )				( __lPtr->obs )

#define GET_LIT_NUMIMP_AVAIL( __lPtr )			( __lPtr->numImpAvail )
#define GET_LIT_NUMIMP_USED( __lPtr )			( __lPtr->numImpUsed )
#define SET_LIT_NUMIMP_AVAIL( __lPtr , __d )	( __lPtr->numImpAvail = __d )
#define SET_LIT_NUMIMP_USED( __lPtr, __d )		( __lPtr->numImpUsed  = __d )

#define GET_LIT_IMPNUM( __lPtr , __n )			( (int32_t)(addr_t)(__lPtr->implies[__n]) )
#define GET_LIT_IMPNUM_REASON( __lPtr, __n )	( (clause*)(addr_t)(__lPtr->implies[__n+1]) )

#define GET_IMPLIED_LIT_AS_PTR( __lPtr, __n )	( main_litData + ((int32_t)(addr_t)(__lPtr->implies[__n])) )
#define GET_IMPLIED_LIT_REASON_AS_PTR( __lPtr, __n ) ( __lPtr->implies[__n+1] )

#define GET_LIT_NUMOCCS_USED( __lPtr )			( __lPtr->numOccsUsed )
#define SET_LIT_NUMOCCS_USED( __lPtr , __u )	( __lPtr->numOccsUsed  = __u )

#define GET_LIT_OCCNUM( __lPtr, __n )			( __lPtr->occs[__n] )
#define GET_LIT_OCCLITPOSNUM( __lPtr, __n )		( __lPtr->occLitPos[__n] )

#define GET_LIT_NUMW1_USED( __lPtr )			( __lPtr->numw1Used )
#define GET_LIT_NUMW1_AVAIL( __lPtr )			( __lPtr->numw1Avail )
#define GET_LIT_NUMW2_USED( __lPtr )			( __lPtr->numw2Used )
#define GET_LIT_NUMW2_AVAIL( __lPtr )			( __lPtr->numw2Avail )

#define GET_LIT_W1_WATCHED_CLS( __lPtr , __n) 	( __lPtr->w1[__n] )
#define GET_LIT_W2_WATCHED_CLS( __lPtr , __n) 	( __lPtr->w2[__n] )

#define GET_LIT_TRL_ISINQ( __lPtr )				( __lPtr->obs )
#define SET_LIT_TRL_ISINQ( __lPtr , __d )		( __lPtr->obs = __d )

//Literal INTEGER.
#define SET_LIT_NUM_Q_POS( __l , __value ) 		( main_litData[__l].lQPos = __value )
#define SET_LIT_NUM_Q_SCORE( __l , __value )	( main_litData[__l].lQScore = __value )
#define GET_LIT_NUM_Q_POS( __l ) 				( main_litData[__l].lQPos )
#define GET_LIT_NUM_Q_SCORE( __l )				( main_litData[__l].lQScore )
#define SET_LIT_NUM_S_POS( __l , __value ) 		( main_litData[__l].lSPos = __value )
#define GET_LIT_NUM_S_POS( __l ) 				( main_litData[__l].lSPos )

#define SET_LIT_NUM_STAMP( __l , __value )		( main_litData[__l].stamp = __value )
#define GET_LIT_NUM_STAMP( __l )				( main_litData[__l].stamp )

#define SET_LIT_NUM_UPSTAMP( __l , __value )	( main_litData[__l].upStamp = __value )
#define GET_LIT_NUM_UPSTAMP( __l )				( main_litData[__l].upStamp )

#define SET_LIT_NUM_DSC( __l , __d )			( main_litData[__l].dsc = __d )
#define SET_LIT_NUM_FIN( __l , __d )			( main_litData[__l].fin = __d )
#define SET_LIT_NUM_OBS( __l , __d )			( main_litData[__l].obs = __d )
#define SET_LIT_NUM_PRT( __l , __llPtr )		( main_litData[__l].prt = __llPtr )
#define SET_LIT_NUM_ROOT( __l, __llPtr )		( main_litData[__l].root = __llPtr )

#define GET_LIT_NUM_DSC( __l )					( main_litData[__l].dsc )
#define GET_LIT_NUM_FIN( __l )					( main_litData[__l].fin )
#define GET_LIT_NUM_OBS( __l )					( main_litData[__l].obs )
#define GET_LIT_NUM_PRT( __l )					( main_litData[__l].prt )
#define GET_LIT_NUM_ROOT( __l )					( main_litData[__l].root )

#define SET_LIT_NUM_ELS_INDEX( __l , __d )		( main_litData[__l].dsc = __d )
#define SET_LIT_NUM_ELS_LOWLINK( __l , __d )	( main_litData[__l].fin = __d )
#define SET_LIT_NUM_ELS_ISINS( __l , __d )		( main_litData[__l].obs = __d )

#define GET_LIT_NUM_ELS_INDEX( __l )			( main_litData[__l].dsc )
#define GET_LIT_NUM_ELS_LOWLINK( __l )			( main_litData[__l].fin )
#define GET_LIT_NUM_ELS_ISINS( __l )			( main_litData[__l].obs )

#define GET_LIT_NUM_NUMIMP_AVAIL( __l )			( main_litData[__l].numImpAvail )
#define GET_LIT_NUM_NUMIMP_USED( __l )			( main_litData[__l].numImpUsed )
#define SET_LIT_NUM_NUMIMP_AVAIL( __l , __d )	( main_litData[__l].numImpAvail = __d )
#define SET_LIT_NUM_NUMIMP_USED( __l, __d )		( main_litData[__l].numImpUsed  = __d )

#define GET_LIT_NUM_IMPNUM( __l , __n )			( (int32_t)(addr_t)(main_litData[__l].implies[__n]) )
#define GET_LIT_NUM_IMPNUM_REASON( __l, __n )	( (clause*)(addr_t)(main_litData[__l].implies[__n+1]) )

#define GET_LIT_NUM_NUMOCCS_USED( __l )			( main_litData[__l].numOccsUsed )
#define SET_LIT_NUM_NUMOCCS_USED( __l , __u )	( main_litData[__l].numOccsUsed  = __u )

#define GET_LIT_NUM_OCCNUM( __l, __n )			( main_litData[__l].occs[__n] )
#define GET_LIT_NUM_OCCLITPOSNUM( __l, __n )	( main_litData[__l].occLitPos[__n] )

#define GET_LIT_NUM_NUMW1_USED( __l )			( main_litData[__l].numw1Used )
#define GET_LIT_NUM_NUMW1_AVAIL( __l )			( main_litData[__l].numw1Avail )
#define GET_LIT_NUM_NUMW2_USED( __l )			( main_litData[__l].numw2Used )
#define GET_LIT_NUM_NUMW2_AVAIL( __l )			( main_litData[__l].numw2Avail )

#define GET_LIT_NUM_W1_WATCHED_CLS( __l , __n) 	( main_litData[__l].w1[__n] )
#define GET_LIT_NUM_W2_WATCHED_CLS( __l , __n) 	( main_litData[__l].w2[__n] )

#define GET_LIT_NUM_TRL_ISINQ( __l )			( main_litData[__l].obs )
#define SET_LIT_NUM_TRL_ISINQ( __l , __d )		( main_litData[__l].obs = __d )

//The following macro is used to add a clause (given as pointer in __cPtr) to the occurrence list of the literal (given as
//integer __l), which is at the provided position (given as integer in __lPos) in the clause. It will dynamically extend the
//occurrence list if necessary.
#define ADD_CLS_TO_LIT_OCCS( __cPtr , __l, __lPos) {\
		main_litData[ __l ].occs[main_litData[ __l ].numOccsUsed] = __cPtr; \
		main_litData[ __l ].occLitPos[main_litData[ __l ].numOccsUsed] = __lPos; \
		__cPtr->occPos[ __lPos ] = main_litData[ __l ].numOccsUsed++; \
		if (main_litData[ __l ].numOccsAvail == main_litData[ __l ].numOccsUsed){ \
			main_litData[ __l ].numOccsAvail *= 2; \
			main_litData[ __l ].occs = realloc(main_litData[ __l ].occs, sizeof(clause*)*main_litData[ __l ].numOccsAvail); \
			main_litData[ __l ].occLitPos = realloc(main_litData[ __l ].occLitPos, \
												sizeof(uint32_t)*main_litData[ __l ].numOccsAvail); \
		} \
}

//The following macro is used to remove a single literal from the given clause __cPtr at position __pos. This only
//works correctly if the literal is not a watcher, and the clause is not binary. Furthermore, we assume that the clause has
//already been removed from the literals occurrence list.
#define REM_CLS_LIT_NUM_AT_POS( __cPtr, __pos ) { \
	__cPtr->lits[__pos]   = __cPtr->lits[--__cPtr->size]; \
	__cPtr->occPos[__pos] = __cPtr->occPos[__cPtr->size]; \
	__cPtr->msgs[__pos]   = __cPtr->msgs[ __cPtr->size ]; \
	if (__pos < __cPtr->size) { \
		(main_litData + __cPtr->lits[__pos])->occLitPos[__cPtr->occPos[__pos]] = __pos; \
	} \
	__cPtr->lits[__cPtr->size] = 0; \
}


//The following macro is used to remove a clause from an occurrence list of a specific literal. The array size is not reduced.
#define REM_CLS_FROM_LIT_OCCS( __l, __occPos ) {\
		main_litData[ __l ].occs[ __occPos ] = main_litData[ __l ].occs[ --main_litData[ __l ].numOccsUsed ]; \
		main_litData[ __l ].occLitPos[ __occPos ] = main_litData[ __l ].occLitPos[ main_litData[ __l ].numOccsUsed ]; \
		main_litData[ __l ].occs[ __occPos ]->occPos[ main_litData[ __l ].occLitPos[ __occPos ] ] = __occPos; \
}

#define REM_CLS_FROM_LIT_OCCS_PTR( __lPtr, __occPos ) {\
		__lPtr->occs[ __occPos ] = __lPtr->occs[ --__lPtr->numOccsUsed ]; \
		__lPtr->occLitPos[ __occPos ] = __lPtr->occLitPos[ __lPtr->numOccsUsed ]; \
		__lPtr->occs[ __occPos ]->occPos[ __lPtr->occLitPos[ __occPos ] ] = __occPos; \
}
//The following two macros are used to add a clause (given as pointer in __cPtr) to a literals (given as integer in __l) watch
//list (either first or second watcher). It will dynamically extend the size of the watcher lists.
#define ADD_CLS_TO_LIT_W1LIST( __cPtr , __l ) {\
		main_litData[ __l ].w1[main_litData[ __l ].numw1Used] = __cPtr; \
		__cPtr->whereW1 = main_litData[ __l ].numw1Used++; \
		if (main_litData[ __l ].numw1Avail == main_litData[ __l ].numw1Used){ \
			main_litData[ __l ].numw1Avail *= 2; \
			main_litData[ __l ].w1 = realloc(main_litData[ __l ].w1, sizeof(clause*)*main_litData[ __l ].numw1Avail); \
		} \
}
#define ADD_CLS_TO_LIT_W2LIST( __cPtr , __l ) {\
		main_litData[ __l ].w2[main_litData[ __l ].numw2Used] = __cPtr; \
		__cPtr->whereW2 = main_litData[ __l ].numw2Used++; \
		if (main_litData[ __l ].numw2Avail == main_litData[ __l ].numw2Used){ \
			main_litData[ __l ].numw2Avail *= 2; \
			main_litData[ __l ].w2 = realloc(main_litData[ __l ].w2, sizeof(clause*)*main_litData[ __l ].numw2Avail); \
		} \
}
//The following two macros are used to remove a clause from a literals (integer __l) watch lists at position __pos. The memory
//for the sizes of the watch lists is not reduced.
#define REM_CLS_FROM_LIT_W1LIST( __l , __pos) {\
		main_litData[ __l ].w1[ __pos ] = main_litData[ __l ].w1[ --main_litData[ __l ].numw1Used ]; \
		main_litData[ __l ].w1[ __pos ]->whereW1 = __pos; \
}
#define REM_CLS_FROM_LIT_W2LIST( __l , __pos) {\
		main_litData[ __l ].w2[ __pos ] = main_litData[ __l ].w2[ --main_litData[ __l ].numw2Used ]; \
		main_litData[ __l ].w2[ __pos ]->whereW2 = __pos; \
}

//The following macro is used to add the necessary literal implications given a binary clause. The clause MUST be binary.
#define ADD_LIT_IMPLICATIONS( __cPtr, __lit1 , __lit2 ) {\
		__lit1 = -__cPtr->lits[0]; \
		__lit2 = __cPtr->lits[1]; \
		__cPtr->whereW2 = main_litData[ __lit1 ].numImpUsed; \
		main_litData[ __lit1 ].implies[main_litData[ __lit1 ].numImpUsed++] = (clause*)(addr_t)__lit2; \
		main_litData[ __lit1 ].implies[main_litData[ __lit1 ].numImpUsed++] = __cPtr; \
		if (main_litData[ __lit1 ].numImpAvail == main_litData[ __lit1 ].numImpUsed){ \
			main_litData[ __lit1 ].numImpAvail *= 2; \
			main_litData[ __lit1 ].implies = realloc(main_litData[ __lit1 ].implies,\
												sizeof(clause*)*main_litData[ __lit1 ].numImpAvail); \
		} \
		__lit1 = __cPtr->lits[0]; \
		__lit2 = -__cPtr->lits[1]; \
		__cPtr->whereW1 = main_litData[ __lit2 ].numImpUsed; \
		main_litData[ __lit2 ].implies[main_litData[ __lit2 ].numImpUsed++] = (clause*)(addr_t)__lit1; \
		main_litData[ __lit2 ].implies[main_litData[ __lit2 ].numImpUsed++] = __cPtr; \
		if (main_litData[ __lit2 ].numImpAvail == main_litData[ __lit2 ].numImpUsed){ \
			main_litData[ __lit2 ].numImpAvail *= 2; \
			main_litData[ __lit2 ].implies = realloc(main_litData[ __lit2 ].implies,\
												sizeof(clause*)*main_litData[ __lit2 ].numImpAvail); \
		} \
}

//The following macro is used to remove literal implications in case a binary clause is disabled or deleted. The clause MUST
//be binary for this!
#define REM_LIT_IMPLICATIONS( __cPtr, __l, __p) {\
		__l = -__cPtr->lits[0]; \
		__p = __cPtr->whereW2; \
		main_litData[ __l ].implies[__p] = main_litData[ __l ].implies[main_litData[ __l ].numImpUsed-2]; \
		main_litData[ __l ].implies[__p+1] = main_litData[ __l ].implies[main_litData[ __l ].numImpUsed-1]; \
		main_litData[ __l ].numImpUsed -= 2; \
		if ((clause*)(addr_t)(main_litData[ __l ].implies[__p])==(clause*)(addr_t)(main_litData[__l].implies[__p+1]->lits[0])){\
			main_litData[ __l ].implies[__p+1]->whereW1 = __p;\
		} else { \
			main_litData[ __l ].implies[__p+1]->whereW2 = __p;\
		} \
		__l = -__cPtr->lits[1]; \
		__p = __cPtr->whereW1; \
		main_litData[ __l ].implies[__p] = main_litData[ __l ].implies[main_litData[ __l ].numImpUsed-2]; \
		main_litData[ __l ].implies[__p+1] = main_litData[ __l ].implies[main_litData[ __l ].numImpUsed-1]; \
		main_litData[ __l ].numImpUsed -= 2; \
		if ((clause*)(addr_t)(main_litData[ __l ].implies[__p])==(clause*)(addr_t)(main_litData[__l].implies[__p+1]->lits[0])){\
			main_litData[ __l ].implies[__p+1]->whereW1 = __p;\
		} else { \
			main_litData[ __l ].implies[__p+1]->whereW2 = __p;\
		} \
}

void literals_reset();		//For resetting all the literals for a clean start.
uint32_t literals_init();	//For initializing once f.n_initial is known. Returns 0 if unsuccessful.
void literals_dispose();	//For freeing all related memory.

#ifdef VERBOSE
void literals_printLiteralImplications(literal*);
void literals_printLiteralOccs(literal*);
void literals_printLiteralOccLitPos(literal*);
void literals_printLiteralW1List(literal*);
void literals_printLiteralW2List(literal*);
void literals_printLiteral(literal*);	//This will output a single comment line with all the literal data.
#endif

#endif /* LITERALS_H_ */
