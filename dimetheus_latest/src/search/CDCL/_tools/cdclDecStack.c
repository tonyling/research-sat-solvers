/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "cdclDecStack.h"

void cdcl_decStack_backJump(uint32_t toLevel, uint32_t clearTargetLevel){
	//This method back-jumps to the given level. It will reset the UP to append to this level and removes all assignments
	//and reasons above this level up to the current end-pointer of the UP.
	cdclLiteral **clearTo = cdcl_up_end, **clearFrom;	//We clear the decision stack between clearFrom and clearTo.
	cdclLiteral *l;
	cdclVariable *v;

	//We check if the target level is to be cleared as well. If 0, we will just append to the target level in next UP.
	if (clearTargetLevel){
		CDCL_UP_SET_DECLEVEL(toLevel);//We reset the UP pointers and the decision stack to override the level completely.
	} else {
		CDCL_UP_SET_DECLEVEL_APPEND(toLevel);//We reset the UP pointers and the decision stack to append to the target level.
	}
	//Depending on the above, the cdcl_up_current pointer now points to the beginning of a level or its end. No matter what,
	//everything from this position onwards must be cleared -- remove assignments, reasons and so on.
	clearFrom = cdcl_up_current;

	while (clearFrom < clearTo){
		l = *clearFrom;
		//We grabbed the element from the stack. We skip NULL values in between the decision levels we jump over.
		if (l != NULL){
			//If the literal had a reason (was no decision), we tell the clause that it is no reason anymore.
			v = cdcl_variables + GET_CDCL_VAR_ID_FROM_LIT_PTR(l);
			if (GET_CDCL_VAR_PTR_REASON(v) != NULL) SET_CDCL_CLS_REASON_0(GET_CDCL_VAR_PTR_REASON(v));
			SET_CDCL_VAR_PTR_REASON(v, NULL);
			//Then, if the corresponding variable is assigned, we re-enable it and remove its assignment. If the variable
			//is not assigned, we do not need to re-enable it as it must still be enabled. This implies that it is still
			//not assigned.
			if (!IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				cdcl_restartRule_incAgility(v);				//Increases the variable agility.
				cdcl_selectDirRule_save(v);					//Save the variable phase.
				SET_CDCL_VAR_PTR_ASS_2(v);					//Take back the variable assignment.
				cdcl_variables_Q_enqueue(v);				//Putting the variable back into the variable queue.
			}
		}
		++clearFrom;
	}

	#ifdef COLLECTSTATS
	++stats_cdcl_numBackjumps;
	#endif

}

void cdcl_decStack_prepare(){
	//Right after calling the CDCL, we are in decision level 1 and the stack pointers are initialized for DL 0 and DL 1.
	cdcl_decLevel = 1;
	//The first 4 values in the decision stack must be NULL. DL 0/1 are both empty and we need a preceding and following NULL.
	cdcl_decStack[0] = NULL;
	cdcl_decStack[1] = NULL;
	cdcl_decStack[2] = NULL;
	cdcl_decStack[3] = NULL;
	//We update the pointers to DL 0 and DL 1.
	cdcl_decStackPtrs[0] = cdcl_decStack + 1;	//We point to position 1 here because we need the preceding NULL.
	cdcl_decStackPtrs[1] = cdcl_decStack + 2;	//Since DL 0 contains only the NULL, DL 1 starts right behind it.
}

void cdcl_decStack_reset(){
	cdcl_decLevel = 0U;
	cdcl_decStack = NULL;
	cdcl_decStackPtrs = NULL;
}

