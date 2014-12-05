/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "abortRules.h"

void cdcl_abortRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_ABORTRULE_NULL);
    printf("c           Behavior: The search of the CDCL is not aborted.\n");
}

void cdcl_abortRule_null_prepare(){
	//Nothing is to be done to initialize a call to the NULL abort rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     AR-Plug-in [%-3d: NULL]:\n", CDCL_ABORTRULE_NULL);
	#endif
}

uint32_t cdcl_abortRule_null_check(){
	//Never abort in the NULL abort strategy.
	return 0u;
}

void cdcl_abortRule_null_signalMaintenance(){

}

void cdcl_abortRule_null_signalRestart(){

}
