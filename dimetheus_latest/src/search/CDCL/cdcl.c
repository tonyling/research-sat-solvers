/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "cdcl.h"

void cdcl_extern_printVersion(){
	printf("[Search          ] [Conflict Driven Clause Learning] :: %3d.%-4d :: %s",
			CDCL_VERSION_MA, CDCL_VERSION_MI, CDCL_VERSION_NAME);
}

void cdcl_intern_finalizeLearned(){
	//This method will check all learned clauses and updates their first and second watcher such that the first watcher has
	//the highest decLevel in which it was assigned and the second watcher is the literal in the clause that has the second
	//highest Level in which it was assigned. Furthermore, this method updates the representative literal for the clause and
	//its signature.
	uint64_t sig;
	cdclClause *c;
	cdclVariable *v;
	int32_t w1, w1level, w2, w2level, lit, litNum;
	uint32_t learnedNum, smallestLbd = UINT32_MAX;
	for (learnedNum = cdcl_cls_l_used - cdcl_numLearned; learnedNum < cdcl_cls_l_used; ++learnedNum){
		c = cdcl_cls_l[learnedNum];

		//We can ignore unit clauses here because they do not have watchers, signatures or representative literals.
		if (GET_CDCL_CLS_SIZE(c) == 1) {
			#ifdef COLLECTSTATS
			if (STATS_CDCL_LEARNEDLENGTH_MAX > 1) ++stats_cdcl_learnedLength[1];
			#endif
			smallestLbd = 0;
			continue;
		}
		w1 = 0;
		w1level = 0;
		w2 = 0;
		w2level = 0;

		sig = 0ULL;
		for (litNum = 0; litNum < GET_CDCL_CLS_SIZE(c); ++litNum){
			lit = GET_CDCL_CLS_LITNUM(c, litNum);
			sig |= GET_CDCL_LIT_SIG(lit);
			v = cdcl_variables + abs(lit);
			if (GET_CDCL_VAR_PTR_DECLEVEL(v) > w1level){
				//Replace the old second watcher with the old first watcher.
				w2level = w1level;
				w2 = w1;
				//Set the new best watcher found so far.
				w1level = GET_CDCL_VAR_PTR_DECLEVEL(v);
				w1 = lit;
			} else if (GET_CDCL_VAR_PTR_DECLEVEL(v) > w2level){
				w2level = GET_CDCL_VAR_PTR_DECLEVEL(v);
				w2 = lit;
			}
		}
		//We have grabbed the w1, w2 and updated signature. We update the clause with this information.
		SET_CDCL_CLS_W1(c, w1);
		SET_CDCL_CLS_W2(c, w2);
		SET_CDCL_CLS_REPLIT(c, w1);
		SET_CDCL_CLS_SIG(c, sig);
		//We shrink the clause literals and occurrence position arrays to optimal size.
		SHRINK_CDCL_CLS_TO_OPTIMAL_SIZE(c);
		//We set the clauses LBD value based on the current assignments on the stack.
		if (GET_CDCL_CLS_SIZE(c) > 2) {
			cdcl_intern_setClsLbd(c);
			if (GET_CDCL_CLS_LBD(c) < smallestLbd){
				smallestLbd = GET_CDCL_CLS_LBD(c);
			}
		} else {
			SET_CDCL_CLS_LBD(c, 2);
			if (smallestLbd > 2){
				smallestLbd = 2;
			}
		}
		#ifdef COLLECTSTATS
		if (GET_CDCL_CLS_SIZE(c) > STATS_CDCL_LEARNEDLENGTH_MAX){
			++stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
		} else {
			++stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(c)];
		}
		#endif
	}
	//In case the variable selection heuristic supports LBD knowledge, we now allow it to increase the variable activity. This
	//is independent of the conflict analysis increase. This time, the new learned clauses LBD value is take into account.
	if (smallestLbd > 1){
		cdcl_selectVarRule_reward(smallestLbd, cdcl_conflictClause);
	}

}

