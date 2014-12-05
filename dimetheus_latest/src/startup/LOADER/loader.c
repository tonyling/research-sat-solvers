/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "loader.h"

void loader_extern_printVersion(){
	printf("[Startup         ] [CNF Formula Loader             ] :: %3d.%-4d :: %s",
			LOADER_VERSION_MA, LOADER_VERSION_MI, LOADER_VERSION_NAME);
}

void loader_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_loader_component_totalCalls;
	#endif
	#ifdef VERBOSE_LOADER
	printf("c     LOADER: Component reset... \n");
	#endif
	loader_returnCode = LOADER_UNKNOWN;

	//Reset all plug-ins.
	loader_loadClauses_dimacscnf_resetPlugin();

	#ifdef COLLECTSTATS
	stats_loader_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void loader_initModule(){
	#ifdef VERBOSE_LOADER
	printf("c     LOADER: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_loader_component_totalCalls;
	#endif
	loader_returnCode = LOADER_UNKNOWN;

	//Initialize all plug-ins.
	loader_loadClauses_dimacscnf_initPlugin(); if (loader_returnCode != LOADER_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_loader_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void loader_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_loader_component_totalCalls;
	#endif
	#ifdef VERBOSE_LOADER
	printf("c     LOADER: Component dispose... \n");
	#endif
	loader_returnCode = LOADER_UNKNOWN;

	//Dispose all plug-ins.
	loader_loadClauses_dimacscnf_disposePlugin();

	#ifdef COLLECTSTATS
	stats_loader_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void loader_extern_loadAsCNF(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_loader_component_totalCalls;
	#endif
	#ifdef VERBOSE_LOADER
	printf("c     LOADER: Loading input into CNF formula data-structure.\n");
	#endif
	#ifndef COMPILEASLIBRARY
		#ifdef VERBOSE_LOADER
		printf("c     LOADER:   Using file.\n");
		#endif
		if (param_formula == NULL){
			printf("c ERROR. LOADER cannot load a problem from file if there is no open input file.\n");
			loader_returnCode = LOADER_ERROR; return;
		}
	#else
		#ifdef VERBOSE_LOADER
		printf("c     LOADER:   Using interface.\n");
		#endif
	#endif

	if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
		loader_loadClauses_dimacscnf();
	} else if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
		loader_loadClauses_aplefoo();
	} else {
		printf("c ERROR. LOADER cannot handle the provided input format for loading the formula as CNF.\n");
		loader_returnCode = LOADER_ERROR; return;
	}

	#ifdef COLLECTSTATS
	stats_loader_numVariables = f.n_initial;
	stats_loader_numInitialUnits = (int32_t)(main_simpleUPQueue_current - main_simpleUPQueue);
	stats_loader_numInitialClauses = f.m_eo_used;
	#endif

	#ifdef VERBOSE_LOADER
	if (loader_returnCode == LOADER_UNKNOWN){
		printf("c     LOADER: Variables: %d remaining (%d unit assignments).\n",
			f.n_initial-(int32_t)(main_simpleUPQueue_current - main_simpleUPQueue), (int32_t)(main_simpleUPQueue_current - main_simpleUPQueue));
		if ((int32_t)(main_simpleUPQueue_current - main_simpleUPQueue) == 1){
			printf("c     LOADER:   Single unit literal: %d\n", *main_simpleUPQueue);
		}
		printf("c     LOADER: Clauses: %d remaining.\n", f.m_eo_used);
	} else if (loader_returnCode == LOADER_UNSAT){
		printf("c     LOADER: Formula is UNSATISFIABLE.\n");
	}
	#endif
	#ifdef COLLECTSTATS
	stats_loader_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

