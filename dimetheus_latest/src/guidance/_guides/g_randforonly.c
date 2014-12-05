/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_randforonly_printHelp(){
	printf("c      %-3d: RANDFORONLY\n", GUIDE_RANDFORONLY);
    printf("c           Behavior: Just compute the formula attributes.\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST]\n");
    printf("c           Enforces: -classifyStrategy %d,\n"
    	   "c                     -adaptStrategy %d,\n"
    	   "c                     -prepStrategy %d, -adaptPrepStrategy 0, -adaptPrepStrategyParams 0,\n"
    	   "c                     -inpStrategy %d, -adaptInpStrategy 0, -adaptInpStrategyParams 0,\n"
    	   "c                     -searchStrategy %d, -adaptSearchStrategy 0, -adaptSearchStrategyParams 0\n",
    	                          CLASSIFY_STRATEGY_RANDFOR,
    	                          ADAPT_STRATEGY_NULL,
                                  PREP_STRATEGY_NULL,
                                  INP_STRATEGY_NULL,
                                  SEARCH_STRATEGY_NULL);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n");
}

void guide_randforonly_apply(int argc, char** argv){
	//This guide will enforce that only attribute computation and random forest classification is performed.

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

	//Third, check and enforce parameter settings for this guide.
	//CLASSIFY
	GUIDANCE_CHECKANDSET(guidance_classifyStrategy, param_classifyStrategy, CLASSIFY_STRATEGY_RANDFOR);

	//ADAPT
	GUIDANCE_CHECKANDSET(guidance_adaptStrategy, param_adaptStrategy, ADAPT_STRATEGY_NULL);

	//PREP
	GUIDANCE_CHECKANDSET(guidance_prepStrategy, param_prepStrategy, PREP_STRATEGY_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptPrepStrategy, param_adaptPrepStrategy, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptPrepStrategyParams, param_adaptPrepStrategyParams, 0);

	//INP
	GUIDANCE_CHECKANDSET(guidance_inpStrategy, param_inpStrategy, INP_STRATEGY_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategy, param_adaptInpStrategy, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategyParams, param_adaptInpStrategyParams, 0);

	//SEARCH
	GUIDANCE_CHECKANDSET(guidance_searchStrategy, param_searchStrategy, SEARCH_STRATEGY_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategy, param_adaptSearchStrategy, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategyParams, param_adaptSearchStrategyParams, 0);

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_randforonly_enforceAfterAdaptation(){
	param_classifyStrategy = CLASSIFY_STRATEGY_RANDFOR;
	param_adaptStrategy = ADAPT_STRATEGY_NULL;
	param_prepStrategy = PREP_STRATEGY_NULL;
	param_inpStrategy = INP_STRATEGY_NULL;
	param_searchStrategy = SEARCH_STRATEGY_NULL;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   classifyStrategy = CLASSIFY_STRATEGY_RANDFOR\n");
	printf("c     GUIDANCE:   adaptStrategy = ADAPT_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   prepStrategy = PREP_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   inpStrategy = INP_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_NULL\n");
	#endif
}