uint32_t cdcl_intern_addUnitsForgetOther(){
	//This method assumes that we are in DL0, that UP has the pointers updated correctly, and at least one clause in
	//cdcl_cls_l[cdcl_cls_l_used - cdcl_numNewLearnedClauses] to  cdcl_cls_l[cdcl_cls_l_used - 1] is a unit clause.
	//It will check all these clauses and checks if it is unit. If not, it simply reduces this clause back to initial
	//state. If it is a unit, it will put the assignment on the stack and then reduce the clause. After it performed
	//all its operations, it will reduce the cdcl_cls_l_used value by the value in cdcl_numNewLearnedClauses (no new
	//clauses are added to the formula).
	cdclClause *c;
	cdclLiteral *l;
	cdclVariable *v;
	uint32_t i = cdcl_cls_l_used - cdcl_numLearned;
	while (i < cdcl_cls_l_used){
		//Get the next learned clause.
		c = cdcl_cls_l[i];
		//Check if the clause is of size 1.
		if (GET_CDCL_CLS_SIZE(c) == 1u){
			//Yes, it is. We must add this clause to the stack. If this fails, we can deduce the unsatisfiability of the
			//formula (in case two opposite units have been learned and not already removed).
			//The first literal in the clause must be the one we want to propagate. We grab the corresponding literal pointer
			//and check if the opposite is already on the stack. If so, we have a global conflict and can deduce UNSAT. If
			//not, we check if the literal is already on the stack with same sign. If so we do not add it. If not, we add it.
			l = cdcl_literals + c->lits[0];
			if (CDCL_UP_IS_OPPLIT_ON_S_PTR(l)){
				//This is a conflict.
				return 0u;
			} else if (!CDCL_UP_IS_LIT_ON_S_PTR(l)){
				//The literal itself is not on the stack. Add it.
				v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
				CDCL_UP_ADD_DEC_PTR(l, v);
			}
		}
		//Anyway, we reduce the clause back to initial state.
		SHRINK_LEARNED_CDCL_CLS_INPLACE(c);
		++i;
	}
	//In case everything went out without detecting a conflict, we reduce the number of learned clauses again -- no clauses
	//shall be added after learning a unit.
	cdcl_cls_l_used -= cdcl_numLearned;
	return 1u;
}

void cdcl_intern_addNonUnitsWithAsserts(){
	//This method assumes that we are in the target decision level after learning, and that UP has the pointers updated
	//correctly for this level. This method adds all the learned clauses to the formula. Each of the learned clauses that
	//have only one unassigned watcher will provide this watcher as unit propagation in the current decision level.
	cdclClause *c;
	cdclLiteral *l;
	cdclVariable *v;
	uint32_t i = cdcl_cls_l_used - cdcl_numLearned, num;
	int32_t litC, litC2;

	while (i < cdcl_cls_l_used){
		c = cdcl_cls_l[i];

		//Add the clause to all the occurrence lists of the literals it contains.
		for (num = 0; num < GET_CDCL_CLS_SIZE(c); ++num){
			litC = GET_CDCL_CLS_LITNUM(c,num);
			ADD_CDCL_CLS_TO_LIT_OCCS(c, litC, num);
		}

		//If it is a binary clause, append its implications. If it is larger, add it to the watcher lists.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			//The clause is binary.
			ADD_CDCL_LIT_IMPLICATIONS(c, litC, litC2);
		} else {
			//The clause is of size at least 3.
			ADD_CDCL_CLS_TO_LIT_W1LIST(c, c->w1);
			ADD_CDCL_CLS_TO_LIT_W2LIST(c, c->w2);
		}

		//Check if WL2 is already assigned. If this is the case then this clause adds WL1 as assignment on the stack for this
		//decision level (asserting literal). If WL2 is still unassigned, we have at least two unassigned literals, and as
		//such, the clause will not imply anything in this level.
		if (IS_CDCL_LIT_UNSAT(GET_CDCL_CLS_W2(c))){
			//The second watcher is assigned -- the clause implies its first watcher in this decision level.
			l = GET_CDCL_LIT_PTR_FROM_ID(GET_CDCL_CLS_W1(c));
			if (CDCL_UP_IS_OPPLIT_ON_S_PTR(l)){
				//This is a conflict.
				printf("s UNKNOWN\n");
				printf("c ERROR. Adding an implication assert did not work.\n"); exit(0);
			} else if (!CDCL_UP_IS_LIT_ON_S_PTR(l)){
				//The literal itself is not on the stack. Add it.
				v = cdcl_variables + abs(GET_CDCL_CLS_W1(c));
				CDCL_UP_ADD_IMP_PTR(l, v, c);
			}
		}
		++i;
	}
	//We have now included all the newly learned clauses in the occurrence lists and appended them in the implication or
	//watcher lists. We have also put all the implications provided by these clauses into the current decision level.
}

