/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "conflictAnalysisRules.h"

void cdcl_conflictAnalysisRule_alluip_printHelp(){
	printf("c      %-3d: ALLUIP:\n", CDCL_CONFLICTANALYSISRULE_ALLUIP);
    printf("c           Behavior: Creates a learned clause for each UIP in the conflict graph.\n");
}

void cdcl_conflictAnalysisRule_alluip_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     CAR-Plug-in [%-3d: ALLUIP]:\n", CDCL_CONFLICTANALYSISRULE_ALLUIP);
	#endif
}


uint32_t cdcl_conflictAnalysisRule_alluip_analyze(){
	//Learns a new clause for each UIP in the conflict graph.
	cdclLiteral *lit, **stackPtr = cdcl_up_end-1;
	cdclVariable *v;
	cdclClause *currentReason, *newClause, *previousClause;
	uint32_t paths, litPos, numAssigned, numLearned = 0;
	int32_t lid;

	#ifdef COLLECTSTATS
	++stats_cdcl_numConflictsFound;
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	//First, we ensure that we can learn a new worst case number of clauses. This number is given by the assignments made in
	//the last decision level. We store this number in the litPos variable temporarily.
	litPos = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LAST_LEVEL();
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(litPos, paths, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used];
	//We ensure that this clause has enough space to hold all the literals. The trivial upper border is the number of
	//assignments we currently made.
	numAssigned = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() + 1u;
	REALLOC_CDCL_CLS_TO_SIZE(newClause, numAssigned);
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
		//Check if we have found a UIP.
		if (paths == 0 && currentReason != NULL){
			//We have found a new UIP. We need to add the UIP literal to the current learned clause, and then copy all the
			//literals in this clause to the next clause except the UIP.
			//Add the UIP to the current clause (with opposite sign) and terminate it.
			lit = *(stackPtr+1);
			lid = GET_CDCL_LIT_ID_FROM_PTR(lit);
			SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), -lid);
			SET_CDCL_CLS_SIZE_INC(newClause);
			SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), 0);
			//Switch to the next clause we are about to add, and allocate it such that it can hold all the literals. The size
			//of the new clause will have the size of the previous clause minus one literal that we do not add.
			previousClause = newClause;
			newClause = cdcl_cls_l[++cdcl_cls_l_used];
			REALLOC_CDCL_CLS_TO_SIZE(newClause, numAssigned);
			RESET_CDCL_CLS(newClause);
			SET_CDCL_CLS_SIZE(newClause, (GET_CDCL_CLS_SIZE(previousClause) - 1u));
			//Copy all the literals from the previous clause but the UIP.
			for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(previousClause)-1u; ++litPos){
				SET_CDCL_CLS_LITNUM(newClause, litPos, GET_CDCL_CLS_LITNUM(previousClause, litPos));
			}
			//The newClause is now a copy of the previousClause ignoring the current UIP.
			++numLearned;
		}
	} while (currentReason != NULL);
	//After that, lit must be the first UIP now. We add it to the clause with inverted sign. Then we terminate the clause.
	lit = *(stackPtr+1);
	lid = GET_CDCL_LIT_ID_FROM_PTR(lit);
	SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), -lid);
	SET_CDCL_CLS_SIZE_INC(newClause);
	SET_CDCL_CLS_LITNUM(newClause, GET_CDCL_CLS_SIZE(newClause), 0);
	++numLearned;

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
	stats_cdcl_numClausesLearned += numLearned;
	stats_cdcl_time_confAnalysis += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

	return numLearned;
}

