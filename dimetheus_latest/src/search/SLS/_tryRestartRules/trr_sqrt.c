/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_tryRestartRule_sqrt_printHelp(){
	printf("c      %-3d: SQRT:\n", SLS_TRYRESTARTRULE_SQRT);
    printf("c           Behavior: Performs the given number of tries with a square root decreasing number of flips.\n");
    printf("c           Formulas: slsMaxNumTries = slsMaxTries\n");
    printf("c                     slsMaxNumFlips = n * (slsMaxFlipsA)^( slsMaxFlipsB / (try+1) )\n");
}

void sls_tryRestartRule_sqrt_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     TRR-Plug-in [%-3d: SQRT]:\n", SLS_TRYRESTARTRULE_SQRT);
	printf("c     SLS:       -slsMaxFlipsA %f.\n", param_slsMaxFlipsA);
	printf("c     SLS:       -slsMaxFlipsB %f.\n", param_slsMaxFlipsB);
	printf("c     SLS:       -slsMaxTries %d.\n", param_slsMaxTries);
	#endif
}

void sls_tryRestartRule_sqrt_prepareTry(){
	//Set the fall-back values.
	sls_maxNumFlips = 0;
	sls_maxNumTries = 0;

	//We set the allowed number of flips to a geometric increasing value.
	sls_maxNumTries = param_slsMaxTries;
	sls_maxNumFlips = pow(param_slsMaxFlipsA,param_slsMaxFlipsB/(((float_ty)sls_numTries)+1.0)) * sls_varNum;
	sls_numFlips = 0;

	#ifdef VERBOSE_SLS
	printf("c     SLS:       slsMaxNumFlips = %d.\n", (uint32_t)sls_maxNumFlips);
	printf("c     SLS:       slsMaxNumTries = %d.\n", (uint32_t)sls_maxNumTries);
	#endif
}