void cdcl_intern_addNonUnitsWithoutAsserts(){
	//This method assumes that we are in the target decision level after learning, and that none of the learned clauses is
	//a unit clause in this level (all the asserting levels of the learned clauses are larger then the current DL).
	cdclClause *c;
	uint32_t i = cdcl_cls_l_used - cdcl_numLearned, num;
	int32_t litC, litC2;
	while (i < cdcl_cls_l_used){
		c = cdcl_cls_l[i];

		//Add the clause to all the occurrence lists of the literals it contains.
		for (num = 0; num < GET_CDCL_CLS_SIZE(c); ++num){
			litC = GET_CDCL_CLS_LITNUM(c,num);
			ADD_CDCL_CLS_TO_LIT_OCCS(c, litC, num);
		}

		//If it is a binary clause, append its implications. If it is larger, add it to the watcher lists.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			//The clause is binary.
			ADD_CDCL_LIT_IMPLICATIONS(c, litC, litC2);
		} else {
			//The clause is of size at least 3.
			ADD_CDCL_CLS_TO_LIT_W1LIST(c, c->w1);
			ADD_CDCL_CLS_TO_LIT_W2LIST(c, c->w2);
		}

		++i;
	}
	//We have now included all the newly learned clauses in the occurrence lists and appended them in the implication or
	//watcher lists.
}

void cdcl_intern_assignEquivVars_assign(cdclVariable *v){
	cdclVariable *anc;
	int32_t ancID = GET_CDCL_VAR_PTR_ISEQUIVTO(v);

	//Check if the variable is already assigned or if it has an equivalence ancestor.
	if (!IS_CDCL_VAR_PTR_UNASSIGNED(v) || ancID == 0) return;

	//The variable is unassigned and has an equivalence ancestor. We ensure that the ancestor is assigned.
	anc = cdcl_variables + abs(ancID);
	cdcl_intern_assignEquivVars_assign(anc);
	if (IS_CDCL_VAR_PTR_UNASSIGNED(anc)) return;
	//The ancestor of variable v has now been assigned to its correct value. We will now assign v. If the ancID is negative,
	//we will assign it to the opposite assignment, otherwise we will assign it to the same assignment.
	if (ancID > 0){
		//Variable v must have the same assignment as its ancestor.
		if (IS_CDCL_VAR_PTR_TRUE(anc)){
			SET_CDCL_VAR_PTR_ASS_1(v);
		} else {
			SET_CDCL_VAR_PTR_ASS_0(v);
		}
	} else {
		//Variable v must have the opposite assignment as its ancestor.
		if (IS_CDCL_VAR_PTR_TRUE(anc)){
			SET_CDCL_VAR_PTR_ASS_0(v);
		} else {
			SET_CDCL_VAR_PTR_ASS_1(v);
		}
	}
	#ifdef VERBOSE_CDCL
	printf("%d[%d]->%d ", GET_CDCL_VAR_PTR_ID(v), ancID, IS_CDCL_VAR_PTR_TRUE(v) ? 1 : 0);
	#endif
}

void cdcl_intern_assignEquivVars(){
	//This method ensures that all equivalent variables are assigned correctly. Wech check all the not yet assigned variables
	//and try to find their assignment recursively.
	cdclVariable *v;
	uint32_t i;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:   Performing assignment of equivalent variables...");fflush(stdout);
	#endif
	for (i = 1; i < f.n_initial+1U; ++i){
		//We again ignore disabled variables.
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) continue;
		v = cdcl_variables + i;
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
			cdcl_intern_assignEquivVars_assign(v);
		}
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
}

