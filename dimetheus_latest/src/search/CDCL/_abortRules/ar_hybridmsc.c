/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "abortRules.h"

void cdcl_abortRule_hybridmsc_printHelp(){
	printf("c      %-3d: HYBRIDMSC:\n", CDCL_ABORTRULE_HYBRIDMSC);
    printf("c           Behavior: Aborting the search of the CDCL is controlled by the HYBRIDMSC search strategy.\n");
}

void cdcl_abortRule_hybridmsc_prepare(){
	cdcl_abortRule_hybridmsc_lastCheckedLearned = 0;
	cdcl_abortRule_hybridmsc_lastNumUnits = 0;
	cdcl_abortRule_hybridmsc_nonRestarting = 1;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     AR-Plug-in [%-3d: HYBRIDMSC]:\n", CDCL_ABORTRULE_HYBRIDMSC);
	printf("c     CDCL:       cdcl_abortRule_hybridmsc_lastCheckedLearned       = %u\n", cdcl_abortRule_hybridmsc_lastCheckedLearned);
	#endif
}

uint32_t cdcl_abortRule_hybridmsc_check(){
	//In HYBRIDMSC, we want to acknowledge all newly learned unit clauses. Therefore, we signal to return every time we learned
	//a unit assignment that is yet unknown to the search strategy. Afterwards, HYBRIDMSC will reduce the main formula and the
	//CDCL is forced to completely re-initialize, again having an empty decision level 0.
	//Otherwise, we also want to return if the CDCL fixed a certain amount of variables that is controlled by the search
	//strategy. If the CDCL fixed this amount of variables we return as well.
	if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() > cdcl_abortRule_hybridmsc_lastNumUnits
			|| CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() >= search_strategy_hybridmsc_assignmentMax
			|| (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() < search_strategy_hybridmsc_assignmentMin && cdcl_abortRule_hybridmsc_nonRestarting)
			){
		#ifdef VERBOSE_CDCL
		printf("c     CDCL:   ABORT: %d conflicts, %d units, %d assigned (%d minimum, %d maximum)\n",
				cdcl_numConflicts,
				CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO(),
				CDCL_DECSTACK_GET_NUM_ASSIGNMENTS(),
				search_strategy_hybridmsc_assignmentMin,
				search_strategy_hybridmsc_assignmentMax);
		#endif
		#ifdef COLLECTSTATS
		++stats_cdcl_numAbort;
		if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() > 0U){
			++stats_cdcl_numAbortDLZero;
		} else if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() >= search_strategy_hybridmsc_assignmentMax) {
			++stats_cdcl_numAbortAssignmentMax;
		} else if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() < search_strategy_hybridmsc_assignmentMin && cdcl_abortRule_hybridmsc_nonRestarting) {
			++stats_cdcl_numAbortAssignmentMin;
		}
		#endif
		cdcl_abortRule_hybridmsc_lastNumUnits = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO();
		return 1u;
	}
	if (!cdcl_abortRule_hybridmsc_nonRestarting && CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() >= search_strategy_hybridmsc_assignmentMin){
		cdcl_abortRule_hybridmsc_nonRestarting = 1;
	}
	return 0u;
}

void cdcl_abortRule_hybridmsc_signalMaintenance(){
	//We just reset the last checked clause to the one that is the last that was learned.
	cdcl_abortRule_hybridmsc_lastCheckedLearned = cdcl_cls_l_used;
}

void cdcl_abortRule_hybridmsc_signalRestart(){
	//We just reset the nonRestarting value to zero, because we just performed a restart and unless we get over the minimum
	//assignment border, we will not abort the search because of too few assignments.
	cdcl_abortRule_hybridmsc_nonRestarting = 0;
}
