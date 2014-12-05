/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCLCLAUSES_H_
#define CDCLCLAUSES_H_

typedef struct {
	uint64_t sig;			//The original signature of the clause, assigned literals are not removed.
	int32_t* lits;			//The literals of the clause, terminated by 0.
	int32_t* occPos;		//The positions of this clause in the occurrence lists of the contained literals.
	float_ty activity;		//The activity of the clause. This value is only set if the clause is a learned clause.
	float_ty cQScore;		//The score of the clause in the CDCL clause priority queue.
	uint32_t baseData;		//If its a reason (1 bit) a learned (1 bit) a blocked (1 bit) clause and the offset (29 bit).
	int32_t repLit;			//The representative literal of the clause. If SAT, this should be a satisfying literal. Lazy!
	int32_t w1;				//Watcher 1.
	int32_t whereW1;		//What is the position in the watcher list of the first watcher?
	int32_t w2;				//Watcher 2.
	int32_t whereW2;		//What is the position in the watcher list of the second watcher?
	uint32_t size;			//The original size of the clause, temporary assigned literals are not removed.
	uint32_t lbd;			//The literal blocks distance of the clauses literals.
	uint32_t cSPos;			//The position of this clause in the CDCL clause stack.
	uint32_t cQPos;			//The position of this clause in the CDCL clause priority queue.
} cdclClause;

typedef struct {			//The structure of a removed clause (NIVER/BLOCKED/COVERED/...)
	int32_t* lits;			//The literals of the clause.
	int32_t litOfInterest;	//The literal of interest (i.e. the resolution literal in NIVER or the blocking literal).
	uint32_t size;			//The original size of the clause, assigned literals are not removed.
	uint32_t offset;		//The position in the cdcl_cls_rem array of this clause.
	uint32_t byAction;		//By what action was the clause removed? NIVER, BLOCKED, COVERED, ATE, GE...
} remCdclClause;

#define CDCL_CLS_HEADER_MASK 3758096384U	//1110 0000 0000 0000 0000 0000 0000 0000
#define CDCL_CLS_REASON_MASK 2147483648U	//1000 0000 0000 0000 0000 0000 0000 0000
#define CDCL_CLS_LEARNED_MASK 1073741824U	//0100 0000 0000 0000 0000 0000 0000 0000
#define CDCL_CLS_BLOCKED_MASK 536870912U	//0010 0000 0000 0000 0000 0000 0000 0000
#define CDCL_CLS_OFFSET_MASK 536870911U		//0001 1111 1111 1111 1111 1111 1111 1111

//The following defines identify by what action a clause was removed during in-processing.
#define CDCL_IR_REMCLS_NIVER 0
#define CDCL_IR_REMCLS_GE 1

//The following macros are used to query the base data.
#define IS_CDCL_CLS_REASON( __cPtr )			( ( __cPtr->baseData & CDCL_CLS_REASON_MASK ) == CDCL_CLS_REASON_MASK )
#define IS_CDCL_CLS_LEARNED( __cPtr )			( ( __cPtr->baseData & CDCL_CLS_LEARNED_MASK ) == CDCL_CLS_LEARNED_MASK )
#define IS_CDCL_CLS_BLOCKED( __cPtr )			( ( __cPtr->baseData & CDCL_CLS_BLOCKED_MASK ) == CDCL_CLS_BLOCKED_MASK )

//The following macro checks if a clause cannot be removed (i.e. by clause database maintenance).
#define IS_CDCL_CLS_IRREMOVABLE( __cPtr )		( GET_CDCL_CLS_SIZE(__cPtr) < 3 || IS_CDCL_CLS_REASON(__cPtr) || IS_CDCL_CLS_BLOCKED(__cPtr) )

