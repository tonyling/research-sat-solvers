/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "updateRules.h"

void mp_extern_updateRules_switch(int32_t switchTo){
	//Use to switch the update rule without re-initializing the whole MP module. We first dispose the old plug-in.
	mp_updateRules_disposePlugin();
	//We override the parameter to use the new update rule indicated.
	param_mpUpdateRule = switchTo;
	//We re-initialize the plug-in.
	mp_updateRules_initPlugin();
}

void mp_updateRules_resetPlugin(){
	#ifdef VERBOSE_MP
	printf("c     MP: Resetting update rule plug-in...\n");
	#endif
	mp_updateRule_msgUpdate 	= &mp_updateRule_L0_BP_msgUpdate;
	mp_updateRule_prepare		= &mp_updateRule_L0_BP_prepare;
	mp_updateRule_computeBias	= &mp_updateRule_L0_BP_computeBias;

	mp_clauseDeltas = NULL;
}

void mp_updateRules_initPlugin(){
	#ifdef VERBOSE_MP
	printf("c     MP: Picking update rule...\n");
	#endif
	//Pick the message update rule.
	if (param_mpUpdateRule == MP_UPDATERULE_L0_BP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L0-BP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L0_BP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L0_BP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L0_BP_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L0_SP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L0-SP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L0_SP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L0_SP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L0_SP_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L0_EMBPG){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L0-EMBPG update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L0_EMBPG_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L0_EMBPG_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L0_EMBPG_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L0_EMSPG){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L0-EMSPG update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L0_EMSPG_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L0_EMSPG_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L0_EMSPG_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L1I_RHOSP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L1I-RHOSP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L1I_RHOSP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L1I_RHOSP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L1I_RHOSP_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L1I_RHOEMSPG){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L1I-RHOEMSPG update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L1I_RHOEMSPG_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L1I_RHOEMSPG_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L1I_RHOEMSPG_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L1I_SIGMAEMBPG){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L1I-SIGMAEMBPG update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L1I_SIGMAEMBPG_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L1I_SIGMAEMBPG_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L1I_SIGMAEMBPG_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L1I_SIGMAEMSPG){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L1I-SIGMAEMSPG update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L1I_SIGMAEMSPG_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L1I_SIGMAEMSPG_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L1I_SIGMAEMSPG_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L2I_RHOSIGMAPMP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L2I-RHOSIGMAPMP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L2I_RHOSIGMAPMP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L2I_RHOSIGMAPMP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L2I_RHOSIGMAPMP_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L2IM_RHOSIGMAPMP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L2IM-RHOSIGMAPMP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L2IM_RHOSIGMAPMP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L2IM_RHOSIGMAPMP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L2IM_RHOSIGMAPMP_computeBias;
	} else if (param_mpUpdateRule == MP_UPDATERULE_L2E_RHOSIGMAPMP){
		#ifdef VERBOSE_MP
		printf("c     MP: Using L2E-RHOSIGMAPMP update rule.\n");
		#endif
		mp_updateRule_msgUpdate 	= &mp_updateRule_L2E_RHOSIGMAPMP_msgUpdate;
		mp_updateRule_prepare		= &mp_updateRule_L2E_RHOSIGMAPMP_prepare;
		mp_updateRule_computeBias	= &mp_updateRule_L2E_RHOSIGMAPMP_computeBias;
	}

	mp_clauseDeltas = malloc(sizeof(float_ty)*BLOCKSIZE);
	if (mp_clauseDeltas == NULL){
		printf("c ERROR. MP was unable to initialize the memory for the message factors. Out of memory?\n");
		mp_returnCode = MP_ERROR;
		#ifdef COLLECTSTATS
		++stats_mp_return_ERROR;
		#endif
		return;
	}
}

void mp_updateRules_disposePlugin(){
	#ifdef VERBOSE_MP
	printf("c     MP: Disposing update rule plug-in...\n");
	#endif
	mp_updateRule_msgUpdate 	= &mp_updateRule_L0_BP_msgUpdate;
	mp_updateRule_prepare		= &mp_updateRule_L0_BP_prepare;
	mp_updateRule_computeBias	= &mp_updateRule_L0_BP_computeBias;

	if (mp_clauseDeltas != NULL){
		free(mp_clauseDeltas);
		mp_clauseDeltas = NULL;
	}
}
