/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategy_testing_printHelp(){
	printf("c      %-3d: TESTING\n", SEARCH_STRATEGY_TESTING);
    printf("c           Behavior: Subject to changes in the source-code. Do not use.\n");
}

void search_strategy_testing_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (TESTING)...\n");
	#endif

}

void search_strategy_testing_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (TESTING)...\n");
	#endif

}

void search_strategy_testing_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (TESTING)...\n");
	#endif

}

void search_strategy_testing_execute(){
	//The behavior of this strategy is subject to changes. Do not use.
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (TESTING)...\n");
	#endif

}
