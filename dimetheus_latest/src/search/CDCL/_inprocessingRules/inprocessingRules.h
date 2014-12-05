/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef INPROCESSINGRULES_H_
#define INPROCESSINGRULES_H_

#include "../cdcl.h"

#define CDCL_INPROCESSINGRULE_MINVALUE 0
#define CDCL_INPROCESSINGRULE_MAXVALUE 3

#define CDCL_INPROCESSINGRULE_NULL 0	//To disable in-processing.
#include "ir_null.h"

#define CDCL_INPROCESSINGRULE_REDUCEONLY 1	//To only clean-up the formula.
#include "ir_reduceonly.h"

#define CDCL_INPROCESSINGRULE_SIMPLE 2	//To clean-up the formula and perform simple in-processing.
#include "ir_simple.h"

#define CDCL_INPROCESSINGRULE_FULL 3	//To clean-up the formula and perform full in-processing.
#include "ir_full.h"

#define CDCL_INPROCESSINGRULE_TESTING 999	//To clean-up the formula. Anything else is subject to frequent changes.
#include "ir_testing.h"

//In-Processing Rule plug-in internal data-structures.
cdclLiteral** cdcl_inprocessingRules_reduceFrom;		//Check DL0 from this point onwards and use the assignments to
														//reduce the clause database (remove satisfied clauses and the
														//unsatisfied literals).

uint32_t cdcl_inprocessingRules_lastConflict;			//In what conflict number did we do the last in-processing.

int32_t** cdcl_inprocessingRules_res;					//The set of resolvents, mostly used by NIVER.
int32_t cdcl_inprocessingRules_resAvail, cdcl_inprocessingRules_resUsed;//How many such resolvents have been initialized/used?
int32_t *cdcl_inprocessingRules_resMemAvail;			//How much memory is allocated for a specific resolvent?
int32_t *cdcl_inprocessingRules_resMemUsed;				//How much memory is in use (size)?

cdclClause** cdcl_inprocessingRules_G_negV;				//During GE, this is the set of gate defining clauses containing +v.
int32_t cdcl_inprocessingRules_G_negVAvail;				//The amount of memory positions allocated in G_negV.
int32_t cdcl_inprocessingRules_G_negVUsed;				//The amount of memory used.
cdclClause** cdcl_inprocessingRules_G_posV;				//During GE, this is the set of gate defining clauses containing -v.
int32_t cdcl_inprocessingRules_G_posVAvail;				//The amount of memory positions allocated in G_posV.
int32_t cdcl_inprocessingRules_G_posVUsed;				//The amount of memory used.

cdclLiteral** cdcl_inprocessingRules_unhiding_SPlus;	//The SPlus array for UNHIDING.
cdclLiteral** cdcl_inprocessingRules_unhiding_SMinus;	//The SMinus array for UNHIDING.
cdclLiteral** cdcl_inprocessingRules_unhiding_lStack;	//The literal stack used by UNHIDING.
cdclLiteral** cdcl_inprocessingRules_unhiding_lStack_current;//The literal stack used by UNHIDING.

cdclLiteral** cdcl_inprocessingRules_litPerm;			//An array that can be used to create literal permutations.

cdclLiteral*** cdcl_inprocessingRules_els_classes;		//Equivalence classes of literals found during ELS.
uint32_t cdcl_inprocessingRules_els_classes_avail;		//The number of ELS classes that have been initialized.
uint32_t cdcl_inprocessingRules_els_classes_used;		//The current number of ELS classes used.
uint32_t* cdcl_inprocessingRules_els_classes_memAvail;	//The size of each used class.
uint32_t* cdcl_inprocessingRules_els_classes_memUsed;	//The amount of elements for each class.
cdclLiteral** cdcl_inprocessingRules_els_lStack;		//The literal stack used by Equivalent Literal Substitution.
cdclLiteral** cdcl_inprocessingRules_els_lStack_current;//The literal stack current position for a new element.

cdclLiteral** cdcl_inprocessingRules_trl_lQ;			//The literal queue for the tree-look - no priority queue.
cdclLiteral** cdcl_inprocessingRules_trl_lQ_c;			//The current pointer pointing to the next element to dequeue.
cdclLiteral** cdcl_inprocessingRules_trl_lQ_e;			//The end pointer pointing to the next position for queuing in.
cdclLiteral** cdcl_inprocessingRules_trl_faileds;		//The set of literals that have been discovered as failed.

//The following macro is used to reset the resolvents.
#define CDCL_IR_RESET_ALL_RESOLVENTS()	( cdcl_inprocessingRules_resUsed = 0 )

