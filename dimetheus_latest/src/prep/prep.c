/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prep.h"

void prep_intern_QSUpdateClauseDisabling(clause* c){
	//Handle all the queue and stack updates if a clause is deleted.
	uint32_t i;
	literal* l;

	//CLAUSE PRIORITY QUEUE.
	cQ_delete(c);
	//We touch the necessary clauses.
	if (cQ_touchClause == cQ_touchClause_se){
		//During subsumption elimination, dropping a clause does not result in any touch operations.
	}

	//CLAUSE STACKS.
	cS_delete(c);
	if (cS_touchClause == cS_touchClause_str){
		//During strengthening, the removal of a clause has no effect.
	} else if (cS_touchClause == cS_touchClause_ate){
		//We are currently doing asymmetric tautology elimination. Deleting a clause using ATE cannot make another clause
		//an ATE. So there is nothing to do at the moment.
	}
	cSS0_delete(c);
	if (cSS0_touchClause == cSS0_touchClause_satelite){
		//We might need to add something here.
	}
	cSS1_delete(c);
	if (cSS1_touchClause == cSS1_touchClause_satelite){
		//We might need to add something here.
	}

	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (lQ_touchLiteral == lQ_touchLiteral_fld){
		//Update the literals in the queue if the disabled clause is binary.
		if (GET_CLS_SIZE(c) == 2){
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,0))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,1))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_acce){
		//We are currently doing clause elimination.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i];	//The opposite literal from the occurrence.
			lQ_touchLiteral(l);
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_nhbr){
		if (GET_CLS_SIZE(c) == 2){
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,0))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,0));
			if (GET_LIT_Q_POS((main_litData - GET_CLS_LITNUM(c,1))) > 0) lQ_touchLiteral(main_litData - GET_CLS_LITNUM(c,1));
		}
	}

	//LITERAL STACK.
	//None so far.

	//VARIABLE PRIORITY QUEUE.
	//Then, we touch all the variables.
	if (vQ_touchVariable == vQ_touchVariable_niver){
		//We are currently doing NIVER. If a clause is removed, then all variables currently in this clause must be touched, as
		//they all lose a resolution partner.
		for (i = 0; i < c->size; ++i){
			vQ_touchVariable(main_varData + abs(c->lits[i]));
		}
	} else if (vQ_touchVariable == vQ_touchVariable_ge) {
		//We are currently doing GE. If a clause is removed, then this has no effect on gate extraction.
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_pl){
		//We touch all the variables for the variable stack, because they now might have only one occurrence in one literal.
		for (i = 0; i < c->size; ++i){
			vS_touchVariable(main_varData + abs(c->lits[i]));
		}
	}

}

