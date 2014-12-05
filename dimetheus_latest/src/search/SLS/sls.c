/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "sls.h"

void sls_intern_prepareTry(){
	//Used to create a new random assignment.
	//Randomly assign the SLS assignment of the variables.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	#ifdef VERBOSE_SLS
	printf("c     SLS:     Preparing variable assignments...\n");
	#endif
	sls_assInitRule_ai();
	#ifdef VERBOSE_SLS
	printf("c     SLS:     Preparing clauses...\n");
	#endif
	sls_clauses_prepareTry();
	#ifdef VERBOSE_SLS
	printf("c     SLS:     Preparing literals...\n");
	#endif
	sls_literals_prepareTry();
	#ifdef VERBOSE_SLS
	printf("c     SLS:     Preparing variables...\n");
	#endif
	sls_variables_prepareTry();
	#ifdef VERBOSE_SLS
	printf("c     SLS:     Preparing try length...\n");
	#endif
	sls_tryRestartRule_prepareTry();
	#ifdef VERBOSE_SLS
	printf("c     SLS:   Preparing random number generator...\n");
	#endif
	rand_init();
	#ifdef VERBOSE_SLS
	printf("c     SLS:       ");rand_printConfig_inline();printf("\n");fflush(stdout);
	#endif

	#ifdef COLLECTSTATS
	stats_sls_time_initTry += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void sls_extern_printVersion(){
	printf("[Search          ] [Local Search                   ] :: %3d.%-4d :: %s",
			SLS_VERSION_MA, SLS_VERSION_MI, SLS_VERSION_NAME);
}

void sls_extern_localSearch(uint32_t prepareAnew){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_sls_component_totalCalls;
	#endif
	sls_returnCode = SLS_UNKNOWN;

	if (prepareAnew){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Preparing call...\n");
		printf("c     SLS:   Data-structures...\n");fflush(stdout);
		#endif
		//Prepare the snapshot view of the formula for the SLS module.
		sls_variables_prepareCall();
		sls_literals_prepareCall();
		sls_clauses_prepareCall();
		#ifdef VERBOSE_SLS
		printf("c     SLS:     Variables: %u, Clauses %u\n", sls_varNum, sls_clsUsed);
		printf("c     SLS:   Plug-ins...\n");fflush(stdout);
		#endif
		//Prepare the call to all plug-ins.
		sls_pickAndFlipVarRule_prepare();
		sls_assInitRule_prepare();
		sls_tryRestartRule_prepare();
		#ifdef VERBOSE_SLS
		printf("c     SLS:   Local variables...\n");fflush(stdout);
		#endif
		sls_numTries = 0;
		sls_numFlips = 0;
	} else {
		#ifdef VERBOSE_SLS
		printf("c     SLS: Preparing call (CONTINUOUS Variables: %u, Clauses: %u)...\n", sls_varNum, sls_clsUsed);
		printf("c     SLS:   Local variables...\n");fflush(stdout);
		#endif
		sls_numTries = 0;
		sls_numFlips = 0;
	}

	#ifdef COLLECTSTATS
	stats_sls_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

	#ifdef VERBOSE_SLS
	printf("c     SLS: Performing search...\n");fflush(stdout);
	#endif
	do {
		//In each try, we call for the try preparation.
		#ifdef VERBOSE_SLS
		printf("c     SLS:   Try %u ...\n", (uint32_t)sls_numTries);
		#endif
		sls_intern_prepareTry();
		#ifdef VERBOSE_SLS
		printf("c     SLS:     Flipping (%u unsatisfied clauses)...\n", (uint32_t)sls_numUnsat);fflush(stdout);
		#endif
		#ifdef COLLECTSTATS
		float_ty time_start_flips = STATS_GET_CURRENT_TIME_IN_SEC();
		++stats_sls_ls_totalTries;
		#endif
		//Then, we perform the try.
		sls_pickAndFlipVarRule_paf();
		#ifdef COLLECTSTATS
		stats_sls_time_flips += STATS_GET_CURRENT_TIME_IN_SEC() - time_start_flips;
		#endif
		if (sls_returnCode == SLS_SAT){
			//We found a solution. Time to stop.
			#ifdef VERBOSE_SLS
			printf("c     SLS:     The try succeeded.\n");fflush(stdout);
			#endif
			++sls_numTries;	break;
		} else if (sls_returnCode == SLS_UNKNOWN){
			//We assume the failure of the try and get on with the next.
			#ifdef VERBOSE_SLS
			printf("c     SLS:     TRY FAILED (%u unsatisfied clauses).\n", (uint32_t)sls_numUnsat);fflush(stdout);
			#endif
			++sls_numTries;
		} else {
			//We assume an error because the only return codes allowed are the above two.
			#ifdef VERBOSE_SLS
			printf("c     SLS:     An unexpected return code was encountered. We assume an error.\n");fflush(stdout);
			#endif
			++sls_numTries;	break;
		}
	} while (sls_numTries != sls_maxNumTries);

	#ifdef VERBOSE_SLS
	printf("c     SLS: Done flipping, %u flips in %u tries performed.\n", (uint32_t) sls_numFlips, (uint32_t) sls_numTries);
	#endif

	#ifdef COLLECTSTATS
	if (sls_returnCode == SLS_SAT){
		++stats_sls_return_SAT;
	} else if (sls_returnCode == SLS_UNKNOWN){
		++stats_sls_return_UNKNOWN;
	} else {
		++stats_sls_return_ERROR;
	}
	stats_sls_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void sls_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_sls_component_totalCalls;
	#endif
	#ifdef VERBOSE_SLS
	printf("c     SLS: Component reset... \n");
	#endif
	sls_returnCode 			= SLS_UNKNOWN;

	sls_clauses_reset();
	sls_literals_reset();
	sls_variables_reset();

	sls_pickAndFlipVarRules_resetPlugin();
	sls_assInitRules_resetPlugin();
	sls_tryRestartRules_resetPlugin();
	#ifdef COLLECTSTATS
	stats_sls_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void sls_initModule(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_sls_component_totalCalls;
	#endif
	sls_returnCode 	= SLS_UNKNOWN;

	sls_clauses_init();	if (sls_returnCode != SLS_UNKNOWN) return;
	sls_literals_init(); if (sls_returnCode != SLS_UNKNOWN) return;
	sls_variables_init(); if (sls_returnCode != SLS_UNKNOWN) return;

	sls_pickAndFlipVarRules_initPlugin(); if (sls_returnCode != SLS_UNKNOWN) return;
	sls_assInitRules_initPlugin(); if (sls_returnCode != SLS_UNKNOWN) return;
	sls_tryRestartRules_initPlugin(); if (sls_returnCode != SLS_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_sls_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_sls_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void sls_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_sls_component_totalCalls;
	#endif
	#ifdef VERBOSE_SLS
	printf("c     SLS: Component dispose... \n");
	#endif
	sls_returnCode 			= SLS_UNKNOWN;

	sls_clauses_dispose();
	sls_literals_dispose();
	sls_variables_dispose();

	sls_pickAndFlipVarRules_disposePlugin();
	sls_assInitRules_disposePlugin();
	sls_tryRestartRules_disposePlugin();
	#ifdef COLLECTSTATS
	stats_sls_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

#ifdef VERBOSE_SLS
int32_t sls_verifyAll(){
	int32_t result = 1;

	int32_t* brake = malloc(sizeof(int32_t) * (f.n_initial+1));
	int32_t numTrue, i, *litC, aLit;
	slsClause *c;

	for (i = 0; i < f.n_initial+1; ++i){
		brake[i] = 0;
	}

	for (i = 0; i < sls_clsUsed; ++i){
		numTrue = 0;
		c = sls_clauses + i;
		litC = GET_SLS_CLS_LITS(c);
		while (*litC != 0){
			if (IS_SLS_LIT_SAT(*litC)){
				++numTrue;
			}
			++litC;
		}
		if (numTrue != GET_SLS_CLS_NUMTRUE(c)){
			printf("c ERROR. SLS numTrue for the following clause seems to be wrong.\n");
			sls_clauses_printClause(c);
			result = 0;
			break;
		}
		if (numTrue == 0){
			if (sls_unsatClauses[GET_SLS_CLS_WHEREUNSAT(c)] != c){
				printf("c ERROR. SLS whereUnsat for the following clause seems to be wrong.\n");
				sls_clauses_printClause(c);
				sls_clauses_printClause(sls_unsatClauses[GET_SLS_CLS_WHEREUNSAT(c)]);
				result = 0;
				break;
			}
		}
		if (numTrue == 1){
			aLit = GET_SLS_CLS_TRUELIT(c);
			if (!IS_SLS_LIT_SAT(aLit)){
				printf("c ERROR. SLS true literal is not true.\n");
				sls_clauses_printClause(c);
				result = 0;
				break;
			}
			++brake[abs(GET_SLS_CLS_TRUELIT(c))];
		}
	}
	if (result){
		for (i = 0; i < f.n_vars_e_used; ++i){
			if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

			aLit = f.vars_e[i]->id;
			if (brake[aLit] != GET_SLS_VAR_BRAKE(aLit)){
				printf("c ERROR. SLS variable brake seems to be wrong.\n");
				result = 0;
				break;
			}
		}
	}

	free(brake);

	return result;
}
#endif
