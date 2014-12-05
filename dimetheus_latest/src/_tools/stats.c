/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "stats.h"

#ifdef COLLECTSTATS
void stats_reset(){
	//Global.
	stats_main_time_init				= 0;

	//Reset related.
	stats_reset_time_total				= 0;
	stats_reset_component_totalCalls	= 3;

	//Parameter related.
	stats_params_time_total				= 0;
	stats_params_component_totalCalls	= 0;

	//Guidance related.
	stats_guidance_time_total			= 0;
	stats_guidance_component_totalCalls	= 0;

	//Startup related.
	stats_startup_time_total			= 0;
	stats_startup_component_totalCalls	= 0;

	//DETERM
	stats_determ_time_total				= 0;
	stats_determ_component_totalCalls	= 0;

	//SPECCER
	stats_speccer_time_total			= 0;
	stats_speccer_component_totalCalls	= 0;

	//LOADER
	stats_loader_time_total				= 0;
	stats_loader_component_totalCalls	= 0;
	stats_loader_numVariables			= 0;
	stats_loader_numInitialUnits		= 0;
	stats_loader_numInitialClauses		= 0;

	//Classification related.
	stats_classify_time_total			= 0;
	stats_classify_time_simplify		= 0;
	stats_classify_assignedVars			= 0;
	stats_classify_component_totalCalls	= 0;

	//ATTRIBUTOR
	stats_attributor_time_init			= 0;
	stats_attributor_time_total			= 0;
	stats_attributor_time_BASIC			= 0;
	stats_attributor_time_VCG			= 0;
	stats_attributor_time_VG			= 0;
	stats_attributor_time_CG			= 0;
	stats_attributor_time_B				= 0;
	stats_attributor_time_HP			= 0;
	stats_attributor_time_BIG			= 0;
	stats_attributor_time_UP			= 0;
	stats_attributor_time_RW			= 0;
	stats_attributor_time_ELS			= 0;
	stats_attributor_numTimeouts		= 0;
	stats_attributor_numSkipped			= 0;
	stats_attributor_component_totalCalls = 0;

	//RANDOMFOREST
	stats_randomforest_time_init		= 0;
	stats_randomforest_time_total		= 0;
	stats_randomforest_component_totalCalls = 0;

	//Adaptation related.
	stats_adapt_time_total				= 0;
	stats_adapt_component_totalCalls	= 0;

	//ITEADAPTER
	stats_iteadapter_time_total			= 0;
	stats_iteadapter_time_init			= 0;
	stats_iteadapter_component_totalCalls = 0;

	//Preprocessor related.
	stats_prep_time_total 				= 0;
	stats_prep_clauseCoverRateOrig 		= 0;
	stats_prep_clauseCoverRatePrep 		= 0;
	stats_prep_component_totalCalls		= 0;

	//PRE
	stats_pre_time_total				= 0;
	stats_pre_component_totalCalls		= 0;
	stats_pre_varRemoved				= 0;
	stats_pre_varRemoved_SE				= 0;
	stats_pre_varRemoved_FLD			= 0;
	stats_pre_varRemoved_PLE			= 0;
	stats_pre_varRemoved_STR			= 0;
	stats_pre_varRemoved_NIVER			= 0;
	stats_pre_varRemoved_ATE			= 0;
	stats_pre_varRemoved_ACCE			= 0;
	stats_pre_varRemoved_GE				= 0;
	stats_pre_varRemoved_UNHIDING 		= 0;
	stats_pre_varRemoved_NHBR	 		= 0;
	stats_pre_varRemoved_TERNARY 		= 0;
	stats_pre_varRemoved_ELS			= 0;
	stats_pre_clsRemoved				= 0;
	stats_pre_clsRemoved_SE				= 0;
	stats_pre_clsRemoved_FLD			= 0;
	stats_pre_clsRemoved_PLE			= 0;
	stats_pre_clsRemoved_STR			= 0;
	stats_pre_clsRemoved_NIVER			= 0;
	stats_pre_clsRemoved_ATE			= 0;
	stats_pre_clsRemoved_ACCE			= 0;
	stats_pre_clsRemoved_GE				= 0;
	stats_pre_clsRemoved_UNHIDING 		= 0;
	stats_pre_clsRemoved_NHBR			= 0;
	stats_pre_clsRemoved_TERNARY		= 0;
	stats_pre_clsRemoved_ELS			= 0;

	//SATELITE
	stats_satelite_time_total			= 0;
	stats_satelite_component_totalCalls	= 0;

	//Search related.
	stats_search_time_total				= 0;
	stats_search_time_varSorting		= 0;
	stats_search_time_prepareAss		= 0;
	stats_search_component_totalCalls	= 0;
	stats_search_childClausesRetainedB	= 0;
	stats_search_impactsB				= NULL;
	stats_search_childClausesRetainedL	= 0;
	stats_search_impactsL				= NULL;
	stats_search_restarts				= 0;

	//In-processor related.
	stats_inp_time_total 				= 0;
	stats_inp_component_totalCalls 		= 0;

	//IN
	stats_in_time_total					= 0;
	stats_in_component_totalCalls		= 0;
	stats_in_varRemoved					= 0;
	stats_in_varRemoved_SE				= 0;
	stats_in_varRemoved_FLD				= 0;
	stats_in_varRemoved_PLE				= 0;
	stats_in_varRemoved_STR				= 0;
	stats_in_clsRemoved					= 0;
	stats_in_clsRemoved_SE				= 0;
	stats_in_clsRemoved_FLD				= 0;
	stats_in_clsRemoved_PLE				= 0;
	stats_in_clsRemoved_STR				= 0;

	//MP
	stats_mp_time_total 				= 0;
	stats_mp_time_init					= 0;
	stats_mp_time_biasCompute			= 0;
	stats_mp_time_iterating				= 0;
	stats_mp_time_retaining				= 0;
	stats_mp_component_totalCalls		= 0;
	stats_mp_return_CONVERGED			= 0;
	stats_mp_return_UNCONVERGED			= 0;
	stats_mp_return_PARAMAG				= 0;
	stats_mp_return_NONPARA				= 0;
	stats_mp_return_ERROR				= 0;
	stats_mp_passing_numIterations 		= 0;
	stats_mp_passing_totalMsgUpdates	= 0;
	stats_mp_computation_numDiv			= 0;
	stats_mp_computation_numMult		= 0;
	stats_mp_countedClauses				= 0;
	stats_mp_hwLits						= NULL;
	stats_mp_nwLits						= NULL;
	stats_mp_lwLits						= NULL;

	//SLS
	stats_sls_time_total				= 0;
	stats_sls_time_init					= 0;
	stats_sls_time_initTry				= 0;
	stats_sls_time_flips				= 0;
	stats_sls_component_totalCalls		= 0;
	stats_sls_return_UNKNOWN			= 0;
	stats_sls_return_SAT				= 0;
	stats_sls_return_ERROR				= 0;
	stats_sls_ls_totalTries				= 0;
	stats_sls_ls_totalFlips				= 0;

	//CDCL
	stats_cdcl_time_total				= 0;
	stats_cdcl_time_init				= 0;
	stats_cdcl_time_up					= 0;
	stats_cdcl_time_confAnalysis		= 0;
	stats_cdcl_time_str					= 0;
	stats_cdcl_time_maint				= 0;
	stats_cdcl_time_reduce				= 0;
	stats_cdcl_time_inp					= 0;
	stats_cdcl_component_totalCalls		= 0;
	stats_cdcl_return_UNKNOWN			= 0;
	stats_cdcl_return_SAT				= 0;
	stats_cdcl_return_UNSAT				= 0;
	stats_cdcl_return_ERROR				= 0;
	stats_cdcl_numDecisions				= 0;
	stats_cdcl_numDecisionsRandom		= 0;
	stats_cdcl_numReductions			= 0;
	stats_cdcl_numRedSatCls				= 0;
	stats_cdcl_numRedFalsLits			= 0;
	stats_cdcl_numMaintenances			= 0;
	stats_cdcl_numMaintWithGlue			= 0;
	stats_cdcl_numMaintWithActivity		= 0;
	stats_cdcl_numInprocessing			= 0;
	stats_cdcl_numLearnedRemoved		= 0;
	stats_cdcl_numConflictsFound		= 0;
	stats_cdcl_numClausesLearned		= 0;
	stats_cdcl_numBackjumps				= 0;
	stats_cdcl_numRestarts				= 0;
	stats_cdcl_numRestartProhibits		= 0;
	stats_cdcl_numVarAgiRescales		= 0;
	stats_cdcl_numPropagations			= 0;
	stats_cdcl_numVarActRescales		= 0;
	stats_cdcl_numClsActRescales		= 0;
	stats_cdcl_numStrChecks				= 0;
	stats_cdcl_numStrSuccess			= 0;
	stats_cdcl_numStrSigChecks			= 0;
	stats_cdcl_numStrSigSuccess			= 0;
	stats_cdcl_numStrSuccessAreReasons	= 0;
	stats_cdcl_numStrSuccessNoReasons	= 0;
	stats_cdcl_numStrSuccessIsRepl		= 0;
	stats_cdcl_numSorChecks				= 0;
	stats_cdcl_numSorSuccLarge			= 0;
	stats_cdcl_numSorSuccBin			= 0;
	stats_cdcl_numSorSuccUnit			= 0;
	stats_cdcl_numAbort					= 0;
	stats_cdcl_numAbortAssignmentMin	= 0;
	stats_cdcl_numAbortAssignmentMax	= 0;
	stats_cdcl_numAbortDLZero			= 0;
	stats_cdcl_numAbortClsImpact		= 0;
	stats_cdcl_decLevelVisited			= NULL;
	stats_cdcl_learnedLength			= NULL;
	stats_cdcl_glueDev					= NULL;
	stats_cdcl_glueAvg					= NULL;

	//HANDLERESULT related.
	stats_handleresult_time_total		= 0;
	stats_handleresult_component_totalCalls = 0;

	//SOLWRITER
	stats_solwriter_time_total			= 0;
	stats_solwriter_component_totalCalls = 0;
	stats_solwriter_assignmentBiasAux	= -2;
	stats_solwriter_assignmentBiasInp 	= -2;
	stats_solwriter_assignmentBiasAll 	= -2;
	stats_solwriter_numAuxVars			= 0;
	stats_solwriter_numInpVars			= 0;
	stats_solwriter_numAllVars			= 0;
	stats_solwriter_numPosVars			= 0;
	stats_solwriter_numNegVars			= 0;
	stats_solwriter_numUnaVars			= 0;

	//SHUTDOWN related.
	stats_shutdown_time_total			= 0;
	stats_shutdown_component_totalCalls	= 0;

	//GENERAL SEARCH related.
	stats_general_countedClauses		= 0;
	stats_general_satLitsByLength 		= NULL;

}

