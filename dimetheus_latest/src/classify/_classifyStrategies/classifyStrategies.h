/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CLASSIFYSTRATEGIES_H_
#define CLASSIFYSTRATEGIES_H_

#include "../classify.h"

#define CLASSIFY_STRATEGY_MINVALUE 0
#define CLASSIFY_STRATEGY_MAXVALUE 2

#define CLASSIFY_STRATEGY_NULL 0
#include "cs_null.h"

#define CLASSIFY_STRATEGY_ATTONLY 1
#include "cs_attonly.h"

#define CLASSIFY_STRATEGY_RANDFOR 2
#include "cs_randfor.h"

#define CLASSIFY_STRATEGY_TESTING 999
#include "cs_testing.h"

static inline void classify_strategies_printHelp(){
    classify_strategy_null_printHelp();
    classify_strategy_attonly_printHelp();
    classify_strategy_randfor_printHelp();
    classify_strategy_testing_printHelp();
}

static inline void classify_strategies_extern_printStrategyNamesAndIDs(char* prefix){
	printf("%s CLASSIFY_STRATEGY_NULL %d\n", prefix, CLASSIFY_STRATEGY_NULL);
	printf("%s CLASSIFY_STRATEGY_ATTONLY %d\n", prefix, CLASSIFY_STRATEGY_ATTONLY);
	printf("%s CLASSIFY_STRATEGY_RANDFOR %d\n", prefix, CLASSIFY_STRATEGY_RANDFOR);
}

//Methods provided by this component (called by the one who wants to use the component).
void (*classify_strategy_reset)();
void (*classify_strategy_init)();
void (*classify_strategy_execute)();
void (*classify_strategy_dispose)();

//Methods to initialize the component itself (called by the one who wants to use the component).
void classify_strategies_pointer_reset();
void classify_strategies_pointer_init();
void classify_strategies_pointer_dispose();

#endif /* CLASSIFYSTRATEGIES_H_ */
