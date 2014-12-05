/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "selectDirRules.h"

void cdcl_selectDirRule_random_printHelp(){
	printf("c      %-3d: RANDOM:\n", CDCL_SELECTDIRRULE_RANDOM);
    printf("c           Behavior: Picks the direction for the decision variable at random.\n");
}

void cdcl_selectDirRule_random_prepare(){
	//Nothing is to be done to initialize a call to the random direction selection rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SDR-Plug-in [%-3d: RANDOM]:\n", CDCL_SELECTDIRRULE_RANDOM);
	#endif
}

void cdcl_selectDirRule_random_save(cdclVariable* v){
	//This method does not do anything. The random scheme does not retain phases.
}

cdclLiteral* cdcl_selectDirRule_random_select(){
	//Picks an assignment at random, represented as a literal.
	if (rand_mod(2) == 1U){
		return (cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_decVar));
	} else {
		return (cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_decVar));
	}
}
