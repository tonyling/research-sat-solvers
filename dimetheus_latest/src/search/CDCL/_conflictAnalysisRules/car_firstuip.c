/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "conflictAnalysisRules.h"

void cdcl_conflictAnalysisRule_firstuip_printHelp(){
	printf("c      %-3d: FIRSTUIP:\n", CDCL_CONFLICTANALYSISRULE_FIRSTUIP);
    printf("c           Behavior: Creates a single learned clause by analyzing the conflict graph with FirstUIP.\n");
}

void cdcl_conflictAnalysisRule_firstuip_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     CAR-Plug-in [%-3d: FIRSTUIP]:\n", CDCL_CONFLICTANALYSISRULE_FIRSTUIP);
	#endif

}


uint32_t cdcl_conflictAnalysisRule_firstuip_analyze(){
	//Learns a new clause using FirstUIP (first unique implication point) scheme.
	cdclLiteral *lit, **stackPtr = cdcl_up_end-1;
	cdclVariable *v;
	cdclClause *currentReason, *newClause;
	uint32_t paths, litPos;
	int32_t lid;

	#ifdef COLLECTSTATS
	++stats_cdcl_numConflictsFound;
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	//First, we ensure that we can learn a new clause.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1u, paths, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used];
	//We ensure that this clause has enough space to hold all the literals. The trivial upper border is the number of
	//assignments we currently made. We store this information into the path variable temporarily.
	paths = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() +1u;
	REALLOC_CDCL_CLS_TO_SIZE(newClause, paths);
	RESET_CDCL_CLS(newClause);

	//We begin the analysis of the conflict graph with the conflict clause that became empty during the assignments.
	currentReason = cdcl_conflictClause;
	//The number of paths from the conflict side towards the reasons is initialized to zero. It will be increased for each
	//new reason we encounter by the number of literals in this reason we did not already see in the analysis.
	paths = 0;
	//We increase the literal stamp to be able to stamp literals as seen.
	++cdcl_litStamp;
	do {
		//The reason clause we currently hold gets an activity increase.
		if (IS_CDCL_CLS_LEARNED(currentReason)) cdcl_maintenanceRule_incAct(currentReason);
		//Check on the literals in the current reason if they have been seen before or if they must be added to the paths
		//or to the newly learned clause in the end.
		for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(currentReason); ++litPos){
			lid = GET_CDCL_CLS_LITNUM(currentReason, litPos);
			v = cdcl_variables + abs(lid);
			//First, we check if we have already seen this literal. And that the literal has not been assigned in DL0.
			if (GET_CDCL_VAR_PTR_STAMP(v) != cdcl_litStamp && GET_CDCL_VAR_PTR_DECLEVEL(v) > 0){
				//We have not yet seen this literal and the variable was not assigned in DL0. We bump the activity of the
				//corresponding variable.
				cdcl_selectVarRule_incAct(v);
				SET_CDCL_VAR_PTR_STAMP(v, cdcl_litStamp);
				//If the literal was falsified in the current decision level, we have found a new path. Otherwise, this
				//literals must be added as a precondition into the newly learned clause.
				if (GET_CDCL_VAR_PTR_DECLEVEL(v) == cdcl_decLevel){
					++paths;
				} else {
					SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), lid);
					SET_CDCL_CLS_SIZE_INC(newClause);
				}
			}
		}
		//After working through this reason, we move backwards through the stack and skip unseen variables. Once we find one
		//we have already seen, we must analyze its reason to possibly detect new paths.
		do {
			v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR((*stackPtr)));
			--stackPtr;
		} while (GET_CDCL_VAR_PTR_STAMP(v) != cdcl_litStamp);
		//We have found another literal and grab its reason.
		currentReason = GET_CDCL_VAR_PTR_REASON(v);
		//Since we have already seen this literal we can reduce the paths by one.
		--paths;
	} while (paths > 0);
	//After that, lit must be the first UIP now. We add it to the clause with inverted sign. Then we terminate the clause.
	lit = *(stackPtr+1);
	lid = GET_CDCL_LIT_ID_FROM_PTR(lit);
	SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), -lid);
	SET_CDCL_CLS_SIZE_INC(newClause);
	SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), 0);

	//We do not update the watcher information or the representative literals here because the clause might get strengthened
	//further. We do, however, ensure that the clause has an increased activity.
	cdcl_selectVarRule_bump();
	cdcl_maintenanceRule_bump();
	cdcl_maintenanceRule_incAct(newClause);

	//After having learned the clause, we check if it is time to re-scale the activities of variables and learned clauses.
	cdcl_selectVarRule_rescale(0);
	cdcl_maintenanceRule_rescale();

	++cdcl_cls_l_used;
	#ifdef COLLECTSTATS
	++stats_cdcl_numClausesLearned;
	stats_cdcl_time_confAnalysis += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

	return 1u;
}
