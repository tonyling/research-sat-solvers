/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "tryRestartRules.h"

void sls_extern_tryRestartRules_switch(int32_t switchTo){
	//Use to switch the try restart rule without re-initializing the whole SLS module. We first dispose the old plug-in.
	sls_tryRestartRules_disposePlugin();
	//We override the parameter to use the new try restart rule indicated.
	param_slsTryRestartRule = switchTo;
	//We re-initialize the plug-in.
	sls_tryRestartRules_initPlugin();
}

void sls_tryRestartRules_resetPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Resetting try restart rule plug-in...\n");
	#endif
	sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_singleunlimited_prepareTry;
	sls_tryRestartRule_prepare 		= &sls_tryRestartRule_singleunlimited_prepare;

	sls_numFlips			= 0;
	sls_numTries			= 0;
}

void sls_tryRestartRules_initPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Pick the try restart rule...\n");
	#endif
	//Pick the try restart rule.
	if (param_slsTryRestartRule == SLS_TRYRESTARTRULE_SINGLEUNLIMITED){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using SINGLEUNLIMITED try restart rule.\n");
		#endif
		sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_singleunlimited_prepareTry;
		sls_tryRestartRule_prepare 		= &sls_tryRestartRule_singleunlimited_prepare;
	} else 	if (param_slsTryRestartRule == SLS_TRYRESTARTRULE_CONST){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using CONST try restart rule.\n");
		#endif
		sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_const_prepareTry;
		sls_tryRestartRule_prepare 		= &sls_tryRestartRule_const_prepare;
	} else 	if (param_slsTryRestartRule == SLS_TRYRESTARTRULE_LINEAR){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using LINEAR try restart rule.\n");
		#endif
		sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_linear_prepareTry;
		sls_tryRestartRule_prepare 		= &sls_tryRestartRule_linear_prepare;
	} else 	if (param_slsTryRestartRule == SLS_TRYRESTARTRULE_SQRT){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using SQRT try restart rule.\n");
		#endif
		sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_sqrt_prepareTry;
		sls_tryRestartRule_prepare 		= &sls_tryRestartRule_sqrt_prepare;
	} else 	if (param_slsTryRestartRule == SLS_TRYRESTARTRULE_EXP){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using EXP try restart rule.\n");
		#endif
		sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_exp_prepareTry;
		sls_tryRestartRule_prepare 		= &sls_tryRestartRule_exp_prepare;
	}

	sls_numFlips			= 0;
	sls_numTries			= 0;
}

void sls_tryRestartRules_disposePlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Disposing try restart rule plug-in...\n");
	#endif
	sls_tryRestartRule_prepareTry 	= &sls_tryRestartRule_singleunlimited_prepareTry;
	sls_tryRestartRule_prepare 		= &sls_tryRestartRule_singleunlimited_prepare;

	sls_numFlips			= 0;
	sls_numTries			= 0;
}

