/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CLAUSES_H_
#define CLAUSES_H_

#include "../dimetheus.h"

typedef struct {			//The structure of a standard clause.
	uint64_t sig;			//The original signature of the clause, assigned literals are not removed.
	int32_t* lits;			//The literals of the clause.
	float_ty* msgs;			//The messages send by this clause to each variable according to a message passing logic.
	int32_t* occPos;		//The positions of this clause in the occurrence lists.
	uint32_t baseData;		//The array the clause is found in (first 2 bit), if its a reason (1 bit) and the offset (29 bit).
	int32_t repLit;			//The representative literal of the clause. If SAT, this should be a satisfying literal. Lazy!
	int32_t w1;				//Watcher 1.
	int32_t whereW1;		//What is the position in the watcher list of the first watcher?
	int32_t w2;				//Watcher 2.
	int32_t whereW2;		//What is the position in the watcher list of the second watcher?
	uint32_t size;			//The original size of the clause, temporary assigned literals are not removed.
	int32_t cQScore;		//The score the clause has in the clause priority queue. Caution: the smaller, the better!
	uint32_t cQPos;			//The position of this clause in the clauses priority queue. Zero means not in queue.
	uint32_t cSPos;			//The position of this clause in the clauses stack. Zero means not in stack.
	uint32_t cSS0Pos;		//The position of this clause in the S0 clauses stack. Zero means not in stack.
	uint32_t cSS1Pos;		//The position of this clause in the S1 clauses stack. Zero means not in stack.
} clause;

typedef struct {			//The structure of a covering literal.
	int32_t lit;			//The covered literal.
	int32_t coveredBy;		//The literal that covers it.
} coveredLit;

typedef struct {			//The structure of a removed clause (NIVER/BLOCKED/COVERED/...)
	int32_t* lits;			//The literals of the clause.
	coveredLit* coveredLits;//The literals that are covered.
	int32_t numCovered;		//The number of covering literals for this clause.
	int32_t litOfInterest;	//The literal of interest (i.e. the resolution literal in NIVER or the blocking literal).
	uint32_t size;			//The original size of the clause, assigned literals are not removed.
	uint32_t offset;		//The position in the f.clauses_rem array of this clause.
	uint32_t byAction;		//By what action was the clause removed? NIVER, BLOCKED, COVERED, ATE, GE...
} remClause;

//The following masks are used in conjunction with the baseData value of uint32_t values.
#define CLS_HEADER_MASK	3758096384U		//1110 0000 0000 0000 0000 0000 0000 0000
#define CLS_ID_MASK 3221225472U			//1100 0000 0000 0000 0000 0000 0000 0000
#define CLS_DISABLED_MASK 1073741824U	//0100 0000 0000 0000 0000 0000 0000 0000
#define CLS_LEARNED_MASK 2147483648U	//1000 0000 0000 0000 0000 0000 0000 0000
#define CLS_REASON_MASK 536870912U		//0010 0000 0000 0000 0000 0000 0000 0000
#define CLS_OFFSET_MASK	536870911U		//0001 1111 1111 1111 1111 1111 1111 1111
#define CLS_ID_EO 0U					//0000 0000 0000 0000 0000 0000 0000 0000
#define CLS_ID_DO 1073741824U 			//0100 0000 0000 0000 0000 0000 0000 0000
#define CLS_ID_EL 2147483648U			//1000 0000 0000 0000 0000 0000 0000 0000
#define CLS_ID_DL 3221225472U			//1100 0000 0000 0000 0000 0000 0000 0000

//The following definitions contain all the reasons for clauses to be removed.
#define PREP_REMCLS_NIVER 0
#define PREP_REMCLS_BLOCKED 1
#define PREP_REMCLS_COVERED 2
#define PREP_REMCLS_ATE 3
#define PREP_REMCLS_GE 4

//The following macros are used to allocate the memory for a new clause. The __rLit is the representative literal.
#define ALLOCATE_LEARNED_CLS( __c , __size , __rLit) { \
	__c = (f.clauses_el[f.m_el_used] = malloc(sizeof(clause))); \
	__c->sig = 0ULL; \
	__c->lits =  malloc(sizeof(int32_t)*(__size)); \
	__c->msgs =  malloc(sizeof(float_ty)*(__size)); \
	__c->occPos =  malloc(sizeof(int32_t)*(__size)); \
	__c->baseData = CLS_ID_EL + f.m_el_used; \
	__c->repLit = __rLit; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = __size; \
	__c->cQScore = 0; \
	__c->cQPos = 0; \
	__c->cSPos = 0; \
	__c->cSS0Pos = 0; \
	__c->cSS1Pos = 0; \
}

