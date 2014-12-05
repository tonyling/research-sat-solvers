/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "handleresult.h"

void handleresult_disposeEQVars(){
	uint32_t i;
	if (handleresult_eqVars != NULL){
		for (i = 1; i < f.n_initial+1; ++i){
			if (handleresult_eqVars[i].equiv != NULL){
				free(handleresult_eqVars[i].equiv);
				handleresult_eqVars[i].equiv = NULL;
			}
		}
		free(handleresult_eqVars);
		handleresult_eqVars = NULL;
	}
}

void handleresult_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_handleresult_component_totalCalls;
	#endif
	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT: RESET...\n");
	#endif
	handleresult_returnCode = HANDLERESULT_UNKNOWN;
	handleresult_eqVars = NULL;

	//Reset all modules.
	solwriter_resetModule();

	#ifdef COLLECTSTATS
	stats_handleresult_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void handleresult_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_handleresult_component_totalCalls;
	#endif
	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT: INITIALIZE...\n");
	#endif
	handleresult_returnCode = HANDLERESULT_UNKNOWN;
	handleresult_eqVars = NULL;

	//Initialize all modules.
	solwriter_initModule();	if (solwriter_returnCode != SOLWRITER_UNKNOWN){ handleresult_returnCode = STARTUP_ERROR; return; }

	#ifdef COLLECTSTATS
	stats_handleresult_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void handleresult_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_handleresult_component_totalCalls;
	#endif
	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT: DISPOSE...\n");
	#endif
	handleresult_returnCode = HANDLERESULT_UNKNOWN;
	handleresult_disposeEQVars();

	//Dispose all modules.
	solwriter_disposeModule();

	#ifdef COLLECTSTATS
	stats_handleresult_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

