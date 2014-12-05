/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "searchStrategies.h"

void search_strategy_sls_printHelp(){
	printf("c      %-3d: SLS\n", SEARCH_STRATEGY_SLS);
    printf("c           Behavior: Performs only stochastic local search [SLS].\n");
}

void search_strategy_sls_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (SLS)...\n");
	#endif
	sls_resetModule();
}

void search_strategy_sls_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (SLS)...\n");
	#endif
	sls_initModule(); if (sls_returnCode != SLS_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
}

void search_strategy_sls_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (SLS)...\n");
	#endif
	sls_disposeModule();
}

void search_strategy_sls_execute(){
	//This search strategy simply calls for the SLS module and have it perform local search until it either finds a solution
	//or aborts because of the maximum number of flips is reached.
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (SLS)...\n");
	#endif
	sls_extern_localSearch(1);
	if (sls_returnCode == SLS_UNKNOWN){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: SLS was unable to find a satisfying assignment. SLS failed.\n");
		#endif
		//The SLS did not find a satisfying assignment. This does not necessarily mean that there is non, but in
		//the current search strategy, this is considered a failure.
		search_returnCode = SEARCH_UNKNOWN;
	} else if (sls_returnCode == SLS_SAT){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: SLS found a solution. SLS succeeded. Copying variable assignments.\n");
		#endif
		//The SLS did indeed find a solution.
		search_intern_transferSLSAss();
		search_returnCode = SEARCH_SAT;
	} else {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: SLS return code unexpected after local search. We assume an error. SLS failed.\n");
		#endif
		//In case we get this far, the SLS returned an unexpected return code. The strategy failed.
		search_returnCode = SEARCH_ERROR;
	}
}
