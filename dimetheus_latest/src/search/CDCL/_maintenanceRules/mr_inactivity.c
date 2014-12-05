/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "maintenanceRules.h"

void cdcl_maintenanceRule_inactivity_printHelp(){
	printf("c      %-3d: INACTIVITY:\n", CDCL_MAINTENANCERULE_INACTIVITY);
    printf("c           Behavior: Removes non-reason-non-binary clauses with an activity below the median activity.\n");
}

void cdcl_maintenanceRule_inactivity_prepare(){
	//The inactivity based CDBM preparation ensures that we are allowed to learn at least one new clause before we do
	//the maintenance. Furthermore, it ensures that increasing this number results in at least one additional clause we
	//are allowed to learn.
	//Set the initial number of clauses that we want to learn before we do the first CDBM.
	cdcl_maintenanceRules_targetLearned = param_cdclMaintenanceInitial * cdcl_cls_o_used;
	//We ensure that we must learn at least one clause before we do CDBM.
	if (cdcl_maintenanceRules_targetLearned == 0U){
		++cdcl_maintenanceRules_targetLearned;
	}
	//We ensure that multiplying this value with the increase parameter actually increases the value.
	if ((uint32_t) (cdcl_maintenanceRules_targetLearned*param_cdclMaintenanceIncrease)==cdcl_maintenanceRules_targetLearned){
		//It does not, we must enforce a larger increase value to ensure that more clauses can be learned after maintenance.
		param_cdclMaintenanceIncrease =
				((float_ty)(cdcl_maintenanceRules_targetLearned) + 1.001) / ((float_ty)cdcl_maintenanceRules_targetLearned);
	}
	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = ONE;
	//The number of variables that can be assigned in this formula is given by the number of variables that are active in
	//the beginning of the search. We add one here such that we will later just check with "smaller than".
	cdcl_maintenanceRules_assignableVars = cdcl_vars_e_used + 1U;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     MR-Plug-in [%-3d: INACTIVITY]:\n", CDCL_MAINTENANCERULE_INACTIVITY);
	printf("c     CDCL:       param_cdclMaintenanceInitial         = %f\n", param_cdclMaintenanceInitial);
	printf("c     CDCL:       param_cdclMaintenanceIncrease        = %f\n", param_cdclMaintenanceIncrease);
	printf("c     CDCL:       param_cdclMaintenanceActIncBump      = %f\n", param_cdclMaintenanceActIncBump);
	printf("c     CDCL:       cdcl_maintenanceRules_assignableVars = %u\n", cdcl_maintenanceRules_assignableVars);
	printf("c     CDCL:       cdcl_maintenanceRules_targetLearned  = %u\n", cdcl_maintenanceRules_targetLearned);
	printf("c     CDCL:       cdcl_maintenanceRules_maxActivity    = %f\n", cdcl_maintenanceRules_maxActivity);
	printf("c     CDCL:       cdcl_maintenanceRules_currentActInc  = %f\n", cdcl_maintenanceRules_currentActInc);
	#endif
}

uint32_t cdcl_maintenanceRule_inactivity_check(){
	//If the number of learned clauses exceeds the number of target clauses, we want to do some maintenance. We do, however,
	//exclude the number of reasons which is equal to the number of assigned variables. We increase the target number by the
	//number of assigned variables to reflect this.
	if (cdcl_cls_l_used > cdcl_maintenanceRules_targetLearned + (cdcl_maintenanceRules_assignableVars - cdcl_vars_e_used)){
		//Yes, we have learned enough clauses and enough of them are not reasons. We perform CDBM.
		return 1U;
	}

	return 0U;
}

void cdcl_maintenanceRule_inactivity_incAct(cdclClause* c){
	//Increase the activity of the clause.
	if (SET_CDCL_CLS_ACT_INC_BY(c, cdcl_maintenanceRules_currentActInc) > cdcl_maintenanceRules_maxActivity){
		cdcl_maintenanceRules_maxActivity = GET_CDCL_CLS_ACTIVITY(c);
	}
}

