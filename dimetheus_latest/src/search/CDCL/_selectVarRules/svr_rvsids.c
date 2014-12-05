/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectVarRules.h"

void cdcl_selectVarRule_rvsids_printHelp(){
	printf("c      %-3d: RVSIDS:\n", CDCL_SELECTVARRULE_RVSIDS);
    printf("c           Behavior: Picks the variable according to the highest VSIDS activity, but sometimes a random one.\n");
}

void cdcl_selectVarRule_rvsids_prepare(){
	cdcl_selectVarRules_maxActivity = ZERO;
	cdcl_selectVarRules_currentActInc = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SVR-Plug-in [%-3d: RVSIDS]:\n", CDCL_SELECTVARRULE_RVSIDS);
	printf("c     CDCL:       cdcl_selectVarRules_maxActivity   = %f\n", cdcl_selectVarRules_maxActivity);
	printf("c     CDCL:       cdcl_selectVarRules_currentActInc = %f\n", cdcl_selectVarRules_currentActInc);
	printf("c     CDCL:       param_cdclSelectVarActIncBump     = %f\n", param_cdclSelectVarActIncBump);
	printf("c     CDCL:       param_cdclSelectVarRandProb       = %f\n", param_cdclSelectVarRandProb);
	#endif
	cdcl_selectVarRules_intern_varActInit();
}

void cdcl_selectVarRule_rvsids_incAct(cdclVariable* v){
	//We increase the variable activity and retain the maximum value.
	if (SET_CDCL_VAR_PTR_ACT_INC_BY(v, cdcl_selectVarRules_currentActInc) > cdcl_selectVarRules_maxActivity){
		cdcl_selectVarRules_maxActivity = GET_CDCL_VAR_PTR_ACTIVITY(v);
	}
	if (GET_CDCL_VAR_PTR_OFFSET(v) > 0U) cdcl_variables_Q_moveUp(v);
}

void cdcl_selectVarRule_rvsids_reward(uint32_t lbd, cdclClause* conflictClause){
	//Does not reward variables.
}

void cdcl_selectVarRule_rvsids_rescale(uint32_t force){
	//Check if the maximum activity value is larger than what we allow. If so, re-scale all activities.
	float_ty newMaxAct = ONE;
	uint32_t i;

	if (!force && cdcl_selectVarRules_maxActivity < VAR_MAX_ACTIVITY_VALUE) return;

	for (i = 1; i < f.n_initial+1; ++i){
		//We respect only variables that the CDCL can assign.
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) continue;
		SET_CDCL_VAR_ACT_NORM_BY(i, cdcl_selectVarRules_maxActivity);
		if (GET_CDCL_VAR_ACTIVITY(i) > newMaxAct){
			newMaxAct = GET_CDCL_VAR_ACTIVITY(i);
		}
	}

	#ifdef COLLECTSTATS
	++stats_cdcl_numVarActRescales;
	#endif

	//Finally, we reset the current activity increase value and set the new maximum value.
	cdcl_selectVarRules_currentActInc = ONE;
	cdcl_selectVarRules_maxActivity = newMaxAct;
}

void cdcl_selectVarRule_rvsids_bump(){
	//This method bumps the increase value of the activities.
	cdcl_selectVarRules_currentActInc *= param_cdclSelectVarActIncBump;
}

cdclVariable* cdcl_selectVarRule_rvsids_select(){
	//Pick the variable that has the highest activity value at the moment.
	#ifdef COLLECTSTATS
	++stats_cdcl_numDecisions;
	if (cdcl_decLevel > STATS_CDCL_DECLEVELVISITED_MAX){
		++stats_cdcl_decLevelVisited[STATS_CDCL_DECLEVELVISITED_MAX+1];
	} else {
		++stats_cdcl_decLevelVisited[cdcl_decLevel];
	}
	#endif

	if (rand_realOne() < param_cdclSelectVarRandProb){
		#ifdef COLLECTSTATS
		++stats_cdcl_numDecisionsRandom;
		#endif
		if (cdcl_vars_e_used == 0) {
			return NULL;
		} else {
			return cdcl_vars_e[rand_mod(cdcl_vars_e_used)];
		}
	} else {
		return cdcl_variables_Q_readBest();
	}
}