#define ALLOCATE_ORIGINAL_CLS( __c , __size , __rLit) { \
	__c = (f.clauses_eo[f.m_eo_used] = malloc(sizeof(clause))); \
	__c->sig = 0ULL; \
	__c->lits =  malloc(sizeof(int32_t)*(__size)); \
	__c->msgs =  malloc(sizeof(float_ty)*(__size)); \
	__c->occPos =  malloc(sizeof(int32_t)*(__size)); \
	__c->baseData = CLS_ID_EO + f.m_eo_used; \
	__c->repLit = __rLit; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = __size; \
	__c->cQScore = 0; \
	__c->cQPos = 0; \
	__c->cSPos = 0; \
	__c->cSS0Pos = 0; \
	__c->cSS1Pos = 0; \
}

#define SHRINK_CLS_TO_PERFECT_SIZE( __c ) { \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(__c->size+1)); \
	__c->msgs = realloc(__c->msgs, sizeof(float_ty)*(__c->size+1)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(__c->size+1)); \
}

//The following macros are used to determine whether the clause is a disabled, enabled, learned or original clause.
#define IS_CLS_DISABLED( __cPtr )	( __cPtr->baseData & CLS_DISABLED_MASK )
#define IS_CLS_LEARNED( __cPtr )	( __cPtr->baseData & CLS_LEARNED_MASK )

//The following macro can be used to test if the clause is currently a reason for some assignment under UP.
#define IS_CLS_REASON( __cPtr )		( __cPtr->baseData & CLS_REASON_MASK )

//To help iterate through the literals and ALA literals of clauses.
#define HAS_CLS_MORE_LITS( __cPtr , __lPtr )	( (uint32_t)(addr_t)(__lPtr - __cPtr->lits) < __cPtr->size )
#define HAS_REM_CLS_MORE_ALALITS( __cPtr , __lPtr )	( (int32_t)(addr_t)(__lPtr - __cPtr->alaLits) < __cPtr->numALA )

//The following macros are used to get the base data of the clause.
#define GET_CLS_HEADER( __cPtr )	( __cPtr->baseData & CLS_HEADER_MASK )
#define GET_CLS_LISTID( __cPtr )	( __cPtr->baseData & CLS_ID_MASK )
#define GET_CLS_OFFSET( __cPtr )	( __cPtr->baseData & CLS_OFFSET_MASK )

//The following macros are used to set specific bits in the clauses header data.
#define SET_CLS_REASON_1( __cPtr )	( __cPtr->baseData |= CLS_REASON_MASK )
#define SET_CLS_REASON_0( __cPtr )	( __cPtr->baseData = __cPtr->baseData & ~CLS_REASON_MASK )

#define SET_CLS_DISABL_1( __cPtr )	( __cPtr->baseData |= CLS_DISABLED_MASK )
#define SET_CLS_DISABL_0( __cPtr )	( __cPtr->baseData = __cPtr->baseData & ~CLS_DISABLED_MASK )

#define SET_CLS_LEARNED_1( __cPtr )	( __cPtr->baseData |= CLS_LEARNED_MASK )
#define SET_CLS_LEARNED_0( __cPtr )	( __cPtr->baseData = __cPtr->baseData & ~CLS_LEARNED_MASK )

//The following macros are used to access the clause priority and stack relevant data.
#define SET_CLS_Q_POS( __cPtr , __value ) 	( __cPtr->cQPos = __value )
#define SET_CLS_Q_SCORE( __cPtr , __value )	( __cPtr->cQScore = __value )
#define GET_CLS_Q_POS( __cPtr ) 			( __cPtr->cQPos )
#define GET_CLS_Q_SCORE( __cPtr )			( __cPtr->cQScore )

#define SET_CLS_S_POS( __cPtr , __value ) 	( __cPtr->cSPos = __value )
#define GET_CLS_S_POS( __cPtr ) 			( __cPtr->cSPos )

#define SET_CLS_SS0_POS( __cPtr , __value ) ( __cPtr->cSS0Pos = __value )
#define GET_CLS_SS0_POS( __cPtr ) 			( __cPtr->cSS0Pos )

#define SET_CLS_SS1_POS( __cPtr , __value ) ( __cPtr->cSS1Pos = __value )
#define GET_CLS_SS1_POS( __cPtr ) 			( __cPtr->cSS1Pos )

#define GET_CLS_SIZE( __cPtr )				( __cPtr->size )
#define SET_CLS_SIZE( __cPtr , __s )		( __cPtr->size = __s )

#define SET_CLS_SIZE_DEC( __cPtr )			( --(__cPtr->size) )
#define SET_CLS_SIZE_INC( __cPtr )			( ++(__cPtr->size) )

#define GET_CLS_LITNUM( __cPtr, __n )		( __cPtr->lits[__n] )
#define SET_CLS_LITNUM( __cPtr, __n, __l )  ( __cPtr->lits[__n] = __l)

#define GET_CLS_SIG( __cPtr )				( __cPtr->sig )
#define SET_CLS_SIG( __cPtr , __sig )		( __cPtr->sig = __sig )

