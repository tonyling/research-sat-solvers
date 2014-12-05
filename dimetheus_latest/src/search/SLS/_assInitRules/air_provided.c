/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "assInitRules.h"

void sls_assInitRule_provided_printHelp(){
	printf("c      %-3d: PROVIDED:\n", SLS_ASSINITRULE_PROVIDED);
    printf("c           Behavior: Initialize the SLS starting assignment using the provided variable phases of the search.\n");
}

void sls_assInitRule_provided_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     AIR-Plug-in [%-3d: PROVIDED]:\n", SLS_ASSINITRULE_PROVIDED);
	#endif
}

void sls_assInitRule_provided_ai(){
	//Initialize the variable assignments using the provided variable phases of the search.
	variable *v;
	int32_t i;
	for (i = 1; i < f.n_initial+1; ++i){
		v = main_varData + i;
		//Check if the variable is unassigned.
		if (IS_VAR_UNASSIGNED(v)){
			//Yes, it is unassigned, we pick the phase for this variable to initialize its assignment.
			if (GET_VAR_PHASE(v)){
				SET_SLS_VAR_ASSIGNMENT_TRUE(v->id);
			} else {
				SET_SLS_VAR_ASSIGNMENT_FALSE(v->id);
			}
		} else {
			//No, the variable is assigned. We pick its assignment for the initialization.
			if (IS_VAR_TRUE(v)){
				SET_SLS_VAR_ASSIGNMENT_TRUE(v->id);
			} else {
				SET_SLS_VAR_ASSIGNMENT_FALSE(v->id);
			}
		}
	}
}