void prep_intern_QSUpdateClauseEnabling(clause* c){
	//Handle all the queue and stack updates if a clause is added.
	int32_t i,j;
	literal* l;

	//CLAUSE PRIORITY QUEUE.
	//We touch the necessary clauses.
	cQ_touchClause(c);
	if (cQ_touchClause == cQ_touchClause_se){
		//Other clauses might subsume the new clause, we touch all that have an occurrence in common.
		for (i = 0; i < c->size; ++i){
			l = main_litData + c->lits[i];
			for (j = 0; j < l->numOccsUsed; ++j){
				cQ_touchClause(l->occs[j]);
			}
		}
	}

	//CLAUSE STACKS.
	cS_touchClause(c);
	if (cS_touchClause == cS_touchClause_str){
		//Other clauses might strengthen the new clause, we touch all that have an opposite occurrence in common.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i];
			for (j = 0; j < l->numOccsUsed; ++j){
				cS_touchClause(l->occs[j]);
			}
		}
	} else if (cS_touchClause == cS_touchClause_ate){
		//Furthermore, all other clauses containing the same literals might now get more implications. So we add all these
		//clauses too to check them again as ATEs.
		for (i = 0; i < c->size; ++i){
			l = main_litData + c->lits[i];
			for (j = 0; j < l->numOccsUsed; ++j){
				cS_touchClause(l->occs[j]);
			}
		}
	}
	cSS0_touchClause(c);
	if (cSS0_touchClause == cSS0_touchClause_satelite){
		//We might need to add something here.
	}
	cSS1_touchClause(c);
	if (cSS1_touchClause == cSS1_touchClause_satelite){
		//We might need to add something here.
	}

	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (lQ_touchLiteral == lQ_touchLiteral_fld){
		//All literals contained in the clause are added to the failed literal queue with opposite sign.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i];
			lQ_touchLiteral(l);
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_acce){
		//We are currently doing clause elimination.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i];	//The literal from the occurrence. We update the literal score if in the queue.
			//If not, we do not add it there.
			if (GET_LIT_Q_POS(l) > 0) lQ_touchLiteral(l);
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_nhbr){
		//If the clause is binary, we touch all the opposite literals to add it to the NHBR queue.
		if (c->size == 2){
			l = main_litData - c->lits[0];
			lQ_touchLiteral(l);
			l = main_litData - c->lits[1];
			lQ_touchLiteral(l);
		}
	}

	//LITERAL STACK.
	if (lS_touchLiteral == lS_touchLiteral_ternary){
		//If the clause has size 3, we want to add the literals of this clause to the stack.
		if (c->size == 3){
			l = main_litData + c->lits[0];
			lS_touchLiteral(l);
			l = main_litData + c->lits[1];
			lS_touchLiteral(l);
			l = main_litData + c->lits[2];
			lS_touchLiteral(l);
		}
	}

	//VARIABLE PRIORITY QUEUE.
	//Then, we touch all the variables.
	if (vQ_touchVariable == vQ_touchVariable_niver){
		//We are currently NIVER. If a clause is removed, then all variables currently in this clause must be touched, as
		//they all gain a resolution partner. We do touch only those variable that are already on the stack.
		for (i = 0; i < c->size; ++i){
			if ((main_varData + abs(c->lits[i]))->vQPos > 0)	vQ_touchVariable(main_varData + abs(c->lits[i]));
		}
	} else if (vQ_touchVariable == vQ_touchVariable_ge) {
		//We are currently doing GE. If a clause is added, then an additional gate might be available now for any of the
		//variables that is contained in this added clause.
		for (i = 0; i < c->size; ++i){
			vQ_touchVariable(main_varData + abs(c->lits[i]));
		}
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_pl){
		//Adding a clause cannot create pure literals. Nothing to be done here.
	}
}

void prep_intern_QSUpdateLiteralDeletion(clause* c, int32_t lit){
	//Handle all the queue and stack updates if a clauses literal is deleted.
	int32_t i, j;
	literal* l;

	//CLAUSE PRIORITY QUEUE.
	//We touch all the necessary clauses.
	if (cQ_touchClause == cQ_touchClause_se){
		cQ_touchClause(c);
		//If we do subsumption elimination and we remove a literal from clause c, then the above touching did all the
		//work already. There is nothing else to be done.
	}

	//CLAUSE STACKS.
	if (cS_touchClause == cS_touchClause_str){
		cS_touchClause(c);
	} else if (cS_touchClause == cS_touchClause_ate){
		//We are currently doing asymmetric tautology elimination. Clause c became shorter, all clauses sharing a remaining
		//literal might get new implications because. of this. They must all be checked.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i];
			for (j = 0; j < l->numOccsUsed; ++j){
				cQ_touchClause(l->occs[j]);
			}
		}
	}

	if (cSS0_touchClause == cSS0_touchClause_satelite){
		//We might need to add something here.
	}
	if (cSS1_touchClause == cSS1_touchClause_satelite){
		//We might need to add something here.
	}


	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (lQ_touchLiteral == lQ_touchLiteral_fld){
		//We touch all the opposite literals of the clause. Since c just got shorter, this might trigger a unit propagation.
		for (i = 0; i < c->size; ++i){
			l = main_litData - c->lits[i]; //The literal can become false and trigger a UP.
			lQ_touchLiteral(l);
			//We also grab all the literals that can falsify the literal itself. Therefore, we walk through the literals
			//implication list and add all these literals as well.
			for (j = 0; j < l->numImpUsed; j+=2){
				lQ_touchLiteral(main_litData - (int32_t) (addr_t) l->implies[j]);
			}
		}
	} else if (lQ_touchLiteral == lQ_touchLiteral_acce){
		//We are currently doing clause elimination. We add the opposite of the literal we just removed.
		l = (main_litData - lit);
		lQ_touchLiteral(l);
	} else if (lQ_touchLiteral == lQ_touchLiteral_nhbr){
		//If the clause became binary, we touch all the literals in it with opposite sign.
		if (c->size == 2){
			for (i = 0; i < c->size; ++i){
				l = main_litData - c->lits[i]; //The literal can become false and trigger a UP.
				lQ_touchLiteral(l);
			}
		}
	}

	//LITERAL STACK.
	if (lS_touchLiteral == lS_touchLiteral_ternary){
		//If the clause has size 3, we want to add the literals of this clause to the stack.
		if (c->size == 3){
			l = main_litData + c->lits[0];
			lS_touchLiteral(l);
			l = main_litData + c->lits[1];
			lS_touchLiteral(l);
			l = main_litData + c->lits[2];
			lS_touchLiteral(l);
		}
	}

	//VARIABLE PRIORITY QUEUE.
	//Then, we touch the variables.
	if (vQ_touchVariable == vQ_touchVariable_niver){
		//Since we removed another occurrence of the variable belonging to lit, the variable must again be put on the
		//variable priority queue during NIVER.
		vQ_touchVariable(main_varData + abs(lit));
	} else if (vQ_touchVariable == vQ_touchVariable_ge) {
		//We are currently doing GE. Removing a literal from a clause might enable the clause to act in a gate definition.
		//We touch all the remaining variables in this clause.
		for (i = 0; i < c->size; ++i){
			vQ_touchVariable(main_varData + abs(c->lits[i]));
		}
	}

	//VARIABLE STACK.
	if (vS_touchVariable == vS_touchVariable_pl){
		//We touch the variable for the variable stack, as the variable might now have lost all occurrences for one literal.
		vS_touchVariable(main_varData + abs(lit));
	}

}

