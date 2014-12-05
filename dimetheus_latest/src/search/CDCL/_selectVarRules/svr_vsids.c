/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectVarRules.h"

void cdcl_selectVarRule_vsids_printHelp(){
	printf("c      %-3d: VSIDS:\n", CDCL_SELECTVARRULE_VSIDS);
    printf("c           Behavior: Picks the variable according to the highest VSIDS activity value.\n");
}

void cdcl_selectVarRule_vsids_prepare(){
	cdcl_selectVarRules_maxActivity = ZERO;
	cdcl_selectVarRules_currentActInc = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SVR-Plug-in [%-3d: VSIDS]:\n", CDCL_SELECTVARRULE_VSIDS);
	printf("c     CDCL:       cdcl_selectVarRules_maxActivity   = %f\n", cdcl_selectVarRules_maxActivity);
	printf("c     CDCL:       cdcl_selectVarRules_currentActInc = %f\n", cdcl_selectVarRules_currentActInc);
	printf("c     CDCL:       param_cdclSelectVarActIncBump     = %f\n", param_cdclSelectVarActIncBump);
	#endif
	cdcl_selectVarRules_intern_varActInit();
}

void cdcl_selectVarRule_vsids_incAct(cdclVariable* v){
	//We increase the variable activity and retain the maximum value.
	if (SET_CDCL_VAR_PTR_ACT_INC_BY(v, cdcl_selectVarRules_currentActInc) > cdcl_selectVarRules_maxActivity){
		cdcl_selectVarRules_maxActivity = GET_CDCL_VAR_PTR_ACTIVITY(v);
	}
	if (GET_CDCL_VAR_PTR_OFFSET(v) > 0U) cdcl_variables_Q_moveUp(v);
}

void cdcl_selectVarRule_vsids_reward(uint32_t lbd, cdclClause* conflictClause){
	//This method does nothing in the VSIDS scheme.
}

void cdcl_selectVarRule_vsids_rescale(uint32_t force){
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

void cdcl_selectVarRule_vsids_bump(){
	//This method bumps the increase value of the activities.
	cdcl_selectVarRules_currentActInc *= param_cdclSelectVarActIncBump;
}

cdclVariable* cdcl_selectVarRule_vsids_select(){
	//Pick the variable that has the highest activity value at the moment.
	#ifdef COLLECTSTATS
	++stats_cdcl_numDecisions;
	if (cdcl_decLevel > STATS_CDCL_DECLEVELVISITED_MAX){
		++stats_cdcl_decLevelVisited[STATS_CDCL_DECLEVELVISITED_MAX+1];
	} else {
		++stats_cdcl_decLevelVisited[cdcl_decLevel];
	}
	#endif

	return cdcl_variables_Q_readBest();
}
