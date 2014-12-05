/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "restartRules.h"

void cdcl_extern_restartRules_switch(int32_t switchTo){
	//Use to switch the restart rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_restartRules_disposePlugin();
	//We override the parameter to use the new restart rule indicated.
	param_cdclRestartRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_restartRules_initPlugin();
}

void cdcl_restartRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting restart rule (RR) plug-in...\n");
	#endif
	cdcl_restartRule_check		    = &cdcl_restartRule_null_check;
	cdcl_restartRule_prepare	    = &cdcl_restartRule_null_prepare;
	cdcl_restartRule_incAgility     = &cdcl_restartRule_null_incAgility;
	cdcl_restartRule_signalBackjump = &cdcl_restartRule_null_signalBackjump;

	//Reset RR data-structures here.
	cdcl_restartRules_currentAgiInc = ONE;
	cdcl_restartRules_maxAgility = ONE;
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight = 0;
}

void cdcl_restartRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the restart rule... ");
	#endif
	//Pick the restart rule.
	if (param_cdclRestartRule == CDCL_RESTARTRULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_null_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_null_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_null_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_null_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_LUBY){
		#ifdef VERBOSE_CDCL
		printf("LUBY");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_luby_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_luby_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_luby_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_luby_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_LUBYAGILITY){
		#ifdef VERBOSE_CDCL
		printf("LUBYAGILITY");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_lubyagility_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_lubyagility_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_lubyagility_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_lubyagility_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_LUBYSTAG){
		#ifdef VERBOSE_CDCL
		printf("LUBYSTAG");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_lubystag_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_lubystag_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_lubystag_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_lubystag_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_INNEROUTER){
		#ifdef VERBOSE_CDCL
		printf("INNEROUTER");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_innerouter_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_innerouter_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_innerouter_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_innerouter_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_INNEROUTERAGILITY){
		#ifdef VERBOSE_CDCL
		printf("INNEROUTERAGILITY");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_innerouteragility_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_innerouteragility_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_innerouteragility_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_innerouteragility_signalBackjump;
	} else if (param_cdclRestartRule == CDCL_RESTARTRULE_INNEROUTERAGILITY){
		#ifdef VERBOSE_CDCL
		printf("INNEROUTERSTAG");
		#endif
		cdcl_restartRule_check   	    = &cdcl_restartRule_innerouterstag_check;
		cdcl_restartRule_prepare 	    = &cdcl_restartRule_innerouterstag_prepare;
		cdcl_restartRule_incAgility     = &cdcl_restartRule_innerouterstag_incAgility;
		cdcl_restartRule_signalBackjump = &cdcl_restartRule_innerouterstag_signalBackjump;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize RR data-structures here.
	cdcl_restartRules_currentAgiInc = ONE;
	cdcl_restartRules_maxAgility = ONE;
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight = 0;
}

void cdcl_restartRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing restart rule (RR) plug-in...\n");
	#endif
	cdcl_restartRule_check   	    = &cdcl_restartRule_null_check;
	cdcl_restartRule_prepare 	    = &cdcl_restartRule_null_prepare;
	cdcl_restartRule_incAgility     = &cdcl_restartRule_null_incAgility;
	cdcl_restartRule_signalBackjump = &cdcl_restartRule_null_signalBackjump;

	//Dispose RR data-structures here.
	cdcl_restartRules_currentAgiInc = ONE;
	cdcl_restartRules_maxAgility = ONE;
	cdcl_restartRules_backjumpDistance = 0;
	cdcl_restartRules_backjumpHeight = 0;
}

