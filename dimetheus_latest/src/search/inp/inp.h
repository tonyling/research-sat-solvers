/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef INP_H_
#define INP_H_

#include "../search.h"
#include "_inpStrategies/inpStrategies.h"

//Modules in this phase.
#include "IN/in.h"

//Data-structures exclusive to this phase.
int32_t inp_returnCode;						//The return-code of the in-processing phase.
literal **inp_reduceFrom;					//From what point in DL0 does a clause database reduction happen?

literal** inp_unhiding_SPlus;				//The SPlus array for UNHIDING.
literal** inp_unhiding_SMinus;				//The SMinus array for UNHIDING.
literal** inp_unhiding_lStack;				//The literal stack used by UNHIDING.
literal** inp_unhiding_lStack_current;		//The literal stack used by UNHIDING.

literal** inp_litPerm;						//An array that can be used to create literal permutations.

int32_t** inp_res;							//The set of resolvents, mostly used by NIVER.
int32_t inp_resAvail, inp_resUsed;			//How many such resolvents have been initialized/used?
int32_t *inp_resMemAvail;					//How much memory is allocated for a specific resolvent?
int32_t *inp_resMemUsed;					//How much memory is in use (size)?

clause** inp_G_negV;						//During GE, this is the set of gate defining clauses containing +v.
int32_t inp_G_negVAvail;					//The amount of memory positions allocated in G_negV.
int32_t inp_G_negVUsed;						//The amount of memory used.
clause** inp_G_posV;						//During GE, this is the set of gate defining clauses containing -v.
int32_t inp_G_posVAvail;					//The amount of memory positions allocated in G_posV.
int32_t inp_G_posVUsed;						//The amount of memory used.

literal*** inp_els_classes;					//Equivalence classes of literals found during ELS.
uint32_t inp_els_classes_avail;				//The number of ELS classes that have been initialized.
uint32_t inp_els_classes_used;				//The current number of ELS classes used.
uint32_t* inp_els_classes_memAvail;			//The size of each used class.
uint32_t* inp_els_classes_memUsed;			//The amount of elements for each class.
literal** inp_els_lStack;					//The literal stack used by Equivalent Literal Substitution.
literal** inp_els_lStack_current;			//The literal stack current position for a new element.

literal** inp_trl_lQ;						//The literal queue for the tree-look - no priority queue.
literal** inp_trl_lQ_c;						//The current pointer pointing to the next element to dequeue.
literal** inp_trl_lQ_e;						//The end pointer pointing to the next position for queuing in.
literal** inp_trl_faileds;					//The set of literals that have been discovered as failed.

//The following macro is used to reset the resolvents.
#define INP_RESET_ALL_RESOLVENTS()	( inp_resUsed = 0 )

//The following macro is used to reset a specific resolvent before using it anew for resolution.
#define INP_RESET_RESOLVENT( __resNum ) {\
		inp_resMemUsed[ __resNum ] = 0;\
}

//The following macro is used to add another literal to the current resolvent. It increases the resolvent if necessary.
#define INP_ADD_LIT_TO_CURRENT_RESOLVENT( __lit ) { \
		inp_res[inp_resUsed][inp_resMemUsed[inp_resUsed]++] = __lit; \
		if (inp_resMemUsed[inp_resUsed] == inp_resMemAvail[inp_resUsed]) {\
			inp_resMemAvail[inp_resUsed] *= 2; \
			inp_res[inp_resUsed] = realloc(inp_res[inp_resUsed], sizeof(int32_t) * inp_resMemAvail[inp_resUsed]); \
		} \
}

//The following macro is used every time a new resolvent was generated. The __counter is just a for-loop variable. We must
//increase with a factor of 2 here otherwise we must rewrite the for loop initialization.
#define INP_INCREASE_NUM_RESOLVENTS( __counter ) {\
		if (++inp_resUsed == inp_resAvail) {\
			inp_resAvail *= 2; \
			inp_resMemAvail = realloc(inp_resMemAvail, sizeof(int32_t)*inp_resAvail); \
			inp_resMemUsed = realloc(inp_resMemUsed, sizeof(int32_t)*inp_resAvail); \
			inp_res = realloc(inp_res, sizeof(int32_t*)*inp_resAvail);\
			for (__counter = inp_resAvail>>1; __counter < inp_resAvail; ++__counter){\
				inp_res[ __counter ] = malloc(sizeof(int32_t)*BLOCKSIZE); \
				inp_resMemAvail[ __counter ] = BLOCKSIZE; \
				inp_resMemUsed[ __counter ] = 0; \
			}\
		} \
}

//The following macro is used to add a clause to the set G_posV. It increases the set if necessary. The set is never reduced.
#define INP_ADD_CLS_TO_GPOSV( __cPtr ) { \
		inp_G_posV[ inp_G_posVUsed++ ] = __cPtr; \
	if ( inp_G_posVUsed == inp_G_posVAvail ) { \
		inp_G_posVAvail *= 2; \
		inp_G_posV = realloc(inp_G_posV, sizeof(clause*)*inp_G_posVAvail); \
	} \
}