void cdcl_intern_addLearnedBinaryClause(int32_t lit1, int32_t lit2){
	//Adds a new binary as learned clause.

	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;

	//First, we ensure that we can learn a new clause of sufficient size.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, 3);
	SET_CDCL_CLS_SIZE(newClause, 2);
	SET_CDCL_CLS_ACTIVITY(newClause, ONE);
	SET_CDCL_CLS_LBD(newClause, 2);

	//Set the clauses literals. Terminate with 0.
	SET_CDCL_CLS_LITNUM(newClause, 0, lit1);
	SET_CDCL_CLS_LITNUM(newClause, 1, lit2);
	SET_CDCL_CLS_LITNUM(newClause, 2, 0);

	//Set the clauses representative.
	SET_CDCL_CLS_REPLIT(newClause, lit1);

	//Set the clauses signature.
	sig |= GET_CDCL_LIT_SIG(lit1);
	sig |= GET_CDCL_LIT_SIG(lit2);
	SET_CDCL_CLS_SIG(newClause, sig);

	//Add the clause to the occurrence lists.
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit1, 0);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit2, 1);

	//Add the clauses implications.
	ADD_CDCL_LIT_IMPLICATIONS(newClause, lit1, lit2);
}

void cdcl_intern_addLearnedTernaryClause(int32_t lit1, int32_t lit2, int32_t lit3){
	//Adds a new learned clause of size 3.

	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;

	//First, we ensure that we can learn a new clause of sufficient size.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, 4);
	SET_CDCL_CLS_SIZE(newClause, 3);
	SET_CDCL_CLS_ACTIVITY(newClause, ONE);
	SET_CDCL_CLS_LBD(newClause, 3);

	//Set the clauses literals. Terminate with 0.
	SET_CDCL_CLS_LITNUM(newClause, 0, lit1);
	SET_CDCL_CLS_LITNUM(newClause, 1, lit2);
	SET_CDCL_CLS_LITNUM(newClause, 2, lit3);
	SET_CDCL_CLS_LITNUM(newClause, 3, 0);

	//Set the clauses representative.
	SET_CDCL_CLS_REPLIT(newClause, lit1);

	//Set the clauses signature.
	sig |= GET_CDCL_LIT_SIG(lit1);
	sig |= GET_CDCL_LIT_SIG(lit2);
	sig |= GET_CDCL_LIT_SIG(lit3);
	SET_CDCL_CLS_SIG(newClause, sig);

	//Add the clause to the occurrence lists.
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit1, 0);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit2, 1);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit3, 2);

	//Add the clauses watchers.
	SET_CDCL_CLS_W1(newClause, lit1);
	ADD_CDCL_CLS_TO_LIT_W1LIST(newClause, lit1);
	SET_CDCL_CLS_W2(newClause, lit2);
	ADD_CDCL_CLS_TO_LIT_W2LIST(newClause, lit2);
	cdcl_maintenanceRule_incAct(newClause);
}

void cdcl_intern_setClsLbd(cdclClause *c){
	//This method computes the LBD value of the clause. It uses the current assignments found on the decision stack.
	uint32_t i;
	int32_t lit;

	//We use the literal stamp to stamp the variables understood as decision levels.
	++cdcl_litStamp;
	SET_CDCL_CLS_LBD(c, 0);
	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		lit = GET_CDCL_CLS_LITNUM(c,i);
		//Check if the decision level in which this literal was assigned is already marked.
		if (GET_CDCL_VAR_STAMP(GET_CDCL_VAR_DECLEVEL(abs(lit))) != cdcl_litStamp){
			SET_CDCL_VAR_STAMP(GET_CDCL_VAR_DECLEVEL(abs(lit)), cdcl_litStamp);
			SET_CDCL_CLS_LBD_INC(c);
		}
	}
}