void prep_intern_disableClause(clause* c){
	//A helper method to disable clauses (temporarily). It updates queues.
	if (!IS_CLS_DISABLED(c)){
		//Disable the unwanted clause.
		clauses_disable(c);
		//Update the preprocessors queues and stacks.
		prep_intern_QSUpdateClauseDisabling(c);
	}
}

void prep_intern_enableClause(clause* c){
	//A helper method to enable clauses (that where disabled temporarily). Updates queues.
	if (IS_CLS_DISABLED(c)){
		//Enable the clause.
		clauses_enable(c);
		//Update the preprocessor queues and stacks.
		prep_intern_QSUpdateClauseEnabling(c);
	}
}

void prep_intern_abandonClause(clause* c){
	if (!IS_CLS_DISABLED(c)) {
		//Disable the unwanted clause.
		clauses_disable(c);
		//Update the preprocessors queues and stacks.
		prep_intern_QSUpdateClauseDisabling(c);
	}
	//If the clause was a learned one, we can even free its memory by totally deleting it.
	if (IS_CLS_LEARNED(c)){	clauses_deleteCls(c); }
}

void prep_intern_deleteClause(clause* c){
	if (!IS_CLS_DISABLED(c)) {
		//Disable the unwanted clause.
		clauses_disable(c);
		//Update the preprocessors queues and stacks.
		prep_intern_QSUpdateClauseDisabling(c);
		//We delete the clause.
	}
	clauses_deleteCls(c);
}


void prep_intern_deleteLitInClause(clause* c, int32_t lit, int32_t litPos){
	//Delete the literal in the clause.
	clauses_deleteLit(c, lit, litPos);
	//Update the preprocessors queues and stacks.
	prep_intern_QSUpdateLiteralDeletion(c, lit);
}

