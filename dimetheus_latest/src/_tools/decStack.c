/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "decStack.h"

void main_decStack_backJump(uint32_t toLevel, uint32_t clearTargetLevel, uint32_t reEnable){
	//This method back-jumps to the given level. It will reset the UP to append to this level and removes all assignments
	//and reasons above this level up to the current end-pointer of the UP.
	literal **clearTo = main_complexUP_end, **clearFrom;	//We clear the decision stack between clearFrom and clearTo.
	literal *l;
	variable *v;
	//We check if the target level is to be cleared as well. If 0, we will just append to the target level in next UP.
	if (clearTargetLevel){
		MAIN_COMPLEXUP_SET_DECLEVEL(toLevel);//We reset the UP pointers and the decision stack to override the level completely.
	} else {
		MAIN_COMPLEXUP_SET_DECLEVEL_APPEND(toLevel);//We reset the UP pointers and the decision stack to append to the target level.
	}
	//Depending on the above, the main_complexUP_current pointer now points to the beginning of a level or its end. No matter
	//what, everything from this position onwards must be cleared -- remove assignments, reasons and so on.
	clearFrom = main_complexUP_current;
	while (clearFrom < clearTo){
		l = *clearFrom;
		//We grabbed the element from the stack. We skip NULL values in between the decision levels we jump over.
		if (l != NULL){
			//If the literal had a reason (was no decision), we tell the clause that it is no reason anymore.
			v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
			if (GET_VAR_REASON(v) != NULL) SET_CLS_REASON_0(GET_VAR_REASON(v));
			SET_VAR_REASON(v, NULL);
			//Then, if the corresponding variable is assigned, we un-assign it.
			if (!IS_VAR_UNASSIGNED(v)){
				SET_VAR_ASS_2(v);
			}
			if (reEnable && IS_VAR_DISABLED(v)){
				variables_enable(v);
			}
		}
		++clearFrom;
	}
}

void main_decStack_reset(){
	main_decLevel = 0U;
	main_decStack = NULL;
	main_decStackPtrs = NULL;
}

uint32_t main_decStack_init(){
	uint32_t i;
	main_decStack = malloc(sizeof(literal*)*(2*f.n_initial + 4));
	if (main_decStack == NULL){
		printf("c ERROR. MAIN was unable to allocate memory for the decision stack. Out of memory?\n");
		return 0;
	}
	for (i = 0; i < 2*f.n_initial+4; ++i){
		main_decStack[i] = NULL;
	}

	main_decStackPtrs = malloc(sizeof(literal*)*(f.n_initial+4));
	if (main_decStackPtrs == NULL){
		printf("c ERROR. MAIN was unable to allocate memory for the decision stack pointers. Out of memory?\n");
		return 0;
	}
	main_decLevel = 1;
	//The first 4 values in the decision stack must be NULL. DL 0/1 are both empty and we need a preceding and following NULL.
	main_decStack[0] = NULL;
	main_decStack[1] = NULL;
	main_decStack[2] = NULL;
	main_decStack[3] = NULL;
	//We update the pointers to DL 0 and DL 1.
	main_decStackPtrs[0] = main_decStack + 1;	//We point to position 1 here because we need the preceding NULL.
	main_decStackPtrs[1] = main_decStack + 2;	//Since DL 0 contains only the NULL, DL 1 starts right behind it.

	return 1;
}

void main_decStack_dispose(){
	if (main_decStack != NULL){
		free(main_decStack);
		main_decStack = NULL;
	}
	if (main_decStackPtrs != NULL){
		free(main_decStackPtrs);
		main_decStackPtrs = NULL;
	}
	main_decLevel = 0U;
}

#ifdef VERBOSE
void main_decStack_printLevel(uint32_t level){
	literal *l;
	literal **main_stackFrame = main_decStackPtrs[level];
	printf("c     MAIN:     DL %6u (%p): ", level, (void*)main_stackFrame);
	while ((l = *main_stackFrame) != NULL && main_stackFrame < main_complexUP_end){
		printf("%d ", GET_LIT_NUM_FROM_PTR(l));
		++main_stackFrame;
	}
	printf("NULL\n");
}

void main_decStack_printUpToLevel(uint32_t toLevel){
	uint32_t i;
	printf("c     MAIN:   DECISION STACK %p, current DL %u:\n", (void*)main_decStack, main_decLevel);
	for (i=0; i < toLevel+1; ++i){
		main_decStack_printLevel(i);
	}

}

void main_decStack_printLevelWithReasons(uint32_t level){
	literal *l;
	variable *v;
	literal **main_stackFrame = main_decStackPtrs[level];
	printf("c     MAIN:     DL %6u (%p): ", level, (void*)main_stackFrame);
	while ((l = *main_stackFrame) != NULL && main_stackFrame < main_complexUP_end){
		v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
		printf("%d[%p] ", GET_LIT_NUM_FROM_PTR(l), (void*)GET_VAR_REASON(v));
		++main_stackFrame;
	}
	printf("NULL\n");
}

void main_decStack_printUpToLevelWithReasons(uint32_t toLevel){
	uint32_t i;
	printf("c     MAIN:   DECISION STACK WITH REASON POINTERS %p, current DL %u:\n", (void*)main_decStack, main_decLevel);
	for (i=0; i < toLevel+1; ++i){
		main_decStack_printLevelWithReasons(i);
	}

}

void main_decStack_printLevelWithClauses(uint32_t level){
	literal *l;
	variable *v;
	literal **main_stackFrame = main_decStackPtrs[level];
	printf("c     MAIN:     DL %6u (%p): ", level, (void*)main_stackFrame);
	while ((l = *main_stackFrame) != NULL && main_stackFrame < main_complexUP_end){
		v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
		printf("%d[", GET_LIT_NUM_FROM_PTR(l));
		if (GET_VAR_REASON(v) != NULL) {
			clauses_printClauseLiterals_inline(GET_VAR_REASON(v));
		}
		printf("] ");
		++main_stackFrame;
	}
	printf("NULL\n");
}

void main_decStack_printUpToLevelWithClauses(uint32_t toLevel){
	uint32_t i;
	printf("c     MAIN:   DECISION STACK WITH REASON CLAUSES %p, current DL %u:\n", (void*)main_decStack, main_decLevel);
	for (i=0; i < toLevel+1; ++i){
		main_decStack_printLevelWithClauses(i);
	}

}
#endif
