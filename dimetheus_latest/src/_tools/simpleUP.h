/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SIMPLEUP_H_
#define SIMPLEUP_H_

#include "tools.h"

//This implements simple unit propagation independent of a decision stack.
int32_t* main_simpleUPQueue;				//The queue that holds all literal pointers to literals that must be made true.
int32_t* main_simpleUPQueue_current;		//The current position to add something to the UP queue.

//The following macros are used to determine the UP status of a given literal.
#define IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE( __l )	( main_litData[ -__l ].upStamp == main_upStamp )
#define IS_LIT_IN_SIMPLEUPQUEUE( __l )			( main_litData[ __l ].upStamp == main_upStamp )

#define IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE_PTR( __lPtr )	( GET_LIT_OPPOSITE(__lPtr)->upStamp == main_upStamp )
#define IS_LIT_IN_SIMPLEUPQUEUE_PTR( __lPtr )			( __lPtr->upStamp == main_upStamp )

//The following macro is used to check if the UP queue contains any elements.
#define IS_SIMPLEUPQUEUE_EMPTY() ( main_simpleUPQueue == main_simpleUPQueue_current )

//The following macro is used to compute the number of elements found on the UP queue.
#define GET_NUMELT_IN_SIMPLEUPQUEUE() ( (uint32_t) (main_simpleUPQueue_current - main_simpleUPQueue) )

//The following macro is used to add a literal to the UP queue.
#define ADD_LIT_TO_SIMPLEUPQUEUE( __l ) {\
		main_litData[ __l ].upStamp = main_upStamp; \
		*(main_simpleUPQueue_current++) = __l; \
}

#define ADD_LIT_TO_SIMPLEUPQUEUE_PTR( __lPtr ) {\
		__lPtr->upStamp = main_upStamp; \
		*(main_simpleUPQueue_current++) = GET_LIT_NUM_FROM_PTR( __lPtr ); \
}

//The following macro is used to add a literal to the UP queue, but treat it as a decision without reason.
#define ADD_LIT_TO_SIMPLEUPQUEUE_AT_POS( __l , __pos ) {\
		main_litData[ __l ].upStamp = main_upStamp; \
		*(main_simpleUPQueue + (__pos) ) = __l; \
}

//The following macro will handle the reset after a up has been performed.
#define RESET_SIMPLEUPQUEUE() { \
		++main_upStamp; \
		main_simpleUPQueue_current = main_simpleUPQueue;\
}

//The following macro will handle the restore after a reset of the queue. Be careful: only the last unit propagation
//will be respected by this and only if the UP stamp has not been changed since. It will not re-assign the variables,
//you have to take care of this yourself if you need that too. It needs the number of values that had been
//propagated the last time.
#define RESTORE_SIMPLEUPQUEUE( __numProped ) { \
		--main_upStamp; \
		main_simpleUPQueue_current = main_simpleUPQueue + __numProped; \
}

//The following macro will unassign everything found on the UP queue, and as a matter of precaution, reduce the main_upStamp.
#define UNASSIGN_SIMPLEUPQUEUE( __vPtr , __upQ , __fromPos) { \
		__upQ = main_simpleUPQueue + __fromPos; \
		while (__upQ < main_simpleUPQueue_current){ \
			__vPtr = main_varData + abs(*(__upQ)); \
			SET_VAR_ASS_2(__vPtr); \
			--(main_litData + *(__upQ++))->upStamp; \
		} \
}

//The following macro is used to print the UP queue.
#define PRINT_SIMPLEUPQUEUE( __counter ) { \
		printf("c UPQ[%d]: ", (int32_t) (addr_t) (main_simpleUPQueue_current - main_simpleUPQueue));\
		for (__counter = 0; __counter < (int32_t) (addr_t) (main_simpleUPQueue_current - main_simpleUPQueue); ++__counter) {\
			printf("%d ", main_simpleUPQueue[__counter]);\
		} \
		printf("\n");\
}

void main_simpleUP_reset();		//For resetting the simple UP queue.
uint32_t main_simpleUP_init();	//For initializing it based on the f.n_initial and f.m_initial values. Returns 0 if problems.
void main_simpleUP_dispose();	//For disposing the simple UP queue.

clause* main_simpleUP_propagate_final();//This will make final assignments. It deletes the satisfied clauses and unsatisfied
					//literals it will also return a clause if a conflict arises (the conflicting clause). It resets the unit
					//propagation buffer itself.

clause* main_simpleUP_propagate_prep();//A special unit propagation that also performs preprocessing queue and stack updates.

clause* main_simpleUP_propagate_temporary(int32_t*, int32_t, int32_t, int32_t, clause*);
					//This will make a temporary unit propagation with all the literals found in the queue.
					//The 1. parameter points to an integer whose field will contain the number of propagated literals.
					//The 2. parameter controls whether assignments are cleaned. 0 only when UNSAT, 1 only when SAT, 2 always.
					//The 3. parameter controls from what position exactly we want to clean in case of cleaning is executed.
					//The 4. parameter controls if the literals are also stamped in their litStamp, not only the upStamp.
					//The 5. parameter tells the method to ignore a specific clause.

clause* main_simpleUP_propagate_ignore(clause*); //This method propagates what is found on the UP stack, but ignores the given
					//clause during the UP.

clause* main_simpleUP_propagate_ala(int32_t, clause*);
					//This will make a temporary unit propagation with all the literals found in the queue INVERTED.
					//The 1. parameter controls from what point in the UP queue we start the propagation.
					//The 2. parameter controls what clause to ignore during the propagation.

#endif /* SIMPLEUP_H_ */
