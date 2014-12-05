/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "maintenanceRules.h"

void cdcl_maintenanceRule_lbdinactluby_printHelp(){
	printf("c      %-3d: LBDINACTLUBY:\n", CDCL_MAINTENANCERULE_LBDINACTLUBY);
    printf("c           Behavior: Like LBDINACT, but reduces the target learned clauses using a luby series.\n");
}

void cdcl_maintenanceRule_lbdinactluby_prepare(){
	//The LBDINACTLUBY based CDBM preparation.
	cdcl_maintenanceRule_lbdinactluby_u = 1;
	cdcl_maintenanceRule_lbdinactluby_v = 1;

	//Set the initial number of clauses that we want to learn before we do the first CDBM.
	cdcl_maintenanceRules_targetLearned = param_cdclMaintenanceInitial*cdcl_cls_o_used;

	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     MR-Plug-in [%-3d: LBDINACTLUBY]:\n", CDCL_MAINTENANCERULE_LBDINACTLUBY);
	printf("c     CDCL:       param_cdclMaintenanceInitial         = %f\n", param_cdclMaintenanceInitial);
	printf("c     CDCL:       param_cdclMaintenanceIncrease        = %f\n", param_cdclMaintenanceIncrease);
	printf("c     CDCL:       param_cdclMaintenanceActIncBump      = %f\n", param_cdclMaintenanceActIncBump);
	printf("c     CDCL:       param_cdclMaintenanceMinDev          = %f\n", param_cdclMaintenanceMinDev);
	printf("c     CDCL:       param_cdclMaintenanceMaxAvg          = %f\n", param_cdclMaintenanceMaxAvg);
	printf("c     CDCL:       cdcl_maintenanceRules_targetLearned  = %u\n", cdcl_maintenanceRules_targetLearned);
	printf("c     CDCL:       cdcl_maintenanceRule_lbdinactluby_u  = %u\n", cdcl_maintenanceRule_lbdinactluby_u);
	printf("c     CDCL:       cdcl_maintenanceRule_lbdinactluby_v  = %u\n", cdcl_maintenanceRule_lbdinactluby_v);
	#endif
}

uint32_t cdcl_maintenanceRule_lbdinactluby_check(){
	//If the number of learned clauses exceeds the number of target clauses, we want to do some maintenance.
	if (cdcl_cls_l_used > cdcl_maintenanceRules_targetLearned){
		//Yes, we have learned enough clauses and perform CDBM.
		return 1U;
	}

	return 0U;
}

void cdcl_maintenanceRule_lbdinactluby_incAct(cdclClause* c){
	//Increase the activity of a clause.
	if (SET_CDCL_CLS_ACT_INC_BY(c, cdcl_maintenanceRules_currentActInc) > cdcl_maintenanceRules_maxActivity){
		cdcl_maintenanceRules_maxActivity = GET_CDCL_CLS_ACTIVITY(c);
	}
}

