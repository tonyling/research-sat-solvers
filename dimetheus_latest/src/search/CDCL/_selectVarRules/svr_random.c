/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectVarRules.h"

void cdcl_selectVarRule_random_printHelp(){
	printf("c      %-3d: RANDOM:\n", CDCL_SELECTVARRULE_RANDOM);
    printf("c           Behavior: Picks one of the not yet assigned variables at random.\n");
}

void cdcl_selectVarRule_random_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SVR-Plug-in [%-3d: RANDOM]:\n", CDCL_SELECTVARRULE_RANDOM);
	#endif
}

void cdcl_selectVarRule_random_incAct(cdclVariable* v){
	//The activity of variables is not touched when randomly selecting variables.
}

void cdcl_selectVarRule_random_reward(uint32_t lbd, cdclClause* conflictClause){
	//This method does nothing in the random rule.
}

void cdcl_selectVarRule_random_rescale(uint32_t force){
	//The variable activities must never be rescaled, as they are never touched.
}

void cdcl_selectVarRule_random_bump(){
	//The variable activity increase must never be bumped, as the activities are never touched.
}

cdclVariable* cdcl_selectVarRule_random_select(){
	//Picks the next variable to assign randomly of all the variables still enabled.
	#ifdef COLLECTSTATS
	++stats_cdcl_numDecisions;
	if (cdcl_decLevel > STATS_CDCL_DECLEVELVISITED_MAX){
		++stats_cdcl_decLevelVisited[STATS_CDCL_DECLEVELVISITED_MAX+1];
	} else {
		++stats_cdcl_decLevelVisited[cdcl_decLevel];
	}
	#endif
	if (cdcl_vars_e_used == 0) {
		return NULL;
	}
	return cdcl_vars_e[rand_mod(cdcl_vars_e_used)];
}
