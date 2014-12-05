/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "conflictAnalysisRules.h"

void cdcl_conflictAnalysisRule_alldec_printHelp(){
	printf("c      %-3d: ALLDEC:\n", CDCL_CONFLICTANALYSISRULE_ALLDEC);
    printf("c           Behavior: Creates a single learned clause by inverting all the decisions.\n");
}

void cdcl_conflictAnalysisRule_alldec_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     CAR-Plug-in [%-3d: ALLDEC]:\n", CDCL_CONFLICTANALYSISRULE_ALLDEC);
	#endif
}

uint32_t cdcl_conflictAnalysisRule_alldec_analyze(){
	//Learns a new clause by inverting all the decisions currently on the decision stack.
	//This conflict analysis learns exactly one clause.
	uint32_t i, litNum;
	int32_t lid;
	cdclClause *c;
	cdclLiteral *l, **lPtrs;
	cdclVariable *v;

	#ifdef COLLECTSTATS
	++stats_cdcl_numConflictsFound;
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1u, i, c);
	//We learn into clause c.
	c = cdcl_cls_l[cdcl_cls_l_used];

	//We check how many decisions are currently on the stack. This is given in the cdcl_decLevel variable. The amount of
	//literals our new clause must hold is given by this number (plus one extra field to store the terminating 0).
	c->lits = realloc(c->lits, sizeof(int32_t)*(cdcl_decLevel+1u));
	c->occPos = realloc(c->occPos, sizeof(int32_t)*(cdcl_decLevel+1u));
	RESET_CDCL_CLS(c);

	//We now walk backwards through the stack and add as many literals as we find.
	litNum = 0;

	//The level in which the conflict occurred is special in this analysis. We bump the activity of clauses and literals in
	//the conflicting decision level. All the variables in here are somehow important for the conflict and all the reasons
	//providing their assignments are as well.
	l = *(lPtrs = cdcl_decStackPtrs[cdcl_decLevel]);
	lid = -(GET_CDCL_LIT_ID_FROM_PTR(l));
	c->lits[litNum] = lid;
	++litNum;
	while (lPtrs < cdcl_up_end){
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(*lPtrs));
		cdcl_selectVarRule_incAct(v);
		if (GET_CDCL_VAR_PTR_REASON(v) != NULL && IS_CDCL_CLS_LEARNED(GET_CDCL_VAR_PTR_REASON(v)))
			cdcl_maintenanceRule_incAct(GET_CDCL_VAR_PTR_REASON(v));
		++lPtrs;
	}


	//The other levels only contribute the other literals for the learned clause.
	for(i = cdcl_decLevel-1; i > 0u; --i){
		l = *cdcl_decStackPtrs[i];
		lid = -(GET_CDCL_LIT_ID_FROM_PTR(l));
		c->lits[litNum] = lid;
		++litNum;

	}
	//We do not update the watcher information yet, because the clauses might get strengthened further. This MUST be done
	//later. Finally, the new clause should have an increased activity.
	cdcl_maintenanceRule_incAct(c);

	//After having learned the clause, we check if it is time to re-scale the activities of variables and learned clauses.
	cdcl_selectVarRule_rescale(0);
	cdcl_selectVarRule_bump();
	cdcl_maintenanceRule_rescale();
	cdcl_maintenanceRule_bump();

	++cdcl_cls_l_used;
	#ifdef COLLECTSTATS
	++stats_cdcl_numClausesLearned;
	stats_cdcl_time_confAnalysis += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif

	return 1u;
}