#define GET_CLS_W1( __cPtr )				( __cPtr->w1 )
#define SET_CLS_W1( __cPtr , __w )			( __cPtr->w1 = __w )

#define GET_CLS_W2( __cPtr )				( __cPtr->w2 )
#define SET_CLS_W2( __cPtr , __w )			( __cPtr->w2 = __w )

#define GET_CLS_WHEREW1( __cPtr )			( __cPtr->whereW1 )
#define SET_CLS_WHEREW1( __cPtr , __ww )	( __cPtr->whereW1 = __ww )

#define GET_CLS_WHEREW2( __cPtr )			( __cPtr->whereW2 )
#define SET_CLS_WHEREW2( __cPtr , __ww )	( __cPtr->whereW2 = __ww )

#define GET_CLS_REPLIT( __cPtr )			( __cPtr->repLit )
#define SET_CLS_REPLIT( __cPtr , __r )		( __cPtr->repLit = __r )

#define GET_CLS_OCCPOSNUM( __cPtr , __n )	( __cPtr->occPos[__n] )
#define SET_CLS_OCCPOSNUM( __cPtr , __n , __p )	( __cPtr->occPos[__n] = __p )

#define GET_CLS_MSGS( __cPtr )				( __cPtr->msgs )
#define SET_CLS_MSGS( __cPtr, __m )			( __cPtr->msgs = __m )

#define GET_CLS_MSGNUM( __cPtr, __n )		( __cPtr->msgs[__n] )
#define SET_CLS_MSGNUM( __cPtr, __n , __m )	( __cPtr->msgs[__n] = __m )

//The following macros are used to remove or add a clause address from or to the respective list.
#define REM_CLS_FROM_CLAUSES_LIST( __list , __pos , __used ) {\
		__list[ __pos ] = __list[ --__used ]; \
		__list[ __pos ]->baseData = ( __list[ __pos ]->baseData & CLS_HEADER_MASK ) | __pos;\
}

#define ADD_CLS_TO_CLAUSES_LIST( __list , __listID , __cPtr , __used , __avail ) { \
		__list[ __used ] = __cPtr; \
		__cPtr->baseData = __listID | ( __cPtr->baseData & CLS_REASON_MASK ) | __used++; \
		if ( __used == __avail ) { \
			__avail *= 2; \
			__list = realloc(__list, sizeof(clause*)*__avail); \
		} \
}

#define ADD_REMCLS_TO_REMCLAUSES_LIST( __rcPtr ) { \
		f.clauses_rem[ f.m_rem_used ] = __rcPtr; \
		__rcPtr->offset = f.m_rem_used++; \
		if ( f.m_rem_used == f.m_rem_avail ) { \
			f.m_rem_avail *= 2; \
			f.clauses_rem = realloc(f.clauses_rem, sizeof(remClause*)*f.m_rem_avail); \
		} \
}

void clauses_reset();		//For resetting all the clause related data.
void clauses_dispose();		//For freeing all clause related data.

void clauses_addOrig(int32_t*, int32_t);	//For adding a single original clause provided only the literals. Must not be unit.
void clauses_addLearned(int32_t*, int32_t);	//For adding a single learned clause provided only the literals. Must not be unit.
void clauses_addLearnedBinary(int32_t, int32_t);//For adding a single learned clause provided the two literals.
void clauses_addLearnedTernary(int32_t, int32_t, int32_t);//For adding a single learned clause provided the three literals.

void clauses_deleteCls(clause*);		//For completely deleting a clause, including its memory.
void clauses_deleteLastNClauses(clause**, int32_t, int32_t*); //For deleting a set of clauses found at the end of the array.

void clauses_deleteLit(clause*, int32_t, int32_t); //For deleting a literal from a NON-BINARY clause.

void clauses_disable(clause*);//For temporarily disabling the clause (removing it from watcher and occurrence lists and so on).
void clauses_enable(clause*);	//For re-enabling one of the disabled clauses (adding it back to the lists).

void clauses_makeLearned(clause*);//This will transfer an original clause to the corresponding learned clause array.
void clauses_makeOriginal(clause*);//This will transfer a learned clause to the corresponding original clause array.

void clauses_remove(clause*, int32_t);//This will remove the clause from the formula and make it a removed one.

#ifdef VERBOSE
void clauses_printClauseLiterals_inline(clause*);
void clauses_printRemClauseLiterals_inline(remClause*);
void clauses_printClauseMessages_inline(clause*);
void clauses_printClauseOccPos_inline(clause*);

void clauses_printClause(clause*);		//This will output a single comment line with all the clause data.
void clauses_printClauseInline(clause*);//This will output a clauses literals without newlines or any other information.
void clauses_printRemClause(remClause*);//This will output a single comment line with all the removed clauses data.

void clauses_printAll();				//This will print out all clauses.

uint32_t clauses_verifyClause(clause*);
uint32_t clauses_verifyAll();
#endif

#endif /* CLAUSES_H_ */