void cdcl_intern_updateLbdInLastLevel(){
	//For all clauses that are currently acting as reasons in the last level and that are learned, we update the LBD.
	cdclLiteral **stackPtr = cdcl_up_end-1;
	cdclClause *c;

	while (stackPtr > cdcl_decStackPtrs[cdcl_decLevel]){
		c = GET_CDCL_VAR_REASON(abs(GET_CDCL_LIT_ID_FROM_PTR(*stackPtr)));
		if (c != NULL){
			cdcl_intern_setClsLbd(c);
		}
		--stackPtr;
	}
}

void cdcl_intern_writeFormulaToFile(){
	//This method will write the formula to the given filename in param_cdclOutput.
	int32_t i,j;
	cdclClause *c;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Writing preprocessing result to file... ");
	#endif

	fprintf(param_cdclOutput, "c DIMETHEUS SAT Solver -- CDCL Module output.\n");
	fprintf(param_cdclOutput, "c   Version: %d.%d.%d\n", VERSION_MA, VERSION_MI, VERSION_RE);
	fprintf(param_cdclOutput, "c\n");

	fprintf(param_cdclOutput, "p cnf %d %d\n", f.n_initial, cdcl_cls_o_used + cdcl_cls_l_used);
	for (j = 0; j < cdcl_cls_o_used; ++j){
		c = cdcl_cls_o[j];
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			fprintf(param_cdclOutput, "%d ", GET_CDCL_CLS_LITNUM(c,i));
		}
		fprintf(param_cdclOutput, "0\n");
	}
	for (j = 0; j < cdcl_cls_l_used; ++j){
		c = cdcl_cls_l[j];
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			fprintf(param_cdclOutput, "%d ", GET_CDCL_CLS_LITNUM(c,i));
		}
		fprintf(param_cdclOutput, "0\n");
	}

	#ifdef VERBOSE_CDCL
	printf(" done.\n");
	#endif
}

