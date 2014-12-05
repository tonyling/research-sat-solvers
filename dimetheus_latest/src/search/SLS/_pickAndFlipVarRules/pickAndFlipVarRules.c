/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "pickAndFlipVarRules.h"

void sls_extern_pickAndFlipVarRules_switch(int32_t switchTo){
	//Use to switch the flip rule without re-initializing the whole SLS module. We first dispose the old plug-in.
	sls_pickAndFlipVarRules_disposePlugin();
	//We override the parameter to use the new flip rule indicated.
	param_slsPickAndFlipVarRule = switchTo;
	//We re-initialize the plug-in.
	sls_pickAndFlipVarRules_initPlugin();
}

void sls_pickAndFlipVarRules_resetPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Resetting pick and flip variable rule plug-in...\n");
	#endif
	sls_pickAndFlipVarRule_paf 	= &sls_pickAndFlipVarRule_walksat_paf;
	sls_pickAndFlipVarRule_prepare = &sls_pickAndFlipVarRule_walksat_prepare;

	sls_probsat_scoreLookupB = NULL;
}

void sls_pickAndFlipVarRules_initPlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Pick the pick and flip variable rule...\n");
	#endif
	//Pick the pick and flip variable rule.
	if (param_slsPickAndFlipVarRule == SLS_PICKANDFLIPVARRULE_WALKSAT){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using WALKSAT pick and flip variable rule.\n");
		#endif
		sls_pickAndFlipVarRule_paf 	= &sls_pickAndFlipVarRule_walksat_paf;
		sls_pickAndFlipVarRule_prepare = &sls_pickAndFlipVarRule_walksat_prepare;
	} else if (param_slsPickAndFlipVarRule == SLS_PICKANDFLIPVARRULE_PROBSATPB){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using PROBSATPB pick and flip variable rule.\n");
		#endif
		sls_pickAndFlipVarRule_paf 	= &sls_pickAndFlipVarRule_probsatPB_paf;
		sls_pickAndFlipVarRule_prepare = &sls_pickAndFlipVarRule_probsatPB_prepare;
	} else if (param_slsPickAndFlipVarRule == SLS_PICKANDFLIPVARRULE_PROBSATEB){
		#ifdef VERBOSE_SLS
		printf("c     SLS: Using PROBSATEB pick and flip variable rule.\n");
		#endif
		sls_pickAndFlipVarRule_paf 	= &sls_pickAndFlipVarRule_probsatEB_paf;
		sls_pickAndFlipVarRule_prepare = &sls_pickAndFlipVarRule_probsatEB_prepare;
	}

	sls_probsat_scoreLookupB = malloc(sizeof(float_ty)*BLOCKSIZE);
	if (sls_probsat_scoreLookupB == NULL){
		printf("c ERROR. Failed to initialize the SLS ProbSAT score lookup break table. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}
}

void sls_pickAndFlipVarRules_disposePlugin(){
	#ifdef VERBOSE_SLS
	printf("c     SLS: Disposing pick and flip variable rule plug-in...\n");
	#endif
	sls_pickAndFlipVarRule_paf 	= &sls_pickAndFlipVarRule_walksat_paf;
	sls_pickAndFlipVarRule_prepare = &sls_pickAndFlipVarRule_walksat_prepare;

	if (sls_probsat_scoreLookupB != NULL){
		free(sls_probsat_scoreLookupB);
		sls_probsat_scoreLookupB = NULL;
	}
}