//The following macro is used to add a clause to the set G_negV. It increases the set if necessary. The set is never reduced.
#define INP_ADD_CLS_TO_GNEGV( __cPtr ) { \
		inp_G_negV[ inp_G_negVUsed++ ] = __cPtr; \
	if ( inp_G_negVUsed == inp_G_negVAvail ) { \
		inp_G_negVAvail *= 2; \
		inp_G_negV = realloc(inp_G_negV, sizeof(clause*)*inp_G_negVAvail); \
	} \
}

//The following macros are used to determine if G_negV or G_posV are still empty.
#define INP_IS_GNEGV_EMPTY() (!inp_G_negVUsed)
#define INP_IS_GPOSV_EMPTY() (!inp_G_posVUsed)

//The following macro is used to reset the gate definition arrays.
#define INP_RESET_GDEFARRAYS() inp_G_negVUsed = 0; inp_G_posVUsed = 0;

//The following macro increases the amount of ELS classes, if necessary.
#define INP_ENLARGE_ELS_CLASSES_IF_NECESSARY( __counter ) {\
	if (inp_els_classes_used == inp_els_classes_avail){\
		__counter = inp_els_classes_avail;\
		inp_els_classes_avail += BLOCKSIZE;\
		inp_els_classes = realloc(inp_els_classes, sizeof(literal**) * inp_els_classes_avail);\
		inp_els_classes_memUsed = realloc(inp_els_classes_memUsed, sizeof(uint32_t) * inp_els_classes_avail);\
		inp_els_classes_memAvail = realloc(inp_els_classes_memAvail, sizeof(uint32_t) * inp_els_classes_avail);\
		while (__counter < inp_els_classes_avail){\
			inp_els_classes[__counter] = malloc(sizeof(literal*) * BLOCKSIZE);\
			inp_els_classes_memAvail[__counter] = BLOCKSIZE;\
			inp_els_classes_memUsed[__counter] = 0;\
			++__counter;\
		}\
	}\
}

//The following macro adds a literal to the current equivalence class and enlarges its size, if necessary.
#define INP_ADD_LIT_TO_CURRENT_ELS_CLASS(__l) {\
	inp_els_classes[inp_els_classes_used][inp_els_classes_memUsed[inp_els_classes_used]++] = __l;\
	if (inp_els_classes_memUsed[inp_els_classes_used] == inp_els_classes_memAvail[inp_els_classes_used]){\
		inp_els_classes_memAvail[inp_els_classes_used] *= 2;\
		inp_els_classes[inp_els_classes_used] = realloc(inp_els_classes[inp_els_classes_used], \
				sizeof(literal*) * inp_els_classes_memAvail[inp_els_classes_used]);\
	}\
}

//Method exclusive to the phase (INTERN).
void inp_intern_reduceDB();					//Reduces the clause database based on the literals from inp_reduceFrom.
clause* inp_intern_upPropagate();			//Performs UP and updates queues and stacks for the in-processor.
void inp_intern_QSUpdateClauseDisabling(clause*);//Handle all the queue and stack updates if a clause is disabled.
void inp_intern_QSUpdateClauseEnabling(clause*);//Handle all the queue and stack updates if a clause is enabled.
void inp_intern_QSUpdateLiteralDeletion(clause*, int32_t);//Handle the queue/stack updates if a clauses literal is deleted.
void inp_intern_disableClause(clause*);		//To disable a clause.
void inp_intern_enableClause(clause*);		//To enable a clause.
void inp_intern_abandonClause(clause*);		//To just disable or delete a clause, depending on its type.
void inp_intern_deleteClause(clause*);		//A helper method to delete clauses during preprocessing. It updates the queue.
void inp_intern_deleteLitInClause(clause*, int32_t, int32_t);//A method used during preprocessing to get rid of literals.
void inp_intern_makeClauseRemoved(clause*, int32_t, int32_t);//To transfer clause to the set of removed.
void inp_intern_addOriginalClause(int32_t*, int32_t);//To add a new clause as original clause in (i.e. NIVER clauses).
void inp_intern_addLearnedClause(int32_t*, int32_t); //To add a new clause as learned clause.
void inp_intern_addLearnedBinaryClause(int32_t, int32_t);//To add a new clause as learned clause in (i.e. NHBR).
void inp_intern_addLearnedTernaryClause(int32_t, int32_t, int32_t);//To add a new clause as learned clause in (i.e. TERNARY).
void inp_intern_backJump(uint32_t, uint32_t);//performs a back-jump to the given level and might erase the level as well.
void inp_intern_disableVariable(variable*);	//To disable a variable.

//Methods PROVIDED by this phase (EXTERN).
void inp_extern_inprocessing();				//For executing the in-processing.

//Methods to initialize this phase (are called by the component that wants to execute the provided methods).
void inp_resetPhase();						//Reset the in-processing phase.
void inp_initPhase();						//For initializing it.
void inp_disposePhase();					//For disposing it.
#endif /* INP_H_ */