void cdcl_maintenanceRule_inactivity_rescale(){
	//Check if the maximum activity value is larger than what we allow. If so, re-scale all activities.
	cdclClause *c;
	float_ty newMaxAct = ONE;
	uint32_t i;

	//We re-scale if the maximum is too large.
	if (cdcl_maintenanceRules_maxActivity < CLS_MAX_ACTIVITY_VALUE) {
		return;
	}

	//We check on all the learned clauses.
	for (i = 0; i < cdcl_cls_l_used-1; ++i){
		c = cdcl_cls_l[i];
		SET_CDCL_CLS_ACT_NORM_BY(c, cdcl_maintenanceRules_maxActivity);
		if (GET_CDCL_CLS_ACTIVITY(c) > newMaxAct){
			newMaxAct = GET_CDCL_CLS_ACTIVITY(c);
		}
	}

	#ifdef COLLECTSTATS
	++stats_cdcl_numClsActRescales;
	#endif

	//Finally, we reset the current activity increase value and set the new maximum value found.
	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = newMaxAct;
}

void cdcl_maintenanceRule_inactivity_bump(){
	//We bump the activity increase value for clause activities.
	cdcl_maintenanceRules_currentActInc *= param_cdclMaintenanceActIncBump;
}

void cdcl_maintenanceRule_inactivity_maintain(){
	cdclClause *c, *d;
	uint32_t toDelete, pos, litNum;
	int32_t lit;
	//The INACTIVITY rule removes half of the learned clauses.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	//We sort all the clauses, by putting it in the clauses priority queue. We can ignore binary clauses and those clauses
	//that are acting as reasons. When touching clauses in this mode, their score is MAX_SCORE - activity. The clauses with
	//the worst activity are found at the beginning of the queue later on.
	cdcl_clauses_Q_ensureSize(cdcl_cls_l_used);
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_maintenance_inactivity;
	for (pos = 0; pos < cdcl_cls_l_used; ++pos){
		c = cdcl_cls_l[pos];
		if (IS_CDCL_CLS_IRREMOVABLE(c)) continue;
		cdcl_clauses_Q_touchClause(c);
	}

	//We can now delete the worst half of the clauses in the queue.
	toDelete = cdcl_clauses_Q_leaf >> 1;
	while (toDelete-- > 0U){
		c = cdcl_clauses_Q_dequeueBest();
		//First, we delete clause c from all the occurrence lists and watcher lists.
		lit = GET_CDCL_CLS_W1(c);
		pos = GET_CDCL_CLS_WHEREW1(c);
		REM_CDCL_CLS_FROM_LIT_W1LIST(lit, pos);
		lit = GET_CDCL_CLS_W2(c);
		pos = GET_CDCL_CLS_WHEREW2(c);
		REM_CDCL_CLS_FROM_LIT_W2LIST(lit, pos);
		for (litNum = 0; litNum < GET_CDCL_CLS_SIZE(c); ++litNum){
			lit = GET_CDCL_CLS_LITNUM(c, litNum);
			pos = GET_CDCL_CLS_OCCPOSNUM(c, litNum);
			REM_CDCL_LIT_OCC(lit, pos);
		}

		//We retain the position of c in the array of learned clauses.
		pos = GET_CDCL_CLS_OFFSET(c);

		//Then, we pick the replacement clause that we transfer to this position that c is currently in.
		d = cdcl_cls_l[--cdcl_cls_l_used];

		//Then, we shrink clause c such that it appears freshly initialized and place it to where d was (to the end).
		SHRINK_LEARNED_CDCL_CLS(c, cdcl_cls_l_used);

		//Finally, we put d to the current position.
		cdcl_cls_l[pos] = d;
		SET_CDCL_CLS_OFFSET(d, pos);
		#ifdef COLLECTSTATS
		++stats_cdcl_numLearnedRemoved;
		#endif
	}
	//Finally, we can flush all the remaining elements in the queue as they are retained.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;

	//After we have performed the CDBM, we increase the target number of clauses we need before we check the next time.
	cdcl_maintenanceRules_targetLearned *= param_cdclMaintenanceIncrease;
	#ifdef COLLECTSTATS
	++stats_cdcl_numMaintenances;
	++stats_cdcl_numMaintWithActivity;
	stats_cdcl_time_maint += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}
