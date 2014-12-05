/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategies_pointer_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Resetting strategy pointers...\n");
	#endif
	prep_strategy_reset 	= &prep_strategy_null_reset;
	prep_strategy_init 		= &prep_strategy_null_init;
	prep_strategy_execute 	= &prep_strategy_null_execute;
	prep_strategy_dispose 	= &prep_strategy_null_dispose;
}

void prep_strategies_pointer_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Initializing strategy pointers...\n");
	#endif
	if (param_prepStrategy == PREP_STRATEGY_SATELITE){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, SATELITE.\n", PREP_STRATEGY_SATELITE);
		#endif
		prep_strategy_reset 	= &prep_strategy_satelite_reset;
		prep_strategy_init 		= &prep_strategy_satelite_init;
		prep_strategy_execute 	= &prep_strategy_satelite_execute;
		prep_strategy_dispose 	= &prep_strategy_satelite_dispose;
	} else if (param_prepStrategy == PREP_STRATEGY_GIGRID){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, GIGRID.\n", PREP_STRATEGY_GIGRID);
		#endif
		prep_strategy_reset 	= &prep_strategy_gigrid_reset;
		prep_strategy_init 		= &prep_strategy_gigrid_init;
		prep_strategy_execute 	= &prep_strategy_gigrid_execute;
		prep_strategy_dispose 	= &prep_strategy_gigrid_dispose;
	} else if (param_prepStrategy == PREP_STRATEGY_RAND){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, RAND.\n", PREP_STRATEGY_RAND);
		#endif
		prep_strategy_reset 	= &prep_strategy_rand_reset;
		prep_strategy_init 		= &prep_strategy_rand_init;
		prep_strategy_execute 	= &prep_strategy_rand_execute;
		prep_strategy_dispose 	= &prep_strategy_rand_dispose;

	} else if (param_prepStrategy == PREP_STRATEGY_FULL){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, FULL.\n", PREP_STRATEGY_FULL);
		#endif
		prep_strategy_reset 	= &prep_strategy_full_reset;
		prep_strategy_init 		= &prep_strategy_full_init;
		prep_strategy_execute 	= &prep_strategy_full_execute;
		prep_strategy_dispose 	= &prep_strategy_full_dispose;

	} else if (param_prepStrategy == PREP_STRATEGY_STRONG){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, STRONG.\n", PREP_STRATEGY_STRONG);
		#endif
		prep_strategy_reset 	= &prep_strategy_strong_reset;
		prep_strategy_init 		= &prep_strategy_strong_init;
		prep_strategy_execute 	= &prep_strategy_strong_execute;
		prep_strategy_dispose 	= &prep_strategy_strong_dispose;
	} else if (param_prepStrategy == PREP_STRATEGY_SIMPLE){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, SIMPLE.\n", PREP_STRATEGY_SIMPLE);
		#endif
		prep_strategy_reset 	= &prep_strategy_simple_reset;
		prep_strategy_init 		= &prep_strategy_simple_init;
		prep_strategy_execute 	= &prep_strategy_simple_execute;
		prep_strategy_dispose 	= &prep_strategy_simple_dispose;

	} else if (param_prepStrategy == PREP_STRATEGY_TESTING){
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: %d, TESTING.\n", PREP_STRATEGY_TESTING);
		#endif
		prep_strategy_reset 	= &prep_strategy_testing_reset;
		prep_strategy_init 		= &prep_strategy_testing_init;
		prep_strategy_execute 	= &prep_strategy_testing_execute;
		prep_strategy_dispose 	= &prep_strategy_testing_dispose;
	} else {
		#ifdef VERBOSE_PREP
		printf("c   PREP: Preprocessing strategy: 0, NULL.\n");
		#endif
		prep_strategy_reset 	= &prep_strategy_null_reset;
		prep_strategy_init 		= &prep_strategy_null_init;
		prep_strategy_execute 	= &prep_strategy_null_execute;
		prep_strategy_dispose 	= &prep_strategy_null_dispose;
	}
}

void prep_strategies_pointer_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Disposing strategy pointers...\n");
	#endif
	prep_strategy_reset 	= &prep_strategy_null_reset;
	prep_strategy_init 		= &prep_strategy_null_init;
	prep_strategy_execute 	= &prep_strategy_null_execute;
	prep_strategy_dispose 	= &prep_strategy_null_dispose;
}
