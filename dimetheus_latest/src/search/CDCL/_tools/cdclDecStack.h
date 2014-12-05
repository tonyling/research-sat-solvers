/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CDCLDECSTACK_H_
#define CDCLDECSTACK_H_

#include "cdclTools.h"

uint32_t cdcl_decLevel;				//Gives the number of the current decision level.

cdclLiteral** cdcl_decStack;		//The decision stack is a one dimensional array of literal pointers. Each decision
									//level is preceded by a NULL. Each decision level is terminated by a NULL. The size is
									//2*f.ninitial+2.
cdclLiteral*** cdcl_decStackPtrs;	//Contains n+1 pointers, for each decision level one pointer that points to the beginnig
									//of the decision level in cdcl_decStack (directly behind the NULL of the previous level).

//The following macros are used to prepare decision levels for UP.
//Terminating a decision level means adding a NULL at the end of the decision level at position cdcl_up_end. Then, we
//move forward by one position and set another NULL. This null is the beginning of the next decision level.
#define CDCL_DECSTACK_PREPARE_NEW_LEVEL() { \
	*cdcl_up_end = NULL; \
	cdcl_decStackPtrs[++cdcl_decLevel] = cdcl_up_end; \
}

#define CDCL_DECSTACK_REVERT_TO_LEVEL( __dl ) ( cdcl_decLevel = __dl )

//The following macro is used to compute the number of assignments made -- including literals put into the stack but not
//propagated. This can be used to compute a trivial upper bound for the size of learned clauses in conflicts (in all
//conflict analysis schemes).
#define CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() ( ((uint32_t) (cdcl_up_end - cdcl_decStackPtrs[0])) - ((int32_t)cdcl_decLevel) + 1u )

#define CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() ( ((int32_t)(cdcl_decStackPtrs[1] - cdcl_decStackPtrs[0])) - 1 )

//The following macro computes the number of assignments currently made on the last decision level.
#define CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LAST_LEVEL() ( (uint32_t)( cdcl_up_current - cdcl_decStackPtrs[cdcl_decLevel] ) +1u )

#define CDCL_GET_DECLEVEL()	( cdcl_decLevel )

#define CDCL_GET_DECISION_ID_IN_LEVEL( __dl ) ( GET_CDCL_LIT_ID_FROM_PTR(cdcl_decStackPtrs[__dl][0]) )

void cdcl_decStack_backJump(uint32_t, uint32_t);	//Back-jumps to the given level in parameter one. Removes all assignments
												//and reasons. The second parameter controls if the target level is cleared
												//as well (for restarts in DL1).

void cdcl_decStack_prepare();		//To prepare the decision stack for the next call to the CDCL module.

void cdcl_decStack_reset();			//To reset the decision stack when the module is reset.
void cdcl_decStack_init();			//To initialize the decision stack when the module is initialized.
void cdcl_decStack_dispose();		//To dispose the decision stack when the module is disposed.

#ifdef VERBOSE_CDCL
void cdcl_decStack_printLevel(uint32_t);
void cdcl_decStack_printUpToLevel(uint32_t);
void cdcl_decStack_printLevelWithReasons(uint32_t);
void cdcl_decStack_printUpToLevelWithReasons(uint32_t);
void cdcl_decStack_printLevelWithClauses(uint32_t);
void cdcl_decStack_printUpToLevelWithClauses(uint32_t);

uint32_t cdcl_decStack_verifyDecLevelHasNoEquivs(uint32_t);
uint32_t cdcl_decStack_verifyDecLevelHasNoDuplicates(uint32_t);
uint32_t cdcl_decStack_verifyAll();
#endif

#endif /* CDCLDECSTACK_H_ */
