/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef UPDATERULES_H_
#define UPDATERULES_H_

#include "../mp.h"

#define MP_UPDATERULE_MINVALUE 0
#define MP_UPDATERULE_MAXVALUE 10

#define MP_UPDATERULE_L0_BP 0				//Level 0: Belief Propagation (L0_BP).
#include "ur_l0_bp.h"

#define MP_UPDATERULE_L0_SP 1				//Level 0: Survey Propagation (L0_SP).
#include "ur_l0_sp.h"

#define MP_UPDATERULE_L0_EMBPG 2			//Level 0: Expectation Maximization BP Global (L0_EMBPG).
#include "ur_l0_embpg.h"

#define MP_UPDATERULE_L0_EMSPG 3			//Level 0: Expectation Maximization SP Global (L0_EMSPG).
#include "ur_l0_emspg.h"

#define MP_UPDATERULE_L1I_RHOSP 4			//Level 1 ISI: Rho Survey Propagation ISI-based (L1I_RHOSP).
#include "ur_l1i_rhosp.h"

#define MP_UPDATERULE_L1I_RHOEMSPG 5		//Level 1 ISI: Rho EMSPG ISI-based (L1I_RHOEMSPG).
#include "ur_l1i_rhoemspg.h"

#define MP_UPDATERULE_L1I_SIGMAEMBPG 6		//Level 1 ISI: Sigma EMBPG ISI-based (L1I_SIGMAEMBPG).
#include "ur_l1i_sigmaembpg.h"

#define MP_UPDATERULE_L1I_SIGMAEMSPG 7		//Level 1 ISI: Sigma EMSPG ISI-based (L1I_SIGMAEMSPG).
#include "ur_l1i_sigmaemspg.h"

#define MP_UPDATERULE_L2I_RHOSIGMAPMP 8		//Level 2 ISI: Rho Sigma PMP ISI-based (L2I_RHOSIGMAPMP).
#include "ur_l2i_rhosigmapmp.h"

#define MP_UPDATERULE_L2IM_RHOSIGMAPMP 9	//Level 2 ISI: Rho Sigma PMP ISI-based with force-field (L2IM_RHOSIGMAPMP).
#include "ur_l2im_rhosigmapmp.h"

#define MP_UPDATERULE_L2E_RHOSIGMAPMP 10	//Level 2 ISI: Rho Sigma PMP ISI-based with ePMPF (L2IE_RHOSIGMAPMP).
#include "ur_l2e_rhosigmapmp.h"

static inline void mp_updateRules_printHelp(){
	mp_updateRule_L0_BP_printHelp();
	mp_updateRule_L0_SP_printHelp();
	mp_updateRule_L0_EMBPG_printHelp();
	mp_updateRule_L0_EMSPG_printHelp();
	mp_updateRule_L1I_RHOSP_printHelp();
	mp_updateRule_L1I_RHOEMSPG_printHelp();
	mp_updateRule_L1I_SIGMAEMBPG_printHelp();
	mp_updateRule_L1I_SIGMAEMSPG_printHelp();
	mp_updateRule_L2I_RHOSIGMAPMP_printHelp();
	mp_updateRule_L2IM_RHOSIGMAPMP_printHelp();
	mp_updateRule_L2E_RHOSIGMAPMP_printHelp();
}

//Methods that are provided by this module.
static inline void mp_extern_updateRules_printNamesAndIDs(char* prefix){
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L0_BP %d\n", prefix, MP_UPDATERULE_L0_BP);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L0_SP %d\n", prefix, MP_UPDATERULE_L0_SP);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L0_EMBPG %d\n", prefix, MP_UPDATERULE_L0_EMBPG);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L0_EMSPG %d\n", prefix, MP_UPDATERULE_L0_EMSPG);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L1I_RHOSP %d\n", prefix, MP_UPDATERULE_L1I_RHOSP);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L1I_RHOEMSPG %d\n", prefix, MP_UPDATERULE_L1I_RHOEMSPG);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L1I_SIGMAEMBPG %d\n", prefix, MP_UPDATERULE_L1I_SIGMAEMBPG);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L1I_SIGMAEMSPG %d\n", prefix, MP_UPDATERULE_L1I_SIGMAEMSPG);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L2I_RHOSIGMAPMP %d\n", prefix, MP_UPDATERULE_L2I_RHOSIGMAPMP);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L2IM_RHOSIGMAPMP %d\n", prefix, MP_UPDATERULE_L2IM_RHOSIGMAPMP);
	printf("%s MP-UPDATE-RULES: MP_UPDATERULE_L2E_RHOSIGMAPMP %d\n", prefix, MP_UPDATERULE_L2E_RHOSIGMAPMP);
}

void (*mp_updateRule_prepare)();		//This points to the prepare methods of the message update rule used.
void (*mp_updateRule_msgUpdate)();		//This points to one of the message passing update rules that are available.
void (*mp_updateRule_computeBias)();	//This points to one of the message passing bias computations that are available.

//Methods to initialize this module (are being called by the one who wants to use the above methods).
void mp_extern_updateRules_switch(int32_t);//Use to switch the update rule without re-initializing the whole MP module.
void mp_updateRules_resetPlugin();
void mp_updateRules_initPlugin();
void mp_updateRules_disposePlugin();

#endif /* UPDATERULES_H_ */
