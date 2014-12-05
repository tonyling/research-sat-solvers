/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "restartRules.h"

void cdcl_restartRule_innerouteragility_printHelp(){
	printf("c      %-3d: INNEROUTERAGILITY:\n", CDCL_RESTARTRULE_INNEROUTERAGILITY);
    printf("c           Behavior: Performs restarts according to an inner/outer geometric series (respects agility).\n");
}

void cdcl_restartRule_innerouteragility_prepare(){
	cdcl_restartRule_innerouteragility_t = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouteragility_i = param_cdclRestartInnerouterInit;
	cdcl_restartRule_innerouteragility_o = param_cdclRestartInnerouterInit;
	cdcl_restartRules_maxAgility 	     = ONE;
	cdcl_restartRules_currentAgiInc      = ONE;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     RR-Plug-in [%-3d: INNEROUTERAGILITY]:\n", CDCL_RESTARTRULE_INNEROUTERAGILITY);
	printf("c     CDCL:       param_cdclRestartInnerouterInit        = %d\n", param_cdclRestartInnerouterInit);
	printf("c     CDCL:       param_cdclRestartInnerouterInc         = %f\n", param_cdclRestartInnerouterInc);
	printf("c     CDCL:       param_cdclRestartVarAgiIncBump         = %f\n", param_cdclRestartVarAgiIncBump);
	printf("c     CDCL:       param_cdclRestartMaxAgiFrac            = %f\n", param_cdclRestartMaxAgiFrac);
	printf("c     CDCL:       param_cdclRestartMaxAgiOutFrac         = %f\n", param_cdclRestartMaxAgiOutFrac);
	printf("c     CDCL:       cdcl_restartRule_innerouteragility_t   = %d\n", cdcl_restartRule_innerouteragility_t);
	printf("c     CDCL:       cdcl_restartRule_innerouteragility_i   = %d\n", cdcl_restartRule_innerouteragility_i);
	printf("c     CDCL:       cdcl_restartRule_innerouteragility_o   = %d\n", cdcl_restartRule_innerouteragility_o);
	printf("c     CDCL:       cdcl_restartRules_currentAgiInc        = %f\n", cdcl_restartRules_currentAgiInc);
	printf("c     CDCL:       cdcl_restartRules_maxAgility           = %f\n", cdcl_restartRules_maxAgility);
	#endif
}

uint32_t cdcl_restartRule_innerouteragility_check(){
	//Check, if the number of conflicts reached the target.
	if (cdcl_numConflicts < cdcl_restartRule_innerouteragility_t){
		//No, so no restart happens.
		return 0u;
	}

	//Next, we will perform a restart. For this, we increase the inner value by the increase factor.
	cdcl_restartRule_innerouteragility_i *= param_cdclRestartInnerouterInc;

	//Check if the inner cycle is over and a new outer cycle is started with an increased outer length.
	if (cdcl_restartRule_innerouteragility_i >= cdcl_restartRule_innerouteragility_o){
		//Yes, we therefore reset the inner and increase the outer.
		cdcl_restartRule_innerouteragility_i = param_cdclRestartInnerouterInit;
		cdcl_restartRule_innerouteragility_o *= param_cdclRestartInnerouterInc;

	}

	//We set the new target of conflicts for the next restart.
	cdcl_restartRule_innerouteragility_t += cdcl_restartRule_innerouteragility_i;

	//We now check if a high variable agility prohibits a restart.
	uint32_t numVars = 1, numAgileVars = 1, i;
	for (i = 1; i < f.n_initial+1; ++i){
		//We respect only variables that the CDCL can assign.
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) {
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

	//Check how many variable are currently considered very agile. We distinct between inner and outer restarts.
	if (cdcl_restartRule_innerouteragility_i == param_cdclRestartInnerouterInit){
		//It is an outer restart.
		if (param_cdclRestartMaxAgiOutFrac < ((float_ty)numAgileVars) / ((float_ty)numVars)){
			//The amount of agile variables exceeds the amount of what is allowed for a restart.
			#ifdef COLLECTSTATS
			++stats_cdcl_numRestartProhibits;
			#endif
			return 0u;
		}
	} else {
		//It is an inner restart.
		if (param_cdclRestartMaxAgiFrac < ((float_ty)numAgileVars) / ((float_ty)numVars)){
			//The amount of agile variables exceeds the amount of what is allowed for a restart.
			#ifdef COLLECTSTATS
			++stats_cdcl_numRestartProhibits;
			#endif
			return 0u;
		}
	}

	#ifdef COLLECTSTATS
	++stats_cdcl_numRestarts;
	#endif
	return 1u;
}

void cdcl_restartRule_innerouteragility_incAgility(cdclVariable* v){
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
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) continue;

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

void cdcl_restartRule_innerouteragility_signalBackjump(){
	//No further use of this information in this restart rule.
}
