/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "maintenanceRules.h"

void cdcl_maintenanceRule_lbd_printHelp(){
	printf("c      %-3d: LBD:\n", CDCL_MAINTENANCERULE_LBD);
    printf("c           Behavior: Removes non-reason-non-binary clauses with a LBD below the median LBD.\n");
}

void cdcl_maintenanceRule_lbd_prepare(){
	//The LBD based CDBM preparation.
	cdcl_maintenanceRules_numPerformed = 0U;
	//Set the initial number of clauses that we want to learn before we do the first CDBM.
	cdcl_maintenanceRules_targetLearned = param_cdclMaintenanceBase;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     MR-Plug-in [%-3d: LBD]:\n", CDCL_MAINTENANCERULE_LBD);
	printf("c     CDCL:       param_cdclMaintenanceBase            = %d\n", param_cdclMaintenanceBase);
	printf("c     CDCL:       param_cdclMaintenanceBoost           = %d\n", param_cdclMaintenanceBoost);
	printf("c     CDCL:       cdcl_maintenanceRules_targetLearned  = %u\n", cdcl_maintenanceRules_targetLearned);
	printf("c     CDCL:       cdcl_maintenanceRules_numPerformed   = %u\n", cdcl_maintenanceRules_numPerformed);
	#endif
}

uint32_t cdcl_maintenanceRule_lbd_check(){
	//If the number of learned clauses exceeds the number of target clauses, we want to do some maintenance.
	if (cdcl_cls_l_used > cdcl_maintenanceRules_targetLearned){
		//Yes, we have learned enough clauses and perform CDBM.
		return 1U;
	}

	return 0U;
}

void cdcl_maintenanceRule_lbd_incAct(cdclClause* c){
	//Increasing activities does not happen in the LBD scheme. The literal blocks distance is computed on the fly later.
}

void cdcl_maintenanceRule_lbd_rescale(){
	//Re-scaling of activities is never performed in the LBD scheme, as no activity increases happen.
}

void cdcl_maintenanceRule_lbd_bump(){
	//We do not bump the activity increase value.
}

void cdcl_maintenanceRule_lbd_maintain(){
	cdclClause *c, *d;
	uint32_t pos, litNum, toDelete;
	int32_t lit;
	//The LBD rule removes half of the learned clauses.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdcl_clauses_Q_ensureSize(cdcl_cls_l_used);
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_maintenance_lbd;
	//We enqueue all learned clauses in the priority queue. The smaller the LBD, the farther at the end the clause will be.
	for (pos = 0; pos < cdcl_cls_l_used-1; ++pos){
		c = cdcl_cls_l[pos];
		if (IS_CDCL_CLS_IRREMOVABLE(c)){
			//We ignore small clauses and clauses that currently act as reasons.
			continue;
		}

		//The large clauses or clauses currently not acting as reasons are added to the priority queue.
		cdcl_clauses_Q_touchClause(c);
	}

	//We can now delete the worst half of the clauses in the queue. First elements have worst LBD.
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
	++cdcl_maintenanceRules_numPerformed;
	cdcl_maintenanceRules_targetLearned = cdcl_cls_l_used + param_cdclMaintenanceBase
			+ param_cdclMaintenanceBoost*cdcl_maintenanceRules_numPerformed;
	#ifdef COLLECTSTATS
	++stats_cdcl_numMaintenances;
	++stats_cdcl_numMaintWithGlue;
	stats_cdcl_time_maint += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

