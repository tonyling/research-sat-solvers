/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef INPSTRATEGIES_H_
#define INPSTRATEGIES_H_

#include "../inp.h"

#define INP_STRATEGY_MINVALUE 0
#define INP_STRATEGY_MAXVALUE 4

#define INP_STRATEGY_NULL 0
#include "is_null.h"

#define INP_STRATEGY_REDUCEONLY 1
#include"is_reduceOnly.h"

#define INP_STRATEGY_SIMPLE 2
#include "is_simple.h"

#define INP_STRATEGY_FULL 3
#include "is_full.h"

#define INP_STRATEGY_RAND 4
#include "is_rand.h"

#define INP_STRATEGY_TESTING 999
#include "is_testing.h"

static inline void inp_strategies_printHelp(){
	inp_strategy_null_printHelp();
	inp_strategy_reduceOnly_printHelp();
	inp_strategy_simple_printHelp();
	inp_strategy_full_printHelp();
	inp_strategy_rand_printHelp();
	inp_strategy_testing_printHelp();
}

static inline void inp_strategies_extern_printStrategyNamesAndIDs(char* prefix){
	printf("%s INP_STRATEGY_NULL %d\n", prefix,INP_STRATEGY_NULL);
	printf("%s INP_STRATEGY_REDUCEONLY %d\n", prefix,INP_STRATEGY_REDUCEONLY);
	printf("%s INP_STRATEGY_SIMPLE %d\n", prefix,INP_STRATEGY_SIMPLE);
	printf("%s INP_STRATEGY_FULL %d\n", prefix,INP_STRATEGY_FULL);
	printf("%s INP_STRATEGY_RAND %d\n", prefix,INP_STRATEGY_RAND);
}

//Methods provided by this component (are being called).
void (*inp_strategy_reset)();
void (*inp_strategy_init)();
void (*inp_strategy_execute)();
void (*inp_strategy_dispose)();

//Methods to initialize this component (called by the one who wants to call the above methods).
void inp_strategies_pointer_reset();
void inp_strategies_pointer_init();
void inp_strategies_pointer_dispose();

#endif /* INPSTRATEGIES_H_ */
