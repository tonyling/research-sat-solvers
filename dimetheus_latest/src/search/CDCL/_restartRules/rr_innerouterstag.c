/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "restartRules.h"

void cdcl_restartRule_innerouterstag_printHelp(){
	printf("c      %-3d: INNEROUTERSTAG:\n", CDCL_RESTARTRULE_INNEROUTERSTAG);
    printf("c           Behavior: Performs restarts according to an inner/outer geometric series if the stagnation is high.\n");
}

void cdcl_restartRule_innerouterstag_prepare(){
	cdcl_restartRule_innerouterstag_t  = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouterstag_i  = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouterstag_o  = param_cdclRestartInnerouterInit;
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight   = 0;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: INNEROUTERSTAG]:\n", CDCL_RESTARTRULE_INNEROUTERSTAG);
	printf("c     CDCL:       param_cdclRestartInnerouterInit     = %d\n", param_cdclRestartInnerouterInit);
	printf("c     CDCL:       param_cdclRestartInnerouterInc      = %f\n", param_cdclRestartInnerouterInc);
	printf("c     CDCL:       param_cdclRestartStagMaxFrac        = %f\n", param_cdclRestartStagMaxFrac);
	printf("c     CDCL:       cdcl_restartRule_innerouterstag_t   = %d\n", cdcl_restartRule_innerouterstag_t);
	printf("c     CDCL:       cdcl_restartRule_innerouterstag_i   = %d\n", cdcl_restartRule_innerouterstag_i);
	printf("c     CDCL:       cdcl_restartRule_innerouterstag_o   = %d\n", cdcl_restartRule_innerouterstag_o);
	printf("c     CDCL:       cdcl_restartRules_backjumpDistance  = %d\n", cdcl_restartRules_backjumpDistance);
	printf("c     CDCL:       cdcl_restartRules_backjumpHeight    = %d\n", cdcl_restartRules_backjumpHeight);
	#endif
}

uint32_t cdcl_restartRule_innerouterstag_check(){
	//Check, if the number of conflicts reached the target.
	if (cdcl_numConflicts < cdcl_restartRule_innerouterstag_t){
		//No, so no restart happens.
		return 0u;
	}

	//Next, we will perform a restart. For this, we increase the inner value by the increase factor.
	cdcl_restartRule_innerouterstag_i *= param_cdclRestartInnerouterInc;

	//Check if the inner cycle is over and a new outer cycle is started with an increased outer length.
	if (cdcl_restartRule_innerouterstag_i >= cdcl_restartRule_innerouterstag_o){
		//Yes, we therefore reset the inner and increase the outer.
		cdcl_restartRule_innerouterstag_i = param_cdclRestartInnerouterInit;
		cdcl_restartRule_innerouterstag_o *= param_cdclRestartInnerouterInc;

	}

	//We set the new target of conflicts for the next restart.
	cdcl_restartRule_innerouterstag_t += cdcl_restartRule_innerouterstag_i;

	//Check if the stack does stagnate. If not, do not restart.
	float_ty stagFrac = ((float_ty) cdcl_restartRules_backjumpDistance) / ((float_ty) cdcl_restartRules_backjumpHeight);
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight = 0;
	if (stagFrac > param_cdclRestartStagMaxFrac){
		//The decision stack seems to undergo a lot of changes. We do not restart.
		#ifdef COLLECTSTATS
		++stats_cdcl_numRestartProhibits;
		#endif
		return 0u;
	}

	#ifdef COLLECTSTATS
	++stats_cdcl_numRestarts;
	#endif
	return 1u;
}

void cdcl_restartRule_innerouterstag_incAgility(cdclVariable* v){
	//No agility increase in the inner/outer restarts.
}

void cdcl_restartRule_innerouterstag_signalBackjump(){
	//Store the back-jump distance and current height.
	cdcl_restartRules_backjumpDistance += (cdcl_decLevel - cdcl_targetBJLevel);
	cdcl_restartRules_backjumpHeight += cdcl_decLevel;
}