void stats_init(){
	int32_t i;

	stats_search_impactsB = malloc(sizeof(uint64_t)*(101));
	if (stats_search_impactsB == NULL){
		printf("c ERROR. We were unable to allocate the memory for the impactsB array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i= 0; i < 101; ++i){
		stats_search_impactsB[i] = 0;
	}

	stats_search_impactsL = malloc(sizeof(uint64_t)*(101));
	if (stats_search_impactsL == NULL){
		printf("c ERROR. We were unable to allocate the memory for the impactsL array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i= 0; i < 101; ++i){
		stats_search_impactsL[i] = 0;
	}

	stats_mp_hwLits = malloc(sizeof(uint32_t)*(STATS_MP_COUNTHWLITS_MAX+2));
	if (stats_mp_hwLits == NULL){
		printf("c ERROR. We were unable to allocate the memory for the hwLits array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for(i=0; i < STATS_MP_COUNTHWLITS_MAX+2; ++i){
		stats_mp_hwLits[i] = 0;
	}

	stats_mp_nwLits = malloc(sizeof(uint32_t)*(STATS_MP_COUNTNWLITS_MAX+2));
	if (stats_mp_nwLits == NULL){
		printf("c ERROR. We were unable to allocate the memory for the nwLits array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for(i=0; i < STATS_MP_COUNTNWLITS_MAX+2; ++i){
		stats_mp_nwLits[i] = 0;
	}

	stats_mp_lwLits = malloc(sizeof(uint32_t)*(STATS_MP_COUNTLWLITS_MAX+2));
	if (stats_mp_lwLits == NULL){
		printf("c ERROR. We were unable to allocate the memory for the lwLits array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for(i=0; i < STATS_MP_COUNTLWLITS_MAX+2; ++i){
		stats_mp_lwLits[i] = 0;
	}

	stats_cdcl_decLevelVisited = malloc(sizeof(uint64_t)*(STATS_CDCL_DECLEVELVISITED_MAX + 2));
	if (stats_cdcl_decLevelVisited == NULL){
		printf("c ERROR. We were unable to allocate the memory for the decLevelVisited array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i= 0; i < STATS_CDCL_DECLEVELVISITED_MAX + 2; ++i){
		stats_cdcl_decLevelVisited[i] = 0;
	}

	stats_cdcl_learnedLength = malloc(sizeof(uint64_t)*(STATS_CDCL_LEARNEDLENGTH_MAX + 2));
	if (stats_cdcl_learnedLength == NULL){
		printf("c ERROR. We were unable to allocate the memory for the learnedLength array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}

	for (i= 0; i < STATS_CDCL_LEARNEDLENGTH_MAX + 2; ++i){
		stats_cdcl_learnedLength[i] = 0;
	}

	stats_cdcl_glueDev = malloc(sizeof(uint64_t)*(101));
	if (stats_cdcl_glueDev == NULL){
		printf("c ERROR. We were unable to allocate the memory for the glueDev array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i= 0; i < 101; ++i){
		stats_cdcl_glueDev[i] = 0;
	}

	stats_cdcl_glueAvg = malloc(sizeof(uint64_t)*(101));
	if (stats_cdcl_glueAvg == NULL){
		printf("c ERROR. We were unable to allocate the memory for the glueAvg array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i= 0; i < 101; ++i){
		stats_cdcl_glueAvg[i] = 0;
	}

	stats_general_satLitsByLength = malloc(sizeof(uint32_t)*(STATS_GENERAL_COUNTSATLITS_MAX+2));
	if (stats_general_satLitsByLength == NULL){
		printf("c ERROR. We were unable to allocate the memory for the satLitsByLength array. Out of memory?\n");
		main_returnCode = ERROR;
		return;
	}
	for (i=0; i < STATS_GENERAL_COUNTSATLITS_MAX+2;++i){
		stats_general_satLitsByLength[i] = 0;
	}
}

void stats_dispose(){
	if (stats_search_impactsB != NULL){
		free(stats_search_impactsB);
		stats_search_impactsB = NULL;
	}
	if (stats_search_impactsL != NULL){
		free(stats_search_impactsL);
		stats_search_impactsL = NULL;
	}
	if (stats_mp_hwLits != NULL){
		free(stats_mp_hwLits);
		stats_mp_hwLits = NULL;
	}
	if (stats_mp_nwLits != NULL){
		free(stats_mp_nwLits);
		stats_mp_nwLits = NULL;
	}
	if (stats_mp_lwLits != NULL){
		free(stats_mp_lwLits);
		stats_mp_lwLits = NULL;
	}
	if (stats_cdcl_decLevelVisited != NULL){
		free(stats_cdcl_decLevelVisited);
		stats_cdcl_decLevelVisited = NULL;
	}
	if (stats_cdcl_learnedLength != NULL){
		free(stats_cdcl_learnedLength);
		stats_cdcl_learnedLength = NULL;
	}
	if (stats_cdcl_glueDev != NULL){
		free(stats_cdcl_glueDev);
		stats_cdcl_glueDev = NULL;
	}
	if (stats_cdcl_glueAvg != NULL){
		free(stats_cdcl_glueAvg);
		stats_cdcl_glueAvg = NULL;
	}
	if (stats_general_satLitsByLength != NULL){
		free(stats_general_satLitsByLength);
		stats_general_satLitsByLength = NULL;
	}
}

void stats_print(){
	int32_t i;
	uint64_t uval;
	double fVal, fVal2, d1, d2;

	printf("c   STATS:\n");
	printf("c   STATS: RESET:\n");
	printf("c   STATS:   RESET calls: %u\n", (uint32_t) stats_reset_component_totalCalls);
	printf("c   STATS:   RESET global time: %.3f seconds\n", stats_reset_time_total);
	printf("c   STATS: PARAMS:\n");
	printf("c   STATS:   PARAMS calls: %u\n", (uint32_t) stats_params_component_totalCalls);
	printf("c   STATS:   PARAMS global time: %.3f seconds\n", stats_params_time_total);
	printf("c   STATS: GUIDANCE:\n");
	printf("c   STATS:   GUIDANCE calls: %u\n", (uint32_t) stats_guidance_component_totalCalls);
	printf("c   STATS:   GUIDANCE global time: %.3f seconds\n", stats_guidance_time_total);
	printf("c   STATS: STARTUP:\n");
	printf("c   STATS:   STARTUP calls: %u\n", (uint32_t) stats_startup_component_totalCalls);
	printf("c   STATS:   STARTUP global time: %.3f seconds\n", stats_startup_time_total);
	printf("c   STATS:   DETERM:\n");
	printf("c   STATS:     DETERM calls: %u\n", (uint32_t) stats_determ_component_totalCalls);
	printf("c   STATS:     DETERM global time: %.3f seconds\n", stats_determ_time_total);
	printf("c   STATS:   SPECCER:\n");
	printf("c   STATS:     SPECCER calls: %u\n", (uint32_t) stats_speccer_component_totalCalls);
	printf("c   STATS:     SPECCER global time: %.3f seconds\n", stats_speccer_time_total);
	printf("c   STATS:   LOADER:\n");
	printf("c   STATS:     LOADER calls: %u\n", (uint32_t) stats_loader_component_totalCalls);
	printf("c   STATS:     LOADER number of initial variables in CNF: %d\n", stats_loader_numVariables);
	printf("c   STATS:     LOADER number of variables in units: %d\n", stats_loader_numInitialUnits);
	printf("c   STATS:     LOADER number of initial clauses in CNF: %d\n", stats_loader_numInitialClauses);
	printf("c   STATS:     LOADER global time: %.3f seconds\n", stats_loader_time_total);
	printf("c   STATS: CLASSIFY:\n");
	printf("c   STATS:   CLASSIFY calls: %u\n", (uint32_t) stats_classify_component_totalCalls);
	printf("c   STATS:   CLASSIFY assigned variables: %d\n", (int32_t) stats_classify_assignedVars);
	printf("c   STATS:   CLASSIFY global time: %.3f seconds\n", stats_classify_time_total);
	printf("c   STATS:     CLASSIFY simplification time: %.3f seconds\n", stats_classify_time_simplify);
	printf("c   STATS:   ATTRIBUTOR:\n");
	printf("c   STATS:     ATTRIBUTOR calls: %u\n", (uint32_t) stats_attributor_component_totalCalls);
	printf("c   STATS:     ATTRIBUTOR attribute set computation timeouts: %u\n", (uint32_t) stats_attributor_numTimeouts);
	printf("c   STATS:     ATTRIBUTOR attribute set computations skipped: %u\n", (uint32_t) stats_attributor_numSkipped);
	printf("c   STATS:     ATTRIBUTOR global time: %.3f seconds\n", stats_attributor_time_total);
	printf("c   STATS:       ATTRIBUTOR initialization time: %.3f seconds.\n", stats_attributor_time_init);
	printf("c   STATS:       ATTRIBUTOR time for computing BASIC attributes: %.3f seconds\n", stats_attributor_time_BASIC);
	printf("c   STATS:       ATTRIBUTOR time for computing VCG attributes: %.3f seconds\n", stats_attributor_time_VCG);
	printf("c   STATS:       ATTRIBUTOR time for computing VG attributes: %.3f seconds\n", stats_attributor_time_VG);
	printf("c   STATS:       ATTRIBUTOR time for computing CG attributes: %.3f seconds\n", stats_attributor_time_CG);
	printf("c   STATS:       ATTRIBUTOR time for computing B attributes: %.3f seconds\n", stats_attributor_time_B);
	printf("c   STATS:       ATTRIBUTOR time for computing HP attributes: %.3f seconds\n", stats_attributor_time_HP);
	printf("c   STATS:       ATTRIBUTOR time for computing BIG attributes: %.3f seconds\n", stats_attributor_time_BIG);
	printf("c   STATS:       ATTRIBUTOR time for computing UP attributes: %.3f seconds\n", stats_attributor_time_UP);
	printf("c   STATS:       ATTRIBUTOR time for computing RW attributes: %.3f seconds\n", stats_attributor_time_RW);
	printf("c   STATS:       ATTRIBUTOR time for computing ELS attributes: %.3f seconds\n", stats_attributor_time_ELS);
	printf("c   STATS:   RANDOMFOREST:\n");
	printf("c   STATS:     RANDOMFOREST calls: %u\n", (uint32_t) stats_randomforest_component_totalCalls);
	printf("c   STATS:     RANDOMFOREST global time: %.3f seconds\n", stats_randomforest_time_total);
	printf("c   STATS:       RANDOMFOREST initialization time: %.3f seconds.\n", stats_randomforest_time_init);
	printf("c   STATS: ADAPT:\n");
	printf("c   STATS:   ADAPT calls: %u\n", (uint32_t) stats_adapt_component_totalCalls);
	printf("c   STATS:   ADAPT global time: %.3f seconds\n", stats_adapt_time_total);
	printf("c   STATS:   ITEADAPTER:\n");
	printf("c   STATS:     ITEADAPTER calls: %u\n", (uint32_t) stats_iteadapter_component_totalCalls);
	printf("c   STATS:     ITEADAPTER global time: %.3f seconds\n", stats_iteadapter_time_total);
	printf("c   STATS:       ITEADAPTER initialization time: %.3f seconds\n", stats_iteadapter_time_init);
	printf("c   STATS: PREP:\n");
	printf("c   STATS:   PREP calls: %u\n", (uint32_t) stats_prep_component_totalCalls);
	printf("c   STATS:   PREP clause cover rate before/after: %.3f/%.3f\n", stats_prep_clauseCoverRateOrig, stats_prep_clauseCoverRatePrep);
	printf("c   STATS:   PREP global time: %.3f seconds\n", stats_prep_time_total);
	printf("c   STATS:   PRE:\n");
	printf("c   STATS:     PRE calls: %u\n", (uint32_t) stats_pre_component_totalCalls);
	printf("c   STATS:     PRE variables removed: %d\n", (int32_t) stats_pre_varRemoved);
	printf("c   STATS:       PRE removed by SE: %d\n", (int32_t) stats_pre_varRemoved_SE);
	printf("c   STATS:       PRE removed by PLE: %d\n", (int32_t) stats_pre_varRemoved_PLE);
	printf("c   STATS:       PRE removed by FLD: %d\n", (int32_t) stats_pre_varRemoved_FLD);
	printf("c   STATS:       PRE removed by STR: %d\n", (int32_t) stats_pre_varRemoved_STR);
	printf("c   STATS:       PRE removed by NIVER: %d\n", (int32_t) stats_pre_varRemoved_NIVER);
	printf("c   STATS:       PRE removed by ATE: %d\n", (int32_t) stats_pre_varRemoved_ATE);
	printf("c   STATS:       PRE removed by ACCE: %d\n", (int32_t) stats_pre_varRemoved_ACCE);
	printf("c   STATS:       PRE removed by GE: %d\n", (int32_t) stats_pre_varRemoved_GE);
	printf("c   STATS:       PRE removed by UNHIDING: %d\n", (int32_t) stats_pre_varRemoved_UNHIDING);
	printf("c   STATS:       PRE removed by NHBR: %d\n", (int32_t) stats_pre_varRemoved_NHBR);
	printf("c   STATS:       PRE removed by TERNARY: %d\n", (int32_t) stats_pre_varRemoved_TERNARY);
	printf("c   STATS:       PRE removed by ELS: %d\n", (int32_t) stats_pre_varRemoved_ELS);
	printf("c   STATS:     PRE clauses removed: %d\n", (int32_t) stats_pre_clsRemoved);
	printf("c   STATS:       PRE removed by SE: %d\n", (int32_t) stats_pre_clsRemoved_SE);
	printf("c   STATS:       PRE removed by PLE: %d\n", (int32_t) stats_pre_clsRemoved_PLE);
	printf("c   STATS:       PRE removed by FLD: %d\n", (int32_t) stats_pre_clsRemoved_FLD);
	printf("c   STATS:       PRE removed by STR: %d\n", (int32_t) stats_pre_clsRemoved_STR);
	printf("c   STATS:       PRE removed by NIVER: %d\n", (int32_t) stats_pre_clsRemoved_NIVER);
	printf("c   STATS:       PRE removed by ATE: %d\n", (int32_t) stats_pre_clsRemoved_ATE);
	printf("c   STATS:       PRE removed by ACCE: %d\n", (int32_t) stats_pre_clsRemoved_ACCE);
	printf("c   STATS:       PRE removed by GE: %d\n", (int32_t) stats_pre_clsRemoved_GE);
	printf("c   STATS:       PRE removed by UNHIDING: %d\n", (int32_t) stats_pre_clsRemoved_UNHIDING);
	printf("c   STATS:       PRE removed by NHBR: %d\n", (int32_t) stats_pre_clsRemoved_NHBR);
	printf("c   STATS:       PRE removed by TERNARY: %d\n", (int32_t) stats_pre_clsRemoved_TERNARY);
	printf("c   STATS:       PRE removed by ELS: %d\n", (int32_t) stats_pre_clsRemoved_ELS);
	printf("c   STATS:     PRE global time: %.3f seconds\n", stats_pre_time_total);
	printf("c   STATS:   SATELITE:\n");
	printf("c   STATS:     SATELITE calls: %u\n", (uint32_t) stats_satelite_component_totalCalls);
	printf("c   STATS:     SATELITE global time: %.3f seconds\n", stats_pre_time_total);
	printf("c   STATS: SEARCH:\n");
	printf("c   STATS:   SEARCH calls: %u\n", (uint32_t) stats_search_component_totalCalls);
	printf("c   STATS:     SEARCH restarts: %u\n", (uint32_t) stats_search_restarts);
	printf("c   STATS:     SEARCH binary clauses retained from child components: %u\n", (uint32_t) stats_search_childClausesRetainedB);
	if (stats_search_childClausesRetainedB > 0){
		printf("c   STATS:     SEARCH binary retained clauses MP impact (intervals omitted if zero count):\n");
		for (i = 0; i < 101; ++i){
			if (stats_search_impactsB[i] > 0) {
				printf("c   STATS:       SEARCH interval 0.%02d: %u\n", i, (uint32_t) stats_search_impactsB[i]);
			}
		}
	}
	printf("c   STATS:     SEARCH large clauses retained from child components: %u\n", (uint32_t) stats_search_childClausesRetainedL);
	if (stats_search_childClausesRetainedL > 0){
		printf("c   STATS:     SEARCH large retained clauses MP impact (intervals omitted if zero count):\n");
		for (i = 0; i < 101; ++i){
			if (stats_search_impactsL[i] > 0 || 100*param_searchMinClsImpact == i) {
				printf("c   STATS:       SEARCH interval 0.%02d: %u", i, (uint32_t) stats_search_impactsL[i]);
				if (100.0f*param_searchMinClsImpact == i){
					printf(" < param_searchMinClsImpact\n");
				} else {
					printf("\n");
				}
			}
		}
	}
	printf("c   STATS:   SEARCH global time: %.3f seconds\n", stats_search_time_total);
	printf("c   STATS:     SEARCH variable sorting time: %.3f seconds\n", stats_search_time_varSorting);
	printf("c   STATS:     SEARCH assignment preparation time: %.3f seconds\n", stats_search_time_prepareAss);
	printf("c   STATS:   INP:\n");
	printf("c   STATS:     INP calls: %u\n", (uint32_t) stats_inp_component_totalCalls);
	printf("c   STATS:     INP global time: %.3f seconds\n", stats_inp_time_total);
	printf("c   STATS:     IN:\n");
	printf("c   STATS:       IN calls: %u\n", (uint32_t) stats_in_component_totalCalls);
	printf("c   STATS:       IN variables removed: %d\n", (int32_t) stats_in_varRemoved);
	printf("c   STATS:         IN removed by SE: %d\n", (int32_t) stats_in_varRemoved_SE);
	printf("c   STATS:         IN removed by PLE: %d\n", (int32_t) stats_in_varRemoved_PLE);
	printf("c   STATS:         IN removed by FLD: %d\n", (int32_t) stats_in_varRemoved_FLD);
	printf("c   STATS:         IN removed by STR: %d\n", (int32_t) stats_in_varRemoved_STR);
	printf("c   STATS:       IN clauses removed: %d\n", (int32_t) stats_in_clsRemoved);
	printf("c   STATS:         IN removed by SE: %d\n", (int32_t) stats_in_clsRemoved_SE);
	printf("c   STATS:         IN removed by PLE: %d\n", (int32_t) stats_in_clsRemoved_PLE);
	printf("c   STATS:         IN removed by FLD: %d\n", (int32_t) stats_in_clsRemoved_FLD);
	printf("c   STATS:         IN removed by STR: %d\n", (int32_t) stats_in_clsRemoved_STR);
	printf("c   STATS:       IN global time: %.3f seconds\n", stats_in_time_total);
	printf("c   STATS:   MP:\n");
	printf("c   STATS:     MP calls: %u\n", (uint32_t) stats_mp_component_totalCalls);
	printf("c   STATS:     MP return codes [ERROR, UNCONVERGED, CONVERGED, PARAMAG, NONPARA]: %u %u %u %u %u\n",
			(uint32_t) stats_mp_return_ERROR, (uint32_t) stats_mp_return_UNCONVERGED, (uint32_t) stats_mp_return_CONVERGED,
			(uint32_t) stats_mp_return_PARAMAG, (uint32_t) stats_mp_return_NONPARA);
	printf("c   STATS:     MP total message passing iterations: %d\n", (uint32_t)stats_mp_passing_numIterations);
	printf("c   STATS:     MP message updates: %u ", (uint32_t)stats_mp_passing_totalMsgUpdates);
	if (stats_mp_time_iterating > ZERO){
		d1 = (double) stats_mp_passing_totalMsgUpdates;
		d2 = (double) stats_mp_time_iterating;
		fVal = d1 / d2;
		printf("(%.3f per second in relation to MP iterating time)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:     MP multiplications performed: %u\n", (uint32_t) stats_mp_computation_numMult);
	printf("c   STATS:     MP divisions performed: %u\n", (uint32_t) stats_mp_computation_numDiv);
	printf("c   STATS:     MP global time: %.3f seconds\n", stats_mp_time_total);
	printf("c   STATS:       MP initialization time: %.3f seconds\n", stats_mp_time_init);
	printf("c   STATS:       MP bias computation time: %.3f seconds\n", stats_mp_time_biasCompute);
	printf("c   STATS:       MP iterating time: %.3f seconds\n", stats_mp_time_iterating);
	printf("c   STATS:       MP message retaining time: %.3f seconds\n", stats_mp_time_retaining);

	printf("c   STATS:     MP Clauses %d, literals with weight > 1.0:\n", stats_mp_countedClauses);
	for (i=0; i < STATS_MP_COUNTHWLITS_MAX+1;++i){
		if (stats_mp_hwLits[i] > 0){
			fVal = stats_mp_hwLits[i];
			fVal2 = stats_mp_countedClauses;
			if (i == STATS_MP_COUNTHWLITS_MAX){
				printf("c   STATS:       MP more: %d (%.3f %%)\n", stats_mp_hwLits[i], (fVal/fVal2) * 100.0);
			} else {
				printf("c   STATS:       MP %4d: %d (%.3f %%)\n", i, stats_mp_hwLits[i], (fVal/fVal2) * 100.0);
			}
		}
	}
	printf("c   STATS:     MP Clauses %d, literals with weight = 1.0:\n", stats_mp_countedClauses);
	for (i=0; i < STATS_MP_COUNTNWLITS_MAX+1;++i){
		if (stats_mp_nwLits[i] > 0){
			fVal = stats_mp_nwLits[i];
			fVal2 = stats_mp_countedClauses;
			if (i == STATS_MP_COUNTNWLITS_MAX){
				printf("c   STATS:       MP more: %d (%.3f %%)\n", stats_mp_nwLits[i], (fVal/fVal2) * 100.0);
			} else {
				printf("c   STATS:       MP %4d: %d (%.3f %%)\n", i, stats_mp_nwLits[i], (fVal/fVal2) * 100.0);
			}
		}
	}
	printf("c   STATS:     MP Clauses %d, literals with weight < 1.0:\n", stats_mp_countedClauses);
	for (i=0; i < STATS_MP_COUNTLWLITS_MAX+1;++i){
		if (stats_mp_lwLits[i] > 0){
			fVal = stats_mp_lwLits[i];
			fVal2 = stats_mp_countedClauses;
			if (i == STATS_MP_COUNTLWLITS_MAX){
				printf("c   STATS:       MP more: %d (%.3f %%)\n", stats_mp_lwLits[i], (fVal/fVal2) * 100.0);
			} else {
				printf("c   STATS:       MP %4d: %d (%.3f %%)\n", i, stats_mp_lwLits[i], (fVal/fVal2) * 100.0);
			}
		}
	}

	printf("c   STATS:   SLS:\n");
	printf("c   STATS:     SLS calls: %u\n", (uint32_t) stats_sls_component_totalCalls);
	printf("c   STATS:     SLS return codes [ERROR, SAT, UNKNOWN]: %u %u %u\n", (uint32_t) stats_sls_return_ERROR,
			(uint32_t) stats_sls_return_SAT, (uint32_t) stats_sls_return_UNKNOWN);
	printf("c   STATS:     SLS total tries performed: %u\n", (uint32_t)stats_sls_ls_totalTries);
	printf("c   STATS:     SLS total flips performed: %u ", (uint32_t)stats_sls_ls_totalFlips);
	if (stats_sls_time_flips > ZERO){
		d1 = (double) stats_sls_ls_totalFlips;
		d2 = (double) stats_sls_time_flips;
		fVal = d1 / d2;
		printf("(%.3f per second in relation to SLS flip time)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:     SLS global time: %.3f seconds\n", stats_sls_time_total);
	printf("c   STATS:       SLS initialization time: %.3f seconds\n", stats_sls_time_init);
	printf("c   STATS:       SLS initialize try time: %.3f seconds\n", stats_sls_time_initTry);
	printf("c   STATS:       SLS flipping time: %.3f seconds\n", stats_sls_time_flips);
	printf("c   STATS:   CDCL:\n");
	printf("c   STATS:     CDCL calls: %u\n", (uint32_t) stats_cdcl_component_totalCalls);
	printf("c   STATS:     CDCL return codes [ERROR, SAT, UNSAT, UNKNOWN]: %u %u %u %u\n", (uint32_t) stats_cdcl_return_ERROR,
			(uint32_t) stats_cdcl_return_SAT, (uint32_t) stats_cdcl_return_UNSAT, (uint32_t) stats_cdcl_return_UNKNOWN);
	printf("c   STATS:     CDCL decisions made: %u\n", (uint32_t) stats_cdcl_numDecisions);
	printf("c   STATS:       CDCL random decisions: %u ", (uint32_t) stats_cdcl_numDecisionsRandom);
	if (stats_cdcl_numDecisions > 0){
		d1 = 100.0* ((double) stats_cdcl_numDecisionsRandom);
		d2 = (double) stats_cdcl_numDecisions;
		fVal = d1 / d2;
		printf("(%.2f %%)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:       CDCL variable agility re-scales: %u\n", (uint32_t) stats_cdcl_numVarAgiRescales);
	printf("c   STATS:       CDCL variable activity re-scales: %u\n", (uint32_t) stats_cdcl_numVarActRescales);
	if (stats_cdcl_numDecisions > 0){
		uval = 0;
		fVal2 = 0;
		printf("c   STATS:       CDCL decision level visits [level: visits, visits up to, percent, percent up to]:\n");
		for (i = 0; i < STATS_CDCL_DECLEVELVISITED_MAX + 2; ++i){
			uval += stats_cdcl_decLevelVisited[i];
			d1 = 100.0f*stats_cdcl_decLevelVisited[i];
			d2 = stats_cdcl_numDecisions;
			fVal = d1/d2;
			fVal2 += fVal;
			printf("c   STATS:       CDCL Level %2d:\t%8u\t%8u\t%.2f\t\t%.2f\n",
					i, (uint32_t)stats_cdcl_decLevelVisited[i], (uint32_t)uval, fVal, fVal2);
			if (fVal2 >= 99.99) break;
		}
	}
	printf("c   STATS:     CDCL back-jumps performed: %u\n", (uint32_t) stats_cdcl_numBackjumps);
	printf("c   STATS:     CDCL restarts performed: %u\n", (uint32_t) stats_cdcl_numRestarts);
	printf("c   STATS:     CDCL restarts prohibited: %u\n", (uint32_t) stats_cdcl_numRestartProhibits);
	printf("c   STATS:     CDCL assignments propagated: %u", (uint32_t) stats_cdcl_numPropagations);
	if (stats_cdcl_time_up > ZERO){
		fVal = ((float_ty)stats_cdcl_numPropagations) / stats_cdcl_time_up;
		printf(" (%.3f per second against UP time)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:     CDCL conflicts found: %u\n", (uint32_t) stats_cdcl_numConflictsFound);
	printf("c   STATS:       CDCL clause activity re-scales: %u\n", (uint32_t) stats_cdcl_numClsActRescales);
	printf("c   STATS:       CDCL learned clause database maintenances performed: %u\n", (uint32_t) stats_cdcl_numMaintenances);
	printf("c   STATS:         CDCL performed using glue: %u\n", (uint32_t) stats_cdcl_numMaintWithGlue);
	printf("c   STATS:         CDCL performed using activity: %u\n", (uint32_t) stats_cdcl_numMaintWithActivity);
	printf("c   STATS:         CDCL learned clauses removed by maintenance: %u\n", (uint32_t) stats_cdcl_numLearnedRemoved);
	if (stats_cdcl_numMaintenances > 0){
		printf("c   STATS:         CDCL glue deviation count (interval omitted if zero count):\n");
		for (i = 0; i < 101; ++i){
			if (stats_cdcl_glueDev[i] > 0 || 100*param_cdclMaintenanceMinDev == i) {
				printf("c   STATS:           CDCL interval 0.%02d: %u", i, (uint32_t) stats_cdcl_glueDev[i]);
				if (100*param_cdclMaintenanceMinDev == i){
					printf(" < param_cdclMaintenanceMinDev\n");
				} else {
					printf("\n");
				}
			}
		}
		printf("c   STATS:         CDCL glue average count (interval omitted if zero count):\n");
		for (i = 0; i < 101; ++i){
			if (stats_cdcl_glueAvg[i] > 0 || floor(param_cdclMaintenanceMaxAvg) == i) {
				printf("c   STATS:           CDCL interval %3d: %u", i, (uint32_t) stats_cdcl_glueAvg[i]);
				if (floor(param_cdclMaintenanceMaxAvg) == i){
					printf(" < param_cdclMaintenanceMaxAvg\n");
				} else {
					printf("\n");
				}
			}
		}
	}
	printf("c   STATS:     CDCL clauses learned: %u", (uint32_t) stats_cdcl_numClausesLearned);
	if (stats_cdcl_time_confAnalysis > ZERO){
		fVal = ((float_ty)stats_cdcl_numClausesLearned) / stats_cdcl_time_confAnalysis;
		printf(" (%.3f per second against conflict analysis time)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:       CDCL learned clauses strengthening checks: %u\n", (uint32_t)stats_cdcl_numStrChecks);
	printf("c   STATS:         CDCL strengthening success / replacements: %u / %u ",(uint32_t) stats_cdcl_numStrSuccess,
			(uint32_t)stats_cdcl_numStrSuccessIsRepl);
	if (stats_cdcl_numStrChecks > 0){
		fVal = 100.0 * (((float_ty) stats_cdcl_numStrSuccess) /  ((float_ty)stats_cdcl_numStrChecks));
		printf(" (%.3f %% / ", fVal);
		fVal = 100.0 * (((float_ty) stats_cdcl_numStrSuccessIsRepl) /  ((float_ty)stats_cdcl_numStrChecks));
		printf(" %.3f %%)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:         CDCL strengthening signature success / checks: %u / %u",(uint32_t) stats_cdcl_numStrSigSuccess,
			(uint32_t)stats_cdcl_numStrSigChecks);
	if (stats_cdcl_numStrSigChecks > 0){
		fVal = 100.0 * (((float_ty) stats_cdcl_numStrSigSuccess) /  ((float_ty)stats_cdcl_numStrSigChecks));
		printf(" (%.3f %%)\n", fVal);
	} else {
		printf("\n");
	}
	printf("c   STATS:         CDCL strengthening used reasons / no reasons: %u / %u\n",(uint32_t) stats_cdcl_numStrSuccessAreReasons,
			(uint32_t)stats_cdcl_numStrSuccessNoReasons);
	if (stats_cdcl_numClausesLearned > 0){
		uval = 0;
		fVal2 = 0;
		printf("c   STATS:         CDCL learned clause size [size: amount, amount up to, percent, percent up to]:\n");
		for (i = 0; i < STATS_CDCL_LEARNEDLENGTH_MAX + 2; ++i){
			uval += stats_cdcl_learnedLength[i];
			d1 = 100.0f*stats_cdcl_learnedLength[i];
			d2 = stats_cdcl_numClausesLearned;
			fVal = d1/d2;
			fVal2 += fVal;
			printf("c   STATS:         CDCL Size %2d:\t%8u\t%8u\t%.2f\t\t%.2f\n",
					i, (uint32_t)stats_cdcl_learnedLength[i], (uint32_t)uval, fVal, fVal2);
			if (fVal2 >= 99.99) break;
		}
	}
	printf("c   STATS:     CDCL other clauses strengthening checks: %u\n", (uint32_t)stats_cdcl_numSorChecks);
	printf("c   STATS:       CDCL success, large: %u\n", (uint32_t)stats_cdcl_numSorSuccLarge);
	printf("c   STATS:       CDCL success, binary: %u\n", (uint32_t)stats_cdcl_numSorSuccBin);
	printf("c   STATS:       CDCL success, unit: %u\n", (uint32_t)stats_cdcl_numSorSuccUnit);
	printf("c   STATS:     CDCL number of in-processings: %u\n", (uint32_t)stats_cdcl_numInprocessing);
	printf("c   STATS:       CDCL number of clause database reductions: %u\n", (uint32_t) stats_cdcl_numReductions);
	printf("c   STATS:         CDCL number of removed satisfied clauses: %u\n", (uint32_t) stats_cdcl_numRedSatCls);
	printf("c   STATS:         CDCL number of removed false literals: %u\n", (uint32_t) stats_cdcl_numRedFalsLits);
	printf("c   STATS:     CDCL number of search aborts: %u\n", (uint32_t)stats_cdcl_numAbort);
	if (stats_cdcl_numAbort > 0U){
		fVal2 = stats_cdcl_numAbort;
		fVal = stats_cdcl_numAbortDLZero; d1 = 100.0 * (fVal / fVal2);
		printf("c   STATS:       CDCL because of new DL0 assignments: %u (%f %%)\n",(uint32_t)stats_cdcl_numAbortDLZero, d1);
		fVal = stats_cdcl_numAbortAssignmentMax; d1 = 100.0 * (fVal / fVal2);
		printf("c   STATS:       CDCL because of maximum assignments: %u (%f %%)\n",
				(uint32_t)stats_cdcl_numAbortAssignmentMax, d1);
		fVal = stats_cdcl_numAbortAssignmentMin; d1 = 100.0 * (fVal / fVal2);
		printf("c   STATS:       CDCL because of minimum assignments: %u (%f %%)\n",
				(uint32_t)stats_cdcl_numAbortAssignmentMin, d1);
		fVal = stats_cdcl_numAbortClsImpact; d1 = 100.0 * (fVal / fVal2);
		printf("c   STATS:       CDCL because of strong learned clauses impact: %u (%f %%)\n",
				(uint32_t)stats_cdcl_numAbortClsImpact, d1);
	}
	printf("c   STATS:     CDCL global time: %.3f seconds\n", stats_cdcl_time_total);
	printf("c   STATS:       CDCL initialization time: %.3f seconds\n", stats_cdcl_time_init);
	printf("c   STATS:       CDCL unit propagation time: %.3f seconds\n", stats_cdcl_time_up);
	printf("c   STATS:       CDCL conflict analysis time: %.3f seconds\n", stats_cdcl_time_confAnalysis);
	printf("c   STATS:       CDCL strengthening learned clauses time: %.3f seconds\n", stats_cdcl_time_str);
	printf("c   STATS:       CDCL maintenance time of learned clauses: %.3f seconds\n", stats_cdcl_time_maint);
	printf("c   STATS:       CDCL in-processing time: %.3f seconds\n", stats_cdcl_time_inp);
	printf("c   STATS:         CDCL clause database reduction time: %.3f seconds\n", stats_cdcl_time_reduce);
	printf("c   STATS: HANDLERESULT:\n");
	printf("c   STATS:   HANDLERESULT calls: %u\n", (uint32_t) stats_handleresult_component_totalCalls);
	printf("c   STATS:   HANDLERESULT global time: %.3f seconds\n", stats_handleresult_time_total);
	printf("c   STATS:   SOLWRITER:\n");
	printf("c   STATS:     SOLWRITER calls: %u\n", (uint32_t) stats_solwriter_component_totalCalls);
	printf("c   STATS:     SOLWRITER number of all variables (assigned to 0, assigned to 1, unassigned): %d, %d, %d\n",
			stats_solwriter_numNegVars, stats_solwriter_numPosVars, stats_solwriter_numUnaVars);
	printf("c   STATS:     SOLWRITER number of assigned auxiliary variables: %d\n", stats_solwriter_numAuxVars);
	printf("c   STATS:     SOLWRITER number of assigned input variables: %d\n", stats_solwriter_numInpVars);
	printf("c   STATS:     SOLWRITER total number of assigned variables: %d\n", stats_solwriter_numAllVars);
	if (stats_solwriter_assignmentBiasAux > -2.0){
		printf("c   STATS:     SOLWRITER assignment bias (auxiliary variables): %.3f\n", stats_solwriter_assignmentBiasAux);
	}
	if (stats_solwriter_assignmentBiasInp > -2.0){
		printf("c   STATS:     SOLWRITER assignment bias (input variables): %.3f\n", stats_solwriter_assignmentBiasInp);
	}
	if (stats_solwriter_assignmentBiasAll > -2.0){
		printf("c   STATS:     SOLWRITER assignment bias (all variables): %.3f\n", stats_solwriter_assignmentBiasAll);
	}
	printf("c   STATS:     SOLWRITER global time: %.3f seconds\n", stats_solwriter_time_total);
	printf("c   STATS: SHUTDOWN:\n");
	printf("c   STATS:   SHUTDOWN calls: %u\n", (uint32_t) stats_shutdown_component_totalCalls);
	printf("c   STATS:   SHUTDOWN global time: %.3f seconds\n", stats_shutdown_time_total);


	printf("c   STATS: GENERAL:\n");
	printf("c   STATS:   Clauses %d, satisfied by number of literals:\n", stats_general_countedClauses);
	for (i=0; i < STATS_GENERAL_COUNTSATLITS_MAX+1;++i){
		if (stats_general_satLitsByLength[i] > 0){
			fVal = stats_general_satLitsByLength[i];
			fVal2 = stats_general_countedClauses;
			if (i == STATS_GENERAL_COUNTSATLITS_MAX){
				printf("c   STATS:    more: %d (%.3f %%)\n", stats_general_satLitsByLength[i], (fVal/fVal2) * 100.0);
			} else {
				printf("c   STATS:    %4d: %d (%.3f %%)\n", i, stats_general_satLitsByLength[i], (fVal/fVal2) * 100.0);
			}
		}
	}


	printf("c   STATS: Global timing overview (in seconds):\n");
	printf("c   STATS:   All: %.3f\n", ((float_ty)clock())/((float_ty)CLOCKS_PER_SEC));
	printf("c   STATS:     Initialization:               %.3f\n", stats_main_time_init);
	printf("c   STATS:       Resetting:                  %.3f\n", stats_reset_time_total);
	printf("c   STATS:       Parameters:                 %.3f\n", stats_params_time_total);
	printf("c   STATS:       Guidance:                   %.3f\n", stats_guidance_time_total);
	printf("c   STATS:       Startup:                    %.3f\n", stats_startup_time_total);
	printf("c   STATS:         DETERM:                   %.3f\n", stats_determ_time_total);
	printf("c   STATS:         SPECCER:                  %.3f\n", stats_speccer_time_total);
	printf("c   STATS:         LOADER:                   %.3f\n", stats_loader_time_total);
	printf("c   STATS:     Classification:               %.3f\n", stats_classify_time_total);
	printf("c   STATS:       Simplification:             %.3f\n", stats_classify_time_simplify);
	printf("c   STATS:       ATTRIBUTOR global time:     %.3f\n", stats_attributor_time_total);
	printf("c   STATS:       RANDOMFOREST global time:   %.3f\n", stats_randomforest_time_total);
	printf("c   STATS:     Adaptation:                   %.3f\n", stats_adapt_time_total);
	printf("c   STATS:       ITEADAPTER global time:     %.3f\n", stats_iteadapter_time_total);
	printf("c   STATS:     Preprocessing:                %.3f\n", stats_prep_time_total);
	printf("c   STATS:       PRE global time:            %.3f\n", stats_pre_time_total);
	printf("c   STATS:       SATELITE global time:       %.3f\n", stats_satelite_time_total);
	printf("c   STATS:     Search global time:           %.3f\n", stats_search_time_total);
	printf("c   STATS:       In-processing global time:  %.3f\n", stats_inp_time_total);
	printf("c   STATS:         IN global time:           %.3f\n", stats_inp_time_total);
	printf("c   STATS:       MP global time:             %.3f\n", stats_mp_time_total);
	printf("c   STATS:       SLS global time:            %.3f\n", stats_sls_time_total);
	printf("c   STATS:       CDCL global time:           %.3f\n", stats_cdcl_time_total);
	printf("c   STATS:     Handling result:              %.3f\n", stats_handleresult_time_total);
	printf("c   STATS:       SOLWRITER global time:      %.3f\n", stats_solwriter_time_total);
	printf("c   STATS:     Shutdown:                     %.3f\n", stats_shutdown_time_total);
}
#endif