void cdcl_maintenanceRule_lbdinactluby_rescale(){
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

void cdcl_maintenanceRule_lbdinactluby_bump(){
	//We bump the activity increase value for clause activities.
	cdcl_maintenanceRules_currentActInc *= param_cdclMaintenanceActIncBump;
}

void cdcl_maintenanceRule_lbdinactluby_maintain(){
	//The LBDINACTLUBY rule removes half of the learned clauses.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdclClause *c, *d;
	uint32_t pos, litNum, toDelete;
	int32_t lit;

	cdcl_maintenanceRules_targetLearned = cdcl_maintenanceRule_lbdinactluby_v*cdcl_cls_o_used*param_cdclMaintenanceInitial;

	//We first handle the Luby series values.
	if ((cdcl_maintenanceRule_lbdinactluby_u & -cdcl_maintenanceRule_lbdinactluby_u) == cdcl_maintenanceRule_lbdinactluby_v){
		++cdcl_maintenanceRule_lbdinactluby_u;
		cdcl_maintenanceRule_lbdinactluby_v = 1;
		param_cdclMaintenanceInitial*= param_cdclMaintenanceIncrease;
	} else {
		//Multiply times two.
		cdcl_maintenanceRule_lbdinactluby_v = cdcl_maintenanceRule_lbdinactluby_v<<1;
	}

	cdcl_clauses_Q_ensureSize(cdcl_cls_l_used);
	cdcl_clauses_Q_flush();

	//The first thing to do is to compute the standard deviation of the LBD values and the average. Given this value, we
	//decide dynamically, if we want to remove half of the clauses by LBD or by ACTIVITY.
	float_ty avg, sum = ZERO, numCls = ZERO, value, maxValue = ZERO;
	for (pos = 0; pos < cdcl_cls_l_used-1; ++pos){
		c = cdcl_cls_l[pos];
		if (IS_CDCL_CLS_IRREMOVABLE(c)){
			//We ignore small clauses and clauses that currently act as reasons.
			continue;
		}
		//Otherwise, we compute the square root of the glue value and sum it up.
		value = sqrt(GET_CDCL_CLS_LBD(c));
		sum += value;
		//We furthermore retain the number of clauses we respect and retain the maximum squared glue value.
		if (value > maxValue) maxValue = value;
		++numCls;
	}
	if (numCls == ZERO){
		//If no clause can be removed, we do not do any maintenance.
		#ifdef COLLECTSTATS
		++stats_cdcl_numMaintenances;
		stats_cdcl_time_maint += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
		#endif
		return;
	}
	//Compute the average value.
	avg = sum/numCls;

	#ifdef COLLECTSTATS
	if (floor(avg) < 100.0) ++stats_cdcl_glueAvg[(uint32_t)floor(avg)];
	#endif

	//We check if the average exceeds the allowed. If so, we do not need to compute the costly deviation value.
	if (avg > param_cdclMaintenanceMaxAvg){
		//That is the case. We set the sum value to something larger than the maximum average such that the inactivity base
		//removal rules are used.
		avg = param_cdclMaintenanceMaxAvg + ONE;
	} else {
		//The average is not too big. We must compute the standard deviation.
		//We now compute the square distance of all elements to the mean, sum them up, divide them by the number of clauses
		//that are removable and compute from that the square root. The square root is then divided by the maximum value in
		//order to normalize it into [0,1].
		sum = ZERO;
		for (pos = 0; pos < cdcl_cls_l_used-1; ++pos){
			c = cdcl_cls_l[pos];
			if (IS_CDCL_CLS_IRREMOVABLE(c)){
				//We ignore small clauses and clauses that currently act as reasons.
				continue;
			}
			//Otherwise, we compute the square distance between the square root LBD value and the mean.
			value = ( sqrt(GET_CDCL_CLS_LBD(c)) - avg);
			sum += (value*value);
		}
		//We now divide the sum by the number of clauses we respect during removals, and pick from it the square root. We then
		//scale the value into [0,1] by dividing it with the maximum value of all the respected clauses.
		sum = (sqrt(sum / numCls)) / maxValue;
		#ifdef COLLECTSTATS
		if (floor(100.0*sum) < 100.0) ++stats_cdcl_glueDev[((uint32_t)floor(100.0*sum))];
		#endif
	}

	//With sum being the normalized standard deviation (or the exceeded average), we now decide what removal scheme to use.
	if (sum < param_cdclMaintenanceMinDev || sum > param_cdclMaintenanceMaxAvg){
		//Either the standard deviation is too small or the average is too large, which indicates that the glue values are
		//useless to decide what clauses to remove. We do it activity based.
		cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_maintenance_inactivity;
		#ifdef COLLECTSTATS
		++stats_cdcl_numMaintWithActivity;
		#endif
	} else {
		//We pick the LBD based scheme.
		cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_maintenance_lbd;
		#ifdef COLLECTSTATS
		++stats_cdcl_numMaintWithGlue;
		#endif
	}

	//We enqueue all learned clauses in the priority queue.
	for (pos = 0; pos < cdcl_cls_l_used-1; ++pos){
		c = cdcl_cls_l[pos];
		if (IS_CDCL_CLS_IRREMOVABLE(c)){
			//We ignore small clauses and clauses that currently act as reasons.
			continue;
		}

		//The large clauses or clauses currently not acting as reasons are added to the priority queue.
		cdcl_clauses_Q_touchClause(c);
	}

	//We remove as many clauses as we can in order to reduce the database to the half of the target.
	toDelete = cdcl_clauses_Q_leaf>>1;
	if (cdcl_clauses_Q_leaf - toDelete > cdcl_maintenanceRules_targetLearned){
		toDelete = cdcl_clauses_Q_leaf - cdcl_maintenanceRules_targetLearned;
	}
	while (toDelete-- > 0U && (c = cdcl_clauses_Q_dequeueBest())!=NULL){
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
	cdcl_maintenanceRules_targetLearned = cdcl_cls_l_used + cdcl_maintenanceRules_targetLearned;
	//Finally, we can flush all the remaining elements in the queue as they are retained.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;

	//After we have performed the CDBM, we increase the target number of clauses we need before we check the next time.
	#ifdef COLLECTSTATS
	++stats_cdcl_numMaintenances;
	stats_cdcl_time_maint += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}


