/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "restartRules.h"

void cdcl_restartRule_lubyagility_printHelp(){
	printf("c      %-3d: LUBYAGILITY:\n", CDCL_RESTARTRULE_LUBYAGILITY);
    printf("c           Behavior: Performs restarts using the Luby series but prohibits restarts if the agility is high.\n");
}

void cdcl_restartRule_lubyagility_prepare(){
	cdcl_restartRule_lubyagility_t 	= param_cdclRestartLubyUnit;
	cdcl_restartRule_lubyagility_c 	= 1;
	cdcl_restartRule_lubyagility_u 	= 1;
	cdcl_restartRule_lubyagility_v 	= 1;
	cdcl_restartRules_maxAgility 	= ONE;
	cdcl_restartRules_currentAgiInc = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: LUBYAGILITY]:\n", CDCL_RESTARTRULE_LUBYAGILITY);
	printf("c     CDCL:       param_cdclRestartLubyUnit       = %d\n", param_cdclRestartLubyUnit);
	printf("c     CDCL:       param_cdclRestartVarAgiIncBump  = %f\n", param_cdclRestartVarAgiIncBump);
	printf("c     CDCL:       param_cdclRestartMaxAgiFrac     = %f\n", param_cdclRestartMaxAgiFrac);
	printf("c     CDCL:       cdcl_restartRule_lubyagility_t  = %d\n", cdcl_restartRule_lubyagility_t);
	printf("c     CDCL:       cdcl_restartRule_lubyagility_c  = %d\n", cdcl_restartRule_lubyagility_c);
	printf("c     CDCL:       cdcl_restartRule_lubyagility_u  = %d\n", cdcl_restartRule_lubyagility_u);
	printf("c     CDCL:       cdcl_restartRule_lubyagility_v  = %d\n", cdcl_restartRule_lubyagility_v);
	printf("c     CDCL:       cdcl_restartRules_currentAgiInc = %f\n", cdcl_restartRules_currentAgiInc);
	printf("c     CDCL:       cdcl_restartRules_maxAgility    = %f\n", cdcl_restartRules_maxAgility);
	#endif
}

uint32_t cdcl_restartRule_lubyagility_check(){
	//The Luby series calculation we implement here was presented by Knuth in 2012.
	//In case param_cdclRestartLubyUnit is 1, this computation looks as follows.
	//(u,v) = (1,1)
	//(u',v') = (u&-u = v) ? (u+1, 1) : (u,2*v).
	//Gives: (1,1) (2,1) (2,2) (3,1) (4,1) (4,2) (4,4) (5,1)...
	//A restart is performed whenever the first value is equal to the first.

	//Check if we have collected a number of conflict that hits the count of conflicts for a restart.
	if (cdcl_restartRule_lubyagility_c < cdcl_restartRule_lubyagility_t){
		//No, to few new conflicts discovered yet.
		++cdcl_restartRule_lubyagility_c;
		return 0u;
	}

	//Yes, a restart is allowed. Compute the new values for u and v.
	if ((cdcl_restartRule_lubyagility_u & -cdcl_restartRule_lubyagility_u) == cdcl_restartRule_lubyagility_v){
		++cdcl_restartRule_lubyagility_u;
		cdcl_restartRule_lubyagility_v = 1;
	} else {
		//Multiply times two.
		cdcl_restartRule_lubyagility_v = cdcl_restartRule_lubyagility_v<<1;
	}

	//Reset the number of conflicts for a restart and the number of conflicts discovered yet.
	cdcl_restartRule_lubyagility_t = param_cdclRestartLubyUnit * cdcl_restartRule_lubyagility_v;
	cdcl_restartRule_lubyagility_c = 1;

	//We now check if a high variable agility prohibits a restart.
	uint32_t numVars = 1, numAgileVars = 1, i;
	for (i = 1; i < f.n_initial+1; ++i){
		//We respect only variables that the CDCL can assign.
		if (!IS_VAR_UNASSIGNED((f.vars + i)) || IS_VAR_DISABLED((f.vars + i))){
			continue;
		}
		if (GET_CDCL_VAR_AGILITY(i) > cdcl_restartRules_currentAgiInc){
			//Count this very agile variable.
			++numAgileVars;
		}
		//Count the number of total variables available to the CDCL.
		++numVars;
	}
	//The number of variables available to the CDCL is decreased by the number of variables assigned in DL0.
	numVars -= CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO();

	//Check how many variable are currently considered very agile.
	if (param_cdclRestartMaxAgiFrac < ((float_ty)numAgileVars) / ((float_ty)numVars)){
		//The amount of agile variables exceeds the amount of what is allowed for a restart.
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

void cdcl_restartRule_lubyagility_incAgility(cdclVariable *v){
	cdcl_restartRules_currentAgiInc *= param_cdclRestartVarAgiIncBump;

	//We first check if the variable receives a new phase.
	if ((IS_CDCL_VAR_PTR_TRUE(v) && GET_CDCL_VAR_PTR_PHASE(v)) || (IS_CDCL_VAR_PTR_FALSE(v) && !GET_CDCL_VAR_PTR_PHASE(v))){
		//This is not the case, we do not increase its agility and return right away.
		return;
	}

	//We increase the variable agility and retain the maximum value.
	if (SET_CDCL_VAR_PTR_AGI_INC_BY(v, cdcl_restartRules_currentAgiInc) > cdcl_restartRules_maxAgility){
		cdcl_restartRules_maxAgility = GET_CDCL_VAR_PTR_AGILITY(v);
	}

	//We check if the agility values must be re-scaled.
	if (cdcl_restartRules_maxAgility < VAR_MAX_AGILITY_VALUE) return;

	//Yes, we must re-scale.
	float_ty newMaxAgi = ZERO;
	uint32_t i;

	for (i = 1; i < f.n_initial+1; ++i){
		//We respect only variables that the CDCL can assign.
		if (!IS_VAR_UNASSIGNED((f.vars + i))) continue;

		SET_CDCL_VAR_AGI_NORM_BY(i, cdcl_restartRules_maxAgility);
		if (GET_CDCL_VAR_AGILITY(i) > newMaxAgi){
			newMaxAgi = GET_CDCL_VAR_AGILITY(i);
		}
	}

	//Finally, we reset the current agility increase value and set the new maximum value.
	cdcl_restartRules_maxAgility = newMaxAgi;
	cdcl_restartRules_currentAgiInc = newMaxAgi;

	#ifdef COLLECTSTATS
	++stats_cdcl_numVarAgiRescales;
	#endif
}

void cdcl_restartRule_lubyagility_signalBackjump(){
	//No further use of this information in this restart rule.
}
