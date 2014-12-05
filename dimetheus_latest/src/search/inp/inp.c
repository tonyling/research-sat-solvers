/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inp.h"

void inp_intern_reduceDB(){
	//This method reduces the clause database from position ** inp_reduceFrom in DL0. All the satisfied clauses are removed,
	//all the false literals are removed. Clauses that lose a literal are added to the queues and stacks.
	clause *c;
	variable *v;
	literal *l, **dlPos = inp_reduceFrom;
	uint64_t sig;
	uint32_t litPos, occLitPos;
	int32_t litID, impLitA, impLitB, litToRemove;

	//If nothing can be reduced, then we return right away.
	if (*inp_reduceFrom == NULL) return;

	//First, we handle the removal of satisfied clauses. Grab one assignment after the other to remove the clauses in which
	//the literal appears satisfied.
	while ((l = *dlPos) != NULL){
		//As long as literal l has more occurrences, we must continue to remove occurrences.
		while (GET_LIT_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence.
			c = GET_LIT_OCCNUM(l, 0);
			inp_intern_abandonClause(c);
		}

		//Go on with the next DL0 assignment.
		++dlPos;
	}

	//Second, we handle the removal of literals that became false from the formula. Since we already removed all the clauses
	//that are satisfied, we must not worry that this literal is a false literal in a binary clauses (these clauses must
	//be satisfied by definition and have been removed by the loop above). In other words, all the clauses that lose a literal
	//now are not satisfied and have at least size 3. Furthermore, all these clauses have at least two unassigned literals.
	//It can, however, happen that the literal removal creates a binary clause, in which case we must remove this clause
	//from the watcher lists and add the implications it imposes.
	//Additionally, if a literal is removed, we must check if the literal was the representative. If so, we just override the
	//representative with the first literal in the clause.
	//All the clauses that receive an update here are subject to further tests and are put into the clause stack.

	//We again check all the assignments made in DL0 from the check position onwards.
	dlPos = inp_reduceFrom;
	while ((l = *dlPos) != NULL){
		//We invert the sign of the literal. The literal l is now the falsified literal.
		litToRemove = -GET_LIT_NUM_FROM_PTR(l);
		l = main_litData + litToRemove;
		//As long as literal l has more occurrences, we must continue to remove this literal from the occurrence.
		while (GET_LIT_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence. Grab the position of the literal in this occurrence.
			c = GET_LIT_OCCNUM(l, 0);
			occLitPos = GET_LIT_OCCLITPOSNUM(l, 0);
			//First, we remove the clause from the occurrence list of l. Clause c must be found at position 0 in the
			//occurrence list of l.
			REM_CLS_FROM_LIT_OCCS_PTR(l, 0);
			//We must now remove l at position occLitPos in clause c.
			REM_CLS_LIT_NUM_AT_POS(c, occLitPos);
			//If the literal was the representative, we must replace it.
			if (GET_CLS_REPLIT(c) == litToRemove){
				SET_CLS_REPLIT(c, GET_CLS_LITNUM(c,0));
			}
			//If the clause became binary, we must now remove it from the watcher lists and add the implication the clause
			//imposes to the implication lists.
			if (GET_CLS_SIZE(c) == 2){
				//Yes, the clause is now binary.
				litID = GET_CLS_W1(c);
				litPos = GET_CLS_WHEREW1(c);
				REM_CLS_FROM_LIT_W1LIST(litID, litPos);
				litID = GET_CLS_W2(c);
				litPos = GET_CLS_WHEREW2(c);
				REM_CLS_FROM_LIT_W2LIST(litID, litPos);
				ADD_LIT_IMPLICATIONS( c, impLitA , impLitB );
			} else {
				//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
				//need to find a replacement.
				if (GET_CLS_W1(c) == litToRemove){
					//Yes, we killed the first watcher and must replace it with something not the second watcher.
					litID = GET_CLS_W1(c);
					litPos = GET_CLS_WHEREW1(c);
					REM_CLS_FROM_LIT_W1LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
						litID = GET_CLS_LITNUM(c,litPos);
						if (litID == GET_CLS_W2(c) || IS_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old first watcher.
						SET_CLS_W1(c, litID);
						ADD_CLS_TO_LIT_W1LIST(c, litID);
						break;
					}
				} else if (GET_CLS_W2(c) == litToRemove){
					//Yes, we killed the second watcher and must replace it with something not the first watcher.
					litID = GET_CLS_W2(c);
					litPos = GET_CLS_WHEREW2(c);
					REM_CLS_FROM_LIT_W2LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
						litID = GET_CLS_LITNUM(c,litPos);
						if (litID == GET_CLS_W1(c) || IS_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old second watcher.
						SET_CLS_W2(c, litID);
						ADD_CLS_TO_LIT_W2LIST(c, litID);
						break;
					}
				}
			}
			//We have removed the literal occurrence of l. We must now update the signature of the reduced clause.
			sig = 0ULL;
			for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
				sig |= GET_LIT_SIG(GET_CLS_LITNUM(c,litPos));
			}
			SET_CLS_SIG(c, sig);
		}
		//We have now removed literal l from all clauses, replaced the watchers or moved the binary clauses to the implication
		//lists. We also updated the representative literal of the clauses. Go on with the next literal.
		++dlPos;
	}

	//We again check all the assignments made in DL0 from the check position onwards.
	dlPos = inp_reduceFrom;
	while ((l = *dlPos) != NULL){
		//We must disable the corresponding variable.
		v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
		inp_intern_disableVariable(v);
		++dlPos;
	}

	//After the reduction is completed, we set the new reduction position to the end of DL0.
	inp_reduceFrom = dlPos;
}

