/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "maintenanceRules.h"

void cdcl_extern_maintenanceRules_switch(int32_t switchTo){
	//Use to switch the maintenance rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_maintenanceRules_disposePlugin();
	//We override the parameter to use the new maintenance rule indicated.
	param_cdclMaintenanceRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_maintenanceRules_initPlugin();
}

void cdcl_maintenanceRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting maintenance rule (MR) plug-in...\n");
	#endif
	cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_null_maintain;
	cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_null_prepare;
	cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_null_check;
	cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_null_incAct;
	cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_null_rescale;
	cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_null_bump;

	//Reset MR data-structures here.
	cdcl_maintenanceRules_numPerformed = 0U;
	cdcl_maintenanceRules_assignableVars = 1U;
	cdcl_maintenanceRules_targetLearned = 1U;
	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = ONE;
}

void cdcl_maintenanceRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the maintenance rule... ");
	#endif
	//Pick the select variable rule.
	if (param_cdclMaintenanceRule == CDCL_MAINTENANCERULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_null_maintain;
		cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_null_prepare;
		cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_null_check;
		cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_null_incAct;
		cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_null_rescale;
		cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_null_bump;
	} else if (param_cdclMaintenanceRule == CDCL_MAINTENANCERULE_INACTIVITY){
		#ifdef VERBOSE_CDCL
		printf("INACTIVITY");
		#endif
		cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_inactivity_maintain;
		cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_inactivity_prepare;
		cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_inactivity_check;
		cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_inactivity_incAct;
		cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_inactivity_rescale;
		cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_inactivity_bump;
	} else if (param_cdclMaintenanceRule == CDCL_MAINTENANCERULE_LBD){
		#ifdef VERBOSE_CDCL
		printf("LBD");
		#endif
		cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_lbd_maintain;
		cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_lbd_prepare;
		cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_lbd_check;
		cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_lbd_incAct;
		cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_lbd_rescale;
		cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_lbd_bump;
	} else if (param_cdclMaintenanceRule == CDCL_MAINTENANCERULE_LBDINACT){
		#ifdef VERBOSE_CDCL
		printf("LBDINACT");
		#endif
		cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_lbdinact_maintain;
		cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_lbdinact_prepare;
		cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_lbdinact_check;
		cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_lbdinact_incAct;
		cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_lbdinact_rescale;
		cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_lbdinact_bump;
	} else if (param_cdclMaintenanceRule == CDCL_MAINTENANCERULE_LBDINACTLUBY){
		#ifdef VERBOSE_CDCL
		printf("LBDINACTLUBY");
		#endif
		cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_lbdinactluby_maintain;
		cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_lbdinactluby_prepare;
		cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_lbdinactluby_check;
		cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_lbdinactluby_incAct;
		cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_lbdinactluby_rescale;
		cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_lbdinactluby_bump;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize MR data-structures here.
	cdcl_maintenanceRules_numPerformed = 0U;
	cdcl_maintenanceRules_assignableVars = 1U;
	cdcl_maintenanceRules_targetLearned = 1U;
	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = ONE;
}

void cdcl_maintenanceRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing maintenance rule (MR) plug-in...\n");
	#endif
	cdcl_maintenanceRule_maintain  = &cdcl_maintenanceRule_null_maintain;
	cdcl_maintenanceRule_prepare   = &cdcl_maintenanceRule_null_prepare;
	cdcl_maintenanceRule_check     = &cdcl_maintenanceRule_null_check;
	cdcl_maintenanceRule_incAct    = &cdcl_maintenanceRule_null_incAct;
	cdcl_maintenanceRule_rescale   = &cdcl_maintenanceRule_null_rescale;
	cdcl_maintenanceRule_bump      = &cdcl_maintenanceRule_null_bump;

	//Dispose MR data-structures here.
	cdcl_maintenanceRules_numPerformed = 0U;
	cdcl_maintenanceRules_assignableVars = 1U;
	cdcl_maintenanceRules_targetLearned = 1U;
	cdcl_maintenanceRules_currentActInc = ONE;
	cdcl_maintenanceRules_maxActivity = ONE;
}
