/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_STRLEARNEDRULE_NULL);
    printf("c           Behavior: Performs no strengthening at all.\n");
}

void cdcl_strLearnedRule_null_prepare(){
	//Nothing is to be done to initialize a call to the null strengthening rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: NULL]:\n", CDCL_STRLEARNEDRULE_NULL);
	#endif
}

void cdcl_strLearnedRule_null_str(){
	//Does absolutely nothing.
}

