/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strOtherRules.h"

void cdcl_extern_strOtherRules_switch(int32_t switchTo){
	//Use to switch the strengthen rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_strOtherRules_disposePlugin();
	//We override the parameter to use the new strengthen rule indicated.
	param_cdclStrOtherRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_strOtherRules_initPlugin();
}

void cdcl_strOtherRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting strengthen other rule (SOR) plug-in...\n");
	#endif
	cdcl_strOtherRule_str		= &cdcl_strOtherRule_null_str;
	cdcl_strOtherRule_prepare 	= &cdcl_strOtherRule_null_prepare;

	//Reset SOR data-structures here.
}

void cdcl_strOtherRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the strengthen other rule... ");
	#endif
	//Pick the select strengthening rule.
	if (param_cdclStrOtherRule == CDCL_STROTHERRULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_strOtherRule_str		= &cdcl_strOtherRule_null_str;
		cdcl_strOtherRule_prepare 	= &cdcl_strOtherRule_null_prepare;
	} else if (param_cdclStrOtherRule == CDCL_STROTHERRULE_WTWO){
		#ifdef VERBOSE_CDCL
		printf("WTWO");
		#endif
		cdcl_strOtherRule_str		= &cdcl_strOtherRule_wtwo_str;
		cdcl_strOtherRule_prepare 	= &cdcl_strOtherRule_wtwo_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize SOR data-structures here.
}

void cdcl_strOtherRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing strengthen other rule (SOR) plug-in...\n");
	#endif
	cdcl_strOtherRule_str		= &cdcl_strOtherRule_null_str;
	cdcl_strOtherRule_prepare 	= &cdcl_strOtherRule_null_prepare;

	//Dispose SOR data-structures here.
}


