/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "solwriter.h"

void solwriter_extern_printVersion(){
	printf("[HandleResult    ] [Solution writer                ] :: %3d.%-4d :: %s",
			SOLWRITER_VERSION_MA, SOLWRITER_VERSION_MI, SOLWRITER_VERSION_NAME);
}

void solwriter_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_solwriter_component_totalCalls;
	#endif
	#ifdef VERBOSE_SOLWRITER
	printf("c     SOLWRITER: Component reset... \n");
	#endif
	solwriter_returnCode = SOLWRITER_UNKNOWN;

	//Reset all plug-ins.
	solwriter_dimacscnf_resetPlugin();
	solwriter_aplefoo_resetPlugin();

	#ifdef COLLECTSTATS
	stats_solwriter_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void solwriter_initModule(){
	#ifdef VERBOSE_SOLWRITER
	printf("c     SOLWRITER: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_solwriter_component_totalCalls;
	#endif
	solwriter_returnCode = SOLWRITER_UNKNOWN;

	//Initialize all plug-ins.
	solwriter_dimacscnf_initPlugin(); if (solwriter_returnCode != SOLWRITER_UNKNOWN) return;
	solwriter_aplefoo_initPlugin(); if (solwriter_returnCode != SOLWRITER_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_solwriter_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void solwriter_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_solwriter_component_totalCalls;
	#endif
	#ifdef VERBOSE_SOLWRITER
	printf("c     SOLWRITER: Component dispose... \n");
	#endif
	solwriter_returnCode = SOLWRITER_UNKNOWN;

	//Reset all plug-ins.
	solwriter_dimacscnf_disposePlugin();
	solwriter_aplefoo_disposePlugin();

	#ifdef COLLECTSTATS
	stats_solwriter_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void solwriter_extern_writeSolution(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_solwriter_component_totalCalls;
	#endif

	#ifdef VERBOSE_SOLWRITER
	printf("c     SOLWRITER: Writing solution... \n");
	#endif

	//First, we check if the provided result.
	if (handleresult_returnCode == HANDLERESULT_UNKNOWN
			|| handleresult_returnCode == HANDLERESULT_ERROR
			|| handleresult_returnCode == HANDLERESULT_WRONGSOLUTION){
		//The result implies the "UNKNOWN" answer.
		if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
			solwriter_dimacscnf_printUNKNOWN();
		} else if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
			solwriter_aplefoo_printUNKNOWN();
		} else {
			printf("c ERROR. SOLWRITER cannot handle the provided input-format.\n");
			solwriter_returnCode = SOLWRITER_ERROR;
		}
	} else if (handleresult_returnCode == HANDLERESULT_CORRECTSOLUTION){
		//The result implies a real answer.
		if (main_returnCode == SAT){
			//And the answer is satisfiable.
			if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
				solwriter_dimacscnf_printSAT();
			} else if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
				solwriter_aplefoo_printSAT();
			} else {
				printf("c ERROR. SOLWRITER cannot handle the provided input-format.\n");
				solwriter_returnCode = SOLWRITER_ERROR;
			}
		} else {
			//And the answer is unsatisfiable.
			if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
				solwriter_dimacscnf_printUNSAT();
			} else if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
				solwriter_aplefoo_printUNSAT();
			} else {
				printf("c ERROR. SOLWRITER cannot handle the provided input-format.\n");
				solwriter_returnCode = SOLWRITER_ERROR;
			}
		}
	} else {
		printf("c ERROR. SOLWRITER cannot handle the provided return-code.\n");
		solwriter_returnCode = SOLWRITER_ERROR;
	}

	#ifdef COLLECTSTATS
	stats_solwriter_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}
