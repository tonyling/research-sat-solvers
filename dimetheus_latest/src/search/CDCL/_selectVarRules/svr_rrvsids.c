/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectVarRules.h"

void cdcl_selectVarRule_rrvsids_printHelp(){
	printf("c      %-3d: RRVSIDS:\n", CDCL_SELECTVARRULE_RRVSIDS);
    printf("c           Behavior: VSIDS activity based (sometimes random) and rewarding implied variables from small LBDs.\n");
}

void cdcl_selectVarRule_rrvsids_prepare(){
	cdcl_selectVarRules_maxActivity = ZERO;
	cdcl_selectVarRules_currentActInc = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SVR-Plug-in [%-3d: RRVSIDS]:\n", CDCL_SELECTVARRULE_RRVSIDS);
	printf("c     CDCL:       cdcl_selectVarRules_maxActivity   = %f\n", cdcl_selectVarRules_maxActivity);
	printf("c     CDCL:       cdcl_selectVarRules_currentActInc = %f\n", cdcl_selectVarRules_currentActInc);
	printf("c     CDCL:       param_cdclSelectVarActIncBump     = %f\n", param_cdclSelectVarActIncBump);
	printf("c     CDCL:       param_cdclSelectVarRandProb       = %f\n", param_cdclSelectVarRandProb);
	#endif
	cdcl_selectVarRules_intern_varActInit();
}

void cdcl_selectVarRule_rrvsids_incAct(cdclVariable* v){
	//We increase the variable activity and retain the maximum value.
	if (SET_CDCL_VAR_PTR_ACT_INC_BY(v, cdcl_selectVarRules_currentActInc) > cdcl_selectVarRules_maxActivity){
		cdcl_selectVarRules_maxActivity = GET_CDCL_VAR_PTR_ACTIVITY(v);
	}
	if (GET_CDCL_VAR_PTR_OFFSET(v) > 0U) cdcl_variables_Q_moveUp(v);
}

void cdcl_selectVarRule_rrvsids_reward(uint32_t lbd, cdclClause* conflictClause){
	cdclLiteral **stackPtr = cdcl_up_end-1;
	cdclVariable *v;
	cdclClause *currentReason = conflictClause, *c;
	uint32_t paths = 0, litPos;
	int32_t lid;

	++cdcl_litStamp;
	do {
		for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(currentReason); ++litPos){
			lid = GET_CDCL_CLS_LITNUM(currentReason, litPos);
			v = cdcl_variables + abs(lid);
			//First, we check if we have already seen this literal. And that the literal has not been assigned in DL0.
			if (GET_CDCL_VAR_PTR_STAMP(v) != cdcl_litStamp && GET_CDCL_VAR_PTR_DECLEVEL(v) > 0){
				SET_CDCL_VAR_PTR_STAMP(v, cdcl_litStamp);
				//If the literal was falsified in the current decision level, we have found a new path.
				if (GET_CDCL_VAR_PTR_DECLEVEL(v) == cdcl_decLevel){
					++paths;
					//If the reason for this variable to be propagated was a learned clause with an LBD smaller than the
					//given value, we increase the variables activity once more.
					c = GET_CDCL_VAR_PTR_REASON(v);
					if (c != NULL && GET_CDCL_CLS_LBD(c) < lbd){
						//The variable was assigned because of a clause that has an even smaller LBD than the
						//clause we learned. We therefore increase the activity of the variable once again (reward it).
						cdcl_selectVarRule_rrvsids_incAct(v);
					}
				}
			}
		}

		do {
			v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR((*stackPtr)));
			--stackPtr;
		} while (GET_CDCL_VAR_PTR_STAMP(v) != cdcl_litStamp);

		currentReason = GET_CDCL_VAR_PTR_REASON(v);
		--paths;
	} while (paths > 0);

	//We might need to re-scale the variable activities.
	cdcl_selectVarRule_rrvsids_rescale(0);
}

void cdcl_selectVarRule_rrvsids_rescale(uint32_t force){
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

void cdcl_selectVarRule_rrvsids_bump(){
	//This method bumps the increase value of the activities.
	cdcl_selectVarRules_currentActInc *= param_cdclSelectVarActIncBump;
}

cdclVariable* cdcl_selectVarRule_rrvsids_select(){
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

