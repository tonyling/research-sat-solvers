/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "classifyStrategies.h"

void classify_strategy_testing_printHelp(){
	printf("c      %-3d: TESTING:\n", CLASSIFY_STRATEGY_TESTING);
    printf("c           Behavior: Subject to changes in the source-code. Do not use.\n");
}

void classify_strategy_testing_reset(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy reset (TESTING)...\n");
	#endif

}

void classify_strategy_testing_init(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy init (TESTING)...\n");
	#endif

}

void classify_strategy_testing_dispose(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy dispose (TESTING)...\n");
	#endif

}

void classify_strategy_testing_execute(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy execute (TESTING)...\n");
	#endif

}
