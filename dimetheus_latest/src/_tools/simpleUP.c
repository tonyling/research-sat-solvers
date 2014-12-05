/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "simpleUP.h"

void main_simpleUP_reset(){
	main_simpleUPQueue = NULL;
	main_simpleUPQueue_current = NULL;
	main_upStamp = 1;
}

uint32_t main_simpleUP_init(){
	main_simpleUPQueue = malloc(sizeof(int32_t) * (2*f.n_initial+2));
	main_simpleUPQueue_current = main_simpleUPQueue;	//Make the addition pointer point to the beginning of the queue.
	if (main_simpleUPQueue == NULL){
		printf("c ERROR. Failed to initialize the simple UP queue. Out of memory?\n");
		return 0;
	}
	return 1;
}

void main_simpleUP_dispose(){
	if (main_simpleUPQueue != NULL){
		free(main_simpleUPQueue);
		main_simpleUPQueue = NULL;
		main_simpleUPQueue_current = NULL;
	}
}

clause* main_simpleUP_propagate_final(){
	int32_t* upQ;
	literal* lPtr;
	variable* v;
	clause* c;
	int32_t upLit, lit, litPos, i, offset;

	//First of all, we will propagate as much as possible.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		upLit =*(upQ++);		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + upLit;	//Satisfied literal.
		v = main_varData + abs(upLit);
		if (upLit > 0){
			SET_VAR_ASS_1( v );
		} else {
			SET_VAR_ASS_0( v );
		}
		//We disable the variable as it is now finally assigned.
		variables_disable(v);

		//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < lPtr->numImpUsed; ++i){
			lit = (int32_t) (addr_t) lPtr->implies[i++];
			if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return lPtr->implies[i];
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		lPtr = main_litData - upLit;	//Unsatisfied literal.
		//Walk through the watcher lists of lPtr (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the UP queue as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (lPtr->numw1Used > offset){
			c = lPtr->w1[offset];
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW1;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w1 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w2;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		//WL2
		offset = 0;
		while (lPtr->numw2Used > offset){
			c = lPtr->w2[offset];
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW2;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w2 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w1;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}
	}

	//We must now finalize our assignments. We will first walk through the queue again and delete all learned clauses that are
	//satisfied. The original ones get disabled.
	//We will then walk through the queue again and remove the falsified literals from all their occurrences.
	//Delete satisfied clauses or disable them.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		lit = *(upQ++);			//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + lit;	//Satisfied literal.
		while (lPtr->numOccsUsed > 0){
			c = lPtr->occs[0];
			//Depending on the type of clause, we either delete it (learned clause) or we disable it (original clause).
			if (IS_CLS_LEARNED( c )) {clauses_deleteCls( c );} else {clauses_disable( c );}
		}
	}

	//Remove falsified literals. This could generate new binary clauses, which must then be removed from the watcher lists and
	//added into the respective implication lists of the variables.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		lit = *(upQ++); 		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData - lit;	//Unsatisfied literal.
		lit = -lit;
		while (lPtr->numOccsUsed > 0){
			c = lPtr->occs[0];
			litPos = lPtr->occLitPos[0];
			//We delete the literal found at position litPos in the clause. We do not care if it is an original clause or not.
			//Since all satisfied clauses have been deleted or disabled, no binary clause with a falsified literal can remain
			//in the occurrence lists. Therefore, we must not be afraid that we accidentally hand over a binary clause here.
			clauses_deleteLit(c, lit, litPos);
		}
	}

	RESET_SIMPLEUPQUEUE();
	return NULL;
}

