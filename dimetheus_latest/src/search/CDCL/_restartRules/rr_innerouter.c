/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "restartRules.h"

void cdcl_restartRule_innerouter_printHelp(){
	printf("c      %-3d: INNEROUTER:\n", CDCL_RESTARTRULE_INNEROUTER);
    printf("c           Behavior: Performs restarts according to an inner/outer geometric series.\n");
}

void cdcl_restartRule_innerouter_prepare(){
	cdcl_restartRule_innerouter_t = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouter_i = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouter_o = param_cdclRestartInnerouterInit;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: INNEROUTER]:\n", CDCL_RESTARTRULE_INNEROUTER);
	printf("c     CDCL:       param_cdclRestartInnerouterInit = %d\n", param_cdclRestartInnerouterInit);
	printf("c     CDCL:       param_cdclRestartInnerouterInc  = %f\n", param_cdclRestartInnerouterInc);
	printf("c     CDCL:       cdcl_restartRule_innerouter_t   = %d\n", cdcl_restartRule_innerouter_t);
	printf("c     CDCL:       cdcl_restartRule_innerouter_i   = %d\n", cdcl_restartRule_innerouter_i);
	printf("c     CDCL:       cdcl_restartRule_innerouter_o   = %d\n", cdcl_restartRule_innerouter_o);
	#endif
}

uint32_t cdcl_restartRule_innerouter_check(){
	//Check, if the number of conflicts reached the target.
	if (cdcl_numConflicts < cdcl_restartRule_innerouter_t){
		//No, so no restart happens.
		return 0u;
	}

	//Next, we will perform a restart. For this, we increase the inner value by the increase factor.
	cdcl_restartRule_innerouter_i *= param_cdclRestartInnerouterInc;

	//Check if the inner cycle is over and a new outer cycle is started with an increased outer length.
	if (cdcl_restartRule_innerouter_i >= cdcl_restartRule_innerouter_o){
		//Yes, we therefore reset the inner and increase the outer.
		cdcl_restartRule_innerouter_i = param_cdclRestartInnerouterInit;
		cdcl_restartRule_innerouter_o *= param_cdclRestartInnerouterInc;

	}

	//We set the new target of conflicts for the next restart.
	cdcl_restartRule_innerouter_t += cdcl_restartRule_innerouter_i;

	#ifdef COLLECTSTATS
	++stats_cdcl_numRestarts;
	#endif
	return 1u;
}

void cdcl_restartRule_innerouter_incAgility(cdclVariable* v){
	//No agility increase in the standard inner/outer restarts.
}

void cdcl_restartRule_innerouter_signalBackjump(){
	//No further use of this information in this restart rule.
}