void prep_intern_makeClauseRemoved(clause* c, int32_t litOfInterest, int32_t byAction, int32_t numCovered, coveredLit *covLits){
	//This method will make the given clause a removed one. Removed clauses are like disabled clauses, except that they
	//cannot be deleted. The litOfInterest is either a blocking literal, or a literal that was resolved on during NIVER or
	//gate extraction. Anyway, if covered literals are used, we get the array with the covered literals and can use this
	//for retaining the information in the clause. WARNING: THIS METHOD WILL CONSUME THE COVERED LITERALS ARRAY. IF
	//COVERED LITERALS ARE TO BE STORED AFTER THIS METHOD WAS CALLED, YOU MUST ALLOCATE A NEW ONE AFTER CALLING THIS METHOD.
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

	//Update the preprocessors queues and stacks.
	prep_intern_QSUpdateClauseDisabling(c);

	//We must now create a new removed clause and transfer all the necessary information of the old clause to the new clause.
	//Afterwards, we delete the clause completely to free the memory.
	remClause* rC = malloc(sizeof(remClause));
	rC->byAction = byAction;
	rC->size = c->size;
	rC->lits = c->lits;
	rC->litOfInterest = litOfInterest;

	//Take care of the CLA literals.
	if (numCovered == 0){
		//We do not need to store covered literals.
		rC->numCovered = 0;
		rC->coveredLits = NULL;
	} else {
		//We need to retain the covered literals computed for this clause, but we can shrink it in size, and make the
		//pointer in the clause point to this array. Thereby, we spare us the copying.
		rC->numCovered = numCovered;
		covLits = realloc(covLits, sizeof(coveredLit)*numCovered);
		rC->coveredLits = covLits;
	}

	ADD_REMCLS_TO_REMCLAUSES_LIST(rC);

	//After the transfer of the clause into the removed clauses set, we can destroy the rest of c. The c->lits must, however,
	//survive, because we address them from the removed clause as well.
	free(c->msgs);
	free(c->occPos);
	free(c);
}

void prep_intern_addOriginalClause(int32_t* literals, int32_t size){
	//We use this method to add a new clause to the formula during preprocessing. It must not be unit. First, we add the clause.
	clauses_addOrig(literals, size);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	prep_intern_QSUpdateClauseEnabling(f.clauses_eo[f.m_eo_used-1]);
}

