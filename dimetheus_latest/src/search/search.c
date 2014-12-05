/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "search.h"

void search_intern_transferSLSAss(){
	variable *v;
	uint32_t i;
	//This method checks all enabled variables and determines if they are assigned. For all of them that are not, we will
	//use the SLS assignment to assign the variable.

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Copying SLS assignments... ");
	#endif

	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		if (IS_VAR_UNASSIGNED(v) && GET_VAR_ISEQUIVTO(v) == 0){
			if (GET_SLS_VAR_ASSIGNMENT(v->id)){
				SET_VAR_ASS_1(v);
			} else {
				SET_VAR_ASS_0(v);
			}
		}
	}

	#ifdef VERBOSE_SEARCH
	printf("done.\n");
	#endif
}

void search_intern_transferCDCLAss(){
	variable *v;
	cdclVariable *cdclV;
	uint32_t i;
	//This method checks all enabled variables and determines if they are assigned. For all of them that are not, we will
	//use the CDCL assignment to assign the variable. If the CDCL variable is unassigned as well, we will not assign the
	//variable.
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Copying CDCL assignments... ");
	#endif

	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		if (IS_VAR_UNASSIGNED(v) && GET_VAR_ISEQUIVTO(v) == 0){
			cdclV = cdcl_variables + v->id;
			if (IS_CDCL_VAR_PTR_TRUE(cdclV)){
				SET_VAR_ASS_1(v);
			} else if (IS_CDCL_VAR_PTR_FALSE(cdclV)){
				SET_VAR_ASS_0(v);
			}
		}
	}

	#ifdef VERBOSE_SEARCH
	printf("done.\n");
	#endif
}

void search_extern_searching(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_search_component_totalCalls;
	#endif
	if (param_searchStrategy != SEARCH_STRATEGY_NULL){
		#ifdef VERBOSE_SEARCH
		printf("c   SEARCH: SEARCHING...\n");fflush(stdout);
		#endif
		search_strategy_reset();
		search_strategy_init();
		if (search_returnCode == SEARCH_UNKNOWN){
			search_strategy_execute();
		}
		search_strategy_dispose();
	}
	#ifdef COLLECTSTATS
	stats_search_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void search_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_search_component_totalCalls;
	#endif
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: RESET...\n");
	#endif
	search_returnCode = SEARCH_UNKNOWN;
	search_strategies_pointer_reset();

	#ifdef COLLECTSTATS
	stats_search_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void search_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_search_component_totalCalls;
	#endif
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: INITIALIZE...\n");
	#endif
	search_returnCode = SEARCH_UNKNOWN;
	search_strategies_pointer_init();
	vQ_flush();

	#ifdef COLLECTSTATS
	stats_search_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void search_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_search_component_totalCalls;
	#endif
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: DISPOSE...\n");
	#endif
	search_returnCode = SEARCH_UNKNOWN;
	search_strategies_pointer_dispose();

	#ifdef COLLECTSTATS
	stats_search_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
