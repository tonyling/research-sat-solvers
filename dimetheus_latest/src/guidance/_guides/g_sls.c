/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_sls_printHelp(){
	printf("c      %-3d: SLS\n", GUIDE_SLS);
    printf("c           Behavior: Perform Stochastic Local Search.\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST] ADAPT[ITEADAPTER] PREP[PRE SATELITE] SEARCH[SLS]\n");
    printf("c           Enforces: -searchStrategy %d, -adaptSearchStrategy 0,\n"
    	   "c                     -inpStrategy %d, -adaptInpStrategy 0, -adaptInpStrategyParams 0,\n"
    	   "c                     -slsAssInitRule %d, -adaptSlsAIR 0, -adaptSlsAIRParams 0,"
    	   "c                     -slsMaxTries 0\n",
    	   	   	      	   	   	  SEARCH_STRATEGY_SLS,
    	   	   	      	   	   	  INP_STRATEGY_NULL,
    	   	   	      	   	   	  SLS_ASSINITRULE_RANDOM);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n"
    	   "c                     -slsMaxTries 0\n");
}

void guide_sls_apply(int argc, char** argv){
	//This guide will enforce stochastic local search.

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

	//Third, check and enforce parameter settings for this guide.
	//INP
	GUIDANCE_CHECKANDSET(guidance_inpStrategy, param_inpStrategy, INP_STRATEGY_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategy, param_adaptInpStrategy, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategyParams, param_adaptInpStrategyParams, 0);

	//SEARCH
	GUIDANCE_CHECKANDSET(guidance_searchStrategy, param_searchStrategy, SEARCH_STRATEGY_SLS);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategy, param_adaptSearchStrategy, 0);

	//SLS
	GUIDANCE_CHECKANDSET(guidance_slsMaxTries, param_slsMaxTries, 0);
	GUIDANCE_CHECKANDSET(guidance_slsAssInitRule, param_slsAssInitRule, SLS_ASSINITRULE_RANDOM);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIR, param_adaptSlsAIR, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptSlsAIRParams, param_adaptSlsAIRParams, 0);

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_sls_enforceAfterAdaptation(){
	param_inpStrategy = INP_STRATEGY_NULL;
	param_searchStrategy = SEARCH_STRATEGY_SLS;
	param_slsMaxTries = 0;
	param_slsAssInitRule = SLS_ASSINITRULE_RANDOM;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   inpStrategy = INP_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_SLS\n");
	printf("c     GUIDANCE:   slsMaxTries = 0\n");
	printf("c     GUIDANCE:   slsAssInitRule = SLS_ASSINITRULE_RANDOM\n");
	#endif
}
