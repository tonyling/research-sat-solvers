/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectDirRules.h"

void cdcl_selectDirRule_phaseflip_printHelp(){
	printf("c      %-3d: PHASEFLIP:\n", CDCL_SELECTDIRRULE_PHASEFLIP);
    printf("c           Behavior: Like PHASESAVING but inverts the phase in DL1 from time to time.\n");
}

void cdcl_selectDirRule_phaseflip_prepare(){
	cdcl_selectDirRule_phaseflip_isFlipping  = 0;
	cdcl_selectDirRule_phaseflip_notFlipped  = 0;
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SDR-Plug-in [%-3d: PHASEFLIP]:\n", CDCL_SELECTDIRRULE_PHASEFLIP);
	printf("c     CDCL:       cdcl_selectDirRule_phaseflip_isFlipping  = %d\n", cdcl_selectDirRule_phaseflip_isFlipping);
	printf("c     CDCL:       cdcl_selectDirRule_phaseflip_notFlipped  = %d\n", cdcl_selectDirRule_phaseflip_notFlipped);
	printf("c     CDCL:       param_cdclSelectDirMinFlipDist           = %d\n", param_cdclSelectDirMinFlipDist);
	#endif
	cdcl_selectDirRules_intern_dirInit();
}

void cdcl_selectDirRule_phaseflip_save(cdclVariable* v){
	//We retain the current variable assignment if and only if we are not currently flipping.
	if (cdcl_selectDirRule_phaseflip_isFlipping) return;
	if (GET_CDCL_VAR_PTR_REASON(v) != NULL){
		if (IS_CDCL_VAR_PTR_TRUE(v)){
			SET_CDCL_VAR_PTR_PHASE(v,1u);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0u);
		}
	}
}

cdclLiteral* cdcl_selectDirRule_phaseflip_select(){
	int32_t l = 0;

	if (GET_CDCL_VAR_PTR_PHASE(cdcl_decVar)) {
		//The positive phase. We return the positive literal.
		l = GET_CDCL_VAR_PTR_ID(cdcl_decVar);
	} else {
		//Otherwise we return the negative literal.
		l = -GET_CDCL_VAR_PTR_ID(cdcl_decVar);
	}

	if (cdcl_decLevel == 1){
		//If we are in DL1, we distinct between the cases we are already flipping and we are not flipping.
		if (cdcl_selectDirRule_phaseflip_isFlipping){
			//We are currently in flipping mode. We turn flipping OFF.
			cdcl_selectDirRule_phaseflip_isFlipping = 0;
		} else {
			//We are currently not in flipping mode.
			if (++cdcl_selectDirRule_phaseflip_notFlipped > param_cdclSelectDirMinFlipDist){
				cdcl_selectDirRule_phaseflip_isFlipping = 1;
				cdcl_selectDirRule_phaseflip_notFlipped = 0;
				l = -l;
			}
		}
	}

	return cdcl_literals + l;
}

