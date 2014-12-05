/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_hybridmsc_printHelp(){
	printf("c      %-3d: HYBRIDMSC\n", GUIDE_HYBRIDMSC);
    printf("c           Behavior: Performs MP and uses CDCL and SLS to simplify the formula and probe the search space.\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST] ADAPT[ITEADAPTER] PREP[PRE SATELITE] INP[IN] SEARCH[MP SLS CDCL]\n");
    printf("c           Enforces: -searchStrategy %d, -adaptSearchStrategy 0,\n"
    	   "c                     -mpUpdateRule %d, -adaptMpUR 0,\n"
    	   "c                     -slsAssInitRule %d, -adaptSlsAIR 0, -adaptSlsAIRParams 0\n"
    	   "c                     -cdclSelectVarRule %d, -adaptCdclSVR 0, -cdclSelectVarRandProb 0.0,\n"
    	   "c                     -cdclSelectVarActInit %d, -adaptCdclSVAI 0, -adaptCdclSVAIParams 0,\n"
    	   "c                     -cdclSelectDirRule %d, -adaptCdclSDR 0, -adaptCdclSDRParams 0,\n"
    	   "c                     -cdclSelectDirDirInit %d, -adaptCdclSDDI 0, -adaptCdclSDDIParams 0,\n"
    	   "c                     -cdclAbortRule %d, -adaptCdclAR 0, -adaptCdclARParams 0\n",
                                  SEARCH_STRATEGY_HYBRIDMSC,
                                  MP_UPDATERULE_L2E_RHOSIGMAPMP,
                                  SLS_ASSINITRULE_PROVIDED,
                                  CDCL_SELECTVARRULE_RRVSIDS,
                                  CDCL_SELECTVARRULE_VARACTINIT_PROVIDED,
                                  CDCL_SELECTDIRRULE_PHASESAVING,
                                  CDCL_SELECTDIRRULE_DIRINIT_PROVIDED,
                                  CDCL_ABORTRULE_HYBRIDMSC);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n");
    printf("c                     -slsTryRestartRule %d, -slsMaxTries 0, -slsMaxFlipsA 0, -slsMaxFlipsB 0\n",
                                  SLS_TRYRESTARTRULE_SINGLEUNLIMITED);

}

