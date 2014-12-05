/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "restartRules.h"

void cdcl_restartRule_null_printHelp(){
	printf("c      %-3d: NULL:\n", CDCL_RESTARTRULE_NULL);
    printf("c           Behavior: Performs no restarts at all.\n");
}

void cdcl_restartRule_null_prepare(){
	//Nothing is to be done to initialize a call to the NULL restart rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: NULL]:\n", CDCL_RESTARTRULE_NULL);
	#endif
}

uint32_t cdcl_restartRule_null_check(){
	//Never restart in the NULL restart strategy.
	return 0u;
}

void cdcl_restartRule_null_incAgility(cdclVariable* v){
	//This method does nothing to the agility.
}

void cdcl_restartRule_null_signalBackjump(){
	//No further use of this information in this restart rule.
}
