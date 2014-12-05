/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategy_pmpsls_printHelp(){
	printf("c      %-3d: PMPSLS\n", SEARCH_STRATEGY_PMPSLS);
    printf("c           Behavior: Performs rho-sigma-PMP-i and uses the biases to guide SLS search [MP, SLS].\n");
}

void search_strategy_pmpsls_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (PMPSLS)...\n");
	#endif
	mp_resetModule();
	sls_resetModule();

	search_strategy_pmpsls_midBlockSize = 1;
	search_strategy_pmpsls_numToAssign = 0;
	search_strategy_pmpsls_maxMagForSLS = ZERO;
	search_strategy_pmpsls_nextToAssign = NULL;
}

void search_strategy_pmpsls_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (PMPSLS)...\n");
	#endif
	mp_initModule(); if (mp_returnCode != MP_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
	sls_initModule(); if (sls_returnCode != SLS_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }

	//Search strategy settings.
	search_strategy_pmpsls_midBlockSize = param_searchMIDBlockSize * f.n_vars_e_used;
	if (search_strategy_pmpsls_midBlockSize == 0U) ++search_strategy_pmpsls_midBlockSize;
	search_strategy_pmpsls_maxMagForSLS = param_searchMaxMagForSLS;
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH:   param_searchMIDBlockSize             = %f\n", param_searchMIDBlockSize);
	printf("c   SEACRH:   param_searchMaxMagForSLS             = %f\n", param_searchMaxMagForSLS);
	printf("c   SEARCH:   search_strategy_pmpsls_midBlockSize  = %u\n", search_strategy_pmpsls_midBlockSize);
	printf("c   SEARCH:   search_strategy_pmpsls_maxMagForSLS  = %f\n", search_strategy_pmpsls_maxMagForSLS);
	#endif

	search_strategy_pmpsls_numToAssign = 0;
	search_strategy_pmpsls_nextToAssign = malloc(sizeof(uint32_t)*(f.n_afterPrep+1));
	if (search_strategy_pmpsls_nextToAssign == NULL){
		printf("c ERROR. PMPSLS failed to initialize the assignment array. Out of memory?\n");
		search_returnCode = SEARCH_ERROR; return;
	}
}

void search_strategy_pmpsls_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (PMPSLS)...\n");
	#endif
	mp_disposeModule();
	sls_disposeModule();

	search_strategy_pmpsls_midBlockSize = 1;
	search_strategy_pmpsls_numToAssign = 0;
	search_strategy_pmpsls_maxMagForSLS = ZERO;
	free(search_strategy_pmpsls_nextToAssign);
	search_strategy_pmpsls_nextToAssign = NULL;
}