//Solution reconstruction.
void handleresult_assignRemainingVars(){
	//This method ensures that all variables are assigned correctly.
	variable *v;
	uint32_t i;

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     A total assignment must be created...\n");
	printf("c   HANDLERESULT:       Assigning remaining non-equivalent variables: ");
	#endif

	//In this sweep, we identify variables that are unassigned and have no equivalence ancestor. We assign them to zero.
	for (i = 1; i < f.n_initial+1U; ++i){
		v = main_varData + i;
		//Ignore variables that are assigned or have an equivalence ancestor.
		if (!IS_VAR_UNASSIGNED(v) || GET_VAR_ISEQUIVTO(v) != 0U) continue;
		//Otherwise, assign the variable to zero.
		SET_VAR_ASS_0(v);
		#ifdef VERBOSE
		printf("%d ", -GET_VAR_ID(v));
		#endif
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("done.\n");
	printf("c   HANDLERESULT:       Assigning remaining equivalent variables: ");
	#endif

	//In this sweep, we look for the unassigned variables that do have an ancestor. We assign them correctly.
	for (i = 1; i < f.n_initial+1U; ++i){
		v = main_varData + i;
		//Ignore assigned variables.
		if (!IS_VAR_UNASSIGNED(v)) continue;
		//If the variable is unassigned, then it must have an equivalence ancestor. We will assign all the equivalent
		//variables recursively.
		variables_assignEquivVars_assign(v);
		#ifdef VERBOSE_HANDLERESULT
		printf("%d[%d] ", IS_VAR_TRUE(v) ? GET_VAR_ID(v) : -GET_VAR_ID(v), GET_VAR_ISEQUIVTO(v));
		#endif
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("done.\n");
	printf("c   HANDLERESULT:     A total assignment has been created.\n");
	#endif
}

variable* handleresult_getRootRepresentative(variable* v){
	//Recursively get the root representative for the variable, return itself if it has no equivalence ancestor.
	variable *anc;
	int32_t ancID = GET_VAR_ISEQUIVTO(v);

	if (ancID == 0){
		return v;
	}
	anc = main_varData + abs(ancID);
	return handleresult_getRootRepresentative(anc);
}

void handleresult_prepareEQVars(){
	//We use this method to initialize the EQVars array, and correctly gather all the equivalence information.
	uint32_t i, rootID;
	variable *v, *root;
	//Allocate the equivalent variables data-structure.
	handleresult_eqVars = malloc(sizeof(eqVar)*(f.n_initial+1));
	if (handleresult_eqVars == NULL){
		printf("c ERROR. The eqVars array could not be initialized by HANDLERESULT. Out of memory?\n");
		handleresult_returnCode = HANDLERESULT_ERROR;
		return;
	}
	//Initialize. Every variable is not represented by anything else and contains only itself in its equivalence class.
	for (i = 1; i < f.n_initial+1; ++i){
		v = f.vars + i;
		handleresult_eqVars[i].representedBy = NULL;
		handleresult_eqVars[i].numRepresents = 1;
		handleresult_eqVars[i].equiv = NULL;
		if (handleresult_returnCode == HANDLERESULT_UNKNOWN){
			handleresult_eqVars[i].equiv = malloc(sizeof(variable*)*1);
			if (handleresult_eqVars[i].equiv == NULL){
				handleresult_returnCode = HANDLERESULT_ERROR;
			} else {
				handleresult_eqVars[i].equiv[0] = v;
			}
		}
	}

	if (handleresult_returnCode == HANDLERESULT_ERROR){
		printf("c ERROR. The eqVars.equiv array could not be initialized by HANDLERESULT. Out of memory?\n");
		return;
	}

	//Now, for every variable that has an equivalence, we find the root representative.
	for (i = 1; i < f.n_initial+1; ++i){
		v = f.vars + i;
		//First, we get the root equivalence representative for this variable.
		root = handleresult_getRootRepresentative(v);
		//If the variable is its own representative, then nothing is to be done.
		if (root == v) continue;
		//Otherwise, we must ensure that v knows it is represented by the root, and add v to the equivalence set of root.
		rootID = GET_VAR_ID(root);
		handleresult_eqVars[i].representedBy = handleresult_eqVars + rootID;
		handleresult_eqVars[rootID].equiv = realloc(handleresult_eqVars[rootID].equiv, sizeof(variable*)*(handleresult_eqVars[rootID].numRepresents+1));
		handleresult_eqVars[rootID].equiv[handleresult_eqVars[rootID].numRepresents++] = v;
	}
}

void handleresult_flipVarWithEquivs(variable *v){
	//Flips the variable v along with all the equivalent variables.
	uint32_t numToFlip, i;
	variable **listToFlip;
	//First, we grab the equivalence class of v that must now be completely flipped.
	if (handleresult_eqVars[GET_VAR_ID(v)].representedBy == NULL){
		//The variable represents its own equivalence class. We flip all the variables in this class.
		numToFlip = handleresult_eqVars[GET_VAR_ID(v)].numRepresents;
		listToFlip = handleresult_eqVars[GET_VAR_ID(v)].equiv;
	} else {
		//The variable is part of a different equivalence class. We pick this one instead for flipping.
		numToFlip = handleresult_eqVars[GET_VAR_ID(v)].representedBy->numRepresents;
		listToFlip = handleresult_eqVars[GET_VAR_ID(v)].representedBy->equiv;
	}
	//Flip all the literals in the class.
	for (i = 0; i < numToFlip; ++i){
		SET_VAR_ASS_FLIP(listToFlip[i]);
		#ifdef VERBOSE_HANDLERESULT
		printf("%d ", IS_VAR_TRUE(listToFlip[i]) ? GET_VAR_ID(listToFlip[i]) : -GET_VAR_ID(listToFlip[i]));
		#endif
	}
}

void handleresult_reconstructSolution(){
	//Given a satisfying assignment for the formula, we use this method to modify the assignment in such a way that it
	//satisfies the removed (blocked/covered/NIVER) clauses. Then, the assignment satisfies the original formula.
	int32_t i,j, lit, *litC, isSat, coveredSat, coveredBy;
	coveredLit *covPtr;
	variable *v;
	remClause *rc;

	if (f.m_rem_used == 0){
		#ifdef VERBOSE_HANDLERESULT
		printf("c   HANDLERESULT:     Solution reconstruction not necessary.\n");
		#endif
		return;
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     Solution reconstruction necessary...\n");
	printf("c   HANDLERESULT:       Preparing equivalent variables data-structure...\n");
	#endif
	handleresult_prepareEQVars();
	if (handleresult_returnCode == HANDLERESULT_ERROR) return;

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:       Flipping: ");fflush(stdout);
	#endif

	//We now walk through all the removed clauses in the REVERSE order they where removed. We distinct between NIVER and
	//standard blocked clauses, and those removed by covered literal addition.
	for (i = f.m_rem_used-1; i > -1; --i){
		rc = f.clauses_rem[i];
		//Either a blocked clause or a clause removed by NIVER. We check if the clause is already satisfied. If so
		//we do nothing. If not, we flip the assignment to the literal of interest in the clause.
		litC = rc->lits;
		isSat = 0;
		while (HAS_CLS_MORE_LITS(rc,litC)){
			if (IS_LIT_SAT(*litC)){
				isSat = 1;
				break;
			}
			++litC;
		}
		if (!isSat){
			if (rc->coveredLits == 0){
				//No covered literals means standard (asymmetric) blocked clause.
				//If the clause is not yet satisfied, we must flip the assignment to the literal of interest.
				v = main_varData + abs(rc->litOfInterest);
				#ifdef VERBOSE_HANDLERESULT
				printf("R[%i]%p(", (int32_t)rc->byAction, (void*)rc);
				#endif
				handleresult_flipVarWithEquivs(v);
				#ifdef VERBOSE_HANDLERESULT
				printf(") ");
				#endif
			} else {
				//Covered literals are present which means this is a clause removed by covered clause elimination.
				//We first check if the clauses additional covered literals are satisfied. If not, we must flip the
				//assignment of the blocking literal. We first check if the clause is satisfied via the covered literals.
				coveredSat = 0;
				for (j = 0; j < rc->numCovered; ++j){
					if (IS_LIT_SAT(rc->coveredLits[j].lit)){
						 ++coveredSat;
						 break;
					}
				}

				if (!coveredSat){
					//Not yet satisfied. We first flip the assignment to the blocking literal.
					v = main_varData + abs(rc->litOfInterest);
					#ifdef VERBOSE_HANDLERESULT
					printf("C%p(", (void*)rc);
					#endif
					handleresult_flipVarWithEquivs(v);
					#ifdef VERBOSE_HANDLERESULT
					printf(") ");
					#endif
				}
				//We now check if the original clause is satisfied. As long as this is not the case, we will keep removing
				//covered literals from the clause and flip the assignment to the literals covering them until the clause is
				//satisfied by the altered assignment.
				isSat = 0;
				litC = rc->lits;
				while (HAS_CLS_MORE_LITS(rc, litC)){
					if (IS_LIT_SAT(*litC)){
						isSat = 1;
						break;
					}
					++litC;
				}
				while (!isSat) {
					//We must remove (but remember) the last covered literal.
					--rc->numCovered;
					coveredBy = rc->coveredLits[rc->numCovered].coveredBy;
					//We now kill the covered literal in the clause that was added last.
					rc->coveredLits[rc->numCovered].lit = 0;
					rc->coveredLits[rc->numCovered].coveredBy = 0;

					//If the assignment falsifies the whole clause, we must flip the truth value of coveredBy. We first check
					//the additional covered literals. And then, we check the original literals. If and only if there is no
					//satisfied literal in any of the sets must the covering literal be flipped.
					coveredSat = 0;
					covPtr = rc->coveredLits;
					while ((lit = covPtr->lit) != 0){
						covPtr++;
						if (IS_LIT_SAT(lit)){
							coveredSat = 1;
							break;
						}
					}

					//Check if the clause is satisfied now.
					isSat = 0;
					litC = rc->lits;
					while (HAS_CLS_MORE_LITS(rc,litC)){
						if (IS_LIT_SAT(*litC)){
							isSat = 1;
							break;
						}
						++litC;
					}
					if (!coveredSat && !isSat){
						//The clause does not have any satisfying literals. We must flip the covering literals assignment.
						v = main_varData + abs(coveredBy);
						#ifdef VERBOSE_HANDLERESULT
						printf("C%p(", (void*) rc);
						#endif
						handleresult_flipVarWithEquivs(v);
						#ifdef VERBOSE_HANDLERESULT
						printf(") ");
						#endif
					}
					//We then check if the clause is satisfied now.
					isSat = 0;
					litC = rc->lits;
					while (HAS_CLS_MORE_LITS(rc,litC)){
						if (IS_LIT_SAT(*litC)){
							isSat = 1;
							break;
						}
						++litC;
					}
				}
			}
		//Now the clause should be satisfied and stay satisfied.
		}
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("done.\n");
	printf("c   HANDLERESULT:       Disposing equivalent variables data-structure...\n");
	#endif
	handleresult_disposeEQVars();
	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     Solution reconstruction done.\n");
	#endif
}

//Result verification.
void handleresult_verifyResultSAT(){
	int32_t i, j, isSat, lit;
	clause *c;
	remClause *rc;
	//This method will walk through all the original clauses (enabled and disabled ones) and check that it finds at least
	//one satisfied literal in all of them. It furthermore checks that all removed clauses are satisfied as well.

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     Verification...\n");
	#endif

	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		isSat = 0;
		for (j = 0; j < c->size; ++j){
			lit = c->lits[j];
			if (IS_LIT_SAT(lit)){
				++isSat;
				#ifndef COLLECTSTATS
				break;
				#endif
			}
		}
		if (!isSat) {
			printf("c ERROR. Solution fails verification. Enabled clause %p is not satisfied.\n", (void*)c);
			#ifdef VERBOSE
			clauses_printClause(c);
			#endif
			handleresult_returnCode = HANDLERESULT_WRONGSOLUTION;
			return;
		}
		#ifdef COLLECTSTATS
		++stats_general_countedClauses;
		if (isSat > STATS_GENERAL_COUNTSATLITS_MAX){
			++stats_general_satLitsByLength[STATS_GENERAL_COUNTSATLITS_MAX];
		} else {
			++stats_general_satLitsByLength[isSat];
		}
		#endif
	}

	for (i = 0; i < f.m_do_used; ++i){
		c = f.clauses_do[i];
		isSat = 0;
		for (j = 0; j < c->size; ++j){
			lit = c->lits[j];
			if (IS_LIT_SAT(lit)){
				++isSat;
				#ifndef COLLECTSTATS
				break;
				#endif
			}
		}
		if (!isSat) {
			printf("c ERROR. Solution fails verification. Disabled clause %p is not satisfied.\n", (void*)c);
			#ifdef VERBOSE
			clauses_printClause(c);
			#endif
			handleresult_returnCode = HANDLERESULT_WRONGSOLUTION;
			return;
		}
		#ifdef COLLECTSTATS
		++stats_general_countedClauses;
		if (isSat > STATS_GENERAL_COUNTSATLITS_MAX){
			++stats_general_satLitsByLength[STATS_GENERAL_COUNTSATLITS_MAX];
		} else {
			++stats_general_satLitsByLength[isSat];
		}
		#endif
	}

	for (i = 0; i < f.m_rem_used; ++i){
		rc = f.clauses_rem[i];
		isSat = 0;
		for (j = 0; j < rc->size; ++j){
			lit = rc->lits[j];
			if (IS_LIT_SAT(lit)){
				++isSat;
				break;
			}
		}
		if (!isSat) {
			printf("c ERROR. Solution fails verification. Removed clause %p is not satisfied.\n", (void*)rc);
			#ifdef VERBOSE
			clauses_printRemClause(rc);
			#endif
			handleresult_returnCode = HANDLERESULT_WRONGSOLUTION;
			return;
		}
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     Solution verified.\n");
	#endif

	//Otherwise, all clauses seem to be satisfied.
	handleresult_returnCode = HANDLERESULT_CORRECTSOLUTION;
}

void handleresult_verifyResultUNSAT(){
	//Not yet implemented.
	handleresult_returnCode = HANDLERESULT_CORRECTSOLUTION;
}

void handleresult_coarsenSolution(){
	//This method will try to remove assignments from a satisfying assignment, in such a way that all clauses contain at least
	//one satisfying literal.
	clause *c;
	variable *v;
	int32_t i, j, trueLit, numTrue, hasStampedSatLit, changes = 1;

	#ifdef VERBOSE_HANDLERESULT
	float_ty percentage = ZERO;
	printf("c   HANDLERESULT:     Coarsening...\n");
	#endif


	//First thing we check if there are any removed clauses in the formula (removed by NIVER, GE, ACCE and such). If so, we
	//return right away because the coarsening cannot correctly use such clauses.
	if (f.m_rem_used > 0){
		#ifndef SILENT
		printf("c   HANDLERESULT:     WARNING: Coarsening is enabled but removed clauses exists (removed\n");
		printf("c   HANDLERESULT:     WARNING: by NIVER, GE or ACCE). Coarsening is turned off in this case.\n");
		printf("c   HANDLERESULT:     WARNING: Disable the above preprocessing techniques to use coarsening.\n");
		#endif
		return;
	}

	//We will use the literal stamp to mark the variables (by marking the literals).
	++main_litStamp;


	//We will use the queues to collect both the currently used clauses as well as the variables. Lets collect the elements.
	cS_flush();
	vS_flush();
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		cS_push(c);
	}
	for (i = 0; i < f.m_do_used; ++i){
		c = f.clauses_do[i];
		cS_push(c);
	}
	for (i = 1; i <= f.n_initial; ++i){
		v = main_varData + i;
		vS_push(v);
	}

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:       Current elements ... in clause stack %d, in variable stack %d.\n", cS_leaf-1, vS_leaf-1);
	#endif

	//We will now start removing clauses from the stack in case they are satisfied by exactly one clause. We then mark the
	//responsible variable as "required" and remove it from the variable stack. In case no clause was removed (because they
	//all have more than one satisfying literal), we will mark one of the remaining variables in the variable stack, and
	//remove all the clauses satisfied by this assignment. We repeat, until no more clauses remain. In the end, all clauses
	//are satisfied by at least one marked literal. The remaining variables on the stack (those that are not yet marked)
	//can be unassigned in order to coarsen the resulting satisfying assignment.

	while(changes){
		changes = 0;
		#ifdef VERBOSE_HANDLERESULT
		printf("c   HANDLERESULT:       Reducing clause stack...");
		#endif
		for (i = 1; i < cS_leaf; ++i){
			//We first pick a clause and reset the data.
			c = cS[i];
			numTrue = 0;
			trueLit = 0;
			hasStampedSatLit = 0;
			//We now check if the clause has but one satisfying not-stamped literal or if it includes a stamped literal.
			for (j = 0; j < GET_CLS_SIZE(c); ++j){
				if (!IS_VAR_UNASSIGNED((main_varData + abs(GET_CLS_LITNUM(c,j))))){
					if (IS_LIT_SAT(GET_CLS_LITNUM(c,j))){
						//The literal is true.
						++numTrue;
						trueLit = GET_CLS_LITNUM(c,j);
						if (GET_LIT_NUM_STAMP(trueLit) == main_litStamp){
							//And the literal is stamped. We can remove the clause.
							hasStampedSatLit = 1;
							break;
						}
					}
				}
			}
			//Check if the clause is already forcibly satisfied by a stamped literal.
			if (hasStampedSatLit){
				//It is. We can just drop it.
				cS_delete(c);
				--i;
			} else if (numTrue == 1) {
				//Otherwise, if the clause contains only one not-stamped literal that satisfies it, we must force this satting.
				v = main_varData + abs(trueLit);
				SET_LIT_NUM_STAMP(trueLit, main_litStamp);
				SET_LIT_NUM_STAMP(-trueLit, main_litStamp);
				//The respective variable is no candidate for unassigning anymore.
				vS_delete(v);
				//We now check if we still have some candidates left.
				if (vS_leaf == 1) {
					//No candidates left. We did all the fuzz in vain.
					break;
				}
				//Yes, we have candidates left. We must perform another pass through the clauses to clean up.
				changes=1;
				//But we can already drop the current clause.
				cS_delete(c);
				--i;
			}
		}
		#ifdef VERBOSE_HANDLERESULT
		printf("%d clauses remaining, %d candidates variables remaining.\n", cS_leaf-1, vS_leaf-1);
		#endif

		//First, check if we can continue or if we have removed all the candidates already.
		if (vS_leaf == 1) {
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:       The set of variables is empty -- all variables must keep their assignment.\n");
			#endif
			break;
		}
		//We then check if the stack was reduced. If not and in case it still contains elements, we must force one more
		//variable assignment to satisfy at least one more clause.
		if (changes == 0 && cS_leaf > 1){
			//That is the case. First grab a clause and find SOME satisfying literal in it.
			c = cS_pop();
			changes=1;
			numTrue = 0;
			trueLit = 0;
			for (j = 0; j < GET_CLS_SIZE(c); ++j){
				if (!IS_VAR_UNASSIGNED((main_varData + abs(GET_CLS_LITNUM(c,j))))){
					if (IS_LIT_SAT(GET_CLS_LITNUM(c,j))){
						++numTrue;
						trueLit = GET_CLS_LITNUM(c,j);
						break;
					}
				}
			}
			//Stamp the literal and remove the corresponding variable from the set of candidates.
			v = main_varData + abs(trueLit);
			SET_LIT_NUM_STAMP(trueLit, main_litStamp);
			SET_LIT_NUM_STAMP(-trueLit, main_litStamp);
			vS_delete(v);
		}
	}

	if (vS_leaf > 1){
		#ifdef VERBOSE_HANDLERESULT
		percentage = (((float_ty)(vS_leaf-1)) / ((float_ty)f.n_afterPrep)) * 100.0;
		printf("c   HANDLERESULT:       Variable assignments that are unnecessary: %d (%f %%).\n", vS_leaf-1, percentage);
		#endif
		while ((v = vS_pop()) != NULL){
			SET_VAR_ASS_2(v);
		}
		#ifdef VERBOSE_HANDLERESULT
		printf("c   HANDLERESULT:       Unassigned: ");
		for (i=1; i <= f.n_initial; ++i){
			v = main_varData + i;
			if (IS_VAR_UNASSIGNED(v)){
				printf("%d ", v->id);
			}
		}
		printf("\n");
		#endif
	}

	cS_flush();
	vS_flush();

	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT:     Coarsening done.\n");
	#endif

}

void handleresult_extern_handle(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_handleresult_component_totalCalls;
	#endif
	#ifdef VERBOSE_HANDLERESULT
	printf("c   HANDLERESULT: HANDLE...\n");
	#endif

	//Check if we terminate in a special way.
	if (main_returnCode == SPECIALPARAM_HELP
				|| main_returnCode == SPECIALPARAM_VERSION
				|| main_returnCode == SPECIALPARAM_SPECS){
		#ifdef VERBOSE_HANDLERESULT
		printf("c   HANDLERESULT:   Nothing to handle (SPECIALPARAM).\n");
		#endif
		//Here, we just print the information requested.
		if (main_returnCode == SPECIALPARAM_HELP){
			params_extern_printHelp();
		} else if (main_returnCode == SPECIALPARAM_VERSION){
			main_printVersion();
		} else if (main_returnCode == SPECIALPARAM_SPECS){
			main_printSpecs();
		}
	} else {
		//No, we terminate in a normal way. Therefore, we print a real solution.
		if (main_returnCode == SIGNAL){
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:   Nothing to handle (SIGNAL).\n");
			#endif
			handleresult_returnCode = HANDLERESULT_UNKNOWN;
		} else if (main_returnCode == ERROR
				|| main_returnCode == CONFLICTINGPARAMS
				|| main_returnCode == BADPARAM
				|| main_returnCode == BADFORMULA
				|| main_returnCode == BADINPUTFORMAT){
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:   Nothing to handle (ERROR).\n");
			#endif
			handleresult_returnCode = HANDLERESULT_UNKNOWN;
		} else if (main_returnCode == UNKNOWN){
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:   Nothing to handle (UNKNOWN).\n");
			#endif
			handleresult_returnCode = HANDLERESULT_UNKNOWN;
		} else if (main_returnCode == UNSAT){
			//Verify the result UNSATISFIABLE.
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:   Handling UNSATISFIABLE.\n");
			#endif
			handleresult_returnCode = HANDLERESULT_UNKNOWN;
			//Verify the answer UNSATISFIABLE.
			handleresult_verifyResultUNSAT();
			if (handleresult_returnCode != HANDLERESULT_CORRECTSOLUTION){
				#ifdef VERBOSE_HANDLERESULT
				printf("c   HANDLERESULT:   The result FAILS VERIFICATION.\n");
				#endif
			}
		} else if (main_returnCode == SAT){
			//Verify the result SATISFIABLE.
			#ifdef VERBOSE_HANDLERESULT
			printf("c   HANDLERESULT:   Handling SATISFIABLE.\n");
			#endif
			handleresult_returnCode = HANDLERESULT_UNKNOWN;
			//We must assign all not yet assigned variables in order to get a total assignment for the formula. This must
			//respect equivalences, of course.
			handleresult_assignRemainingVars();
			//We can now perform solution reconstruction using the removed clauses data-structure.
			handleresult_reconstructSolution();
			if (handleresult_returnCode == HANDLERESULT_ERROR){
				#ifdef VERBOSE_HANDLERESULT
				printf("c   HANDLERESULT:   The result verification was unable to verify answer SATISFIABLE.\n");
				#endif
			} else {
				//Verify the result SATISFIABLE -- before performing coarsening. We can be shure that stuff works then.
				handleresult_verifyResultSAT();
				if (handleresult_returnCode != HANDLERESULT_CORRECTSOLUTION){
					#ifdef VERBOSE_HANDLERESULT
					printf("c   HANDLERESULT:   The result FAILS VERIFICATION.\n");
					#endif
				}
				//Now we will check whether the user wants us to coarsen the satisfying assignment.
				if (param_handleresultCoarsen == 1){
					//Yes, he wants us to.
					handleresult_coarsenSolution();
					//We now verify the result AGAIN based on the coarsened solution.
					handleresult_verifyResultSAT();
					if (handleresult_returnCode != HANDLERESULT_CORRECTSOLUTION){
						#ifdef VERBOSE_HANDLERESULT
						printf("c   HANDLERESULT:   The result FAILS VERIFICATION.\n");
						#endif
					}
				}
			}
		}


		//Finally, we can write the solution.
		solwriter_extern_writeSolution();
		if (solwriter_returnCode != SOLWRITER_UNKNOWN){
			printf("c ERROR. Writing the result somehow failed.\n");
			handleresult_returnCode = HANDLERESULT_ERROR;
			return;
		}
	}

	#ifdef COLLECTSTATS
	stats_handleresult_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
