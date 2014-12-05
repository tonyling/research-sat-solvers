/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "adapt.h"

void adapt_extern_adaptation(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_adapt_component_totalCalls;
	#endif

	if (param_adaptStrategy != ADAPT_STRATEGY_NULL){
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: ADAPTATION...\n");fflush(stdout);
		#endif

		adapt_strategy_reset();
		adapt_strategy_init();
		if (adapt_returnCode == ADAPT_UNKNOWN){
			adapt_strategy_execute();
			if (adapt_returnCode == ADAPT_UNKNOWN){
				guidance_extern_enforceGuideAfterAdaptation();
				#ifdef VERBOSE_ADAPT
				params_extern_printAll("c     ADAPT:");
				#endif
			}
		}
		adapt_strategy_dispose();
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: Done with adaptation.\n");fflush(stdout);
		#endif
	}

	#ifdef COLLECTSTATS
	stats_adapt_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void adapt_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_adapt_component_totalCalls;
	#endif
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: RESET...\n");
	#endif
	//Reset the adaptation.
	adapt_returnCode = ADAPT_UNKNOWN;
	adapt_strategies_pointer_reset();

	#ifdef COLLECTSTATS
	stats_adapt_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void adapt_initPhase(){
	//Initialize the adaptation.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_adapt_component_totalCalls;
	#endif
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: INITIALIZE...\n");
	#endif
	adapt_returnCode = ADAPT_UNKNOWN;
	adapt_strategies_pointer_init();

	#ifdef COLLECTSTATS
	stats_adapt_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void adapt_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_adapt_component_totalCalls;
	#endif

	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: DISPOSE...\n");
	#endif
	//For freeing all adaptation related memory.

	adapt_returnCode = ADAPT_UNKNOWN;
	adapt_strategies_pointer_dispose();
	#ifdef COLLECTSTATS
	stats_adapt_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