//The following macros are used to get and set the fields in a CDCL clause.
#define GET_CDCL_CLS_SIG( __cPtr )				( __cPtr->sig )
#define GET_CDCL_CLS_LITS( __cPtr )				( __cPtr->lits )
#define GET_CDCL_CLS_OCCPOS( __cPtr )			( __cPtr->occPos )
#define GET_CDCL_CLS_ACTIVITY( __cPtr )			( __cPtr->activity )
#define GET_CDCL_CLS_BASEDATA( __cPtr )			( __cPtr->baseData )
#define GET_CDCL_CLS_SIZE( __cPtr )				( __cPtr->size )
#define GET_CDCL_CLS_REPLIT( __cPtr )			( __cPtr->repLit )
#define GET_CDCL_CLS_W1( __cPtr )				( __cPtr->w1 )
#define GET_CDCL_CLS_WHEREW1( __cPtr )			( __cPtr->whereW1 )
#define GET_CDCL_CLS_W2( __cPtr )				( __cPtr->w2 )
#define GET_CDCL_CLS_WHEREW2( __cPtr )			( __cPtr->whereW2 )
#define GET_CDCL_CLS_OFFSET( __cPtr )			( __cPtr->baseData & CDCL_CLS_OFFSET_MASK )
#define GET_CDCL_CLS_LITNUM( __cPtr, __n )		( __cPtr->lits[__n] )
#define GET_CDCL_CLS_OCCPOSNUM( __cPtr, __n )	( __cPtr->occPos[__n] )
#define GET_CDCL_CLS_SPOS( __cPtr )				( __cPtr->cSPos )
#define GET_CDCL_CLS_LBD( __cPtr )				( __cPtr->lbd )
#define GET_CDCL_CLS_QPOS( __cPtr )				( __cPtr->cQPos )
#define GET_CDCL_CLS_QSCORE( __cPtr )			( __cPtr->cQScore )

#define SET_CDCL_CLS_SIG( __cPtr , __s )		( __cPtr->sig = __s )
#define SET_CDCL_CLS_LITS( __cPtr , __lits )	( __cPtr->lits = __lits )
#define SET_CDCL_CLS_OCCPOS( __cPtr, __op )		( __cPtr->occPos = __op )
#define SET_CDCL_CLS_ACTIVITY( __cPtr, __a )	( __cPtr->activity = __a )
#define SET_CDCL_CLS_BASEDATA( __cPtr , __b)	( __cPtr->baseData = __b )
#define SET_CDCL_CLS_SIZE( __cPtr , __s )		( __cPtr->size = __s )
#define SET_CDCL_CLS_REPLIT( __cPtr , __rl )	( __cPtr->repLit = __rl )
#define SET_CDCL_CLS_W1( __cPtr , __w )			( __cPtr->w1 = __w )
#define SET_CDCL_CLS_WHEREW1( __cPtr , __w )	( __cPtr->whereW1 = __w )
#define SET_CDCL_CLS_W2( __cPtr , __w )			( __cPtr->w2 = __w )
#define SET_CDCL_CLS_WHEREW2( __cPtr , __w )	( __cPtr->whereW2 = __w )
#define SET_CDCL_CLS_OFFSET( __cPtr , __o )		( __cPtr->baseData = (__cPtr->baseData & CDCL_CLS_HEADER_MASK) | __o )
#define SET_CDCL_CLS_LITNUM( __cPtr, __n , __l)	( __cPtr->lits[__n] = __l )
#define SET_CDCL_CLS_OCCPOSNUM( __cPtr, __n , __p )	( __cPtr->occPos[__n] = __p)
#define SET_CDCL_CLS_SPOS( __cPtr , __p )		( __cPtr->cSPos = __p )
#define SET_CDCL_CLS_LBD( __cPtr , __d )		( __cPtr->lbd = __d )
#define SET_CDCL_CLS_QPOS( __cPtr , __d )		( __cPtr->cQPos = __d )
#define SET_CDCL_CLS_QSCORE( __cPtr , __d )		( __cPtr->cQScore = __d )

#define SET_CDCL_CLS_REASON_0( __cPtr )			( __cPtr->baseData = __cPtr->baseData & ~CDCL_CLS_REASON_MASK )
#define SET_CDCL_CLS_REASON_1( __cPtr )			( __cPtr->baseData = __cPtr->baseData | CDCL_CLS_REASON_MASK )

