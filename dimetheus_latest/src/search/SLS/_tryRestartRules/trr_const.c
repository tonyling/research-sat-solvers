/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_tryRestartRule_const_printHelp(){
	printf("c      %-3d: CONST:\n", SLS_TRYRESTARTRULE_CONST);
    printf("c           Behavior: Performs the given number of tries with a constant number of flips.\n");
    printf("c           Formulas: slsMaxNumTries = slsMaxNumTries.\n");
    printf("c                     slsMaxNumFlips = n*slsMaxFlipsA\n");
}

void sls_tryRestartRule_const_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     TRR-Plug-in [%-3d: CONST]:\n", SLS_TRYRESTARTRULE_CONST);
	printf("c     SLS:       -slsMaxFlipsA %f.\n", param_slsMaxFlipsA);
	printf("c     SLS:       -slsMaxTries %d.\n", param_slsMaxTries);
	#endif
}

void sls_tryRestartRule_const_prepareTry(){
	//Set the fall-back values.
	sls_maxNumFlips = 0;
	sls_maxNumTries = 0;

	//We set the allowed number of flips to a constant value.
	sls_maxNumTries = param_slsMaxTries;
	sls_maxNumFlips = param_slsMaxFlipsA*sls_varNum;
	sls_numFlips = 0;

	#ifdef VERBOSE_SLS
	printf("c     SLS:       slsMaxNumFlips = %d.\n", (uint32_t)sls_maxNumFlips);
	printf("c     SLS:       slsMaxNumTries = %d.\n", (uint32_t)sls_maxNumTries);
	#endif
}