void prep_intern_addLearnedClause(int32_t* literals, int32_t size){
	//We use this method to add a new clause to the formula during preprocessing. It must not be unit. First, we add the clause.
	clauses_addLearned(literals, size);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	prep_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void prep_intern_addLearnedBinaryClause(int32_t lit1, int32_t lit2){
	//To add a new clause as learned clause in (i.e. NHBR clauses).
	clauses_addLearnedBinary(lit1, lit2);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	prep_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void prep_intern_addLearnedTernaryClause(int32_t lit1, int32_t lit2, int32_t lit3){
	//To add a new clause as learned clause in (i.e. NHBR clauses).
	clauses_addLearnedTernary(lit1, lit2, lit3);
	//We then call the method to update all the queues and stacks for this new clause. The new clause is the last one added.
	prep_intern_QSUpdateClauseEnabling(f.clauses_el[f.m_el_used-1]);
}

void prep_intern_disableVariable(variable* v){
	//This method is used to disable a variable during the preprocessing. It will, in the end, call for the standard method
	//to disable a variable, but it will also take care of updating queues and stacks that are variable related.
	vQ_delete(v);
	vS_delete(v);
	lQ_delete(main_litData + v->id);
	lQ_delete(main_litData - v->id);
	lS_delete(main_litData + v->id);
	lS_delete(main_litData - v->id);
	variables_disable(v);
}

void prep_intern_reviveCls(){
	remClause *rc;
	clause *d;
	variable *v;
	literal *l;
	uint64_t rcSig;
	int32_t i,j, *litC, *litD, smallLit, smallLitSize;
	#ifdef VERBOSE_PREP
	int32_t numRevived = 0;
	printf("c   PREP: Reviving previously removed clauses...\nc   PREP:\t");
	#endif

	//This method will go through all the clauses we have removed previously (by NIVER, ACCE, GE). Clauses that are brought
	//back are treated as learned clauses. It will bring back those, that meet the following conditions:
	//0. The clause was not removed because of NIVER or GE since they contain variables we would re-introduce.
	//1. The clause does not contain any disables, assigned, or equivalent variables.
	//2. The clause does not contain a literal that has no occurrences.
	//3. The clause is not subsumed by any binary clause.
	//4. The clause is not subsumed by any large clause.

	for (i = 0; i < f.m_rem_used; ++i){
		rc = f.clauses_rem[i];
		//0. We ignore clauses that have been removed because of NIVER and GE.
		if (rc->byAction == PREP_REMCLS_NIVER || rc->byAction == PREP_REMCLS_GE) continue;
		//1. We ignore clauses that contain disabled variables. We stamp all the literals in the clause to know which of
		//them are certainly found in there. We also keep a look out for the literal with the smallest occurrence list.
		++main_litStamp;
		litC = rc->lits;
		smallLitSize = INT32_MAX;
		smallLit = 0;
		rcSig = 0ULL;
		while (HAS_CLS_MORE_LITS(rc, litC)){
			v = main_varData + abs(*litC);
			if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v) || GET_VAR_ISEQUIVTO(v) != 0){
				//We found a disabled, assigned, or equivalent variable in clause rc. Cannot be revived. Early break.
				break;
			}
			(main_litData + *litC)->stamp = main_litStamp;
			rcSig += GET_LIT_SIG(*litC);
			if ((main_litData + *litC)->numOccsUsed < smallLitSize){
				smallLit = *litC;
				smallLitSize = (main_litData + *litC)->numOccsUsed;
				//We will not re-introduce variables that currently have no occurrences.
				if (smallLitSize == 0) break;
			}
			++litC;
		}
		if (HAS_CLS_MORE_LITS(rc, litC)){
			//An early break means we did not check all the literals in the removed clause. We found something undesirable.
			continue;
		}
		//2. We ignore the clause if it is binary subsumed. We grab the opposite of what is found in the clause for one literal
		//after another. If there is a binary clause subsuming rc, then there must be an implication found in any of the lists
		//that is a stamped literal.
		litC = rc->lits;
		while (HAS_CLS_MORE_LITS(rc, litC)){
			l = main_litData - *litC;
			for (j = 0; j < l->numImpUsed; j+=2){
				if ((main_litData + ((int32_t)(addr_t)l->implies[j]))->stamp == main_litStamp){
					//We found a stamped literal. That means, there is a binary clause subsuming rc. We will not revive it.
					break;
				}
			}
			if (j < l->numImpUsed){
				//We did not look at all the implications. This means we found one that was stamped. And that means we do not
				//add rc.
				break;
			}
			++litC;
		}
		if (HAS_CLS_MORE_LITS(rc, litC)){
			//An early break means we did not check all the literals in the removed clause. We found something undesirable.
			continue;
		}

		//3. We ignore the clause if it is subsumed by a large clause of size larger 2. We grab one literal after another
		//and check if the occurrence list contains a clause that subsumes rc.
		litC = rc->lits;
		while (HAS_CLS_MORE_LITS(rc, litC)){
			l = main_litData + *litC;
			for (j = 0; j < l->numOccsUsed; j+=2){
				d = l->occs[j];
				if (d->size < 3 || d->size > rc->size) continue;
				litD = d->lits;
				//Check if any of the literals in this occurrence is not stamped. If so, d cannot subsume rc.
				while (HAS_CLS_MORE_LITS(d, litD)){
					if ((main_litData + *litD)->stamp != main_litStamp){
						//This literal is not stamped. Clause d is not subsuming.
						break;
					}
					++litD;
				}
				if (!HAS_CLS_MORE_LITS(d,litD)){
					//If we did not break early, then all the literals in d are stamped. This means d subsumes rc.
					break;
				}
			}
			if (j < l->numOccsUsed){
				//We did not look at all the occurrences. This means we found one that subsumes rc.
				break;
			}
			++litC;
		}
		if (HAS_CLS_MORE_LITS(rc, litC)){
			//An early break means we did not check all the literals in the removed clause. We found something undesirable.
			continue;
		}

		//Otherwise, all our checks did not hinder us in bringing back the clause. We do it.
		#ifdef VERBOSE_PREP
		printf("%p(%d) ", (void*)rc, rc->size);
		if (++numRevived % 8 == 0) printf("\nc   PREP:\t");
		#endif
		//We must make sure that all clauses that are subsumed by this addition are getting disabled. We walk through the
		//smallest occurrence list and check this (this list belongs to the smallLit we grabbed in the beginning).
		l = main_litData + smallLit;
		for (j = 0; j < l->numOccsUsed; ++j){
			d = l->occs[j];
			//Check if the signature of rc contains something not found in d. If so, rc cannot subsume d. Likewise for the size.
			if (d->size < rc->size || (((rcSig & ~d->sig) != 0ULL))) continue;
			//Check if all literals from rc are found in d.
			litC = rc->lits;
			while (HAS_CLS_MORE_LITS(rc,litC)){
				litD = d->lits;
				while(HAS_CLS_MORE_LITS(d, litD)){
					if (*litD == *litC){
						break;
					}
					++litD;
				}
				if (!HAS_CLS_MORE_LITS(d, litD)){
					//We have checked all the literals in d but did not abort. This means we did not find *litC in there.
					break;
				}
				++litC;
			}
			if (HAS_CLS_MORE_LITS(rc,litC)){
				//If we did not check all literals in d, then one of the literals was not found in rc.
				continue;
			}
			//Otherwise, clause d is subsumed as soon as we re-enable rc.
			prep_intern_abandonClause(d);
		}

		//We now revive rc by just adding a learned clause based on its literals.
		clauses_addLearned(rc->lits, rc->size);
	}

	#ifdef VERBOSE_PREP
	printf("[%d]\n", numRevived);
	#endif

}

