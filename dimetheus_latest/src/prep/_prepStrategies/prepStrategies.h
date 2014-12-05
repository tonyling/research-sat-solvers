/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PREPSTRATEGIES_H_
#define PREPSTRATEGIES_H_

#include "../prep.h"

#define PREP_STRATEGY_MINVALUE 0
#define PREP_STRATEGY_MAXVALUE 6

#define PREP_STRATEGY_NULL 0
#include "ps_null.h"

#define PREP_STRATEGY_SIMPLE 1
#include "ps_simple.h"

#define PREP_STRATEGY_STRONG 2
#include "ps_strong.h"

#define PREP_STRATEGY_FULL 3
#include "ps_full.h"

#define PREP_STRATEGY_RAND 4
#include "ps_rand.h"

#define PREP_STRATEGY_GIGRID 5
#include "ps_gigrid.h"

#define PREP_STRATEGY_SATELITE 6
#include "ps_satelite.h"

#define PREP_STRATEGY_TESTING 999
#include "ps_testing.h"

static inline void prep_strategies_printHelp(){
    prep_strategy_null_printHelp();
    prep_strategy_simple_printHelp();
    prep_strategy_strong_printHelp();
    prep_strategy_full_printHelp();
    prep_strategy_rand_printHelp();
    prep_strategy_gigrid_printHelp();
    prep_strategy_satelite_printHelp();
    prep_strategy_testing_printHelp();
}

static inline void prep_strategies_extern_printStrategyNamesAndIDs(char* prefix){
	printf("%s PREP_STRATEGY_NULL %d\n", prefix, PREP_STRATEGY_NULL);
	printf("%s PREP_STRATEGY_SIMPLE %d\n", prefix, PREP_STRATEGY_SIMPLE);
	printf("%s PREP_STRATEGY_STRONG %d\n", prefix, PREP_STRATEGY_STRONG);
	printf("%s PREP_STRATEGY_FULL %d\n", prefix, PREP_STRATEGY_FULL);
	printf("%s PREP_STRATEGY_RAND %d\n", prefix, PREP_STRATEGY_RAND);
	printf("%s PREP_STRATEGY_GIGRID %d\n", prefix, PREP_STRATEGY_GIGRID);
	printf("%s PREP_STRATEGY_SATELITE %d\n", prefix, PREP_STRATEGY_SATELITE);
}

//Methods provided by this component (called by the one who wants to use the component).
void (*prep_strategy_reset)();
void (*prep_strategy_init)();
void (*prep_strategy_execute)();
void (*prep_strategy_dispose)();

//Methods to initialize the component itself (called by the one who wants to use the component).
void prep_strategies_pointer_reset();
void prep_strategies_pointer_init();
void prep_strategies_pointer_dispose();

#endif /* PREPSTRATEGIES_H_ */