//The following macro is used to reset a specific resolvent before using it anew for resolution.
#define CDCL_IR_RESET_RESOLVENT( __resNum ) {\
		cdcl_inprocessingRules_resMemUsed[ __resNum ] = 0;\
}

//The following macro is used to add another literal to the current resolvent. It increases the resolvent if necessary.
#define CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT( __lit ) { \
		cdcl_inprocessingRules_res[cdcl_inprocessingRules_resUsed][cdcl_inprocessingRules_resMemUsed[cdcl_inprocessingRules_resUsed]++] = __lit; \
		if (cdcl_inprocessingRules_resMemUsed[cdcl_inprocessingRules_resUsed] == cdcl_inprocessingRules_resMemAvail[cdcl_inprocessingRules_resUsed]) {\
			cdcl_inprocessingRules_resMemAvail[cdcl_inprocessingRules_resUsed] *= 2; \
			cdcl_inprocessingRules_res[cdcl_inprocessingRules_resUsed] = realloc(cdcl_inprocessingRules_res[cdcl_inprocessingRules_resUsed], sizeof(int32_t) * cdcl_inprocessingRules_resMemAvail[cdcl_inprocessingRules_resUsed]); \
		} \
}

//The following macro is used every time a new resolvent was generated. The __counter is just a for-loop variable. We must
//increase with a factor of 2 here otherwise we must rewrite the for loop initialization.
#define CDCL_IR_INCREASE_NUM_RESOLVENTS( __counter ) {\
		if (++cdcl_inprocessingRules_resUsed == cdcl_inprocessingRules_resAvail) {\
			cdcl_inprocessingRules_resAvail *= 2; \
			cdcl_inprocessingRules_resMemAvail = realloc(cdcl_inprocessingRules_resMemAvail, sizeof(int32_t)*cdcl_inprocessingRules_resAvail); \
			cdcl_inprocessingRules_resMemUsed = realloc(cdcl_inprocessingRules_resMemUsed, sizeof(int32_t)*cdcl_inprocessingRules_resAvail); \
			cdcl_inprocessingRules_res = realloc(cdcl_inprocessingRules_res, sizeof(int32_t*)*cdcl_inprocessingRules_resAvail);\
			for (__counter = cdcl_inprocessingRules_resAvail>>1; __counter < cdcl_inprocessingRules_resAvail; ++__counter){\
				cdcl_inprocessingRules_res[ __counter ] = malloc(sizeof(int32_t)*BLOCKSIZE); \
				cdcl_inprocessingRules_resMemAvail[ __counter ] = BLOCKSIZE; \
				cdcl_inprocessingRules_resMemUsed[ __counter ] = 0; \
			}\
		} \
}

//The following macro is used to add a clause to the set G_posV. It increases the set if necessary. The set is never reduced.
#define CDCL_IR_ADD_CLS_TO_GPOSV( __cPtr ) { \
		cdcl_inprocessingRules_G_posV[ cdcl_inprocessingRules_G_posVUsed++ ] = __cPtr; \
	if ( cdcl_inprocessingRules_G_posVUsed == cdcl_inprocessingRules_G_posVAvail ) { \
		cdcl_inprocessingRules_G_posVAvail *= 2; \
		cdcl_inprocessingRules_G_posV = realloc(cdcl_inprocessingRules_G_posV, sizeof(clause*)*cdcl_inprocessingRules_G_posVAvail); \
	} \
}

//The following macro is used to add a clause to the set G_negV. It increases the set if necessary. The set is never reduced.
#define CDCL_IR_ADD_CLS_TO_GNEGV( __cPtr ) { \
		cdcl_inprocessingRules_G_negV[ cdcl_inprocessingRules_G_negVUsed++ ] = __cPtr; \
	if ( cdcl_inprocessingRules_G_negVUsed == cdcl_inprocessingRules_G_negVAvail ) { \
		cdcl_inprocessingRules_G_negVAvail *= 2; \
		cdcl_inprocessingRules_G_negV = realloc(cdcl_inprocessingRules_G_negV, sizeof(clause*)*cdcl_inprocessingRules_G_negVAvail); \
	} \
}

//The following macros are used to determine if G_negV or G_posV are still empty.
#define CDCL_IR_IS_GNEGV_EMPTY() (!cdcl_inprocessingRules_G_negVUsed)
#define CDCL_IR_IS_GPOSV_EMPTY() (!cdcl_inprocessingRules_G_posVUsed)

//The following macro is used to reset the gate definition arrays.
#define CDCL_IR_RESET_GDEFARRAYS() cdcl_inprocessingRules_G_negVUsed = 0; cdcl_inprocessingRules_G_posVUsed = 0;

