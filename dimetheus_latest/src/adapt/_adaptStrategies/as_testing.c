/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "adaptStrategies.h"

void adapt_strategy_testing_printHelp(){
	printf("c      %-3d: TESTING:\n", ADAPT_STRATEGY_TESTING);
    printf("c           Behavior: Subject to changes in the source-code. Do not use.\n");
    printf("c           Adaption: Subject to changes in the source-code. Do not use.\n");
}

void adapt_strategy_testing_reset(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy reset (TESTING)...\n");
	#endif

}

void adapt_strategy_testing_init(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy init (TESTING)...\n");
	#endif

}

void adapt_strategy_testing_dispose(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy dispose (TESTING)...\n");
	#endif

}

void adapt_strategy_testing_execute(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy execute (TESTING)...\n");
	#endif

}
