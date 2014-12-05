/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategy_emp_printHelp(){
	printf("c      %-3d: EMP\n", SEARCH_STRATEGY_EMP);
    printf("c           Behavior: Performs SLS-based MPWRL (MP weight reinforcement learning) [MP, SLS].\n");
}

void search_strategy_emp_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (EMP)...\n");
	#endif
	mp_resetModule();
	sls_resetModule();

	search_strategy_emp_weightModSAT = 0.01;
	search_strategy_emp_weightModUNS = 0.01;

}

void search_strategy_emp_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (EMP)...\n");
	#endif
	mp_initModule(); if (mp_returnCode != MP_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
	sls_initModule(); if (sls_returnCode != SLS_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }

	search_strategy_emp_weightModSAT = param_searchMPWeightModSAT;
	search_strategy_emp_weightModUNS = param_searchMPWeightModUNS;
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH:   param_searchMPWeightModSAT       = %f\n", param_searchMPWeightModSAT);
	printf("c   SEARCH:   param_searchMPWeightModUNS       = %f\n", param_searchMPWeightModUNS);
	printf("c   SEARCH:   search_strategy_emp_weightModSAT = %f\n", search_strategy_emp_weightModSAT);
	printf("c   SEARCH:   search_strategy_emp_weightModUNS = %f\n", search_strategy_emp_weightModUNS);
	#endif
}

void search_strategy_emp_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (EMP)...\n");
	#endif
	mp_disposeModule();
	sls_disposeModule();

	search_strategy_emp_weightModSAT = 0.01;
	search_strategy_emp_weightModUNS = 0.01;
}

void search_strategy_emp_execute(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (EMP)...\n");
	#endif

	//This search strategy performs SLS-based MP weight reinforcement learning (SLSMPWRL). We use MP to initialize the SLS
	//assignment, and perform SLS for a limited amount of flips. Then, we return (probably with the answer UNKNOWN) and
	//enforce that all clauses with exactly one satisfied literal in the SLS assignment are keeping the clause satisfied
	//during MP. We do this by decreasing the weight for the satisfying literal and increasing the weight for all non-
	//satisfying literals. This will increase the warning to the single variable that satisfies the clause.
	//We repeat the above until either SLS finds a satisfying assignment, or until MP does not converge anymore (in which
	//case we hand over the search to SLS completely).

	//We perform MP initially (unit weights), and compute biases.
	mp_extern_iterate(1);
	if (mp_returnCode == MP_CONVERGED){
		mp_extern_computeBiases();
	} else {
		//If MP does not converge, we fall back to standard SLS.
		sls_extern_tryRestartRules_switch(SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
	}

	//We perform SLS initially and try to improve the current total assignment as implied by the global phases.
	sls_extern_localSearch(1);
	if (sls_returnCode == SLS_SAT){
		//The SLS did indeed find a solution.
		search_intern_transferSLSAss();
		search_returnCode = SEARCH_SAT;
		return;
	}

	uint32_t rounds = 0, maxRounds = sls_clsMaxSize*10+1;

	do {
		//Update the weights with the current SLS assignment.
		search_strategy_emp_updateMPWeightsSLS();

		//Iterate anew with the updated weights.
		mp_extern_iterate(0);

		//Update the biases and phases.
		if (++rounds > maxRounds || mp_returnCode == MP_UNCONVERGED){
			//If MP does not converge, we fall back to standard SLS.
			sls_extern_tryRestartRules_switch(SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
		} else {
			mp_extern_computeBiases();
		}
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Performed %d rounds of MP.\n", rounds);
		if (rounds > maxRounds){
			printf("c   SEARCH: Maximum rounds performed. Switching to SLS.\n");
		}
		if (mp_returnCode == MP_UNCONVERGED){
			printf("c   SEARCH: MP did not converge anymore. Switching to SLS.\n");
		}
		#endif

		//Call the SLS to check for a satisfying assignment with the updated phases.
		sls_extern_localSearch(1);
		if (sls_returnCode == SLS_SAT){
			//The SLS did indeed find a solution.
			search_intern_transferSLSAss();
			search_returnCode = SEARCH_SAT;
		}


	} while (search_returnCode == SEARCH_UNKNOWN);

}

void search_strategy_emp_updateMPWeightsSLS(){
	int32_t cNum, *litC;
	float_ty *mpW;

	slsClause *slsC;
	mpClause *mpC;

	#ifdef VERBOSE_SEARCH
	int32_t numUpdated = 0, numUnsat = 0;
	float_ty minWeight = UINT32_MAX, maxWeight = 0;
	printf("c   SEARCH: Updating weights (weight mod, SAT:%f, UNS:%f)... ",
			search_strategy_emp_weightModSAT,search_strategy_emp_weightModUNS);
	#endif
	for (cNum = 0; cNum < f.m_eo_used; ++cNum){
		slsC = sls_clauses + cNum;

		if (GET_SLS_CLS_NUMTRUE(slsC) == 1) {
			//The clause is satisfied by exactly one literal.
			mpC = mp_clauses+cNum;
			mpW = GET_MP_CLS_WEIGHTS(mpC);
			litC = GET_SLS_CLS_LITS(slsC);
			#ifdef VERBOSE_SEARCH
			++numUpdated;
			#endif
			//Idea: try to enforce that the literal currently satisfying the clause keeps satisfying it.
			//We increase the weights for the unsatisfied literals.
			//we decrease the weight for the satisfying literal.
			//You might understand this as the "soft form of UP" -- in analogy to the decimation approach.
			while (*litC != 0){
				if (IS_SLS_LIT_SAT(*litC)){
					*mpW = *mpW - search_strategy_emp_weightModSAT;
				} else {
					*mpW = *mpW + search_strategy_emp_weightModUNS;
				}
				if (*mpW < 0.0) *mpW=0.0;
				#ifdef VERBOSE_SEARCH
				if (*mpW > maxWeight) maxWeight = *mpW;
				if (*mpW < minWeight) minWeight = *mpW;
				#endif
				++litC;
				++mpW;
			}
		}
		#ifdef VERBOSE_SEARCH
		else if (GET_SLS_CLS_NUMTRUE(slsC) == 0){
			++numUnsat;
		}
		#endif
	}
	#ifdef VERBOSE_SEARCH
	printf("min:%f max:%f (after %d updates). Unsatisfied clauses:%d.\n", minWeight, maxWeight, numUpdated, numUnsat);
	#endif
}