//The following macro enlarges the amount of ELS classes available, if necessary.
#define CDCL_IR_ENLARGE_ELS_CLASSES_IF_NECESSARY( __counter ) {\
		if (cdcl_inprocessingRules_els_classes_used == cdcl_inprocessingRules_els_classes_avail){\
			__counter = cdcl_inprocessingRules_els_classes_avail;\
			cdcl_inprocessingRules_els_classes_avail += BLOCKSIZE;\
			cdcl_inprocessingRules_els_classes 			= realloc(cdcl_inprocessingRules_els_classes, \
					sizeof(literal**) * cdcl_inprocessingRules_els_classes_avail);\
			cdcl_inprocessingRules_els_classes_memUsed 	= realloc(cdcl_inprocessingRules_els_classes_memUsed, \
					sizeof(uint32_t) * cdcl_inprocessingRules_els_classes_avail);\
			cdcl_inprocessingRules_els_classes_memAvail = realloc(cdcl_inprocessingRules_els_classes_memAvail, \
					sizeof(uint32_t) * cdcl_inprocessingRules_els_classes_avail);\
			while (__counter < cdcl_inprocessingRules_els_classes_avail){\
				cdcl_inprocessingRules_els_classes[__counter] = malloc(sizeof(literal*) * BLOCKSIZE);\
				cdcl_inprocessingRules_els_classes_memAvail[__counter] = BLOCKSIZE;\
				cdcl_inprocessingRules_els_classes_memUsed[__counter] = 0;\
				++__counter;\
			}\
		}\
}

//The following macro adds a literal to the current equivalence class and enlarges its size, if necessary.
#define CDCL_IR_ADD_LIT_TO_CURRENT_ELS_CLASS(__l) {\
		cdcl_inprocessingRules_els_classes[\
			cdcl_inprocessingRules_els_classes_used\
			][cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used]++] = __l;\
		if (cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used] \
				== cdcl_inprocessingRules_els_classes_memAvail[cdcl_inprocessingRules_els_classes_used]){\
			cdcl_inprocessingRules_els_classes_memAvail[cdcl_inprocessingRules_els_classes_used] *= 2;\
			cdcl_inprocessingRules_els_classes[cdcl_inprocessingRules_els_classes_used] \
				= realloc(cdcl_inprocessingRules_els_classes[cdcl_inprocessingRules_els_classes_used], \
					sizeof(cdclLiteral*) * cdcl_inprocessingRules_els_classes_memAvail[cdcl_inprocessingRules_els_classes_used]);\
		}\
}

