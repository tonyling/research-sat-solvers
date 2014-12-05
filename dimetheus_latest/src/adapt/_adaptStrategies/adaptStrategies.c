/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "adaptStrategies.h"

void adapt_strategies_pointer_reset(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Resetting strategy pointers...\n");
	#endif
	adapt_strategy_reset 			= &adapt_strategy_null_reset;
	adapt_strategy_init 			= &adapt_strategy_null_init;
	adapt_strategy_execute 			= &adapt_strategy_null_execute;
	adapt_strategy_dispose 			= &adapt_strategy_null_dispose;
}

void adapt_strategies_pointer_init(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Initializing strategy pointers...\n");
	#endif
	if (param_adaptStrategy == ADAPT_STRATEGY_TESTING){
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: Adaptation strategy: %d, TESTING.\n", ADAPT_STRATEGY_TESTING);
		#endif
		adapt_strategy_reset 			= &adapt_strategy_testing_reset;
		adapt_strategy_init 			= &adapt_strategy_testing_init;
		adapt_strategy_execute 			= &adapt_strategy_testing_execute;
		adapt_strategy_dispose 			= &adapt_strategy_testing_dispose;
	} else if (param_adaptStrategy == ADAPT_STRATEGY_ITEGENERIC){
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: Adaptation strategy: %d, ITEGENERIC.\n", ADAPT_STRATEGY_ITEGENERIC);
		#endif
		adapt_strategy_reset 			= &adapt_strategy_itegeneric_reset;
		adapt_strategy_init 			= &adapt_strategy_itegeneric_init;
		adapt_strategy_execute 			= &adapt_strategy_itegeneric_execute;
		adapt_strategy_dispose 			= &adapt_strategy_itegeneric_dispose;
	} else {
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: Adaptation strategy: 0, NULL.\n");
		#endif
		adapt_strategy_reset 			= &adapt_strategy_null_reset;
		adapt_strategy_init 			= &adapt_strategy_null_init;
		adapt_strategy_execute 			= &adapt_strategy_null_execute;
		adapt_strategy_dispose 			= &adapt_strategy_null_dispose;
	}
}

void adapt_strategies_pointer_dispose(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Disposing strategy pointers...\n");
	#endif
	adapt_strategy_reset 			= &adapt_strategy_null_reset;
	adapt_strategy_init 			= &adapt_strategy_null_init;
	adapt_strategy_execute 			= &adapt_strategy_null_execute;
	adapt_strategy_dispose 			= &adapt_strategy_null_dispose;
}
