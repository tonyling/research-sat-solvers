/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "conflictAnalysisRules.h"

void cdcl_extern_conflictAnalysisRules_switch(int32_t switchTo){
	//Use to switch the analysis rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_conflictAnalysisRules_disposePlugin();
	//We override the parameter to use the new analysis rule indicated.
	param_cdclSelectVarRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_conflictAnalysisRules_initPlugin();
}

void cdcl_conflictAnalysisRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting select conflict analysis rule (CAR) plug-in...\n");
	#endif
	cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_alldec_analyze;
	cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_alldec_prepare;

	//Reset CAR data-structures here.
}

void cdcl_conflictAnalysisRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the select conflict analysis rule... ");
	#endif
	//Pick the conflict analysis rule.
	if (param_cdclConflictAnalysisRule == CDCL_CONFLICTANALYSISRULE_ALLDEC){
		#ifdef VERBOSE_CDCL
		printf("ALLDEC");
		#endif
		cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_alldec_analyze;
		cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_alldec_prepare;
	} else if (param_cdclConflictAnalysisRule == CDCL_CONFLICTANALYSISRULE_FIRSTUIP){
		#ifdef VERBOSE_CDCL
		printf("FIRSTUIP");
		#endif
		cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_firstuip_analyze;
		cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_firstuip_prepare;
	} else if (param_cdclConflictAnalysisRule == CDCL_CONFLICTANALYSISRULE_LASTUIP){
		#ifdef VERBOSE_CDCL
		printf("LASTUIP");
		#endif
		cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_lastuip_analyze;
		cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_lastuip_prepare;
	} else if (param_cdclConflictAnalysisRule == CDCL_CONFLICTANALYSISRULE_ALLUIP){
		#ifdef VERBOSE_CDCL
		printf("ALLUIP");
		#endif
		cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_alluip_analyze;
		cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_alluip_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize CAR data-structures here.
}

void cdcl_conflictAnalysisRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing conflict analysis rule (CAR) plug-in...\n");
	#endif
	cdcl_conflictAnalysisRule_analyze  = &cdcl_conflictAnalysisRule_alldec_analyze;
	cdcl_conflictAnalysisRule_prepare  = &cdcl_conflictAnalysisRule_alldec_prepare;

	//Dispose CAR data-structures here.
}
