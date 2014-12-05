/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_tryRestartRule_singleunlimited_printHelp(){
	printf("c      %-3d: SINGLEUNLIMITED:\n", SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
    printf("c           Behavior: Performs a single try with unlimited flips (no restarts).\n");
    printf("c           Formulas: slsMaxNumTries = 1\n");
    printf("c                     slsMaxNumFlips = 0\n");
}

void sls_tryRestartRule_singleunlimited_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     TRR-Plug-in [%-3d: SINGLEUNLIMITED]:\n", SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
	#endif
}

void sls_tryRestartRule_singleunlimited_prepareTry(){
	sls_maxNumFlips = 0;
	sls_maxNumTries = 1;
	#ifdef VERBOSE_SLS
	printf("c     SLS:       slsMaxNumFlips = %d.\n", (uint32_t)sls_maxNumFlips);
	printf("c     SLS:       slsMaxNumTries = %d.\n", (uint32_t)sls_maxNumTries);
	#endif
}
