/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SEARCHSTRATEGIES_H_
#define SEARCHSTRATEGIES_H_

#include "../search.h"

#define SEARCH_STRATEGY_MINVALUE 0
#define SEARCH_STRATEGY_MAXVALUE 6

#define SEARCH_STRATEGY_NULL 0
#include "ss_null.h"

#define SEARCH_STRATEGY_SLS 1
#include "ss_sls.h"

#define SEARCH_STRATEGY_CDCL 2
#include "ss_cdcl.h"

#define SEARCH_STRATEGY_PMPCDCL 3
#include "ss_pmpcdcl.h"

#define SEARCH_STRATEGY_PMPSLS 4
#include "ss_pmpsls.h"

#define SEARCH_STRATEGY_HYBRIDMSC 5
#include "ss_hybridmsc.h"

#define SEARCH_STRATEGY_EMP 6
#include "ss_emp.h"

#define SEARCH_STRATEGY_TESTING 999
#include "ss_testing.h"

static inline void search_strategies_printHelp(){
    search_strategy_null_printHelp();
    search_strategy_sls_printHelp();
    search_strategy_cdcl_printHelp();
    search_strategy_pmpcdcl_printHelp();
    search_strategy_pmpsls_printHelp();
    search_strategy_hybridmsc_printHelp();
    search_strategy_emp_printHelp();
    search_strategy_testing_printHelp();
}

static inline void search_strategies_extern_printStrategyNamesAndIDs(char* prefix){
	printf("%s SEARCH_STRATEGY_NULL %d\n", prefix,SEARCH_STRATEGY_NULL);
	printf("%s SEARCH_STRATEGY_SLS %d\n", prefix,SEARCH_STRATEGY_SLS);
	printf("%s SEARCH_STRATEGY_CDCL %d\n", prefix,SEARCH_STRATEGY_CDCL);
	printf("%s SEARCH_STRATEGY_PMPCDCL %d\n", prefix,SEARCH_STRATEGY_PMPCDCL);
	printf("%s SEARCH_STRATEGY_PMPSLS %d\n", prefix,SEARCH_STRATEGY_PMPSLS);
	printf("%s SEARCH_STRATEGY_HYBRIDMSC %d\n", prefix,SEARCH_STRATEGY_HYBRIDMSC);
	printf("%s SEARCH_STRATEGY_EMP %d\n", prefix,SEARCH_STRATEGY_EMP);
}

//Methods that can be called to execute this component.
void (*search_strategy_reset)();
void (*search_strategy_init)();
void (*search_strategy_execute)();
void (*search_strategy_dispose)();

//Methods to initialize this component (called by the one who wants to call the above methods).
void search_strategies_pointer_reset();
void search_strategies_pointer_init();
void search_strategies_pointer_dispose();

#endif /* SEARCHSTRATEGIES_H_ */
