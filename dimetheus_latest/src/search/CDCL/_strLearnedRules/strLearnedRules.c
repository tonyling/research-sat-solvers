/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_extern_strLearnedRules_switch(int32_t switchTo){
	//Use to switch the strengthen rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_strLearnedRules_disposePlugin();
	//We override the parameter to use the new strengthen rule indicated.
	param_cdclStrLearnedRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_strLearnedRules_initPlugin();
}

void cdcl_strLearnedRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting strengthen learned rule (SLR) plug-in...\n");
	#endif
	cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_null_str;
	cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_null_prepare;

	//Reset SLR data-structures here.
}

void cdcl_strLearnedRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the strengthen learn rule... ");
	#endif
	//Pick the select strengthening rule.
	if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_null_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_null_prepare;
	} else if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_ALLOCC){
		#ifdef VERBOSE_CDCL
		printf("ALLOCC");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_allocc_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_allocc_prepare;
	} else if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_REASONS){
		#ifdef VERBOSE_CDCL
		printf("REASONS");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_reasons_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_reasons_prepare;
	} else if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_ANTECEDENTS){
		#ifdef VERBOSE_CDCL
		printf("ANTECEDENTS");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_antecedents_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_antecedents_prepare;
	} else if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_LOCAL){
		#ifdef VERBOSE_CDCL
		printf("LOCAL");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_local_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_local_prepare;
	} else if (param_cdclStrLearnedRule == CDCL_STRLEARNEDRULE_LOCALREC){
		#ifdef VERBOSE_CDCL
		printf("LOCALREC");
		#endif
		cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_localrec_str;
		cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_localrec_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize SLR data-structures here.
}

void cdcl_strLearnedRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing strengthen learned rule (SLR) plug-in...\n");
	#endif
	cdcl_strLearnedRule_str		= &cdcl_strLearnedRule_null_str;
	cdcl_strLearnedRule_prepare = &cdcl_strLearnedRule_null_prepare;

	//Dispose SLR data-structures here.
}

