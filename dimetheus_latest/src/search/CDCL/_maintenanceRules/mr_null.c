/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "maintenanceRules.h"

void cdcl_maintenanceRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_MAINTENANCERULE_NULL);
    printf("c           Behavior: Performs no clause database maintenance at all.\n");
}

void cdcl_maintenanceRule_null_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     MR-Plug-in [%-3d: NULL]:\n", CDCL_MAINTENANCERULE_NULL);
	#endif
}

uint32_t cdcl_maintenanceRule_null_check(){
	//We never do clause database maintenance in the NULL rule, so we always return 0.
	return 0U;
}

void cdcl_maintenanceRule_null_incAct(cdclClause* c){
	//The activity of clauses is not touched in the NULL rule.
}

void cdcl_maintenanceRule_null_rescale(){
	//The clause activities must never be rescaled, as they are never touched.
}

void cdcl_maintenanceRule_null_bump(){
	//The clause activity increase must never be bumped, as the activities are never touched.
}

void cdcl_maintenanceRule_null_maintain(){
	//The NULL rule does not do any clause database maintenance.
}