clause* main_simpleUP_propagate_prep(){
	int32_t* upQ;
	literal* lPtr;
	variable* v;
	clause* c;
	int32_t upLit, lit, litPos, i, offset;

	//First of all, we will propagate as much as possible.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		upLit =*(upQ++);		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + upLit;	//Satisfied literal.
		v = main_varData + abs(upLit);
		if (upLit > 0){
			SET_VAR_ASS_1( v );
		} else {
			SET_VAR_ASS_0( v );
		}
		//We disable the variable as it is now finally assigned.
		prep_intern_disableVariable(v);

		//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < lPtr->numImpUsed; ++i){
			lit = (int32_t) (addr_t) lPtr->implies[i++];
			if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return lPtr->implies[i];
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		lPtr = main_litData - upLit;	//Unsatisfied literal.
		//Walk through the watcher lists of lPtr (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the UP queue as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (lPtr->numw1Used > offset){
			c = lPtr->w1[offset];
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW1;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w1 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w2;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		//WL2
		offset = 0;
		while (lPtr->numw2Used > offset){
			c = lPtr->w2[offset];
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW2;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w2 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w1;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}
	}

	//We must now finalize our assignments. We will first walk through the queue again and delete all learned clauses that are
	//satisfied. The original ones get disabled.
	//We will then walk through the queue again and remove the falsified literals from all their occurrences.
	//Delete satisfied clauses or disable them.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		lit = *(upQ++);			//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + lit;	//Satisfied literal.
		while (lPtr->numOccsUsed > 0){
			c = lPtr->occs[0];
			//Depending on the type of clause, we either delete it (learned clause) or we disable it (original clause).
			prep_intern_abandonClause(c);
		}
	}

	//Remove falsified literals. This could generate new binary clauses, which must then be removed from the watcher lists and
	//added into the respective implication lists of the variables.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		lit = *(upQ++); 		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData - lit;	//Unsatisfied literal.
		lit = -lit;
		while (lPtr->numOccsUsed > 0){
			c = lPtr->occs[0];
			litPos = lPtr->occLitPos[0];
			//We delete the literal found at position litPos in the clause. We do not care if it is an original clause or not.
			//Since all satisfied clauses have been deleted or disabled, no binary clause with a falsified literal can remain
			//in the occurrence lists. Therefore, we must not be afraid that we accidentally hand over a binary clause here.
			prep_intern_deleteLitInClause(c, lit, litPos);
		}
	}

	RESET_SIMPLEUPQUEUE();
	return NULL;
}

clause* main_simpleUP_propagate_temporary(int32_t* numPropagated, int32_t clean, int32_t cleanFrom, int32_t stampLits, clause* toIgnore){
	int32_t* upQ;
	literal* lPtr;
	variable* v;
	clause* c;
	int32_t upLit, lit, litPos, i, offset;

	//First of all, we will propagate as much as possible. We start the propagation from the position we would start the
	//cleaning operation from (that is what we consider "new" in this propagation).
	upQ = main_simpleUPQueue + cleanFrom;
	while (upQ < main_simpleUPQueue_current){
		upLit =*(upQ++);		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + upLit;	//Satisfied literal.
		v = main_varData + abs(upLit);
		if (upLit > 0){
			SET_VAR_ASS_1( v );
		} else {
			SET_VAR_ASS_0( v );
		}

		//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < lPtr->numImpUsed; ++i){
			lit = (int32_t) (addr_t) lPtr->implies[i++];
			if (lPtr->implies[i] == toIgnore) continue;
			if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					*numPropagated = (int32_t)(addr_t)(upQ - main_simpleUPQueue);
					if (clean != 1) {
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, cleanFrom);
					}
					RESET_SIMPLEUPQUEUE();
					return lPtr->implies[i];
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
					if (stampLits) {main_litData[lit].stamp = main_litStamp; }
				} //Else, the literal is already enqueued.
			}
		}

		lPtr = main_litData - upLit;	//Unsatisfied literal.
		//Walk through the watcher lists of lPtr (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the UP queue as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (lPtr->numw1Used > offset){
			c = lPtr->w1[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore!
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW1;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w1 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w2;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					*numPropagated = (int32_t)(addr_t)(upQ - main_simpleUPQueue);
					if (clean != 1) {
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, cleanFrom);
					}
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
					if (stampLits) {main_litData[lit].stamp = main_litStamp; }
				} //Else, the literal is already enqueued.
			}
		}

		//WL2
		offset = 0;
		while (lPtr->numw2Used > offset){
			c = lPtr->w2[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore!
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW2;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w2 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w1;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					*numPropagated = (int32_t)(addr_t)(upQ - main_simpleUPQueue);
					if (clean!= 1) {
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, cleanFrom);
					}
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
					if (stampLits) {main_litData[lit].stamp = main_litStamp; }
				} //Else, the literal is already enqueued.
			}
		}
	}

	*numPropagated = (int32_t)(addr_t)(upQ - main_simpleUPQueue);
	if (clean != 0) {
		UNASSIGN_SIMPLEUPQUEUE(v, upQ, cleanFrom);
	}
	RESET_SIMPLEUPQUEUE();
	return NULL;
}

