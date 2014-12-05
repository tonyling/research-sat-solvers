/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "abortRules.h"

void cdcl_abortRule_pmpcdcl_printHelp(){
	printf("c      %-3d: PMPCDCL:\n", CDCL_ABORTRULE_PMPCDCL);
    printf("c           Behavior: Aborting the search of the CDCL is controlled by the PMPCDCL search strategy.\n");
}

void cdcl_abortRule_pmpcdcl_prepare(){
	cdcl_abortRule_pmpcdcl_lastCheckedLearned = 0;
	cdcl_abortRule_pmpcdcl_lastNumUnits = 0;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     AR-Plug-in [%-3d: PMPCDCL]:\n", CDCL_ABORTRULE_PMPCDCL);
	printf("c     CDCL:       cdcl_abortRule_pmpcdcl_lastCheckedLearned       = %u\n", cdcl_abortRule_pmpcdcl_lastCheckedLearned);
	#endif
}

uint32_t cdcl_abortRule_pmpcdcl_check(){
	//In PMPCDCL, we want to acknowledge all newly learned unit clauses. Therefore, we signal to return every time we learned
	//a unit assignment that is yet unknown to the search strategy. Afterwards, PMPCDCL will reduce the main formula and the
	//CDCL is forced to completely re-initialize, again having an empty decision level 0.
	//Otherwise, we also want to return if the CDCL fixed a certain amount of variables that is controlled by the search
	//strategy. If the CDCL fixed this amount of variables we return as well.
	if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() > cdcl_abortRule_pmpcdcl_lastNumUnits	//New units.
			|| CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() >= search_strategy_pmpcdcl_assignmentMax 	//Enough assignments.
			){
		#ifdef VERBOSE_CDCL
		printf("c     CDCL:   ABORT: %d conflicts, %d units, %d assigned (%d target)\n",
				cdcl_numConflicts,
				CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO(),
				CDCL_DECSTACK_GET_NUM_ASSIGNMENTS(),
				search_strategy_pmpcdcl_assignmentMax);
		#endif
		#ifdef COLLECTSTATS
		++stats_cdcl_numAbort;
		if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() > 0U){
			++stats_cdcl_numAbortDLZero;
		} else if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() >= search_strategy_pmpcdcl_assignmentMax) {
			++stats_cdcl_numAbortAssignmentMax;
		}
		#endif
		cdcl_abortRule_pmpcdcl_lastNumUnits = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO();
		return 1u;
	}
	return 0u;
}

void cdcl_abortRule_pmpcdcl_signalMaintenance(){
	//We just reset the last checked clause to the one that is the last that was learned.
	cdcl_abortRule_pmpcdcl_lastCheckedLearned = cdcl_cls_l_used;
}

void cdcl_abortRule_pmpcdcl_signalRestart(){
	//This does not matter in this specific abort rule.
}
