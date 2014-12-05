/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "assInitRules.h"

void sls_assInitRule_providedthenrand_printHelp(){
	printf("c      %-3d: PROVIDEDTHENRAND:\n", SLS_ASSINITRULE_PROVIDEDTHENRAND);
    printf("c           Behavior: First use the search provided phases and then random initializations for the assignment.\n");
}

void sls_assInitRule_providedthenrand_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     AIR-Plug-in [%-3d: PROVIDEDTHENRAND]:\n", SLS_ASSINITRULE_PROVIDEDTHENRAND);
	#endif
}

void sls_assInitRule_providedthenrand_ai(){
	//Initialize the variable assignments using the provided variable phases of the search.
	variable *v;
	int32_t i;

	if (sls_numTries == 0) {
		//Initial try is done based on the phases provided by the search.
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
	} else {
		//All subsequent tries will get a random initialization.
		for (i = 1; i < f.n_initial+1; ++i){
			v = main_varData + i;
			//Check if the variable is unassigned.
			if (IS_VAR_UNASSIGNED(v)){
				//Yes, it is unassigned, we pick a random assignment.
				if ((rand_random() & 1U) == 1U){
					SET_SLS_VAR_ASSIGNMENT_FALSE(v->id);
				} else {
					SET_SLS_VAR_ASSIGNMENT_TRUE(v->id);
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
}