void guide_hybridmsc_apply(int argc, char** argv){
	//This guide will enforce the application of MP (rho-sigma-PMP-i) in order to provide biases. These biases are then used
	//by a CDCL solver to simplify the formula (and possibly learn new clauses). Should the remaining formula be paramagnetic
	//or in case the CDCL learned too many new things, we call the SLS solver to try solving the current remaining formula.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif

	//First, acquire all parameter settings and adaptation capabilities.
	guidance_intern_getAllParams(argc, argv);

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Enforcing and setting defaults...\n");
	#endif

	//Second, apply default values, if possible.
	//CLASSIFY
	GUIDANCE_APPLYDEFAULT(guidance_classifyStrategy, param_classifyStrategy, CLASSIFY_STRATEGY_RANDFOR);

	//ADAPT
	GUIDANCE_APPLYDEFAULT(guidance_adaptStrategy, param_adaptStrategy, ADAPT_STRATEGY_ITEGENERIC);

	//SLS
	GUIDANCE_APPLYDEFAULT(guidance_slsTryRestartRule, param_slsTryRestartRule, SLS_TRYRESTARTRULE_CONST);
	GUIDANCE_APPLYDEFAULT(guidance_slsMaxTries, param_slsMaxTries, 1);
	GUIDANCE_APPLYDEFAULT(guidance_slsMaxFlipsA, param_slsMaxFlipsA, 30);
	GUIDANCE_APPLYDEFAULT(guidance_slsMaxFlipsB, param_slsMaxFlipsB, 2);

	//Third, check and enforce parameter settings for this guide.
	//SEARCH
	GUIDANCE_CHECKANDSET(guidance_searchStrategy, param_searchStrategy, SEARCH_STRATEGY_HYBRIDMSC);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategy, param_adaptSearchStrategy, 0);

	//MP
	GUIDANCE_CHECKANDSET(guidance_mpUpdateRule, param_mpUpdateRule, MP_UPDATERULE_L2E_RHOSIGMAPMP);
	GUIDANCE_CHECKANDSET(guidance_adaptMpUR, param_adaptMpUR, 0);

	//SLS
	//SLS - AIR
	GUIDANCE_CHECKANDSET(guidance_slsAssInitRule, param_slsAssInitRule, SLS_ASSINITRULE_PROVIDED);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIR, param_adaptSlsAIR, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIRParams, param_adaptSlsAIRParams, 0);

	//SLS - TRR
	GUIDANCE_FORBID(guidance_slsTryRestartRule, param_slsTryRestartRule, SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
	GUIDANCE_FORBID(guidance_slsMaxTries, param_slsMaxTries, 0);
	GUIDANCE_FORBID(guidance_slsMaxFlipsA, param_slsMaxFlipsA, 0);
	GUIDANCE_FORBID(guidance_slsMaxFlipsB, param_slsMaxFlipsB, 0);

	//CDCL
	//CDCL - SVR
	GUIDANCE_CHECKANDSET(guidance_cdclSelectVarRule, param_cdclSelectVarRule, CDCL_SELECTVARRULE_RRVSIDS);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSVR, param_adaptCdclSVR, 0);
	GUIDANCE_CHECKANDSET(guidance_cdclSelectVarRandProb, param_cdclSelectVarRandProb, ZERO);

	//CDCL - SVAI
	GUIDANCE_CHECKANDSET(guidance_cdclSelectVarActInit, param_cdclSelectVarActInit, CDCL_SELECTVARRULE_VARACTINIT_PROVIDED);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSVAI, param_adaptCdclSVAI, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSVAIParams, param_adaptCdclSVAIParams, 0);

	//CDCL - SDR
	GUIDANCE_CHECKANDSET(guidance_cdclSelectDirRule, param_cdclSelectDirRule, CDCL_SELECTDIRRULE_PHASESAVING);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSDR, param_adaptCdclSDR, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSDRParams, param_adaptCdclSDRParams, 0);

	//CDCL - SDDI
	GUIDANCE_CHECKANDSET(guidance_cdclSelectDirDirInit, param_cdclSelectDirDirInit, CDCL_SELECTDIRRULE_DIRINIT_PROVIDED);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSDDI, param_adaptCdclSDDI, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSDDIParams, param_adaptCdclSDDIParams, 0);

	//CDCL - AR
	GUIDANCE_CHECKANDSET(guidance_cdclAbortRule, param_cdclAbortRule, CDCL_ABORTRULE_HYBRIDMSC);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclAR, param_adaptCdclAR, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclARParams, param_adaptCdclARParams, 0);

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_hybridmsc_enforceAfterAdaptation(){
	param_searchStrategy = SEARCH_STRATEGY_HYBRIDMSC;
	param_mpUpdateRule = MP_UPDATERULE_L2E_RHOSIGMAPMP;
	param_slsAssInitRule = SLS_ASSINITRULE_PROVIDED;
	param_cdclSelectVarRule = CDCL_SELECTVARRULE_RRVSIDS;
	param_cdclSelectVarRandProb = ZERO;
	param_cdclSelectVarActInit = CDCL_SELECTVARRULE_VARACTINIT_PROVIDED;
	param_cdclSelectDirRule = CDCL_SELECTDIRRULE_PHASESAVING;
	param_cdclSelectDirDirInit = CDCL_SELECTDIRRULE_DIRINIT_PROVIDED;
	param_cdclAbortRule = CDCL_ABORTRULE_HYBRIDMSC;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_HYBRIDMSC\n");
	printf("c     GUIDANCE:   mpUpdateRule = MP_UPDATERULE_L2E_RHOSIGMAPMP\n");
	printf("c     GUIDANCE:   slsAssInitRule = SLS_ASSINITRULE_PROVIDED\n");
	printf("c     GUIDANCE:   cdclSelectVarRule = CDCL_SELECTVARRULE_RRVSIDS\n");
	printf("c     GUIDANCE:   cdclSelectVarRandProb = ZERO\n");
	printf("c     GUIDANCE:   cdclSelectVarActInit = CDCL_SELECTVARRULE_VARACTINIT_PROVIDED\n");
	printf("c     GUIDANCE:   cdclSelectDirRule = CDCL_SELECTDIRRULE_PHASESAVING\n");
	printf("c     GUIDANCE:   cdclSelectDirDirInit = CDCL_SELECTDIRRULE_DIRINIT_PROVIDED\n");
	printf("c     GUIDANCE:   cdclAbortRule = CDCL_ABORTRULE_HYBRIDMSC\n");
	#endif
}