void prep_intern_outputToFile(){
	int32_t i,j;
	remClause *rc;
	clause *c;
	#ifdef VERBOSE_PREP
	printf("c   PREP: Writing preprocessing result to file... ");
	#endif

	fprintf(param_prepOutput, "c DIMETHEUS SAT Solver -- Preprocessor output.\n");
	fprintf(param_prepOutput, "c   Version: %d.%d.%d\n", VERSION_MA, VERSION_MI, VERSION_RE);
	fprintf(param_prepOutput, "c\n");
	fprintf(param_prepOutput, "c Statistics on the simplification.\n");
	fprintf(param_prepOutput, "c   %d var remaining (%3.2f %% removed),\n",
			f.n_vars_e_used, 100.0f - 100.0f*(((float)f.n_vars_e_used) / ((float)f.n_initial)));
	fprintf(param_prepOutput, "c   %d cls remaining (%3.2f %% removed).\n",
			f.m_el_used + f.m_eo_used, 100.0f - 100.0f*(((float)(f.m_el_used + f.m_eo_used)) / ((float)f.m_initial)));
	fprintf(param_prepOutput, "c   %f seconds.\n", ((float_ty)clock())/((float_ty)CLOCKS_PER_SEC));
	fprintf(param_prepOutput, "c\n");
	fprintf(param_prepOutput, "c Clauses that were removed (NIVER, BLOCKED, COVERED) in the order they were removed:\n");
	for (j = 0; j < f.m_rem_used; ++j){
		rc = f.clauses_rem[j];
		fprintf(param_prepOutput, "c RCLS %p", (void*) rc);
		fprintf(param_prepOutput, " LITOFINTEREST:%6d\tNUMCOVERED:%5d\tLITS: ", rc->litOfInterest, rc->numCovered);
		if (rc->lits == NULL){
			fprintf(param_prepOutput, "NULL ");
		} else {
			for (i = 0; i < rc->size; ++i){
				fprintf(param_prepOutput, "%d ", rc->lits[ i ]);
			}
		}
		fprintf(param_prepOutput, "\t COVEREDLITS: ");
		for (i = 0; i < rc->numCovered; ++i){
			fprintf(param_prepOutput, "%d(%d) ", rc->coveredLits[i].lit, rc->coveredLits[i].coveredBy);
		}
		fprintf(param_prepOutput, "\n");
	}
	fprintf(param_prepOutput, "c\n");
	fprintf(param_prepOutput, "p cnf %d %d\n", f.n_initial, f.m_el_used + f.m_eo_used);
	for (j = 0; j < f.m_eo_used; ++j){
		c = f.clauses_eo[j];
		for (i = 0; i < c->size; ++i){
			fprintf(param_prepOutput, "%d ", c->lits[i]);
		}
		fprintf(param_prepOutput, "0\n");
	}
	for (j = 0; j < f.m_el_used; ++j){
		c = f.clauses_el[j];
		for (i = 0; i < c->size; ++i){
			fprintf(param_prepOutput, "%d ", c->lits[i]);
		}
		fprintf(param_prepOutput, "0\n");
	}

	#ifdef VERBOSE_PREP
	printf(" done.\n");
	#endif
}

