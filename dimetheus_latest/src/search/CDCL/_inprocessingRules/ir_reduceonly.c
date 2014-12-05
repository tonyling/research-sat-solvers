/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "inprocessingRules.h"

void cdcl_inprocessingRule_reduceonly_printHelp(){
	printf("c      %-3d: REDUCEONLY:\n", CDCL_INPROCESSINGRULE_REDUCEONLY);
    printf("c           Behavior: Reduces the formula, but does no other in-processing.\n");
}

void cdcl_inprocessingRule_reduceonly_prepare(){
	cdcl_inprocessingRules_lastConflict = 0;
	cdcl_inprocessingRules_reduceFrom = cdcl_decStackPtrs[0];
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     IR-Plug-in [%-3d: REDUCEONLY]:\n", CDCL_INPROCESSINGRULE_REDUCEONLY);
	printf("c     CDCL:       cdcl_inprocessingRules_lastConflict = %d\n", cdcl_inprocessingRules_lastConflict);
	#endif
}

cdclClause* cdcl_inprocessingRule_reduceonly_inprocess(){
	//This in-processing just cleans up the formula.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_cdcl_numInprocessing;
	#endif
	#ifdef VERBOSE_CDCL
	int32_t initNumV = cdcl_vars_e_used;
	int32_t initNumC = cdcl_cls_l_used + cdcl_cls_o_used;
	printf("c     CDCL:   In-Processing [REDUCEONLY]: Starting to in-process (%d conflicts distance to last in-processing)...\n",
			cdcl_numConflicts-cdcl_inprocessingRules_lastConflict);
	printf("c     CDCL:   In-Processing [REDUCEONLY]:   Remaining variables: %d\n", initNumV);
	printf("c     CDCL:   In-Processing [REDUCEONLY]:   Remaining clauses:   %d\n", initNumC);
	printf("c     CDCL:   In-Processing [REDUCEONLY]:   ");
	#endif
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;
	cdcl_clauses_S_flush();
	cdcl_clauses_S_touchClause = &cdcl_clauses_S_touchClause_null;
	cdcl_literals_Q_flush();
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;
	cdcl_literals_S_flush();
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_null;
	cdcl_variables_S_flush();
	cdcl_variables_S_touchVariable = &cdcl_variables_S_touchVariable_null;
	cdcl_inprocessingRules_intern_reduceDB();
	cdcl_inprocessingRules_lastConflict = cdcl_numConflicts;
	#ifdef VERBOSE_CDCL
	printf("\n");
	printf("c     CDCL:   In-Processing [REDUCEONLY]:   Removed variables: %d (%d remaining)\n",
			initNumV - cdcl_vars_e_used, cdcl_vars_e_used);
	printf("c     CDCL:   In-Processing [REDUCEONLY]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(cdcl_cls_l_used + cdcl_cls_o_used), cdcl_cls_l_used + cdcl_cls_o_used);
	#endif

	#ifdef COLLECTSTATS
	stats_cdcl_time_inp += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
	return NULL;
}

