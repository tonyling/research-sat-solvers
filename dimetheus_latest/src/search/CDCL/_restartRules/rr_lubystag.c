/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "restartRules.h"

void cdcl_restartRule_lubystag_printHelp(){
	printf("c      %-3d: LUBYSTAG:\n", CDCL_RESTARTRULE_LUBYSTAG);
    printf("c           Behavior: Performs restarts according to Luby series but prohibits them if no stack stagnation.\n");
}

void cdcl_restartRule_lubystag_prepare(){
	cdcl_restartRule_lubystag_t = param_cdclRestartLubyUnit;
	cdcl_restartRule_lubystag_c = 1;
	cdcl_restartRule_lubystag_u = 1;
	cdcl_restartRule_lubystag_v = 1;
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight = 0;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: LUBYSTAG]:\n", CDCL_RESTARTRULE_LUBY);
	printf("c     CDCL:       param_cdclRestartLubyUnit          = %d\n", param_cdclRestartLubyUnit);
	printf("c     CDCL:       param_cdclRestartStagMaxFrac       = %f\n", param_cdclRestartStagMaxFrac);
	printf("c     CDCL:       cdcl_restartRule_lubystag_t        = %d\n", cdcl_restartRule_lubystag_t);
	printf("c     CDCL:       cdcl_restartRule_lubystag_c        = %d\n", cdcl_restartRule_lubystag_c);
	printf("c     CDCL:       cdcl_restartRule_lubystag_u        = %d\n", cdcl_restartRule_lubystag_u);
	printf("c     CDCL:       cdcl_restartRule_lubystag_v        = %d\n", cdcl_restartRule_lubystag_v);
	printf("c     CDCL:       cdcl_restartRules_backjumpDistance = %d\n", cdcl_restartRules_backjumpDistance);
	printf("c     CDCL:       cdcl_restartRules_backjumpHeight   = %d\n", cdcl_restartRules_backjumpHeight);
	#endif
}

uint32_t cdcl_restartRule_lubystag_check(){
	//The Luby series calculation we implement here was presented by Knuth in 2012.
	//In case param_cdclRestartLubyUnit is 1, this computation looks as follows.
	//(u,v) = (1,1)
	//(u',v') = (u&-u = v) ? (u+1, 1) : (u,2*v).
	//Gives: (1,1) (2,1) (2,2) (3,1) (4,1) (4,2) (4,4) (5,1)...
	//A restart is performed whenever the first value is equal to the first.

	//Check if we have collected a number of conflict that hits the count of conflicts for a restart.
	if (cdcl_restartRule_luby_c < cdcl_restartRule_luby_t){
		//No, to few new conflicts discovered yet.
		++cdcl_restartRule_luby_c;
		return 0u;
	}

	//Compute the new values for u and v.
	if ((cdcl_restartRule_luby_u & -cdcl_restartRule_luby_u) == cdcl_restartRule_luby_v){
		++cdcl_restartRule_luby_u;
		cdcl_restartRule_luby_v = 1;
	} else {
		//Multiply times two.
		cdcl_restartRule_luby_v = cdcl_restartRule_luby_v<<1;
	}

	//Reset the number of conflicts for a restart and the number of conflicts discovered yet.
	cdcl_restartRule_luby_t = param_cdclRestartLubyUnit * cdcl_restartRule_luby_v;
	cdcl_restartRule_luby_c = 1;

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
	//Signal the restart.
	#ifdef COLLECTSTATS
	++stats_cdcl_numRestarts;
	#endif
	return 1u;
}

void cdcl_restartRule_lubystag_incAgility(cdclVariable* v){
	//This method does nothing to the agility.
}

void cdcl_restartRule_lubystag_signalBackjump(){
	cdcl_restartRules_backjumpDistance += (cdcl_decLevel - cdcl_targetBJLevel);
	cdcl_restartRules_backjumpHeight += cdcl_decLevel;
}
