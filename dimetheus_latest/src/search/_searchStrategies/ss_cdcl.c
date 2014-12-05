/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "searchStrategies.h"

void search_strategy_cdcl_printHelp(){
	printf("c      %-3d: CDCL\n", SEARCH_STRATEGY_CDCL);
    printf("c           Behavior: Performs only conflict driven clause learning [CDCL].\n");
}

void search_strategy_cdcl_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (CDCL)...\n");
	#endif
	cdcl_resetModule();
}

void search_strategy_cdcl_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (CDCL)...\n");
	#endif
	cdcl_initModule(); if (cdcl_returnCode != CDCL_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
}

void search_strategy_cdcl_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (CDCL)...\n");
	#endif
	cdcl_disposeModule();
}

void search_strategy_cdcl_execute(){
	//This search strategy simply calls for the CDCL module and have it perform conflict driven clause learning until
	//it either finds a solution.
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (CDCL)...\n");
	#endif

	cdcl_extern_learn(1);
	if (cdcl_returnCode == CDCL_UNKNOWN){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: CDCL was unable to find an answer. CDCL failed.\n");
		#endif
		//The CDCL did not find an answer for us.
		search_returnCode = SEARCH_UNKNOWN;
	} else if (cdcl_returnCode == CDCL_SAT){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: CDCL found a solution. CDCL succeeded. Copying variable assignments.\n");
		#endif
		//The CDCL did indeed find a solution.
		search_intern_transferCDCLAss();
		search_returnCode = SEARCH_SAT;
	} else if (cdcl_returnCode == CDCL_UNSAT){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: CDCL found a global conflict. CDCL succeeded.\n");
		#endif
		//The CDCL did indeed find a global conflict.
		search_returnCode = SEARCH_UNSAT;
	} else {
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: CDCL return code unexpected after leaning. We assume an error. CDCL failed.\n");
		#endif
		//In case we get this far, the CDCL returned an unexpected return code. The strategy failed.
		search_returnCode = SEARCH_ERROR;
	}
}
