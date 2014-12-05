/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "reset.h"

void reset_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_reset_component_totalCalls;
	#endif
	#ifdef VERBOSE_RESET
	printf("c   RESET: RESET...\n");
	#endif
	reset_returnCode = RESET_UNKNOWN;
	#ifdef COLLECTSTATS
	stats_reset_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void reset_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_reset_component_totalCalls;
	#endif
	#ifdef VERBOSE_RESET
	printf("c   RESET: INITIALIZE...\n");
	#endif
	main_returnCode = UNKNOWN;
	reset_returnCode = RESET_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_reset_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void reset_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_reset_component_totalCalls;
	#endif
	#ifdef VERBOSE_RESET
	printf("c   RESET: DISPOSE...\n");
	#endif
	reset_returnCode = RESET_UNKNOWN;
	#ifdef COLLECTSTATS
	stats_reset_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void reset_extern_resetAll(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_reset_component_totalCalls;
	#endif
	#ifdef VERBOSE_RESET
	printf("c   RESET: RESETTING...\n");
	printf("c   RESET: Signal handler...\n");
	#endif
	#ifndef COMPILEASLIBRARY
	signal(SIGTERM, &main_handle_signal);
	signal(SIGINT, &main_handle_signal);
	signal(SIGABRT, &main_handle_signal);
	#endif

	#ifdef VERBOSE_RESET
	printf("c   RESET: Scoring and statistical data...\n");
	#endif
	#ifdef SCORING
	scoring_reset();
	#endif
	#ifdef COLLECTSTATS
	stats_reset();
	#endif

	#ifdef VERBOSE_RESET
	printf("c   RESET: Random number generator...\n");
	#endif
	srand(param_seed);
	rand_reset();

	#ifdef VERBOSE_RESET
	printf("c   RESET: File handlers...\n");
	#endif
	param_formula = NULL;
	param_startupOutput = NULL;
	param_prepOutput = NULL;
	param_cdclOutput = NULL;

	#ifdef VERBOSE_RESET
	printf("c   RESET: Modules...\n");
	#endif
	determ_resetModule();
	speccer_resetModule();
	loader_resetModule();
	iteadapter_resetModule();
	attributor_resetModule();
	randomforest_resetModule();
	pre_resetModule();
	satelite_resetModule();
	in_resetModule();
	mp_resetModule();
	sls_resetModule();
	cdcl_resetModule();

	#ifdef VERBOSE_RESET
	printf("c   RESET: Phases...\n");
	#endif
	params_resetPhase();
	guidance_resetPhase();
	startup_resetPhase();
	classify_resetPhase();
	adapt_resetPhase();
	prep_resetPhase();
	inp_resetPhase();
	search_resetPhase();
	handleresult_resetPhase();
	shutdown_resetPhase();

	#ifdef VERBOSE_RESET
	printf("c   RESET: Main data-structures...\n");
	#endif
	main_varNames = NULL;
	formula_reset();
	variables_reset();
	literals_reset();
	clauses_reset();
	main_simpleUP_reset();
	main_complexup_reset();
	main_decStack_reset();
	cQ_reset();
	cS_reset();
	cSS0_reset();
	cSS1_reset();
	lQ_reset();
	lS_reset();
	vQ_reset();
	vS_reset();

	#ifdef COLLECTSTATS
	stats_reset_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
