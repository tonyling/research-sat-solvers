/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inprocessingRules.h"

void cdcl_inprocessingRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_INPROCESSINGRULE_NULL);
    printf("c           Behavior: Performs absolutely no in-processing operations.\n");
}

void cdcl_inprocessingRule_null_prepare(){
	cdcl_inprocessingRules_lastConflict = 0;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     IR-Plug-in [%-3d: NULL]:\n", CDCL_INPROCESSINGRULE_NULL);
	printf("c     CDCL:       cdcl_inprocessingRules_lastConflict = %d\n", cdcl_inprocessingRules_lastConflict);
	#endif
}

cdclClause* cdcl_inprocessingRule_null_inprocess(){
	//This does absolutely nothing for in-processing.
	return NULL;
}