void prep_extern_preprocessing(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_prep_component_totalCalls;
	#endif

	if (prep_returnCode == PREP_UNKNOWN) prep_intern_disableMissingVars();

	if (param_prepStrategy != PREP_STRATEGY_NULL){
		#ifdef VERBOSE_PREP
		printf("c   PREP: PREPROCESSING...\n");fflush(stdout);
		#endif

		prep_strategy_reset();
		prep_strategy_init();
		if (prep_returnCode == PREP_UNKNOWN){
			prep_strategy_execute();
		}
		prep_strategy_dispose();

		//We revive clauses and output the result of the preprocessing.
		if (prep_returnCode == PREP_UNKNOWN) {
			prep_intern_disableMissingVars();
			if (param_prepReviveCls == 1) prep_intern_reviveCls();
			if (param_prepOutput != NULL) prep_intern_outputToFile();
			f.n_afterPrep = f.n_vars_e_used;
		}
	}

	#ifdef VERBOSE_PREP
	if (param_prepStrategy != PREP_STRATEGY_NULL){
		printf("c   PREP: Done with preprocessing:\n");
		printf("c   PREP:    %d var remaining (%3.2f %% removed),\n",
				f.n_vars_e_used, 100.0f - 100.0f*(((float)f.n_vars_e_used) / ((float)f.n_initial)));
		printf("c   PREP:    %d cls remaining (%3.2f %% removed).\n",
				f.m_el_used + f.m_eo_used, 100.0f - 100.0f*(((float)(f.m_el_used + f.m_eo_used)) / ((float)f.m_initial)));
	}
	#endif

	#ifdef COLLECTSTATS
	if (param_prepStrategy != PREP_STRATEGY_NULL){
		int32_t i;
		for (i = 0; i < f.m_eo_used; ++i){
			stats_prep_clauseCoverRatePrep += pow(2, -((int32_t)f.clauses_eo[i]->size));
		}
		for (i = 0; i < f.m_el_used; ++i){
			stats_prep_clauseCoverRatePrep += pow(2, -((int32_t)f.clauses_el[i]->size));
		}
	} else {
		stats_prep_clauseCoverRatePrep = stats_prep_clauseCoverRateOrig;
	}
	stats_prep_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void prep_intern_disableMissingVars(){
	variable *v;
	literal *lPos, *lNeg;
	int32_t i;
	#ifdef VERBOSE_PREP
	printf("c   PREP: Disabling variables with zero occurrences... ");
	uint32_t numDis = 0;
	#endif

	for (i = 1; i < f.n_initial+1; ++i){
		v = main_varData + i;
		if (IS_VAR_DISABLED(v)) continue;
		lPos = main_litData + i;
		lNeg = main_litData - i;
		if (GET_LIT_NUMOCCS_USED(lPos) == 0 && GET_LIT_NUMOCCS_USED(lNeg) == 0){
			variables_disable(v);
		#ifdef VERBOSE_PREP
		++numDis;
		#endif
		}
	}

	#ifdef VERBOSE_PREP
	printf("done, %d disabled.\n", numDis);
	printf("c   PREP: %d remaining.\n", f.n_vars_e_used);fflush(stdout);
	#endif
}

void prep_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_prep_component_totalCalls;
	#endif
	#ifdef VERBOSE_PREP
	printf("c   PREP: RESET...\n");
	#endif
	//Reset the preprocessing.
	prep_returnCode = PREP_UNKNOWN;

	prep_strategies_pointer_reset();
	#ifdef COLLECTSTATS
	stats_prep_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void prep_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_prep_component_totalCalls;
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: INITIALIZE...\n");
	#endif
	prep_returnCode = PREP_UNKNOWN;
	prep_strategies_pointer_init();

	#ifdef COLLECTSTATS
	stats_prep_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void prep_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_prep_component_totalCalls;
	#endif
	#ifdef VERBOSE_PREP
	printf("c   PREP: DISPOSE...\n");
	#endif
	//For freeing all preprocessing related memory.
	if (param_prepOutput != NULL){
		fclose(param_prepOutput);
		param_prepOutput = NULL;
	}
	prep_returnCode = PREP_UNKNOWN;
	prep_strategies_pointer_dispose();
	#ifdef COLLECTSTATS
	stats_prep_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
