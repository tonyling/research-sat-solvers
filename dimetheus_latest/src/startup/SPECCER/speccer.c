/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "speccer.h"

void speccer_extern_printVersion(){
	printf("[Startup         ] [Formula specifier and tranlator] :: %3d.%-4d :: %s",
			SPECCER_VERSION_MA, SPECCER_VERSION_MI, SPECCER_VERSION_NAME);
}

void speccer_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_speccer_component_totalCalls;
	#endif
	#ifdef VERBOSE_SPECCER
	printf("c     SPECCER: Component reset... \n");
	#endif
	speccer_returnCode = SPECCER_UNKNOWN;

	//Reset all plug-ins.
	speccer_loadSpecs_dimacscnf_resetPlugin();
	speccer_loadSpecs_aplefoo_resetPlugin();

	#ifdef COLLECTSTATS
	stats_speccer_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void speccer_initModule(){
	#ifdef VERBOSE_SPECCER
	printf("c     SPECCER: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_speccer_component_totalCalls;
	#endif
	speccer_returnCode = SPECCER_UNKNOWN;

	//Initialize all plug-ins.
	speccer_loadSpecs_dimacscnf_initPlugin();	if (speccer_returnCode != SPECCER_UNKNOWN) return;
	speccer_loadSpecs_aplefoo_initPlugin(); 	if (speccer_returnCode != SPECCER_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_speccer_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void speccer_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_speccer_component_totalCalls;
	#endif
	#ifdef VERBOSE_SPECCER
	printf("c     SPECCER: Component dispose... \n");
	#endif
	speccer_returnCode = SPECCER_UNKNOWN;

	//Dispose all plug-ins.
	speccer_loadSpecs_dimacscnf_disposePlugin();
	speccer_loadSpecs_aplefoo_disposePlugin();

	#ifdef COLLECTSTATS
	stats_speccer_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void speccer_extern_determineSpecification(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_speccer_component_totalCalls;
	#endif
	#ifdef VERBOSE_SPECCER
	printf("c     SPECCER: Determining input specification.\n");
	#endif
	#ifndef COMPILEASLIBRARY
		#ifdef VERBOSE_SPECCER
		printf("c     SPECCER:   Using file.\n");
		#endif
		if (param_formula == NULL){
			printf("c ERROR. SPECCER cannot read problem specifications from file if there is no open input file.\n");
			speccer_returnCode = SPECCER_ERROR; return;
		}
	#else
		#ifdef VERBOSE_SPECCER
		printf("c     SPECCER:   Using interface.\n");
		#endif
	#endif
	//Below, we check all known input formats and use the appropriate one.
	if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF && speccer_returnCode == SPECCER_UNKNOWN){
		speccer_loadSpecs_dimacscnf();
	} else if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO && speccer_returnCode == SPECCER_UNKNOWN){
		speccer_loadSpecs_aplefoo();
	} else {
		printf("c ERROR. SPECCER cannot handle the provided input format.\n");
		speccer_returnCode = SPECCER_ERROR; return;
	}

	#ifdef VERBOSE_SPECCER
	if (speccer_returnCode == SPECCER_UNKNOWN){
		printf("c     SPECCER: Problem specification: %u variables and %u clauses.\n", f.n_initial, f.m_initial);
	} else {
		printf("c ERROR. SPECCER was unable to determine the problem specifications.\n");
	}
	#endif
	#ifdef COLLECTSTATS
	stats_speccer_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

