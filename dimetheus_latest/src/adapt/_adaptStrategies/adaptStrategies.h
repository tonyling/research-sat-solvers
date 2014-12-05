/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ADAPTSTRATEGIES_H_
#define ADAPTSTRATEGIES_H_

#include "../adapt.h"

#define ADAPT_STRATEGY_MINVALUE 0
#define ADAPT_STRATEGY_MAXVALUE 1

#define ADAPT_STRATEGY_NULL 0
#include "as_null.h"

#define ADAPT_STRATEGY_ITEGENERIC 1
#include "as_itegeneric.h"

#define ADAPT_STRATEGY_TESTING 999
#include "as_testing.h"

static inline void adapt_strategies_printHelp(){
    adapt_strategy_null_printHelp();
    adapt_strategy_itegeneric_printHelp();
    adapt_strategy_testing_printHelp();
}

static inline void adapt_strategies_extern_printStrategyNamesAndIDs(char* prefix){
	printf("%s ADAPT_STRATEGY_NULL %d\n", prefix, ADAPT_STRATEGY_NULL);
	printf("%s ADAPT_STRATEGY_ITEGENERIC %d\n", prefix, ADAPT_STRATEGY_ITEGENERIC);
}

//Methods provided by this component (called by the one who wants to use the component).
void (*adapt_strategy_reset)();
void (*adapt_strategy_init)();
void (*adapt_strategy_execute)();
void (*adapt_strategy_dispose)();

//Methods to initialize the component itself (called by the one who wants to use the component).
void adapt_strategies_pointer_reset();
void adapt_strategies_pointer_init();
void adapt_strategies_pointer_dispose();

#endif /* ADAPTSTRATEGIES_H_ */
