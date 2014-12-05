/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strOtherRules.h"

void cdcl_strOtherRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_STROTHERRULE_NULL);
    printf("c           Behavior: Performs no strengthening with the learned clauses at all.\n");
}

void cdcl_strOtherRule_null_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SOR-Plug-in [%-3d: NULL]:\n", CDCL_STROTHERRULE_NULL);
	#endif
}

uint32_t cdcl_strOtherRule_null_str(){
	//Does absolutely nothing.

	return cdcl_targetBJLevel;
}


