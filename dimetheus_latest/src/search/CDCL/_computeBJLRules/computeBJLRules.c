/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "computeBJLRules.h"

void cdcl_extern_computeBJLRules_switch(int32_t switchTo){
	//Use to switch the compute BJ rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_computeBJLRules_disposePlugin();
	//We override the parameter to use the new computation rule indicated.
	param_cdclComputeBJLRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_computeBJLRules_initPlugin();
}

void cdcl_computeBJLRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting compute back-jump level rule (CBJLR) plug-in...\n");
	#endif
	cdcl_computeBJLRule_compute	= &cdcl_computeBJLRule_firstassert_compute;
	cdcl_computeBJLRule_prepare = &cdcl_computeBJLRule_firstassert_prepare;

	//Reset CBJLR data-structures here.
}

void cdcl_computeBJLRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the compute back-jump level rule... ");
	#endif
	//Pick the compute back-jump level rule.
	if (param_cdclComputeBJLRule == CDCL_COMPUTEBJLRULE_FIRSTASSERT){
		#ifdef VERBOSE_CDCL
		printf("FIRSTASSERT");
		#endif
		cdcl_computeBJLRule_compute	= &cdcl_computeBJLRule_firstassert_compute;
		cdcl_computeBJLRule_prepare = &cdcl_computeBJLRule_firstassert_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize CBJLR data-structures here.
}

void cdcl_computeBJLRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing compute back-jump level rule (CBJLR) plug-in...\n");
	#endif
	cdcl_computeBJLRule_compute	= &cdcl_computeBJLRule_firstassert_compute;
	cdcl_computeBJLRule_prepare = &cdcl_computeBJLRule_firstassert_prepare;

	//Dispose CBJLR data-structures here.
}