void cdcl_extern_learn(uint32_t prepareAnew){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_cdcl_component_totalCalls;
	#endif
	//Starting the search. The prepareAnew value can have three different values. If it is 1, we completely re-initialize the
	//whole CDCL module with the current global formula. This includes updating the activities and phases of the variables.
	//If it is 0, we just continue where we left off.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Starting...\n");
	#endif
	if (prepareAnew == 1){
		//We completely re-prepare the CDCL module with the current global formula.
		#ifdef VERBOSE_CDCL
		printf("c     CDCL: Preparing call (FULL)...\n");
		printf("c     CDCL:   Data-structures...\n");
		#endif
		cdcl_returnCode = CDCL_UNKNOWN;
		//Prepare the snapshot view of the formula for the CDCL module.
		cdcl_variables_prepareCall();
		cdcl_literals_prepareCall();
		cdcl_clauses_prepareCall();
		#ifdef VERBOSE_CDCL
		printf("c     CDCL:     Variables: %u, Clauses: %u original, %u learned\n",cdcl_vars_e_used,cdcl_cls_o_used,cdcl_cls_l_used);
		printf("c     CDCL:   Tools and Plug-ins...\n");
		#endif
		//Prepare the call to all tools of the CDCL.
		cdcl_decStack_prepare();
		cdcl_up_prepare();

		//Prepare the call to all plug-ins.
		cdcl_selectVarRule_prepare();
		cdcl_selectDirRule_prepare();
		cdcl_conflictAnalysisRule_prepare();
		cdcl_strLearnedRule_prepare();
		cdcl_strOtherRule_prepare();
		cdcl_computeBJLRule_prepare();
		cdcl_restartRule_prepare();
		cdcl_maintenanceRule_prepare();
		cdcl_inprocessingRule_prepare();
		cdcl_abortRule_prepare();

		//Prepare local parameters.
		#ifdef VERBOSE_CDCL
		printf("c     CDCL:   Resetting all globals...\n");
		#endif
		cdcl_conflictClause 		= NULL;
		cdcl_numLearned 			= 0;
		cdcl_numConflicts 			= 0;
		cdcl_targetBJLevel 			= 0;
	} else {
		//We want the CDCL to continue where it left off. Nothing must be done here.
		#ifdef VERBOSE_CDCL
		printf("c     CDCL: Preparing call (CONTINUOUS %u assigned variables %u/%u original/learned clauses).\n",
				CDCL_DECSTACK_GET_NUM_ASSIGNMENTS(), cdcl_cls_o_used, cdcl_cls_l_used);
		#endif
	}

	#ifdef COLLECTSTATS
	stats_cdcl_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Performing search...\n");fflush(stdout);
	#endif

	while(1){
		//Check if we are in a conflicting state or must advance the search with a new decision.
		if (cdcl_conflictClause == NULL) {
			//We are not in a conflicting state.
			//We check, if the abort rule holds. If so, the CDCL achieved its task.
			if (cdcl_abortRule_check()){
				break;
			}

			//Advance the search. Check if more variables can be selected for an assignment.
			cdcl_decVar = cdcl_selectVarRule_select();
			if (cdcl_decVar == NULL){
				//No further variables can be assigned. The formula must be satisfiable.
				#ifdef VERBOSE_CDCL
				printf("c     CDCL:   No more variables remain. The (sub-)formula must be SAT.\n");
				#endif
				cdcl_returnCode = CDCL_SAT;
				break;
			}
			//We got another variable to assign. We pick a direction for it. The result is the literal we want to satisfy.
			cdcl_decLit = cdcl_selectDirRule_select();
			//We add the decision literal to the decision stack in the current decision level.
			CDCL_UP_ADD_DEC_PTR(cdcl_decLit, cdcl_decVar);
			//We propagate. This returns NULL if no conflict is found or the conflict clause otherwise.
			cdcl_conflictClause = cdcl_up_propagate();
		} else {
			//We are in a conflicting state.
			++cdcl_numConflicts;

			//First, we check if we are in DL 0. If so, the formula is UNSAT.
			if (cdcl_decLevel == 0){
				//Yes, global conflict. We can stop.
				#ifdef VERBOSE_CDCL
				printf("c     CDCL:   Conflict in DL 0. The (sub-)formula must be UNSAT.\n");
				#endif
				cdcl_returnCode = CDCL_UNSAT;
				break;
			}
			//Conflict analysis. Write the learned clauses in cdcl_cls_l from position cdcl_cls_l_used. The number x of newly
			//learned clauses will be returned. They are then found in the cdcl_cls_l array from position cdcl_cls_l_used - x
			//to position cdcl_cls_l_used - 1.
			cdcl_numLearned = cdcl_conflictAnalysisRule_analyze();

			//Strengthen learned clauses.
			cdcl_strLearnedRule_str();

			//We update the LBD value of all learned clauses that are acting as reasons in the last decision level.
			cdcl_intern_updateLbdInLastLevel();

			//After the learned clauses have been strengthened and all other operations on them have been done, we update the
			//watchers such that they are the literals in the clause with highest and second highest decision level.
			cdcl_intern_finalizeLearned();

			//Compute the target back-jump level based on the learned clauses.
			cdcl_targetBJLevel = cdcl_computeBJLRule_compute();

			//Check, if the learned clauses can strengthen any other clause. This might change the BJ level.
			cdcl_targetBJLevel = cdcl_strOtherRule_str();

			//We forward the information of the back-jump and its distance to the restart rule.
			cdcl_restartRule_signalBackjump();

			//If the target back-jump level is not equal 0, we can still check if it is time for a restart. If it is time
			//for a restart, we back-jump to level 1, but fully erase this level as well. The learned clauses are added, but
			//without any implications. If we do not restart, we back-jump to the desired level, and put all the clauses
			//into the formula -- all clauses that have its asserting literal in this level then add a new unit propagation.
			if (cdcl_targetBJLevel == 0){
				//We have learned at least one unit clause and must revert to DL0. This means that we just back-jump to DL0
				//in the APPEND mode and append all unit assignments. No clauses are added to the formula in this scheme. All
				//the learned clauses will be removed from cdcl_cls_l. Afterwards, we propagate.
				cdcl_decStack_backJump(0, 0);//Back-jump to DL0 in APPEND mode.
				//We now add the unit clauses we have learned. This checks if a conflict is already discovered because two
				//opposite units are learned.
				if (cdcl_intern_addUnitsForgetOther()){
					//No conflict was discovered by just adding the unit clauses. We now perform the propagation. Recall, that
					//we are in DL 0 here. If this works out without running into a conflict, the UP and the decision stack
					//are already updated afterwards, so no additional action must be taken.
					cdcl_conflictClause = cdcl_up_propagate();
					//After propagating something in decision level zero, we can do some in-processing along with a reduction
					//in the clause database. This is only necessary if we do not run into a conflict.
					if (cdcl_conflictClause == NULL) {
						cdcl_conflictClause = cdcl_inprocessingRule_inprocess();
					}
				}
			} else {
				//We have not learned a unit clause. We will now first check if we must back-jump to DL 1. If so, we do it.
				//After that, we add all the newly learned clauses to the formula. Those, only one unassigned watcher will
				//provide unit propagations for DL1 in the other watcher.
				if (cdcl_targetBJLevel == 1u){
					cdcl_decStack_backJump(1,0);//Back-jump to DL1 in APPEND mode.
					cdcl_intern_addNonUnitsWithAsserts();//Add learned clauses and their implications for DL.
					cdcl_conflictClause = cdcl_up_propagate();
				} else {
					//Otherwise, we check if it is time for a restart.
					if (cdcl_restartRule_check()){
						//We restart. That is, we back-jump to DL1 in ERASE mode, add all the learned clauses. We do not do
						//any unit propagation here as none are possible (all clauses must contain at least two unassigned
						//literals). We therefore reset the conflict clause to NULL.
						cdcl_decStack_backJump(1, 1);//Back-jump to DL 1 in erase mode (restart).
						cdcl_intern_addNonUnitsWithoutAsserts();//Add learned clauses (none can be unit here).
						cdcl_conflictClause = NULL;
						cdcl_abortRule_signalRestart();
					} else {
						//We do not restart. We back-jump to the desired decision level in APPEND mode, add all the learned
						//clauses and get their implications on the stack. Afterwards we propagate and set the conflict clause.
						cdcl_decStack_backJump(cdcl_targetBJLevel,0);//Back-jump to target level in APPEND mode.
						cdcl_intern_addNonUnitsWithAsserts();//Add learned clauses and their implications in DL.
						cdcl_conflictClause = cdcl_up_propagate();
						if (cdcl_conflictClause == NULL){
							//We check if it is time to do clause database maintenance.
							if (cdcl_maintenanceRule_check()) {
								cdcl_maintenanceRule_maintain();
								cdcl_abortRule_signalMaintenance();
							}
						}
					}
				}
			}

			#ifdef VERBOSE_CDCL
			if (cdcl_numConflicts % 10000u == 0) cdcl_printProgress();
			#endif
		}
	}

	//Finally, we ensure that the satisfying assignment satisfies the original formula that the CDCL had to solve.
	if (cdcl_returnCode == CDCL_SAT){
		cdcl_variables_assignRemainingVars();
		cdcl_inprocessingRules_extern_reconstructSolution();
	}

	#ifdef COLLECTSTATS
	if (cdcl_returnCode == CDCL_SAT){
		++stats_cdcl_return_SAT;
	} else if (cdcl_returnCode == CDCL_UNKNOWN){
		++stats_cdcl_return_UNKNOWN;
	} else if (cdcl_returnCode == CDCL_UNSAT){
		++stats_cdcl_return_UNSAT;
	} else {
		++stats_cdcl_return_ERROR;
	}
	stats_cdcl_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void cdcl_extern_reOrganizeRemainingVars(){
	//This method will re-organize the variable priority queue for variables that are still enabled -- i.e. not yet assigned.
	//It will respect the guidance delivered by the main algorithm, or simply re-compute the activities and directions.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_cdcl_component_totalCalls;
	#endif
	//First of all, we forcibly re-scale all the variable activities back into [0,1].
	cdcl_selectVarRule_rescale(1);
	//We must now update the variable priority queue. For each of the variable, we check if they are still in the priority
	//queue. If they are, we set their new activity.
	cdcl_selectVarRules_extern_varActUpdate();
	//We must now update the variable phases for all variables in the priority queue.
	cdcl_selectDirRules_extern_dirUpdate();
	#ifdef COLLECTSTATS
	stats_cdcl_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}


void cdcl_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_cdcl_component_totalCalls;
	#endif
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Component reset... \n");
	#endif
	cdcl_returnCode = CDCL_UNKNOWN;

	cdcl_clauses_reset();
	cdcl_literals_reset();
	cdcl_variables_reset();

	cdcl_decStack_reset();
	cdcl_up_reset();

	cdcl_selectVarRules_resetPlugin();
	cdcl_selectDirRules_resetPlugin();
	cdcl_conflictAnalysisRules_resetPlugin();
	cdcl_strLearnedRules_resetPlugin();
	cdcl_strOtherRules_resetPlugin();
	cdcl_computeBJLRules_resetPlugin();
	cdcl_restartRules_resetPlugin();
	cdcl_maintenanceRules_resetPlugin();
	cdcl_inprocessingRules_resetPlugin();
	cdcl_abortRules_resetPlugin();
	#ifdef COLLECTSTATS
	stats_cdcl_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_cdcl_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

}