void cdcl_decStack_init(){
	uint32_t i;
	cdcl_decStack = malloc(sizeof(cdclLiteral*)*(2*f.n_initial + 4));
	if (cdcl_decStack == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the decision stack. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 0; i < 2*f.n_initial+4; ++i){
		cdcl_decStack[i] = NULL;
	}

	cdcl_decStackPtrs = malloc(sizeof(cdclLiteral*)*(f.n_initial+4));
	if (cdcl_decStackPtrs == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the decision stack pointers. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	//We only initialize the stack pointers for decision level zero and one.
	cdcl_decLevel = 1;
	cdcl_decStackPtrs[0] = cdcl_decStack + 1;	//We point to position 1 here because we need the preceding NULL.
	cdcl_decStackPtrs[1] = cdcl_decStack + 2;	//Since DL 0 contains only the NULL, DL 1 starts right behind it.
}

void cdcl_decStack_dispose(){
	if (cdcl_decStack != NULL){
		free(cdcl_decStack);
		cdcl_decStack = NULL;
	}
	if (cdcl_decStackPtrs != NULL){
		free(cdcl_decStackPtrs);
		cdcl_decStackPtrs = NULL;
	}
	cdcl_decLevel = 0U;
}

#ifdef VERBOSE_CDCL
void cdcl_decStack_printLevel(uint32_t level){
	cdclLiteral *l;
	cdclLiteral **cdcl_stackFrame = cdcl_decStackPtrs[level];
	printf("c     CDCL:     DL %6u (%p): ", level, (void*)cdcl_stackFrame);
	while ((l = *cdcl_stackFrame) != NULL && cdcl_stackFrame < cdcl_up_end){
		printf("%d ", GET_CDCL_LIT_ID_FROM_PTR(l));
		++cdcl_stackFrame;
	}
	printf("NULL\n");
}

void cdcl_decStack_printUpToLevel(uint32_t toLevel){
	uint32_t i;
	printf("c     CDCL:   DECISION STACK %p, current DL %u:\n", (void*)cdcl_decStack, cdcl_decLevel);
	for (i=0; i < toLevel+1; ++i){
		cdcl_decStack_printLevel(i);
	}

}

void cdcl_decStack_printLevelWithReasons(uint32_t level){
	cdclLiteral *l;
	cdclVariable *v;
	cdclLiteral **cdcl_stackFrame = cdcl_decStackPtrs[level];
	printf("c     CDCL:     DL %6u (%p): ", level, (void*)cdcl_stackFrame);
	while ((l = *cdcl_stackFrame) != NULL && cdcl_stackFrame < cdcl_up_end){
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
		printf("%d[%p] ", GET_CDCL_LIT_ID_FROM_PTR(l), (void*)GET_CDCL_VAR_PTR_REASON(v));
		++cdcl_stackFrame;
	}
	printf("NULL\n");
}

void cdcl_decStack_printUpToLevelWithReasons(uint32_t toLevel){
	uint32_t i;
	printf("c     CDCL:   DECISION STACK WITH REASON POINTERS %p, current DL %u:\n", (void*)cdcl_decStack, cdcl_decLevel);
	for (i=0; i < toLevel+1; ++i){
		cdcl_decStack_printLevelWithReasons(i);
	}

}

void cdcl_decStack_printLevelWithClauses(uint32_t level){
	cdclLiteral *l;
	cdclVariable *v;
	cdclLiteral **cdcl_stackFrame = cdcl_decStackPtrs[level];
	printf("c     CDCL:     DL %6u (%p): ", level, (void*)cdcl_stackFrame);
	while ((l = *cdcl_stackFrame) != NULL && cdcl_stackFrame < cdcl_up_end){
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
		printf("%d[", GET_CDCL_LIT_ID_FROM_PTR(l));
		if (GET_CDCL_VAR_PTR_REASON(v) != NULL) {
			cdcl_clauses_printClauseLiterals_inline(GET_CDCL_VAR_PTR_REASON(v));
		}
		printf("] ");
		++cdcl_stackFrame;
	}
	printf("NULL\n");
}

void cdcl_decStack_printUpToLevelWithClauses(uint32_t toLevel){
	uint32_t i;
	printf("c     CDCL:   DECISION STACK WITH REASON CLAUSES %p, current DL %u:\n", (void*)cdcl_decStack, cdcl_decLevel);
	for (i=0; i < toLevel+1; ++i){
		cdcl_decStack_printLevelWithClauses(i);
	}

}

uint32_t cdcl_decStack_verifyDecLevelHasNoEquivs(uint32_t level){
	cdclLiteral *l;
	cdclLiteral **cdcl_stackFrame = cdcl_decStackPtrs[level];
	cdclVariable *v;

	while ((l = *cdcl_stackFrame) != NULL && cdcl_stackFrame < cdcl_up_end){
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
		if (GET_CDCL_VAR_PTR_ISEQUIVTO(v) != 0 || GET_VAR_ISEQUIVTO((main_varData + v->id)) != 0){
			printf("\nEquivalent variable found in stack. This is not allowed.\n");
			cdcl_variables_printVariable(v);
			return 0;
		}
		++cdcl_stackFrame;
	}

	return 1;
}

uint32_t cdcl_decStack_verifyDecLevelHasNoDuplicates(uint32_t level){
	cdclLiteral *l;
	cdclLiteral **cdcl_stackFrame = cdcl_decStackPtrs[level];
	cdclVariable *v;

	while ((l = *cdcl_stackFrame) != NULL && cdcl_stackFrame < cdcl_up_end){
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
		if (GET_CDCL_VAR_PTR_STAMP(v) == cdcl_litStamp){
			printf("\nVariable found at least twice in stack. This is not allowed.\n");
			cdcl_variables_printVariable(v);
			return 0;
		} else {
			SET_CDCL_VAR_PTR_STAMP(v, cdcl_litStamp);
		}
		++cdcl_stackFrame;
	}

	return 1;
}

uint32_t cdcl_decStack_verifyAll(){
	uint32_t i;
	for (i = 0; i < cdcl_decLevel+1; ++i){
		if (!cdcl_decStack_verifyDecLevelHasNoEquivs(i)){
			return 0;
		}
	}

	++cdcl_litStamp;
	for (i = 0; i < cdcl_decLevel+1; ++i){
		if (!cdcl_decStack_verifyDecLevelHasNoDuplicates(i)){
			return 0;
		}
	}

	return 1;
}
#endif