#define SET_CDCL_CLS_LEARNED_0( __cPtr )		( __cPtr->baseData = __cPtr->baseData & ~CDCL_CLS_LEARNED_MASK )
#define SET_CDCL_CLS_LEARNED_1( __cPtr )		( __cPtr->baseData = __cPtr->baseData | CDCL_CLS_LEARNED_MASK )

#define SET_CDCL_CLS_BLOCKED_0( __cPtr )		( __cPtr->baseData = __cPtr->baseData & ~CDCL_CLS_BLOCKED_MASK )
#define SET_CDCL_CLS_BLOCKED_1( __cPtr )		( __cPtr->baseData = __cPtr->baseData | CDCL_CLS_BLOCKED_MASK )

#define SET_CDCL_CLS_ACT_INC_BY( __cPtr , __val ) ( __cPtr->activity += __val )
#define SET_CDCL_CLS_ACT_NORM_BY( __cPtr , __val )  ( __cPtr->activity = ONE + ( __cPtr->activity / __val) )

#define SET_CDCL_CLS_SIZE_INC( __cPtr )			( ++(__cPtr->size) )
#define SET_CDCL_CLS_SIZE_DEC( __cPtr )			( --(__cPtr->size) )

#define SET_CDCL_CLS_LBD_INC( __cPtr )			( ++(__cPtr->lbd) )
#define SET_CDCL_CLS_LBD_DEC( __cPtr )			( --(__cPtr->lbd) )

//The following macro is used to remove a single literal from the given clause __cPtr at position __pos. This only
//works correctly if the literal is not a watcher, and the clause is not binary. Furthermore, we assume that the clause has
//already been removed from the literals occurrence list.
#define REM_CDCL_CLS_LIT_ID_AT_POS( __cPtr, __pos ) { \
	__cPtr->lits[__pos]   = __cPtr->lits[--__cPtr->size]; \
	__cPtr->occPos[__pos] = __cPtr->occPos[__cPtr->size]; \
	if (__pos < __cPtr->size) { \
		(cdcl_literals + __cPtr->lits[__pos])->occLitPos[__cPtr->occPos[__pos]] = __pos; \
	} \
	__cPtr->lits[__cPtr->size] = 0; \
}

//The following macro is used to ensure if we have minimally allocated learned clauses left to learn __num new clauses. The
//value __i is just a counter and must be an uint32_t, and __c is a pointer to a CDCL clause.
#define ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR( __num , __i , __c ) { \
	if (cdcl_cls_l_used + __num >= cdcl_cls_l_avail) { \
		__i = cdcl_cls_l_avail; \
		cdcl_cls_l_avail += (__num + BLOCKSIZE); \
		cdcl_cls_l = realloc(cdcl_cls_l, sizeof(cdclClause*)*(cdcl_cls_l_avail)); \
		while (__i < cdcl_cls_l_avail) { \
			ALLOCATE_LEARNED_CDCL_CLS_MINIMAL( __c , __i ); \
			++__i; \
		} \
	} \
}

//The following macro is used to ensure if we have minimally allocated original clauses left to learn __num new clauses. The
//value __i is just a counter and must be an uint32_t, and __c is a pointer to a CDCL clause.
#define ENSURE_CDCL_CLS_ORIGINAL_AVAIL_FOR( __num , __i , __c ) { \
	if (cdcl_cls_o_used + __num >= cdcl_cls_o_avail) { \
		__i = cdcl_cls_o_avail; \
		cdcl_cls_o_avail += (__num + BLOCKSIZE); \
		cdcl_cls_o = realloc(cdcl_cls_o, sizeof(cdclClause*)*(cdcl_cls_o_avail)); \
		while (__i < cdcl_cls_o_avail) { \
			ALLOCATE_ORIGINAL_CDCL_CLS_MINIMAL( __c , __i ); \
			++__i; \
		} \
	} \
}