clause* inp_intern_upPropagate(){
	literal* l;  			//The current literal to propagate.
	variable* v; 			//The corresponding variable.
	clause* c;	 			//The clause we currently work on when updating the watchers.
	int32_t i,lid, lit, litPos;	//The integer representation of the literal and a temporary literal and a literal position.
	int32_t offset,litToRemove; //The offset in the watcher list for the clause we currently work on.
	int32_t litID, impLitA, impLitB;
	uint32_t occLitPos;
	uint64_t sig;
	//This method will perform unit propagation from the current position in the decision stack. We assume that the UP is
	//initialized correctly, that is the main_complexUP_current pointer points to the position in the decision stack that we
	//want to continue a unit propagation from. Additionally, we assume that the main_complexUP_end pointer points to the next
	//position in the decision stack where we want to append new elements at. We also assume that the main_upStamp is fresh.

	literal **reduceFrom = main_complexUP_current, **reduce;

	//We propagate as long as we still have elements left.
	while (main_complexUP_current < main_complexUP_end){
		//We grab the next literal for which we want to assign the variable.
		l = *(main_complexUP_current++);
		lid = GET_LIT_NUM_FROM_PTR(l);
		//We assign the corresponding variable.
		if (lid < 0){
			//The literal is negative. The variable must be assigned to 0.
			v = main_varData - lid;
			SET_VAR_ASS_0(v);
		} else {
			//the literal is positive. The variable must be assigned to 1.
			v = main_varData + lid;
			SET_VAR_ASS_1(v);
		}
		//We disable the variable, such that it does not appear in the variable priority queue anymore.
		inp_intern_disableVariable(v);

		//Walk through the implication list of l. All the implications must be put on the stack. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
			lit = GET_LIT_IMPNUM(l, i);
			v = main_varData + abs(lit);
			if (IS_VAR_UNASSIGNED(v)){
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					return GET_LIT_IMPNUM_REASON(l, i);
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					MAIN_COMPLEXUP_ADD_IMP(lit , v, GET_LIT_IMPNUM_REASON(l, i));
				} //Else, the literal is already enqueued.
			}
		}

		//We must now walk through the watcher lists of the literal that becomes false, that is, -l is checked.
		l = main_litData - lid;
		//Walk through the watcher lists of l (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the COMPLEX UP as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (GET_LIT_NUMW1_USED(l) > offset){
			c = GET_LIT_W1_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CLS_WHEREW1(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CLS_SIZE(c); ++i){
				lit = GET_CLS_LITNUM(c,i);
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the second watcher.
				if (lit == GET_CLS_W2(c)) continue;
				//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
				v = main_varData + abs(lit);
				if (IS_VAR_UNASSIGNED(v)){
					//We have found an unassigned literal that can serve as watcher.
					SET_CLS_W1(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CLS_W1(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-lid, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CLS_W2(c);
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = main_varData + abs(lit);
					MAIN_COMPLEXUP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}

		//WL2
		offset = 0;
		while (GET_LIT_NUMW2_USED(l) > offset){
			c = GET_LIT_W2_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CLS_WHEREW2(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CLS_SIZE(c); ++i){
				lit = GET_CLS_LITNUM(c,i);
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the first watcher.
				if (lit == GET_CLS_W1(c)) continue;
				//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
				v = main_varData + abs(lit);
				if (IS_VAR_UNASSIGNED(v)){
					//We have found an unassigned literal that can serve as watcher.
					SET_CLS_W2(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CLS_W2(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-lid, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CLS_W1(c);
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = main_varData + abs(lit);
					MAIN_COMPLEXUP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}
	}

	//We now reduce the clause database using the satisfied and unsatisfied propagations made.
	reduce = reduceFrom;
	while (reduce < main_complexUP_end){
		l = *(reduce++);
		while (GET_LIT_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence.
			c = GET_LIT_OCCNUM(l, 0);
			//We first disable the clause, then signal the disabling to the queues and stack, and then delete the clause.
			inp_intern_abandonClause(c);
		}
	}

	//We now reduce the clauses by removing false literals.
	reduce = reduceFrom;
	while (reduce < main_complexUP_end){
		l = *(reduce++);
		litToRemove = -GET_LIT_NUM_FROM_PTR(l);
		l = main_litData + litToRemove;
		//As long as literal l has more occurrences, we must continue to remove this literal from the occurrence.
		while (GET_LIT_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence. Grab the position of the literal in this occurrence.
			c = GET_LIT_OCCNUM(l, 0);
			occLitPos = GET_LIT_OCCLITPOSNUM(l, 0);
			//First, we remove the clause from the occurrence list of l. Clause c must be found at position 0 in the
			//occurrence list of l.
			REM_CLS_FROM_LIT_OCCS_PTR(l,0);
			//We must now remove l at position occLitPos in clause c.
			REM_CLS_LIT_NUM_AT_POS(c, occLitPos);
			//If the literal was the representative, we must replace it.
			if (GET_CLS_REPLIT(c) == litToRemove){
				SET_CLS_REPLIT(c, GET_CLS_LITNUM(c,0));
			}
			//If the clause became binary, we must now remove it from the watcher lists and add the implication the clause
			//imposes to the implication lists.
			if (GET_CLS_SIZE(c) == 2){
				//Yes, the clause is now binary.
				litID = GET_CLS_W1(c);
				litPos = GET_CLS_WHEREW1(c);
				REM_CLS_FROM_LIT_W1LIST(litID, litPos);
				litID = GET_CLS_W2(c);
				litPos = GET_CLS_WHEREW2(c);
				REM_CLS_FROM_LIT_W2LIST(litID, litPos);
				ADD_LIT_IMPLICATIONS( c, impLitA , impLitB );
			} else {
				//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
				//need to find a replacement.
				if (GET_CLS_W1(c) == litToRemove){
					//Yes, we killed the first watcher and must replace it with something not the second watcher.
					litID = GET_CLS_W1(c);
					litPos = GET_CLS_WHEREW1(c);
					REM_CLS_FROM_LIT_W1LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
						litID = GET_CLS_LITNUM(c,litPos);
						if (litID == GET_CLS_W2(c) || IS_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old first watcher.
						SET_CLS_W1(c, litID);
						ADD_CLS_TO_LIT_W1LIST(c, litID);
						break;
					}
				} else if (GET_CLS_W2(c) == litToRemove){
					//Yes, we killed the second watcher and must replace it with something not the first watcher.
					litID = GET_CLS_W2(c);
					litPos = GET_CLS_WHEREW2(c);
					REM_CLS_FROM_LIT_W2LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
						litID = GET_CLS_LITNUM(c,litPos);
						if (litID == GET_CLS_W1(c) || IS_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old second watcher.
						SET_CLS_W2(c, litID);
						ADD_CLS_TO_LIT_W2LIST(c, litID);
						break;
					}
				}
			}
			//We have removed the literal occurrence of l. We must now update the signature of the reduced clause.
			sig = 0ULL;
			for (litPos = 0; litPos < GET_CLS_SIZE(c); ++litPos){
				sig |= GET_LIT_SIG(GET_CLS_LITNUM(c,litPos));
			}
			SET_CLS_SIG(c, sig);
			//We signal to all queues and stacks that this clause lost the literal.
			inp_intern_QSUpdateLiteralDeletion(c,litToRemove);
		}
	}

	inp_reduceFrom = main_complexUP_end;

	//In case no conflict occurred, we can terminate the current decision level and return NULL (no conflict clause).
	MAIN_COMPLEXUP_TERMINATE_CURRENT();
	return NULL;
}

void inp_intern_QSUpdateClauseDisabling(clause* c){
	//Handle all the queue and stack updates if a clause is deleted.
	uint32_t i;
	variable *v;

	//CLAUSE PRIORITY QUEUE.
	cQ_delete(c);
	if (cQ_touchClause == cQ_touchClause_inp_se){
		//If the clause is disabled, this has no effect on the clause queue.
	} else if (cQ_touchClause == cQ_touchClause_inp_str){
		//If the clause is disabled, this has no effect on the clause queue.
	} else if (cQ_touchClause == cQ_touchClause_inp_treelook){
		//If the clause is disabled, this has no effect on the clause queue.
	}

	//CLAUSE STACKS.
	cS_delete(c);
	cSS0_delete(c);
	cSS1_delete(c);

	//LITERAL PRIORITY QUEUE.
	if (lQ_touchLiteral == lQ_touchLiteral_inp_fld){
		//Disabling a binary clause means we need to update the literals in the queue, if they are in there.
		if (GET_CLS_SIZE(c) == 2){
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,0))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,1))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_inp_nhbr){
		if (GET_CLS_SIZE(c) == 2){
			if (GET_LIT_NUM_Q_POS(-GET_CLS_LITNUM(c,0)) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			if (GET_LIT_NUM_Q_POS(-GET_CLS_LITNUM(c,0)) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if  (lQ_touchLiteral == lQ_touchLiteral_inp_niver){
		//We must update all the literals in the queue that participated in the clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			v = main_varData + abs(GET_CLS_LITNUM(c, i));
			if (IS_VAR_UNASSIGNED(v) && !IS_VAR_DISABLED(v)){
				lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c, i));
			}
		}
	}

	//LITERAL STACK.


	//VARIABLE PRIORITY QUEUE.
	if (vQ_touchVariable == vQ_touchVariable_inp_ge) {
		//We are currently doing GE. If a clause is removed, then this has no effect on gate extraction.
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_inp_pl){
		//We touch all variables that appear in this disabled clause because they might now be pure.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			vS_touchVariable(main_varData + abs(GET_CLS_LITNUM(c,i)));
		}
	}

}

void inp_intern_QSUpdateClauseEnabling(clause* c){
	//Handle all the queue and stack updates if a clause is added.
	int32_t i;
	literal *l;

	//CLAUSE PRIORITY QUEUE.
	cQ_touchClause(c);
	if (cQ_touchClause == cQ_touchClause_inp_se){
		//If the clause is enabled, this has no effect on the clause queue.
	} else if (cQ_touchClause == cQ_touchClause_inp_str){
		//If the clause is disabled, this has no effect on the clause queue.
	} else if (cQ_touchClause == cQ_touchClause_inp_treelook){
		//If the clause is disabled, this has no effect on the clause queue.
	}


	//CLAUSE STACK.
	cS_touchClause(c);


	//LITERAL PRIORITY QUEUE.
	if (lQ_touchLiteral == lQ_touchLiteral_inp_fld){
		//All literals contained in a binary clause are added/updated to the failed literal queue with opposite sign.
		if (GET_CLS_SIZE(c) == 2){
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_inp_nhbr){
		//If the clause is binary, we touch all the opposite literals to add it to the NHBR queue.
		if (GET_CLS_SIZE(c) == 2){
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if  (lQ_touchLiteral == lQ_touchLiteral_inp_niver){
		//We must update all the literals in the queue that participate in the clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			l = main_litData + GET_CLS_LITNUM(c, i);
			if (GET_LIT_Q_POS(l) > 0) lQ_touchLiteral(l);
		}
	}

	//LITERAL STACK.


	//VARIABLE PRIORITY QUEUE.
	 if (vQ_touchVariable == vQ_touchVariable_inp_ge) {
		//We are currently doing GE. If a clause is added, then an additional gate might be available now for any of the
		//variables that is contained in this added clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			vQ_touchVariable(main_varData + abs(GET_CLS_LITNUM(c,i)));
		}
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_inp_pl){
		//Enabling a clause has no effect on the variable stack in PLE.
	}
}

void inp_intern_QSUpdateLiteralDeletion(clause* c, int32_t lit){
	//Handle all the queue and stack updates if a clauses literal is deleted.
	int32_t i;

	//CLAUSE PRIORITY QUEUE.
	if (cQ_touchClause == cQ_touchClause_inp_se){
		//If the clause lost a literal, we again put it into the clause queue.
		cQ_touchClause(c);
	} else if (cQ_touchClause == cQ_touchClause_inp_str){
		//If the clause lost a literal, we again put it into the clause queue.
		cQ_touchClause(c);
	} else if (cQ_touchClause == cQ_touchClause_inp_treelook){
		//A clause that loses a literal must be re-checked if it is unsatisfied under the tree-look assignment.
		cQ_touchClause(c);
	}

	//CLAUSE STACK.


	//LITERAL PRIORITY QUEUE.
	if (lQ_touchLiteral == lQ_touchLiteral_inp_fld){
		//All literals contained in a binary clause are added/updated to the failed literal queue with opposite sign.
		if (GET_CLS_SIZE(c) == 2){
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_inp_nhbr){
		//If the clause is binary, we touch all the opposite literals to add it to the NHBR queue.
		if (GET_CLS_SIZE(c) == 2){
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if  (lQ_touchLiteral == lQ_touchLiteral_inp_niver){
		//We must update the literal that has been removed from the clause.
		if (GET_LIT_NUM_Q_POS(lit) > 0) lQ_touchLiteral(main_litData + lit);
	}

	//LITERAL STACK.


	//VARIABLE PRIORITY QUEUE.
	if (vQ_touchVariable == vQ_touchVariable_inp_ge) {
		//We are currently doing GE. Removing a literal from a clause might enable the clause to act in a gate definition.
		//We touch all the remaining variables in this clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			vQ_touchVariable(main_varData + abs(GET_CLS_LITNUM(c,i)));
		}
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_inp_pl){
		//The variable behind the removed literal might be pure now.
		vS_touchVariable(main_varData + abs(lit));
	}
}

void inp_intern_disableClause(clause *c){
	//Disables a clause.
	if (!IS_CLS_DISABLED(c)){
		clauses_disable(c);
		inp_intern_QSUpdateClauseDisabling(c);
	}
}

void inp_intern_enableClause(clause *c){
	//Enables a clause.
	if (IS_CLS_DISABLED(c)){
		clauses_enable(c);
		inp_intern_QSUpdateClauseEnabling(c);
	}
}

void inp_intern_abandonClause(clause* c){
	//Disable the unwanted clause.
	if (!IS_CLS_DISABLED(c)){
		clauses_disable(c);
		inp_intern_QSUpdateClauseDisabling(c);
	}
	//If the clause was a learned one, we can even free its memory by totally deleting it.
	if (IS_CLS_LEARNED(c)){	clauses_deleteCls(c); }
}

void inp_intern_deleteClause(clause* c){
	if (!IS_CLS_DISABLED(c)) {
		//Disable the unwanted clause.
		inp_intern_disableClause(c);
		//Update the preprocessors queues and stacks.
		prep_intern_QSUpdateClauseDisabling(c);
		//We delete the clause.
	}
	clauses_deleteCls(c);
}

void inp_intern_deleteLitInClause(clause* c, int32_t lit, int32_t litPos){
	//Delete the literal in the clause.
	clauses_deleteLit(c, lit, litPos);
	//Update the preprocessors queues and stacks.
	inp_intern_QSUpdateLiteralDeletion(c, lit);
}

void inp_intern_makeClauseRemoved(clause* c, int32_t litOfInterest, int32_t byAction){
	//This method will make the given clause a removed one. Removed clauses are like disabled clauses, except that they
	//cannot be deleted. The litOfInterest is either a blocking literal, or a literal that was resolved on during NIVER or
	//gate extraction.
	int32_t i, lit, pos;


	//If the clause is not yet disabled, we will remove it from the respective lists now.
	if (!IS_CLS_DISABLED(c)){
		//We will remove the clause from all the watcher lists, if the clause is indeed not binary.
		if (c->size > 2){
			lit = c->w1;
			pos = c->whereW1;
			REM_CLS_FROM_LIT_W1LIST(lit, pos);
			lit = c->w2;
			pos = c->whereW2;
			REM_CLS_FROM_LIT_W2LIST(lit, pos);
		} else {
			//It is a binary clause and we must remove the implications it imposes.
			REM_LIT_IMPLICATIONS(c, lit, pos);
		}

		//Then, we will remove the clause from the occurrence lists.
		for (i = 0; i < c->size; ++i){
			lit = c->lits[i];
			pos = c->occPos[i];
			REM_CLS_FROM_LIT_OCCS(lit, pos);
		}

		//We now remove the clause from the address list it is in, and append it to the respective disabled array.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//It is a learned clause, so it must be found in the f.clauses_el array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_el , pos , f.m_el_used);
		} else {
			//It is an original clause, so it must be found in the f.clauses_eo array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_eo , pos , f.m_eo_used);
		}
	} else {
		//We do not have to remove the clause from any lists, as it is already disabled.
		//We now remove the clause from the address list it is in, and append it to the respective disabled array.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//It is a learned clause, so it must be found in the f.clauses_dl array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_dl , pos , f.m_dl_used);
		} else {
			//It is an original clause, so it must be found in the f.clauses_do array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_do , pos , f.m_do_used);
		}
	}

	//Update the queues and stacks.
	inp_intern_QSUpdateClauseDisabling(c);

	//We must now create a new removed clause and transfer all the necessary information of the old clause to the new clause.
	//Afterwards, we delete the clause completely to free the memory.
	remClause* rC = malloc(sizeof(remClause));
	rC->byAction = byAction;
	rC->size = c->size;
	rC->lits = c->lits;
	rC->litOfInterest = litOfInterest;

	//We do not need to store covered literals.
	rC->numCovered = 0;
	rC->coveredLits = NULL;

	ADD_REMCLS_TO_REMCLAUSES_LIST(rC);

	//After the transfer of the clause into the removed clauses set, we can destroy the rest of c. The c->lits must, however,
	//survive, because we address them from the removed clause as well.
	free(c->msgs);
	free(c->occPos);
	free(c);
}

void inp_intern_addOriginalClause(int32_t* literals, int32_t size){
	//We use this method to add a new clause to the formula during in-processing. It must not be unit. First, we add the clause.
	clauses_addOrig(literals, size);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	inp_intern_QSUpdateClauseEnabling(f.clauses_eo[f.m_eo_used-1]);
}

void inp_intern_addLearnedClause(int32_t* literals, int32_t size){
	//We use this method to add a new clause to the formula during in-processing. It must not be unit. First, we add the clause.
	clauses_addLearned(literals, size);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	inp_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void inp_intern_addLearnedBinaryClause(int32_t lit1, int32_t lit2){
	//To add a new clause as learned clause in (i.e. NHBR clauses).
	clauses_addLearnedBinary(lit1, lit2);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	inp_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void inp_intern_addLearnedTernaryClause(int32_t lit1, int32_t lit2, int32_t lit3){
	//To add a new clause as learned clause in (i.e. NHBR clauses).
	clauses_addLearnedTernary(lit1, lit2, lit3);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	inp_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void inp_intern_backJump(uint32_t toLevel, uint32_t clearTargetLevel){
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
			if (!IS_VAR_UNASSIGNED(v)){
				SET_VAR_ASS_2(v);
			}
		}
		++clearFrom;
	}
}

void inp_intern_disableVariable(variable *v){
	//This method is used to disable a variable during the in-processing. It will, in the end, call for the standard method
	//to disable a variable, but it will also take care of updating queues and stacks that are variable related.
	vQ_delete(v);
	vS_delete(v);
	lQ_delete(main_litData + v->id);
	lQ_delete(main_litData - v->id);
	lS_delete(main_litData + v->id);
	lS_delete(main_litData - v->id);
	variables_disable(v);
}

void inp_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_inp_component_totalCalls;
	#endif
	#ifdef VERBOSE_INP
	printf("c     INP: RESET...\n");
	#endif
	inp_returnCode 					= INP_UNKNOWN;
	inp_reduceFrom 					= NULL;
	inp_unhiding_SPlus 				= NULL;
	inp_unhiding_SMinus 			= NULL;
	inp_litPerm 					= NULL;
	inp_unhiding_lStack 			= NULL;
	inp_unhiding_lStack_current 	= NULL;

	inp_res							= NULL;
	inp_resAvail 					= 0;
	inp_resUsed 					= 0;
	inp_resMemAvail 				= NULL;
	inp_resMemUsed 					= NULL;

	inp_G_negV 						= NULL;
	inp_G_negVAvail 				= 0;
	inp_G_negVUsed 					= 0;
	inp_G_posV 						= NULL;
	inp_G_posVAvail 				= 0;
	inp_G_posVUsed 					= 0;

	inp_els_classes 				= NULL;
	inp_els_classes_avail 			= 0;
	inp_els_classes_used 			= 0;
	inp_els_classes_memUsed			= NULL;
	inp_els_lStack 					= NULL;
	inp_els_lStack_current 			= NULL;

	inp_trl_lQ						= NULL;
	inp_trl_lQ_c					= NULL;
	inp_trl_lQ_e					= NULL;
	inp_trl_faileds					= NULL;

	inp_strategies_pointer_reset();
	#ifdef COLLECTSTATS
	stats_inp_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void inp_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	#ifdef VERBOSE_INP
	printf("c     INP: INIT...\n");
	#endif
	int32_t i;

	inp_returnCode = INP_UNKNOWN;
	inp_strategies_pointer_init();
	inp_reduceFrom = main_decStackPtrs[0];

	inp_G_negVAvail = BLOCKSIZE;
	inp_G_negVUsed = 0;
	inp_G_negV = malloc(sizeof(clause*)*inp_G_negVAvail);
	if (inp_G_negV == NULL){
		printf("c ERROR. INP failed to initialize the gate extraction array G_negV. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_G_posVAvail = BLOCKSIZE;
	inp_G_posVUsed = 0;
	inp_G_posV = malloc(sizeof(clause*)*inp_G_posVAvail);
	if (inp_G_posV == NULL){
		printf("c ERROR. INP failed to initialize the gate extraction array G_posV. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_unhiding_SPlus = malloc(sizeof(literal*)*(f.n_afterPrep+2));
	if (inp_unhiding_SPlus == NULL){
		printf("c ERROR. The INP was unable to allocate the SPlus array for Unhiding. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_unhiding_SMinus = malloc(sizeof(literal*)*(f.n_afterPrep+2));
	if (inp_unhiding_SMinus == NULL){
		printf("c ERROR. The INP was unable to allocate the SMinus array for Unhiding. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_unhiding_lStack = malloc(sizeof(literal*)*(f.n_afterPrep+2));
	if (inp_unhiding_lStack == NULL){
		printf("c ERROR. The INP was unable to allocate the UNH Stack array for Unhiding. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_unhiding_lStack[0] = NULL;
	inp_unhiding_lStack_current = inp_unhiding_lStack + 1;

	inp_litPerm = malloc(sizeof(literal*)*(2*f.n_afterPrep+1));
	if (inp_litPerm == NULL){
		printf("c ERROR. The INP was unable to allocate the SMinus array for Unhiding. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_resUsed = 0;
	inp_resAvail = 2*BLOCKSIZE;
	inp_res = malloc(sizeof(int32_t*) * inp_resAvail);
	if (inp_res == NULL){
		printf("c ERROR. INP failed to initialize the resolvents array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_resMemAvail = malloc(sizeof(int32_t) * inp_resAvail);
	if (inp_resMemAvail == NULL){
		printf("c ERROR. INP failed to initialize the resolvents memory availability array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_resMemUsed = malloc(sizeof(int32_t) * inp_resAvail);
	if (inp_resMemAvail == NULL){
		printf("c ERROR. INP failed to initialize the resolvents memory usage array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	if (inp_returnCode == INP_ERROR) {
		if (inp_res != NULL){
			for (i = 0; i < inp_resAvail; ++i){	inp_res[i] = NULL; }
		}
		return;
	}
	for (i = 0; i < inp_resAvail; ++i){
		if (inp_returnCode == INP_UNKNOWN){
			inp_res[i] = NULL;
			inp_res[i] = malloc(sizeof(int32_t)*BLOCKSIZE);
			if (inp_res[i] == NULL) {
				inp_returnCode = INP_ERROR;
				inp_resMemAvail[i] = 0;
				inp_resMemUsed[i] = 0;
			} else {
				inp_resMemAvail[i] = BLOCKSIZE;
				inp_resMemUsed[i] = 0;
			}
		} else {
			inp_res[i] = NULL;
			inp_resMemAvail[i] = 0;
			inp_resMemUsed[i] = 0;
		}
	}

	inp_els_classes_avail = BLOCKSIZE;
	inp_els_classes_used = 0;

	inp_els_classes = malloc(sizeof(literal**) * inp_els_classes_avail);
	if (inp_els_classes == NULL){
		printf("c ERROR. The INP failed to initialize the ELS classes array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_els_classes_memUsed = malloc(sizeof(uint32_t) * inp_els_classes_avail);
	if (inp_els_classes_memUsed == NULL){
		printf("c ERROR. The INP failed to initialize the ELS memory usage array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	inp_els_classes_memAvail = malloc(sizeof(uint32_t) * inp_els_classes_avail);
	if (inp_els_classes_memAvail == NULL){
		printf("c ERROR. The INP failed to initialize the ELS memory availability array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	for (i = 0; i < inp_els_classes_avail; ++i){
		if (inp_returnCode == INP_UNKNOWN){
			inp_els_classes[i] = NULL;
			inp_els_classes[i] = malloc(sizeof(literal*) * BLOCKSIZE);
			if (inp_els_classes[i] == NULL){
				inp_returnCode = INP_ERROR;
				inp_els_classes_memUsed[i] = 0;
				inp_els_classes_memAvail[i] = 0;
			} else {
				inp_els_classes_memUsed[i] = 0;
				inp_els_classes_memAvail[i] = BLOCKSIZE;
			}
		} else {
			inp_els_classes[i] = NULL;
			inp_els_classes_memUsed[i] = 0;
			inp_els_classes_memAvail[i] = 0;
		}
	}
	if (inp_returnCode == INP_ERROR) return;

	inp_els_lStack = malloc(sizeof(literal*)*(2*f.n_afterPrep + 2));
	if (inp_els_lStack == NULL){
		printf("c ERROR. The INP failed to initialize the ELS literal stack array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_els_lStack[0] = NULL;
	inp_els_lStack_current = inp_els_lStack + 1;

	inp_trl_lQ = malloc(sizeof(literal*)*(4*f.n_afterPrep + 4));
	if (inp_trl_lQ == NULL){
		printf("c ERROR. The INP failed to initialize the TREELOOK literal queue. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}
	inp_trl_lQ_c = inp_trl_lQ;
	inp_trl_lQ_e = inp_trl_lQ;

	inp_trl_faileds = malloc(sizeof(literal*)*(f.n_afterPrep + 1));
	if (inp_trl_lQ == NULL){
		printf("c ERROR. The INP failed to initialize the TREELOOK failed literal array. Out of memory?\n");
		inp_returnCode = INP_ERROR;
		return;
	}

	#ifdef COLLECTSTATS
	stats_inp_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void inp_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_inp_component_totalCalls;
	#endif
	#ifdef VERBOSE_INP
	printf("c     INP: DISPOSE...\n");
	#endif
	int32_t i;

	inp_returnCode = INP_UNKNOWN;
	inp_strategies_pointer_dispose();
	inp_reduceFrom = NULL;

	if (inp_G_negV != NULL){
		free(inp_G_negV);
		inp_G_negV = NULL;
	}
	if (inp_G_posV != NULL){
		free(inp_G_posV);
		inp_G_posV = NULL;
	}
	inp_G_negVAvail = 0;
	inp_G_negVUsed  = 0;
	inp_G_posVAvail = 0;
	inp_G_posVUsed  = 0;

	if (inp_unhiding_SPlus != NULL){
		free(inp_unhiding_SPlus);
		inp_unhiding_SPlus = NULL;
	}
	if (inp_unhiding_SMinus != NULL){
		free(inp_unhiding_SMinus);
		inp_unhiding_SMinus = NULL;
	}
	if (inp_unhiding_lStack != NULL){
		free(inp_unhiding_lStack);
		inp_unhiding_lStack = NULL;
		inp_unhiding_lStack_current = NULL;
	}
	if (inp_litPerm != NULL){
		free(inp_litPerm);
		inp_litPerm = NULL;
	}

	if (inp_res != NULL){
		for (i = 0; i < inp_resAvail; ++i){
			if (inp_res[i] != NULL){
				free(inp_res[i]);
				inp_res[i] = NULL;
			}
		}
		free(inp_res);
		inp_res = NULL;
	}
	inp_resAvail = 0;
	inp_resUsed = 0;

	if (inp_resMemAvail != NULL){
		free(inp_resMemAvail);
		inp_resMemAvail = NULL;
	}

	if (inp_resMemUsed != NULL){
		free(inp_resMemUsed);
		inp_resMemUsed = NULL;
	}

	if (inp_els_classes != NULL){
		for (i = 0; i < inp_els_classes_avail; ++i){
			if (inp_els_classes[i] != NULL){
				free(inp_els_classes[i]);
				inp_els_classes[i] = NULL;
			}
		}
		free(inp_els_classes);
		inp_els_classes = NULL;
	}
	inp_els_classes_avail = 0;
	inp_els_classes_used = 0;

	if (inp_els_classes_memUsed != NULL){
		free(inp_els_classes_memUsed);
		inp_els_classes_memUsed = NULL;
	}
	if (inp_els_classes_memAvail != NULL){
		free(inp_els_classes_memAvail);
		inp_els_classes_memAvail = NULL;
	}

	if (inp_els_lStack != NULL){
		free(inp_els_lStack);
		inp_els_lStack = NULL;
		inp_els_lStack_current = NULL;
	}

	if (inp_trl_lQ != NULL){
		free(inp_trl_lQ);
		inp_trl_lQ = NULL;
		inp_trl_lQ_c = NULL;
		inp_trl_lQ_e = NULL;
	}

	if (inp_trl_faileds != NULL){
		free(inp_trl_faileds);
		inp_trl_faileds = NULL;
	}
	#ifdef COLLECTSTATS
	stats_inp_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void inp_extern_inprocessing(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_inp_component_totalCalls;
	#endif
	if (param_inpStrategy != INP_STRATEGY_NULL){
		#ifdef VERBOSE_INP
		printf("c     INP: INPROCESS...\n");
		#endif
		inp_strategy_reset();
		inp_strategy_init();
		if (inp_returnCode == INP_UNKNOWN){
			inp_strategy_execute();
		}
		inp_strategy_dispose();
	}
	#ifdef COLLECTSTATS
	stats_inp_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
