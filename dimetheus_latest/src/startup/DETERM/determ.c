/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "determ.h"

void determ_extern_printVersion(){
	printf("[Startup         ] [Input format Determination     ] :: %3d.%-4d :: %s",
			DETERM_VERSION_MA, DETERM_VERSION_MI, DETERM_VERSION_NAME);
}

void determ_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_determ_component_totalCalls;
	#endif
	#ifdef VERBOSE_DETERM
	printf("c     DETERM: Component reset... \n");
	#endif
	determ_returnCode = DETERM_UNKNOWN;

	//Reset all plug-ins.
	determ_determineInputFormat_dimacscnf_resetPlugin();
	determ_determineInputFormat_aplefoo_resetPlugin();

	#ifdef COLLECTSTATS
	stats_determ_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void determ_initModule(){
	#ifdef VERBOSE_DETERM
	printf("c     DETERM: Component initialize... \n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_determ_component_totalCalls;
	#endif
	determ_returnCode = DETERM_UNKNOWN;

	//Initialize all plug-ins.
	determ_determineInputFormat_dimacscnf_initPlugin(); if (determ_returnCode != DETERM_UNKNOWN) return;
	determ_determineInputFormat_aplefoo_initPlugin(); 	if (determ_returnCode != DETERM_UNKNOWN) return;

	#ifdef COLLECTSTATS
	stats_determ_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void determ_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_determ_component_totalCalls;
	#endif
	#ifdef VERBOSE_DETERM
	printf("c     DETERM: Component dispose... \n");
	#endif
	determ_returnCode = DETERM_UNKNOWN;

	//Dispose all plug-ins.
	determ_determineInputFormat_dimacscnf_disposePlugin();
	determ_determineInputFormat_aplefoo_disposePlugin();

	#ifdef COLLECTSTATS
	stats_determ_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void determ_extern_determineInputFormat(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_determ_component_totalCalls;
	#endif
	#ifdef VERBOSE_DETERM
	printf("c     DETERM: Determining input format.\n");
	#endif

	if (param_startupInputFormat != STARTUP_INPUTFORMAT_UNKNOWN){
		#ifdef VERBOSE_DETERM
		printf("c     DETERM:   The input format is enforced by param_startupInputFormat.\n");
		#endif
	} else {
		#ifndef COMPILEASLIBRARY
			#ifdef VERBOSE_DETERM
			printf("c     DETERM:   Using file.\n");
			#endif
			if (param_formula == NULL){
				printf("c ERROR. DETERM cannot determine the input format from file if there is no open input file.\n");
				determ_returnCode = DETERM_ERROR; return;
			}
		#else
			#ifdef VERBOSE_DETERM
			printf("c     DETERM:   Using interface.\n");
			#endif
		#endif

		if (param_startupInputFormat == STARTUP_INPUTFORMAT_UNKNOWN && determ_returnCode == DETERM_UNKNOWN){
			param_startupInputFormat = determ_determineInputFormat_dimacscnf();
			#ifdef VERBOSE_DETERM
			if (param_startupInputFormat == STARTUP_INPUTFORMAT_DIMACSCNF){
				printf("c     DETERM:     DIMACS CNF input format: yes\n");
			} else {
				printf("c     DETERM:     DIMACS CNF input format: no\n");
			}
			#endif
		}

		if (param_startupInputFormat == STARTUP_INPUTFORMAT_UNKNOWN && determ_returnCode == DETERM_UNKNOWN){
			param_startupInputFormat = determ_determineInputFormat_aplefoo();
			#ifdef VERBOSE_DETERM
			if (param_startupInputFormat == STARTUP_INPUTFORMAT_APLEFOO){
				printf("c     DETERM:     APLEFOO input format: yes\n");
			} else {
				printf("c     DETERM:     APLEFOO input format: no\n");
			}
			#endif
		}

		//Further input formats might follow here using the above if-statement scheme.

		//Finally, if no valid input format was detected, we return an error.
		if (param_startupInputFormat == STARTUP_INPUTFORMAT_UNKNOWN){
			printf("c ERROR. DETERM was unable to determine the input format.\n");
			determ_returnCode = DETERM_ERROR;
		}
	}
	#ifdef VERBOSE_DETERM
	printf("c     DETERM: The input format is %d (", param_startupInputFormat);
	startup_extern_printInputFormatName(param_startupInputFormat);
	printf(").\n");
	#endif
	#ifdef COLLECTSTATS
	stats_determ_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}