//The following macro is used to ensure if we have minimally allocated removed clauses left to add __num new clauses. The
//value __i is just a counter and must be an uint32_t, and __c is a pointer to a removed CDCL clause.
#define ENSURE_CDCL_CLS_REMOVED_AVAIL_FOR( __num , __i , __c ) { \
	if (cdcl_cls_r_used + __num >= cdcl_cls_r_avail) { \
		__i = cdcl_cls_r_avail; \
		cdcl_cls_r_avail += (__num + BLOCKSIZE); \
		cdcl_cls_r = realloc(cdcl_cls_r, sizeof(remCdclClause*)*(cdcl_cls_r_avail)); \
		while (__i < cdcl_cls_r_avail) { \
			ALLOCATE_REMOVED_CDCL_CLS_MINIMAL( __c , __i ); \
			++__i; \
		} \
	} \
}

//The following macros are used to allocate the memory for a new clause.
#define ALLOCATE_ORIGINAL_CDCL_CLS_MINIMAL( __c , __pos ) { \
	__c = (cdcl_cls_o[__pos] = malloc(sizeof(cdclClause))); \
	__c->sig = 0ULL; \
	__c->lits =  malloc(sizeof(int32_t)*(1U)); \
	__c->occPos =  malloc(sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = __pos; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
}

#define ALLOCATE_LEARNED_CDCL_CLS_MINIMAL( __c , __pos ) { \
	__c = (cdcl_cls_l[__pos] = malloc(sizeof(cdclClause))); \
	__c->sig = 0ULL; \
	__c->lits =  malloc(sizeof(int32_t)*(1U)); \
	__c->occPos =  malloc(sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = CDCL_CLS_LEARNED_MASK | __pos; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
}

#define ALLOCATE_REMOVED_CDCL_CLS_MINIMAL( __c , __pos ) { \
	__c = (cdcl_cls_r[__pos] = malloc(sizeof(remCdclClause))); \
	__c->lits =  malloc(sizeof(int32_t)*(1U)); \
	__c->offset = i; \
	__c->size = 0; \
	__c->litOfInterest = 0; \
	__c->byAction = 0; \
}

//The following macro is used to shrink a clause back to minimal size and update its position.
#define SHRINK_ORIGINAL_CDCL_CLS( __c , __toPos ) { \
	__c->sig = 0ULL; \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(1U)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = __toPos; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
	cdcl_cls_o[__toPos] = __c; \
}

#define SHRINK_LEARNED_CDCL_CLS( __c , __toPos ) { \
	__c->sig = 0ULL; \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(1U)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = CDCL_CLS_LEARNED_MASK | __toPos; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
	cdcl_cls_l[__toPos] = __c; \
}

#define SHRINK_ORIGINAL_CDCL_CLS_INPLACE( __c ) { \
	__c->sig = 0ULL; \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(1U)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = __c->baseData & ~CDCL_CLS_HEADER_MASK; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
}

#define SHRINK_LEARNED_CDCL_CLS_INPLACE( __c ) { \
	__c->sig = 0ULL; \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(1U)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(1U)); \
	__c->activity = ONE; \
	__c->baseData = (__c->baseData & ~CDCL_CLS_HEADER_MASK) | CDCL_CLS_LEARNED_MASK; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
}

#define SHRINK_CDCL_CLS_TO_OPTIMAL_SIZE( __c ) { \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(GET_CDCL_CLS_SIZE(__c)+1U)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(GET_CDCL_CLS_SIZE(__c)+1U)); \
}

#define REALLOC_CDCL_CLS_TO_SIZE( __c , __s ) { \
	__c->lits = realloc(__c->lits, sizeof(int32_t)*(__s)); \
	__c->occPos = realloc(__c->occPos, sizeof(int32_t)*(__s)); \
}

#define RESET_CDCL_CLS( __c ) {\
	__c->sig = 0ULL; \
	__c->activity = ONE; \
	__c->baseData = __c->baseData & ~CDCL_CLS_REASON_MASK; \
	__c->baseData = __c->baseData & ~CDCL_CLS_BLOCKED_MASK; \
	__c->repLit = 0; \
	__c->w1 = 0; \
	__c->whereW1 = 0; \
	__c->w2 = 0; \
	__c->whereW2 = 0; \
	__c->size = 0; \
	__c->cSPos = 0; \
	__c->lbd = 0; \
	__c->cQPos = 0; \
	__c->cQScore = ZERO; \
}

