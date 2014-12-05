/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "selectDirRules.h"

void cdcl_selectDirRule_phasesaving_printHelp(){
	printf("c      %-3d: PHASESAVING:\n", CDCL_SELECTDIRRULE_PHASESAVING);
    printf("c           Behavior: Retains the last successful assignment to the variable and tries this again.\n");
}

void cdcl_selectDirRule_phasesaving_prepare(){
	//Nothing is to be done to initialize a call to the random direction selection rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SDR-Plug-in [%-3d: PHASESAVING]:\n", CDCL_SELECTDIRRULE_PHASESAVING);
	#endif
	cdcl_selectDirRules_intern_dirInit();
}

void cdcl_selectDirRule_phasesaving_save(cdclVariable* v){
	//We retain the current variable assignment.
	if (GET_CDCL_VAR_PTR_REASON(v) != NULL){
		if (IS_CDCL_VAR_PTR_TRUE(v)){
			SET_CDCL_VAR_PTR_PHASE(v,1u);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0u);
		}
	}
}

cdclLiteral* cdcl_selectDirRule_phasesaving_select(){
	//Picks the last assignment that has been saved for this variable.
	if (GET_CDCL_VAR_PTR_PHASE(cdcl_decVar)) {
		//The positive phase. We return the positive literal.
		return cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar);
	}

	//Otherwise we return the negative literal.
	return cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar);
}
