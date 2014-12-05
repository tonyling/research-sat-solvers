/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inpStrategies.h"

void inp_strategies_pointer_reset(){
	#ifdef VERBOSE_INP
	printf("c     INP: Resetting strategy pointers...\n");
	#endif
	inp_strategy_reset 		= &inp_strategy_null_reset;
	inp_strategy_init 		= &inp_strategy_null_init;
	inp_strategy_execute 	= &inp_strategy_null_execute;
	inp_strategy_dispose 	= &inp_strategy_null_dispose;
}

void inp_strategies_pointer_init(){
	#ifdef VERBOSE_INP
	printf("c     INP: Picking strategy pointers...\n");
	#endif
	if (param_inpStrategy == INP_STRATEGY_RAND){
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: %d, RAND.\n", INP_STRATEGY_RAND);
		#endif
		inp_strategy_reset 		= &inp_strategy_rand_reset;
		inp_strategy_init 		= &inp_strategy_rand_init;
		inp_strategy_execute 	= &inp_strategy_rand_execute;
		inp_strategy_dispose 	= &inp_strategy_rand_dispose;
	} else if (param_inpStrategy == INP_STRATEGY_SIMPLE){
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: %d, SIMPLE.\n", INP_STRATEGY_SIMPLE);
		#endif
		inp_strategy_reset 		= &inp_strategy_simple_reset;
		inp_strategy_init 		= &inp_strategy_simple_init;
		inp_strategy_execute 	= &inp_strategy_simple_execute;
		inp_strategy_dispose 	= &inp_strategy_simple_dispose;
	} else if (param_inpStrategy == INP_STRATEGY_TESTING) {
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: %d, TESTING.\n", INP_STRATEGY_TESTING);
		#endif
		inp_strategy_reset 		= &inp_strategy_testing_reset;
		inp_strategy_init 		= &inp_strategy_testing_init;
		inp_strategy_execute 	= &inp_strategy_testing_execute;
		inp_strategy_dispose 	= &inp_strategy_testing_dispose;
	} else if (param_inpStrategy == INP_STRATEGY_FULL) {
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: %d, FULL.\n", INP_STRATEGY_FULL);
		#endif
		inp_strategy_reset 		= &inp_strategy_full_reset;
		inp_strategy_init 		= &inp_strategy_full_init;
		inp_strategy_execute 	= &inp_strategy_full_execute;
		inp_strategy_dispose 	= &inp_strategy_full_dispose;
	} else if (param_inpStrategy == INP_STRATEGY_REDUCEONLY){
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: %d, REDUCEONLY.\n", INP_STRATEGY_REDUCEONLY);
		#endif
		inp_strategy_reset 		= &inp_strategy_reduceOnly_reset;
		inp_strategy_init 		= &inp_strategy_reduceOnly_init;
		inp_strategy_execute 	= &inp_strategy_reduceOnly_execute;
		inp_strategy_dispose 	= &inp_strategy_reduceOnly_dispose;
	} else {
		#ifdef VERBOSE_INP
		printf("c     INP: In-processing strategy: 0, NULL.\n");
		#endif
		inp_strategy_reset 		= &inp_strategy_null_reset;
		inp_strategy_init 		= &inp_strategy_null_init;
		inp_strategy_execute 	= &inp_strategy_null_execute;
		inp_strategy_dispose 	= &inp_strategy_null_dispose;
	}
}

void inp_strategies_pointer_dispose(){
	#ifdef VERBOSE_INP
	printf("c     INP: Disposing strategy pointers...\n");
	#endif
	inp_strategy_reset 		= &inp_strategy_null_reset;
	inp_strategy_init 		= &inp_strategy_null_init;
	inp_strategy_execute 	= &inp_strategy_null_execute;
	inp_strategy_dispose 	= &inp_strategy_null_dispose;
}