clause* main_simpleUP_propagate_ignore(clause* toIgnore){
	int32_t* upQ;
	literal* lPtr;
	variable* v;
	clause* c;
	int32_t upLit, lit, litPos, i, offset;

	//First of all, we will propagate as much as possible.
	upQ = main_simpleUPQueue;
	while (upQ < main_simpleUPQueue_current){
		upLit =*(upQ++);		//The literal number of the literal that is to be satisfied.
		lPtr = main_litData + upLit;	//Satisfied literal.
		v = main_varData + abs(upLit);
		if (upLit > 0){
			SET_VAR_ASS_1( v );
		} else {
			SET_VAR_ASS_0( v );
		}

		//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them fails,
		//we can return with a conflict. We ignore an implication imposed by toIgnore.
		for (i = 0; i < lPtr->numImpUsed; ++i){
			lit = (int32_t) (addr_t) lPtr->implies[i++];
			if (lPtr->implies[i] == toIgnore) continue;
			if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
					RESET_SIMPLEUPQUEUE();
					return lPtr->implies[i];
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		lPtr = main_litData - upLit;	//Unsatisfied literal.
		//Walk through the watcher lists of lPtr (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the UP queue as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (lPtr->numw1Used > offset){
			c = lPtr->w1[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore!
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW1;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w1 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w2;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		//WL2
		offset = 0;
		while (lPtr->numw2Used > offset){
			c = lPtr->w2[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore!
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW2;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w2 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = c->w1;
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
					RESET_SIMPLEUPQUEUE();
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}
	}

	UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
	RESET_SIMPLEUPQUEUE();
	return NULL;
}


clause* main_simpleUP_propagate_ala(int32_t start, clause* toIgnore){
	int32_t* upQ;
	literal* lPtr;
	variable* v;
	clause* c;
	int32_t upLit, lit, litPos, i, offset;

	//First of all, we will propagate as much as possible.
	upQ = main_simpleUPQueue + start;
	while (upQ < main_simpleUPQueue_current){
		upLit = -*(upQ++);		//The literal number of the literal that is to be satisfied, during ALA, this is INVERTED.
		lPtr = main_litData + upLit;	//Satisfied literal.
		v = main_varData + abs(upLit);

		if (upLit > 0){
			SET_VAR_ASS_1( v );
		} else {
			SET_VAR_ASS_0( v );
		}
		//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < lPtr->numImpUsed; ++i){
			lit = -((int32_t) (addr_t) lPtr->implies[i++]);
			if (lPtr->implies[i] == toIgnore) {
				continue;
			}
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
				//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
				return lPtr->implies[i];
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
				//The element is not yet in the UP queue. We put it there and stamp it.
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
			} //Else, the literal is already enqueued.
		}

		lPtr = main_litData - upLit;	//Unsatisfied literal.
		//Walk through the watcher lists of lPtr (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the UP queue as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (lPtr->numw1Used > offset){
			c = lPtr->w1[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore.
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW1;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w1 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = -(c->w2);
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}

		//WL2
		offset = 0;
		while (lPtr->numw2Used > offset){
			c = lPtr->w2[offset];
			//Very first thing to check here is: is this toIgnore?
			if (c == toIgnore){
				//Yes, so ignore.
				++offset;
				continue;
			}
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = c->repLit;
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			litPos = c->whereW2;
			for (i = 0; i < c->size; ++i){
				lit = c->lits[i];
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					c->repLit = lit;
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (c->w2 != -upLit){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = -(c->w1);
				if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					return c;
				} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
					//The element is not yet in the UP queue. We put it there and stamp it.
					ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				} //Else, the literal is already enqueued.
			}
		}
	}
	return NULL;
}
