/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_tryRestartRule_linear_printHelp(){
	printf("c      %-3d: LINEAR:\n", SLS_TRYRESTARTRULE_LINEAR);
    printf("c           Behavior: Performs the given number of tries with linearly increasing number of flips.\n");
    printf("c           Formulas: slsMaxNumTries = slsMaxTries\n");
    printf("c                     slsMaxNumFlips = n * slsMaxFlipsA * (try+1)\n");
}

void sls_tryRestartRule_linear_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     TRR-Plug-in [%-3d: LINEAR]:\n", SLS_TRYRESTARTRULE_LINEAR);
	printf("c     SLS:       -slsMaxFlipsA %f.\n", param_slsMaxFlipsA);
	printf("c     SLS:       -slsMaxTries %d.\n", param_slsMaxTries);
	#endif
}

void sls_tryRestartRule_linear_prepareTry(){
	//Set the fall-back values.
	sls_maxNumFlips = 0;
	sls_maxNumTries = 0;

	//We increase the amount of allowed flips with each try in a linear way.
	sls_maxNumTries = param_slsMaxTries;
	sls_maxNumFlips = (param_slsMaxFlipsA*(sls_numTries+1)) * sls_varNum;
	sls_numFlips = 0;

	#ifdef VERBOSE_SLS
	printf("c     SLS:       slsMaxNumFlips = %d.\n", (uint32_t)sls_maxNumFlips);
	printf("c     SLS:       slsMaxNumTries = %d.\n", (uint32_t)sls_maxNumTries);
	#endif
}