#define GET_CDCL_LEARNED_CLS_NUM( __i ) ( cdcl_cls_l[ __i ] )

//Methods for handling the clause priority queue. In this queue, a larger score value means better score.
void cdcl_clauses_Q_moveUp(cdclClause*);	//To move an element up in the CDCL clause priority queue.
void cdcl_clauses_Q_moveDown(cdclClause*);	//To move an element down in the CDCL clause priority queue.
cdclClause* cdcl_clauses_Q_dequeueBest();	//To dequeue the best element in the CDCL clause priority queue.
void cdcl_clauses_Q_delete(cdclClause*);	//To delete an element from the CDCL clause priority queue.
void cdcl_clauses_Q_touchClause_null(cdclClause*);//To do nothing when a clause is touched.
void cdcl_clauses_Q_touchClause_maintenance_inactivity(cdclClause*);//To touch a clause when doing clause database maintenance.
void cdcl_clauses_Q_touchClause_maintenance_lbd(cdclClause*);//To touch a clause when doing clause database maintenance.
void cdcl_clauses_Q_touchClause_maintenance_wr(cdclClause*);//To touch a clause when reordering the watcher lists.
void cdcl_clauses_Q_touchClause_inprocessing_se(cdclClause*);//To touch a clause when doing in-processing SE.
void cdcl_clauses_Q_touchClause_inprocessing_str(cdclClause*);//To do touch a clause when doing in-processing STR.
void cdcl_clauses_Q_touchClause_inprocessing_treelook(cdclClause*);//To do touch a clause when doing in-processing TREELOOK.
void (*cdcl_clauses_Q_touchClause)(cdclClause*);//This method points to one of the above touch methods.
void cdcl_clauses_Q_enqueue(cdclClause*);	//To enqueue an element in the CDCL clause priority queue.
void cdcl_clauses_Q_flush();				//To remove all elements in the CDCL clause priority queue.
void cdcl_clauses_Q_ensureSize(uint32_t);   //To make sure that the queue can hold a specific amount of clauses.

//Methods for handling the clause stack.
void cdcl_clauses_S_delete(cdclClause*);	//To delete a clause from the literal stack.
void cdcl_clauses_S_push(cdclClause*);		//To push something on the clauses stack.
void cdcl_clauses_S_touchClause_null(cdclClause*);//To do nothing when a clause is touched.
void (*cdcl_clauses_S_touchClause)(cdclClause*);//This method points to one of the above touch methods.
cdclClause* cdcl_clauses_S_pop();			//To pop the topmost element from the stack.
void cdcl_clauses_S_flush();				//To remove all elements from the clauses stack.
void cdcl_clauses_S_ensureSize(uint32_t);   //To make sure that the stack can hold a specific amount of clauses.

void cdcl_clauses_prepareCall(); //Creates a restriction of the main formula into the clauses of the CDCL.

void cdcl_clauses_reset();
void cdcl_clauses_init();
void cdcl_clauses_dispose();

void cdcl_clauses_deleteClause(cdclClause*);

#ifdef VERBOSE_CDCL
void cdcl_clauses_printClauseLiterals_inline(cdclClause*);
void cdcl_clauses_printClauseOccPos_inline(cdclClause*);
void cdcl_clauses_printClause(cdclClause*);
void cdcl_clauses_printClauseInline(cdclClause*);
void cdcl_clauses_printOriginal();
void cdcl_clauses_printLearned();
void cdcl_clauses_printLastNLearned(uint32_t);
void cdcl_clauses_printAll();

uint32_t cdcl_clauses_verifyClause(cdclClause*);
uint32_t cdcl_clauses_verifyAll();
#endif

#endif /* CDCLCLAUSES_H_ */
