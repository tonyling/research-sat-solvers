/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectDirRules.h"

void cdcl_selectDirRule_failbinary_printHelp(){
	printf("c      %-3d: FAILBINARY:\n", CDCL_SELECTDIRRULE_FAILBINARY);
    printf("c           Behavior: Picks the assignment that fails more binaries, or if there are none the last phase.\n");
}

void cdcl_selectDirRule_failbinary_prepare(){
	//Nothing is to be done to initialize a call to the random direction selection rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SDR-Plug-in [%-3d: FAILBINARY]:\n", CDCL_SELECTDIRRULE_FAILBINARY);
	#endif
	cdcl_selectDirRules_intern_dirInit();
}

void cdcl_selectDirRule_failbinary_save(cdclVariable* v){
	//We retain the current variable assignment.
	if (IS_CDCL_VAR_PTR_TRUE(v)){
		SET_CDCL_VAR_PTR_PHASE(v,1u);
	} else {
		SET_CDCL_VAR_PTR_PHASE(v,0u);
	}
}

cdclLiteral* cdcl_selectDirRule_failbinary_select(){
	//Check if there are binaries that can be failed when assigning this variable.
	if ((cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar))->numImpUsed > 0 || (cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar))->numImpUsed > 0){
		//Yes, the variable occurs in binary clauses.
		if ((cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar))->numImpUsed > (cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar))->numImpUsed){
			//It appears in more binaries false.
			return cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar);
		} else {
			//It appears in more binaries true.
			return cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar);
		}
	}

	//Picks the last assignment that has been saved for this variable in case there are no binaries to fail.
	if (GET_CDCL_VAR_PTR_PHASE(cdcl_decVar)) {
		//The positive phase. We return the positive literal.
		return cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar);
	}

	//Otherwise we return the negative literal.
	return cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar);
}

