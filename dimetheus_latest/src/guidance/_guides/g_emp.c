/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_emp_printHelp(){
	printf("c      %-3d: EMP\n", GUIDE_EMP);
    printf("c           Behavior: Performs SLS-based MPWRL (MP weight reinforcement learning).\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST] ADAPT[ITEADAPTER] PREP[PRE SATELITE] SEARCH[MP SLS]\n");
    printf("c           Enforces: -searchStrategy %d, -adaptSearchStrategy 0,\n"
    	   "c                     -slsAssInitRule %d, -adaptSlsAIR 0, -adaptSlsAIRParams 0,\n"
    	   "c                     -slsTryRestartRule %d, -adaptSlsTRR 0,\n"
    	   "c                     -mpUpdateRule %d, -adaptMpUR 0\n",
    	   	   	   	   	      	  SEARCH_STRATEGY_EMP,
    	   	   	   	   	      	  SLS_ASSINITRULE_PROVIDED,
    	   	   	   	   	      	  SLS_TRYRESTARTRULE_CONST,
    	   	   	   	   	   	      MP_UPDATERULE_L2E_RHOSIGMAPMP);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n");
    printf("c                     -slsTryRestartRule %d, -slsAssInitRule %d, -slsMaxTries 0 -slsMaxFlipsA 0.0 -slsMaxFlipsB 0.0\n",
    		                      SLS_TRYRESTARTRULE_CONST, SLS_ASSINITRULE_PROVIDED);


}

void guide_emp_apply(int argc, char** argv){
	//This guide will enforce the rho-sigma PMP guided SLS. It enforces that MP is performed with the L2 heuristic and
	//it ensures that the SLS picks exactly the provided biases and phases for initializing its starting assignment.
	//It will also monitor which variables are flipped by the SLS, and will enforce those not flipped in a CDCL way.
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
	GUIDANCE_APPLYDEFAULT(guidance_slsMaxFlipsA, param_slsMaxFlipsA, 30);
	GUIDANCE_APPLYDEFAULT(guidance_slsMaxFlipsB, param_slsMaxFlipsB, 2);

	//Third, check and enforce parameter settings for this guide.
	//SEARCH
	GUIDANCE_CHECKANDSET(guidance_searchStrategy, param_searchStrategy, SEARCH_STRATEGY_EMP);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategy, param_adaptSearchStrategy, 0);

	//MP
	GUIDANCE_CHECKANDSET(guidance_mpUpdateRule, param_mpUpdateRule, MP_UPDATERULE_L2E_RHOSIGMAPMP);
	GUIDANCE_CHECKANDSET(guidance_adaptMpUR, param_adaptMpUR, 0);

	//SLS
	GUIDANCE_CHECKANDSET(guidance_slsAssInitRule, param_slsAssInitRule, SLS_ASSINITRULE_PROVIDED);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIR, param_adaptSlsAIR, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIRParams, param_adaptSlsAIRParams, 0);

	GUIDANCE_CHECKANDSET(guidance_slsTryRestartRule, param_slsTryRestartRule, SLS_TRYRESTARTRULE_CONST);
	GUIDANCE_CHECKANDSET(guidance_slsMaxTries, param_slsMaxTries, 1);

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_emp_enforceAfterAdaptation(){
	param_searchStrategy = SEARCH_STRATEGY_EMP;
	param_mpUpdateRule = MP_UPDATERULE_L2E_RHOSIGMAPMP;
	param_slsAssInitRule = SLS_ASSINITRULE_PROVIDED;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_EMP\n");
	printf("c     GUIDANCE:   mpUpdateRule = MP_UPDATERULE_L2E_RHOSIGMAPMP\n");
	printf("c     GUIDANCE:   slsAssInitRule = SLS_ASSINITRULE_PROVIDED\n");
	#endif
}

