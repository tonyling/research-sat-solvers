/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "restartRules.h"

void cdcl_restartRule_luby_printHelp(){
	printf("c      %-3d: LUBY:\n", CDCL_RESTARTRULE_LUBY);
    printf("c           Behavior: Performs restarts using the Luby series (reluctant doubling sequence).\n");
}

void cdcl_restartRule_luby_prepare(){
	cdcl_restartRule_luby_t = param_cdclRestartLubyUnit;
	cdcl_restartRule_luby_c = 1;
	cdcl_restartRule_luby_u = 1;
	cdcl_restartRule_luby_v = 1;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: LUBY]:\n", CDCL_RESTARTRULE_LUBY);
	printf("c     CDCL:       param_cdclRestartLubyUnit = %d\n", param_cdclRestartLubyUnit);
	printf("c     CDCL:       cdcl_restartRule_luby_t   = %d\n", cdcl_restartRule_luby_t);
	printf("c     CDCL:       cdcl_restartRule_luby_c   = %d\n", cdcl_restartRule_luby_c);
	printf("c     CDCL:       cdcl_restartRule_luby_u   = %d\n", cdcl_restartRule_luby_u);
	printf("c     CDCL:       cdcl_restartRule_luby_v   = %d\n", cdcl_restartRule_luby_v);
	#endif
}

uint32_t cdcl_restartRule_luby_check(){
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

	#ifdef COLLECTSTATS
	++stats_cdcl_numRestarts;
	#endif

	//Signal the restart.
	return 1u;
}

void cdcl_restartRule_luby_incAgility(cdclVariable *v){
	//This method does nothing to the agility.
}

void cdcl_restartRule_luby_signalBackjump(){
	//No further use of this information in this restart rule.
}
