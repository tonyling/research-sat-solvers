/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_tryRestartRule_exp_printHelp(){
	printf("c      %-3d: EXP:\n", SLS_TRYRESTARTRULE_EXP);
    printf("c           Behavior: Performs the given number of tries with an exponentially increasing number of flips.\n");
    printf("c           Formulas: slsMaxNumTries = slsMaxTries\n");
    printf("c                     slsMaxNumFlips = n * (slsMaxFlipsA)^( (try+1)*slsMaxFlipsB )\n");
}

void sls_tryRestartRule_exp_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     TRR-Plug-in [%-3d: EXP]:\n", SLS_TRYRESTARTRULE_EXP);
	printf("c     SLS:       -slsMaxFlipsA %f.\n", param_slsMaxFlipsA);
	printf("c     SLS:       -slsMaxFlipsB %f.\n", param_slsMaxFlipsB);
	printf("c     SLS:       -slsMaxTries %d.\n", param_slsMaxTries);
	#endif
}

void sls_tryRestartRule_exp_prepareTry(){
	//Set the fall-back values.
	sls_maxNumFlips = 0;
	sls_maxNumTries = 0;

	//We set the allowed number of flips to an exponentially increasing value.
	sls_maxNumTries = param_slsMaxTries;
	sls_maxNumFlips = pow(param_slsMaxFlipsA, (((float_ty)sls_numTries)+ONE)*param_slsMaxFlipsB) * sls_varNum;
	sls_numFlips = 0;

	#ifdef VERBOSE_SLS
	printf("c     SLS:       slsMaxNumFlips = %d.\n", (uint32_t)sls_maxNumFlips);
	printf("c     SLS:       slsMaxNumTries = %d.\n", (uint32_t)sls_maxNumTries);
	#endif
}


