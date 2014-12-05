/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "abortRules.h"

void cdcl_extern_abortRules_switch(int32_t switchTo){
	//Use to switch the abort rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_abortRules_disposePlugin();
	//We override the parameter to use the new abort rule indicated.
	param_cdclAbortRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_abortRules_initPlugin();
}

void cdcl_abortRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting abort rule (AR) plug-in...\n");
	#endif
	cdcl_abortRule_check   			 = &cdcl_abortRule_null_check;
	cdcl_abortRule_prepare 			 = &cdcl_abortRule_null_prepare;
	cdcl_abortRule_signalMaintenance = &cdcl_abortRule_null_signalMaintenance;
	cdcl_abortRule_signalRestart	 = &cdcl_abortRule_null_signalRestart;
	//Reset AR data-structures here.

}

void cdcl_abortRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the abort rule... ");
	#endif
	//Pick the abort rule.
	if (param_cdclAbortRule == CDCL_ABORTRULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_abortRule_check   			 = &cdcl_abortRule_null_check;
		cdcl_abortRule_prepare 			 = &cdcl_abortRule_null_prepare;
		cdcl_abortRule_signalMaintenance = &cdcl_abortRule_null_signalMaintenance;
		cdcl_abortRule_signalRestart	 = &cdcl_abortRule_null_signalRestart;
	} else if (param_cdclAbortRule == CDCL_ABORTRULE_PMPCDCL){
		#ifdef VERBOSE_CDCL
		printf("PMPCDCL");
		#endif
		cdcl_abortRule_check  			 = &cdcl_abortRule_pmpcdcl_check;
		cdcl_abortRule_prepare			 = &cdcl_abortRule_pmpcdcl_prepare;
		cdcl_abortRule_signalMaintenance = &cdcl_abortRule_pmpcdcl_signalMaintenance;
		cdcl_abortRule_signalRestart	 = &cdcl_abortRule_pmpcdcl_signalRestart;
	} else if (param_cdclAbortRule == CDCL_ABORTRULE_HYBRIDMSC){
		#ifdef VERBOSE_CDCL
		printf("HYBRIDMSC");
		#endif
		cdcl_abortRule_check  			 = &cdcl_abortRule_hybridmsc_check;
		cdcl_abortRule_prepare			 = &cdcl_abortRule_hybridmsc_prepare;
		cdcl_abortRule_signalMaintenance = &cdcl_abortRule_hybridmsc_signalMaintenance;
		cdcl_abortRule_signalRestart	 = &cdcl_abortRule_hybridmsc_signalRestart;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize AR data-structures here.

}

void cdcl_abortRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing abort rule (AR) plug-in...\n");
	#endif
	cdcl_abortRule_check   			 = &cdcl_abortRule_null_check;
	cdcl_abortRule_prepare 			 = &cdcl_abortRule_null_prepare;
	cdcl_abortRule_signalMaintenance = &cdcl_abortRule_null_signalMaintenance;
	cdcl_abortRule_signalRestart	 = &cdcl_abortRule_null_signalRestart;

	//Dispose AR data-structures here.

}

