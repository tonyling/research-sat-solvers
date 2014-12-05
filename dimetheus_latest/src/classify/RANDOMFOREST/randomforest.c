/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "randomforest.h"

void randomforest_extern_printVersion(){
	printf("[Classify        ] [Random Forest classifier       ] :: %3d.%-4d :: %s",
			RANDOMFOREST_VERSION_MA, RANDOMFOREST_VERSION_MI, RANDOMFOREST_VERSION_NAME);
}

void randomforest_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_randomforest_component_totalCalls;
	#endif
	#ifdef VERBOSE_RANDOMFOREST
	printf("c     RANDOMFOREST: Component reset... \n");
	#endif
	randomforest_returnCode = RANDOMFOREST_UNKNOWN;
	#ifdef COLLECTSTATS
	stats_randomforest_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void randomforest_initModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_randomforest_component_totalCalls;
	#endif
	#ifdef VERBOSE_RANDOMFOREST
	printf("c     RANDOMFOREST: Component initialize... \n");
	#endif
	randomforest_returnCode = RANDOMFOREST_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_randomforest_time_init += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	stats_randomforest_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void randomforest_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_randomforest_component_totalCalls;
	#endif
	#ifdef VERBOSE_RANDOMFOREST
	printf("c     RANDOMFOREST: Component dispose... \n");
	#endif
	randomforest_returnCode = RANDOMFOREST_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_randomforest_time_total += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void randomforest_extern_classify(){
	//Performs classification using the random forest.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_randomforest_component_totalCalls;
	#endif
	if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_GENERIC){
		//Generic means that we ask the global generic forest.
		randomforest_intern_generic_voteAll();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_RANDOM){
		//Random means we use the special random forest.
		randomforest_intern_random_voteAll();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_CRAFTED){
		//Crafted means we ask the global generic forest, since we do not have any restrictions so far.
		randomforest_intern_crafted_voteAll();
	} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_APPLICATION){
		//Application means we ask the global generic forest, since we do not have any restrictions so far.
		randomforest_intern_application_voteAll();
	} else {
		printf("c ERROR. RANDOMFOREST does not know input domain %d.\n", param_classifyInputDomain);
		randomforest_returnCode = RANDOMFOREST_ERROR; return;
	}
	#ifdef COLLECTSTATS
	stats_randomforest_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
