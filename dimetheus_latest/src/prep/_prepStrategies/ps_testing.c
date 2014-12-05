/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "prepStrategies.h"

void prep_strategy_testing_printHelp(){
	printf("c      %-3d: TESTING:\n", PREP_STRATEGY_TESTING);
    printf("c           Behavior: Subject to changes in the source-code. Do not use.\n");
}

void prep_strategy_testing_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (TESTING)...\n");
	#endif

}

void prep_strategy_testing_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (TESTING)...\n");
	#endif

}

void prep_strategy_testing_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (TESTING)...\n");
	#endif

}

void prep_strategy_testing_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (TESTING)...\n");
	#endif


	//We make sure that all queues and stacks are empty after preprocessing.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();
}
