/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DECSTACK_H_
#define DECSTACK_H_

#include "tools.h"

uint32_t main_decLevel;

literal** main_decStack;		//The decision stack is a one dimensional array of literal pointers. Each decision
								//level is preceded by a NULL. Each decision level is terminated by a NULL. The size is
								//2*f.ninitial+2.
literal*** main_decStackPtrs;	//Contains n+1 pointers, for each decision level one pointer that points to the beginning
								//of the decision level in main_decStack (directly behind the NULL of the previous level).

//The following macros are used to prepare decision levels for UP.
//Terminating a decision level means adding a NULL at the end of the decision level at position main_complexUP_end. Then, we
//move forward by one position and set another NULL. This null is the beginning of the next decision level.
#define MAIN_DECSTACK_PREPARE_NEW_LEVEL() { \
	*main_complexUP_end = NULL; \
	main_decStackPtrs[++main_decLevel] = main_complexUP_end; \
}

#define MAIN_DECSTACK_REVERT_TO_LEVEL( __dl ) ( main_decLevel = __dl )

//The following macro is used to compute the number of assignments made -- including literals put into the stack but not
//propagated. This can be used to compute a trivial upper bound for the size of learned clauses in conflicts (in all
//conflict analysis schemes).
#define MAIN_DECSTACK_GET_NUM_ASSIGNMENTS() ( ((uint32_t) (main_complexUP_end - main_decStackPtrs[0])) - ((int32_t)main_decLevel) + 1u )

//Check on the amount of DL0 assignments.
#define MAIN_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() ( ((int32_t)(main_decStackPtrs[1] - main_decStackPtrs[0])) - 1 )


//The following macro computes the number of assignments currently made on the last decision level.
#define MAIN_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LAST_LEVEL() ( (uint32_t)( main_complexUP_current - main_decStackPtrs[main_decLevel] ) +1u )

void main_decStack_backJump(uint32_t, uint32_t, uint32_t);//Back-jumps to the given level in parameter one. Removes all assignments
												//and reasons. The second parameter controls if the target level is cleared
												//as well (for restarts in DL1). The third parameter tells it to re-enable the
												//disabled variables.

void main_decStack_reset();			//To reset the decision stack.
uint32_t main_decStack_init();		//To initialize the decision stack. Returns 0 if problem arise.
void main_decStack_dispose();		//To dispose the decision stack.

#ifdef VERBOSE
void main_decStack_printLevel(uint32_t);
void main_decStack_printUpToLevel(uint32_t);
void main_decStack_printLevelWithReasons(uint32_t);
void main_decStack_printUpToLevelWithReasons(uint32_t);
void main_decStack_printLevelWithClauses(uint32_t);
void main_decStack_printUpToLevelWithClauses(uint32_t);
#endif

#endif /* DECSTACK_H_ */
