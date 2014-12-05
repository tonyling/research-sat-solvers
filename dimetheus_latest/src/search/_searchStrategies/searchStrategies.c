/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategies_pointer_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Resetting strategy pointers...\n");
	#endif
	search_strategy_reset 	= &search_strategy_null_reset;
	search_strategy_init 	= &search_strategy_null_init;
	search_strategy_execute = &search_strategy_null_execute;
	search_strategy_dispose = &search_strategy_null_dispose;
}

void search_strategies_pointer_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Picking strategy pointers...\n");
	#endif
	if (param_searchStrategy == SEARCH_STRATEGY_SLS){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, SLS.\n", SEARCH_STRATEGY_SLS);
		#endif
		search_strategy_reset 	= &search_strategy_sls_reset;
		search_strategy_init 	= &search_strategy_sls_init;
		search_strategy_execute = &search_strategy_sls_execute;
		search_strategy_dispose = &search_strategy_sls_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_CDCL){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, CDCL.\n", SEARCH_STRATEGY_CDCL);
		#endif
		search_strategy_reset 	= &search_strategy_cdcl_reset;
		search_strategy_init 	= &search_strategy_cdcl_init;
		search_strategy_execute = &search_strategy_cdcl_execute;
		search_strategy_dispose = &search_strategy_cdcl_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_PMPCDCL){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, PMPCDCL.\n", SEARCH_STRATEGY_PMPCDCL);
		#endif
		search_strategy_reset 	= &search_strategy_pmpcdcl_reset;
		search_strategy_init 	= &search_strategy_pmpcdcl_init;
		search_strategy_execute = &search_strategy_pmpcdcl_execute;
		search_strategy_dispose = &search_strategy_pmpcdcl_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_PMPSLS){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, PMPSLS.\n", SEARCH_STRATEGY_PMPSLS);
		#endif
		search_strategy_reset 	= &search_strategy_pmpsls_reset;
		search_strategy_init 	= &search_strategy_pmpsls_init;
		search_strategy_execute = &search_strategy_pmpsls_execute;
		search_strategy_dispose = &search_strategy_pmpsls_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_HYBRIDMSC){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, HYBRIDMSC.\n", SEARCH_STRATEGY_HYBRIDMSC);
		#endif
		search_strategy_reset 	= &search_strategy_hybridmsc_reset;
		search_strategy_init 	= &search_strategy_hybridmsc_init;
		search_strategy_execute = &search_strategy_hybridmsc_execute;
		search_strategy_dispose = &search_strategy_hybridmsc_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_EMP){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, EMP.\n", SEARCH_STRATEGY_EMP);
		#endif
		search_strategy_reset 	= &search_strategy_emp_reset;
		search_strategy_init 	= &search_strategy_emp_init;
		search_strategy_execute = &search_strategy_emp_execute;
		search_strategy_dispose = &search_strategy_emp_dispose;
	} else if (param_searchStrategy == SEARCH_STRATEGY_TESTING){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: %d, TESTING.\n", SEARCH_STRATEGY_TESTING);
		#endif
		search_strategy_reset 	= &search_strategy_testing_reset;
		search_strategy_init 	= &search_strategy_testing_init;
		search_strategy_execute = &search_strategy_testing_execute;
		search_strategy_dispose = &search_strategy_testing_dispose;
	} else {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: Search strategy: 0, NULL.\n");
		#endif
		search_strategy_reset 	= &search_strategy_null_reset;
		search_strategy_init 	= &search_strategy_null_init;
		search_strategy_execute = &search_strategy_null_execute;
		search_strategy_dispose = &search_strategy_null_dispose;
	}
}

void search_strategies_pointer_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Disposing strategy pointers...\n");
	#endif
	search_strategy_reset 	= &search_strategy_null_reset;
	search_strategy_init 	= &search_strategy_null_init;
	search_strategy_execute = &search_strategy_null_execute;
	search_strategy_dispose = &search_strategy_null_dispose;
}
