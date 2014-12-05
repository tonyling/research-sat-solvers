/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "assInitRules.h"

void sls_extern_assInitRules_switch(int32_t switchTo){
	//Use to switch the initialization rule without re-initializing the whole SLS module. We first dispose the old plug-in.
	sls_assInitRules_disposePlugin();
	//We override the parameter to use the new flip rule indicated.
	param_slsAssInitRule = switchTo;
	//We re-initialize the plug-in.
	sls_assInitRules_initPlugin();
}

void sls_assInitRules_resetPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Resetting assignment initialization rule plug-in...\n");
	#endif
	sls_assInitRule_ai 		= &sls_assInitRule_random_ai;
	sls_assInitRule_prepare = &sls_assInitRule_random_prepare;

}

void sls_assInitRules_initPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Pick the pick and flip variable rule...\n");
	#endif
	//Pick the assignment initialization rule.
	if (param_slsAssInitRule == SLS_ASSINITRULE_RANDOM){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using RANDOM initialization rule.\n");
		#endif
		sls_assInitRule_ai 		= &sls_assInitRule_random_ai;
		sls_assInitRule_prepare = &sls_assInitRule_random_prepare;
	} else if (param_slsAssInitRule == SLS_ASSINITRULE_PROVIDED){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using PROVIDED initialization rule.\n");
		#endif
		sls_assInitRule_ai 		= &sls_assInitRule_provided_ai;
		sls_assInitRule_prepare = &sls_assInitRule_provided_prepare;
	} else if (param_slsAssInitRule == SLS_ASSINITRULE_PROVIDEDTHENRAND){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using PROVIDEDTHENRAND initialization rule.\n");
		#endif
		sls_assInitRule_ai 		= &sls_assInitRule_providedthenrand_ai;
		sls_assInitRule_prepare = &sls_assInitRule_providedthenrand_prepare;
	}
}

void sls_assInitRules_disposePlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Disposing initialization rule plug-in...\n");
	#endif
	sls_assInitRule_ai 		= &sls_assInitRule_random_ai;
	sls_assInitRule_prepare = &sls_assInitRule_random_prepare;
}

