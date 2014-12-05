/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "shutdown.h"

void shutdown_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_shutdown_component_totalCalls;
	#endif
	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN: RESET...\n");
	#endif
	shutdown_returnCode = SHUTDOWN_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_shutdown_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void shutdown_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_shutdown_component_totalCalls;
	#endif
	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN: INITIALIZE...\n");
	#endif
	shutdown_returnCode = SHUTDOWN_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_shutdown_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void shutdown_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_shutdown_component_totalCalls;
	#endif
	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN: DISPOSE...\n");
	#endif
	shutdown_returnCode = SHUTDOWN_UNKNOWN;
	#ifndef SILENT
	if (main_returnCode == ERROR
			|| main_returnCode == CONFLICTINGPARAMS
			|| main_returnCode == BADPARAM
			|| main_returnCode == BADFORMULA
			|| main_returnCode == BADINPUTFORMAT
			|| main_returnCode == WRONGSOLUTION) {
		printf("c   SHUTDOWN: TERMINATING WITH ERRORS (%d).\n", main_returnCode);
	} else if (main_returnCode == SIGNAL) {
		printf("c   SHUTDOWN: TERMINATING ON SIGNAL (%d).\n", main_returnCode);
	} else if (main_returnCode == SPECIALPARAM_HELP
			|| main_returnCode == SPECIALPARAM_VERSION
			|| main_returnCode == SPECIALPARAM_SPECS){
		printf("c   SHUTDOWN: TERMINATING ON SPECIAL PARAMETER (%d).\n", main_returnCode);
	}

	#ifdef COLLECTSTATS
	stats_shutdown_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif

	if (main_returnCode == UNKNOWN
			|| main_returnCode == SAT
			|| main_returnCode == UNSAT){
		#ifdef VERBOSE_SHUTDOWN
		printf("c   SHUTDOWN:   Presenting and disposing scores and statistics (if enabled).\n");fflush(stdout);
		#endif
		#ifdef SCORING
		scoring_print();
		#endif
		#ifdef COLLECTSTATS
		stats_print();
		#endif
	}
	#ifdef SCORING
	scoring_dispose();
	#endif
	#ifdef COLLECTSTATS
	stats_dispose();
	#endif

	#ifdef VERBOSE
	if (main_returnCode == SAT){
		printf("c   SHUTDOWN: TERMINATING WITH RESULT SAT.\n");
	} else if (main_returnCode == UNSAT){
		printf("c   SHUTDOWN: TERMINATING WITH RESULT UNSAT.\n");
	} else if (main_returnCode == UNKNOWN){
		printf("c   SHUTDOWN: TERMINATING WITH RESULT UNKNOWN.\n");
	}
	#endif
	#endif
	#ifndef SILENT
	printf("c ALL DONE.\n");
	fflush(stdout);
	#endif
}

void shutdown_extern_shutdown(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_shutdown_component_totalCalls;
	#endif
	uint32_t var;
	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN: SHUTTING DOWN...\n");
	printf("c   SHUTDOWN:   Disposing file handlers.\n");fflush(stdout);
	#endif
	#ifdef SPECS_PARAMGRAPH
	if (param_specsGraphOutput != NULL){
		fclose(param_specsGraphOutput);
		param_specsGraphOutput = NULL;
	}
	#endif

	if (param_formula != NULL) {
		fclose(param_formula);
		param_formula = NULL;
	}
	if (param_startupOutput != NULL){
		fclose(param_startupOutput);
		param_startupOutput = NULL;
	}
	if (param_prepOutput != NULL){
		fclose(param_prepOutput);
		param_prepOutput = NULL;
	}
	if (param_cdclOutput != NULL){
		fclose(param_cdclOutput);
		param_cdclOutput = NULL;
	}

	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN:   Disposing modules.\n");fflush(stdout);
	#endif
	determ_disposeModule();
	speccer_disposeModule();
	loader_disposeModule();
	iteadapter_disposeModule();
	attributor_disposeModule();
	randomforest_disposeModule();
	pre_disposeModule();
	satelite_disposeModule();
	in_disposeModule();
	mp_disposeModule();
	sls_disposeModule();
	cdcl_disposeModule();
	solwriter_disposeModule();

	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN:   Disposing strategies.\n");fflush(stdout);
	#endif
	inp_strategy_dispose();
	search_strategy_dispose();
	prep_strategy_dispose();
	adapt_strategy_dispose();
	classify_strategy_dispose();

	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN:   Disposing phases.\n");fflush(stdout);
	#endif
	handleresult_disposePhase();
	inp_disposePhase();
	search_disposePhase();
	prep_disposePhase();
	adapt_disposePhase();
	classify_disposePhase();
	guidance_disposePhase();
	params_disposePhase();
	reset_disposePhase();

	#ifdef VERBOSE_SHUTDOWN
	printf("c   SHUTDOWN:   Disposing main data-structures.\n");fflush(stdout);
	#endif
	if (main_varNames != NULL){
		for (var=0; var < f.n_initial+1; ++var){
			if (main_varNames[var] != NULL){
				free(main_varNames[var]);
				main_varNames[var] = NULL;
			}
		}
		free(main_varNames);
		main_varNames = NULL;
	}
	vS_dispose();
	vQ_dispose();
	lS_dispose();
	lQ_dispose();
	cS_dispose();
	cSS0_dispose();
	cSS1_dispose();
	cQ_dispose();
	main_simpleUP_dispose();
	main_complexup_dispose();
	main_decStack_dispose();
	variables_dispose();
	literals_dispose();
	clauses_dispose();
	formula_dispose();


	#ifdef COLLECTSTATS
	stats_shutdown_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