void cdcl_initModule(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdcl_returnCode = CDCL_UNKNOWN;

	cdcl_clauses_init(); 						if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_literals_init();						if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_variables_init(); 						if (cdcl_returnCode != CDCL_UNKNOWN) return;

	cdcl_decStack_init(); 						if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_up_init(); 							if (cdcl_returnCode != CDCL_UNKNOWN) return;

	cdcl_selectVarRules_initPlugin(); 			if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_selectDirRules_initPlugin(); 			if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_conflictAnalysisRules_initPlugin();	if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_strLearnedRules_initPlugin();			if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_strOtherRules_initPlugin();			if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_computeBJLRules_initPlugin(); 			if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_restartRules_initPlugin();				if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_maintenanceRules_initPlugin(); 		if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_inprocessingRules_initPlugin(); 		if (cdcl_returnCode != CDCL_UNKNOWN) return;
	cdcl_abortRules_initPlugin(); 		        if (cdcl_returnCode != CDCL_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_cdcl_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_cdcl_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void cdcl_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Component dispose... \n");
	#endif

	if (param_cdclOutput != NULL){
		fclose(param_cdclOutput);
		param_cdclOutput = NULL;
	}

	cdcl_clauses_dispose();
	cdcl_literals_dispose();
	cdcl_variables_dispose();

	cdcl_decStack_dispose();
	cdcl_up_dispose();

	cdcl_selectVarRules_disposePlugin();
	cdcl_selectDirRules_disposePlugin();
	cdcl_conflictAnalysisRules_disposePlugin();
	cdcl_strLearnedRules_disposePlugin();
	cdcl_strOtherRules_disposePlugin();
	cdcl_computeBJLRules_disposePlugin();
	cdcl_restartRules_disposePlugin();
	cdcl_maintenanceRules_disposePlugin();
	cdcl_inprocessingRules_disposePlugin();
	cdcl_abortRules_disposePlugin();
	#ifdef COLLECTSTATS
	stats_cdcl_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_cdcl_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

#ifdef VERBOSE_CDCL
void cdcl_printProgress(){
	printf("c     CDCL:   %7.2f sec. Conflicts: %7u, Current DL[fixed]: %5u[%5u], DL0: %5u, Learned: %7u[%7u]\n",
			((float_ty)clock())/((float_ty)CLOCKS_PER_SEC),
			cdcl_numConflicts,
			cdcl_decLevel,
			CDCL_DECSTACK_GET_NUM_ASSIGNMENTS(),
			(uint32_t)(cdcl_decStackPtrs[1] - cdcl_decStackPtrs[0] - 1),
			cdcl_cls_l_used,
			cdcl_maintenanceRules_targetLearned);
}
#endif