void search_strategy_pmpsls_execute(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (PMPSLS)...\n");
	#endif

	int32_t MID = 1, conflict = 0;

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Starting MID...\n");
	#endif

	do {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Calling for MP. VARS: %6d...\n", f.n_vars_e_used);
		#endif
		//We perform MP on the global formula and check on its result.
		mp_extern_iterate(1);
		//If MP did converge, we can compute biases, otherwise we stick to the current ones.
		if (mp_returnCode == MP_ERROR){
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: ERROR.\n");
			#endif
			printf("c ERROR. PMPSLS asked for MP but this resulted in an error (A). PMPSLS failed.\n");
			search_returnCode = SEARCH_ERROR; MID = 0; break;
		} else if (mp_returnCode == MP_CONVERGED) {
			//Yes, it did converge. Computing new biases is useful.
			mp_extern_computeBiases();
			#ifdef VERBOSE_SEARCH
			if (mp_returnCode == MP_CONVERGED_NONPARA){
				printf("c   SEARCH: CONVERGED-NON-PARAMAGNETIC (%f).\n", mp_globalMagnetization);
			} else {
				printf("c   SEARCH: CONVERGED-PARAMAGNETIC (%f).\n", mp_globalMagnetization);
			}
			#endif
			//In case the magnetization of the formula is small, we call the SLS for a single try on the sub-formula with
			//limited number of flips allowed.
			if (mp_globalMagnetization < search_strategy_pmpsls_maxMagForSLS){
				#ifdef VERBOSE_SEARCH
				printf("c   SEARCH: Calling for SLS because of low magnetization.\n");
				#endif
				sls_extern_tryRestartRules_switch(param_slsTryRestartRule);
				sls_extern_localSearch(1);
				if (sls_returnCode == SLS_SAT){
					break;
				} else if (sls_returnCode == SLS_ERROR){
					search_returnCode = SEARCH_ERROR;
					break;
				}
			}
		} else {
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: UNCONVERGED.\n");
			#endif
		}

		//After computing biases, we have a (possibly updated) set of variable activities and phases. We add the block size
		//many variables with highest activity to the variable priority queue.
		search_strategy_pmpsls_aquireNextVariables();

		//In case the queue is empty, we have no further assignments to make. The MID phase is over and the search is
		//handed over to the SLS.
		if (vQ_leaf < 2){
			//Nothing was added to the queue. No further MID is possible.
			MID = 0;
		} else {
			//Further variables have been added to the queue, so further MID is possible. We will first sort the variables
			//that are to be assigned. We then perform the assignment.
			search_strategy_pmpsls_orderToAssign();
			//We then perform the assignment. The result is either a conflict or no conflict.
			if (search_strategy_pmpsls_assignOrdered()){
				//The decimation ended in a conflict. From here on, we hand over the search to SLS.
				MID = 0;
				conflict = 1;
			}
		}
	} while (MID);

	//The MID phase is now over.
	if (search_returnCode == SEARCH_ERROR) return;

	//We check if the solution was provided by SLS, or what the MID result is.
	if (sls_returnCode == SLS_SAT){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: MID ended prematurely (%d variables, %d clauses).\n", f.n_vars_e_used, f.m_eo_used + f.m_el_used);
		printf("c   SEARCH: SLS found a solution. PMPSLS succeeded. Copying variable assignments.\n");
		#endif
		//The SLS did indeed find a solution.
		search_intern_transferSLSAss();
		search_returnCode = SEARCH_SAT;
	} else if (conflict) {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: MID ended (%d variables, %d clauses).\n", f.n_vars_e_used, f.m_eo_used + f.m_el_used);
		printf("c   SEARCH: MID ended in a CONFLICT. Storing phases and reverting temporary assignments.\n");
		#endif
		//We will now revert all the assignments that have been made so far. All assignments where only temporarily. After that
		//the SLS will perform the search with all the phases the variables had last.
		search_strategy_pmpsls_retainAllPhases();
		main_decStack_backJump(1,1,1);

		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Calling for SLS.\n");
		#endif

		//We now call for the SLS module to perform the search using the phases that have been gathered so far.
		sls_extern_tryRestartRules_switch(SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
		sls_extern_localSearch(1);
		if (sls_returnCode == SLS_UNKNOWN){
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: SLS was unable to find a satisfying assignment. SLS failed.\n");
			#endif
			//The SLS did not find a satisfying assignment. This does not necessarily mean that there is none, but in
			//the current search strategy, this is considered a failure.
			search_returnCode = SEARCH_UNKNOWN;
		} else if (sls_returnCode == SLS_SAT){
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: SLS found a solution. PMPSLS succeeded. Copying variable assignments.\n");
			#endif
			//The SLS did indeed find a solution.
			search_intern_transferSLSAss();
			search_returnCode = SEARCH_SAT;
		} else {
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: SLS return code unexpected after local search. We assume an error. PMPSLS failed.\n");
			#endif
			//In case we get this far, the SLS returned an unexpected return code. The strategy failed.
			search_returnCode = SEARCH_ERROR;
		}
	} else {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: MID ended (%d variables, %d clauses).\n", f.n_vars_e_used, f.m_eo_used + f.m_el_used);
		printf("c   SEARCH: MID ended after assigning all variables. Calling the SLS is not necessary.\n");
		#endif
		search_returnCode = SEARCH_SAT;
	}
}

void search_strategy_pmpsls_aquireNextVariables(){
	//This method will find the numToSort variables from the f.n_vars_e_used ones that have the highest activity. The idea is
	//as follows: First, pick the numToSort first elements available in f.vars_e and add them to a priority queue,
	//such that the smallest element is at the first position. Then, read the numToSort+1 to n remaining elements.
	//Whenever an element is read with a higher activity than the one at the front of the queue, delete the first element
	//and add the newly read one. After reading the n-th element, we have the numToSort best ones in the queue,
	//such that the one with the smallest activity is at the first position.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	uint32_t usedVars = 0, count = 0;
	variable *v;
	//We enqueue the first vNum elements from the beginning of the f.vars_e list. We do not check their score yet, we need
	//them all to first initialize the priority queue to the minimum size.
	while (usedVars < search_strategy_pmpsls_midBlockSize && count < f.n_vars_e_used){
		v = f.vars_e[count++];
		if (!IS_VAR_UNASSIGNED(v)) continue;
		//Recall, that the priority queue for variables uses an int32_t to keep the scores. Smaller values are better scores.
		//The larger the activity, the farther at the end the element will be. The first element will always be the one with
		//the worst activity.
		SET_VAR_Q_SCORE(v, GET_VAR_Q_SCORE_FROM_VAR_ACTIVITY(v));
		vQ_enqueue(v);
		++usedVars;
	}
	//For the remaining elements in in f.vars_e, we check if they have a higher activity than the worst variable. If so, we
	//kick the old worst variable, add the new variable, and then check which one is at the start of the queue. This will be
	//the new worst element.
	while (count < f.n_vars_e_used){
		v = f.vars_e[count];
		if (IS_VAR_UNASSIGNED(v) && (GET_VAR_Q_SCORE_FROM_VAR_ACTIVITY(v)) > GET_VAR_Q_SCORE(vQ[1])){
			//The new variable is better than the worst one we have in the queue. We kill the old variable and add the new.
			SET_VAR_Q_SCORE(v, GET_VAR_Q_SCORE_FROM_VAR_ACTIVITY(v));
			vQ_replaceRoot(v);
		}
		++count;
	}
	//The priority queue is now filled with the numToSort variables with highest activity. However, in the queue, the worst of
	//them is at the beginning.
	#ifdef COLLECTSTATS
	stats_search_time_varSorting += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void search_strategy_pmpsls_orderToAssign(){
	//This method will put the variables from the variables priority queue into the right order for guiding the assignments.
	//We will simply add the vQ_leaf-1 remaining variables from the queue to the array in reversed order.
	variable *v;
	int32_t addPos = vQ_leaf - 2;

	search_strategy_pmpsls_numToAssign = vQ_leaf - 1;

	while ((v = vQ_dequeueBest()) != NULL){
		search_strategy_pmpsls_nextToAssign[addPos--] = GET_VAR_ID(v);
	}
}

int32_t search_strategy_pmpsls_assignOrdered(){
	//This method assigns the variables from the nextToAssign array in the order the variables have in that array. It will
	//follow the phases the variables currently have and then perform unit propagation. It returns 1 if it ran into a conflict
	//and 0 otherwise.

	variable *v;
	literal *l;
	clause *c;
	uint32_t pos = 0;

	for (pos = 0; pos < search_strategy_pmpsls_numToAssign; ++pos){
		//We ignore variables that have already been assigned.
		v = main_varData + search_strategy_pmpsls_nextToAssign[pos];
		if (!IS_VAR_UNASSIGNED(v)) continue;
		//The variable has not yet been assigned. We will assign it according to its current phase.
		if (GET_VAR_PHASE(v)){
			l = main_litData + GET_VAR_ID(v);
		} else {
			l = main_litData - GET_VAR_ID(v);
		}
		MAIN_COMPLEXUP_ADD_DEC_PTR(l, v);

		//Unit propagate and check the result.
		c = main_complexup_propagate(1);

		//Check if this resulted in a conflict.
		if (c != NULL){
			//It did result in a conflict. We return this result.
			return 1;
		}
	}

	//If it worked out without a conflict we return so.
	return 0;
}

void search_strategy_pmpsls_retainAllPhases(){
	//This method will simply override the phases of all currently assigned variables to whatever they are assigned now.
	variable *v;
	uint32_t i;

	for (i = 1; i < f.n_initial +1; ++i){
		v = main_varData + i;
		if (IS_VAR_UNASSIGNED(v)) continue;
		if (IS_VAR_TRUE(v)){
			SET_VAR_PHASE_1(v);
		} else {
			SET_VAR_PHASE_0(v);
		}
	}

}
