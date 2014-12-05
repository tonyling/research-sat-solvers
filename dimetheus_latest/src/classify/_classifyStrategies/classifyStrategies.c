/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "classifyStrategies.h"

void classify_strategies_pointer_reset(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Resetting strategy pointers...\n");
	#endif
	classify_strategy_reset 			= &classify_strategy_null_reset;
	classify_strategy_init 				= &classify_strategy_null_init;
	classify_strategy_execute 			= &classify_strategy_null_execute;
	classify_strategy_dispose 			= &classify_strategy_null_dispose;
}

void classify_strategies_pointer_init(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Initializing strategy pointers...\n");
	#endif
	if (param_classifyStrategy == CLASSIFY_STRATEGY_TESTING){
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: Classification strategy: %d, TESTING.\n", CLASSIFY_STRATEGY_TESTING);
		#endif
		classify_strategy_reset 			= &classify_strategy_testing_reset;
		classify_strategy_init 				= &classify_strategy_testing_init;
		classify_strategy_execute 			= &classify_strategy_testing_execute;
		classify_strategy_dispose 			= &classify_strategy_testing_dispose;
	} else if (param_classifyStrategy == CLASSIFY_STRATEGY_ATTONLY){
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: Classification strategy: %d, ATTONLY.\n", CLASSIFY_STRATEGY_ATTONLY);
		#endif
		classify_strategy_reset 			= &classify_strategy_attonly_reset;
		classify_strategy_init 				= &classify_strategy_attonly_init;
		classify_strategy_execute 			= &classify_strategy_attonly_execute;
		classify_strategy_dispose 			= &classify_strategy_attonly_dispose;
	} else if (param_classifyStrategy == CLASSIFY_STRATEGY_RANDFOR){
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: Classification strategy: %d, RANDFOR.\n", CLASSIFY_STRATEGY_RANDFOR);
		#endif
		classify_strategy_reset 			= &classify_strategy_randfor_reset;
		classify_strategy_init 				= &classify_strategy_randfor_init;
		classify_strategy_execute 			= &classify_strategy_randfor_execute;
		classify_strategy_dispose 			= &classify_strategy_randfor_dispose;
	} else {
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: Classification strategy: 0, NULL.\n");
		#endif
		classify_strategy_reset 			= &classify_strategy_null_reset;
		classify_strategy_init 				= &classify_strategy_null_init;
		classify_strategy_execute 			= &classify_strategy_null_execute;
		classify_strategy_dispose 			= &classify_strategy_null_dispose;
	}
}

void classify_strategies_pointer_dispose(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Disposing strategy pointers...\n");
	#endif
	classify_strategy_reset 			= &classify_strategy_null_reset;
	classify_strategy_init 				= &classify_strategy_null_init;
	classify_strategy_execute 			= &classify_strategy_null_execute;
	classify_strategy_dispose 			= &classify_strategy_null_dispose;
}