//In-Processing intern methods -- can only be used by all the In-Processing rules.
void cdcl_inprocessingRules_intern_reduceDB();//This cleans up the formula with the DL0 assignments.
void cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(cdclClause*);//Queue and stack updates.
void cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(cdclClause*);//Queue and stack updates.
void cdcl_inprocessingRules_intern_QSUpdateLiteralDeletion(cdclClause*, int32_t);//Queue and stack updates.
cdclVariable* cdcl_inprocessingRules_intern_getRootRepresentative(cdclVariable*);//Gets the equivalence root.
cdclClause* cdcl_inprocessingRules_upPropagate();//Performs UP and updates queues and stacks for the in-processor.
void cdcl_inprocessingRules_intern_disableClause(cdclClause*);//This is used to temporarily disable a clause.
void cdcl_inprocessingRules_intern_enableClause(cdclClause*);//This is used to re-enable a clause.
void cdcl_inprocessingRules_intern_makeClauseRemoved(cdclClause*, int32_t, uint32_t);//Makes a clause removed.
void cdcl_inprocessingRules_intern_addLearnedBinaryClause(int32_t,int32_t);//Adds a new binary as learned clause.
void cdcl_inprocessingRules_intern_addLearnedTernaryClause(int32_t,int32_t,int32_t);//Adds a new ternary as learned clause.
void cdcl_inprocessingRules_intern_addLearnedQuadriaryClause(int32_t,int32_t,int32_t,int32_t);//Adds a new ternary as learned clause.
void cdcl_inprocessingRules_intern_addOriginalClause(int32_t*,int32_t);//Adds a clause as original with the given size.
void cdcl_inprocessingRules_intern_deleteClause(cdclClause*);//This completely removes a clause.
void cdcl_inprocessingRules_intern_disableVariable(cdclVariable*);//To disable a variable.
void cdcl_inprocessingRules_intern_se();//Performs SE with all the clauses in the clause queue.
cdclClause* cdcl_inprocessingRules_intern_str();//Performs STR with all the clauses in the clause stack.
cdclClause* cdcl_inprocessingRules_intern_ple();//This method performs pure literal elimination.
cdclClause* cdcl_inprocessingRules_intern_fld();//This method performs failed literal detection.
void cdcl_inprocessingRules_intern_unhiding_uhle(cdclClause*);//Performs UHLE for the given clause.
uint32_t cdcl_inprocessingRules_intern_unhiding_uhte(cdclClause*);//Checks if the clause is an UHTE.
uint32_t cdcl_inprocessingRules_intern_unhiding_stamp(cdclLiteral*, uint32_t, cdclClause**);
cdclClause* cdcl_inprocessingRules_intern_unhiding(uint32_t);//This method performs UNHIDING for a given number of rounds.
void cdcl_inprocessingRules_backJump(uint32_t, uint32_t);//Special back-jump method that does no enabling or disabling.
cdclClause* cdcl_inprocessingRules_intern_nhbr_detect(cdclLiteral*);//Performs unit propagation prioritizing binary clauses.
cdclClause* cdcl_inprocessingRules_intern_nhbr();//Performs non-transitive hyper binary resolution using unit propagation.
void cdcl_inprocessingRules_intern_ternary();//Performs ternary resolution and adds only new and not-subsumed clauses.
cdclClause* cdcl_inprocessingRules_intern_els_replaceLit(cdclLiteral*, cdclLiteral*);//Replaces a literal.
uint32_t cdcl_inprocessingRules_intern_els_strongConnect(cdclLiteral*, uint32_t);//Creates the strongly connected components.
cdclClause* cdcl_inprocessingRules_intern_els();//Performs equivalent literal substitution.
void cdcl_inprocessingRules_intern_treelook_enqueue(cdclLiteral*);//Enqueues a literal in the queue.
cdclClause* cdcl_inprocessingRules_intern_treelook();//Tree based look-ahead including NHBR and ELS.
cdclClause* cdcl_inprocessingRules_intern_niver();//This method performs NIVER.
cdclClause* cdcl_inprocessingRules_intern_ge_extract(cdclVariable*);//To extract the given variable along with the current GDEF.
cdclClause* cdcl_inprocessingRules_intern_ge_AND(cdclVariable*);//Performs AND GE on the given variable.
cdclClause* cdcl_inprocessingRules_intern_ge_OR(cdclVariable*);//Performs OR GE on the given variable.
cdclClause* cdcl_inprocessingRules_intern_ge_SIM3(cdclVariable*);//Performs SIM GE on the given variable for gates of size 3.
cdclClause* cdcl_inprocessingRules_intern_ge_SIM4(cdclVariable*);//Performs SIM GE on the given variable for gates of size 4.
cdclClause* cdcl_inprocessingRules_intern_ge_XOR(cdclVariable*);//Performs XOR GE on the given variable.
cdclClause* cdcl_inprocessingRules_intern_ge();//This method performs gate extraction.

static inline void cdcl_inprocessingRules_printHelp(){
	cdcl_inprocessingRule_null_printHelp();
	cdcl_inprocessingRule_reduceonly_printHelp();
	cdcl_inprocessingRule_simple_printHelp();
	cdcl_inprocessingRule_testing_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_inprocessingRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-INPROCESSING-RULES: CDCL_INPROCESSINGRULE_NULL %d\n", prefix, CDCL_INPROCESSINGRULE_NULL);
	printf("%s CDCL-INPROCESSING-RULES: CDCL_INPROCESSINGRULE_REDUCEONLY %d\n", prefix, CDCL_INPROCESSINGRULE_REDUCEONLY);
	printf("%s CDCL-INPROCESSING-RULES: CDCL_INPROCESSINGRULE_SIMPLE %d\n", prefix, CDCL_INPROCESSINGRULE_SIMPLE);
	printf("%s CDCL-INPROCESSING-RULES: CDCL_INPROCESSINGRULE_FULL %d\n", prefix, CDCL_INPROCESSINGRULE_FULL);
}
void (*cdcl_inprocessingRule_prepare)();//This points to one of the in-processing rules call preparation methods available.
cdclClause* (*cdcl_inprocessingRule_inprocess)();//This points to one of the in-processing rule implementations
										//available. The method assumes that we are currently in DL1 and a decision would be
										//made now (DL1 is empty). The method returns a clause pointer if a conflict occurred
										//or NULL if no conflict occurred.

void cdcl_extern_inprocessingRules_switch(int32_t);//Use to switch the rule without re-initializing the whole CDCL module.
void cdcl_inprocessingRules_extern_reconstructSolution();//This method is called if the result is SAT.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_inprocessingRules_resetPlugin();
void cdcl_inprocessingRules_initPlugin();
void cdcl_inprocessingRules_disposePlugin();

#endif /* INPROCESSINGRULES_H_ */
