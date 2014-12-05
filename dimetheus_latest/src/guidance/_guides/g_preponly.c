/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_preponly_printHelp(){
	printf("c      %-3d: PREPONLY\n", GUIDE_PREPONLY);
    printf("c           Behavior: Just perform preprocessing.\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST] ADAPT[ITEADAPTER] PREP[PRE SATELITE]\n");
    printf("c           Enforces: -inpStrategy %d, -adaptInpStrategy 0, -adaptInpStrategyParams 0,\n"
    	   "c                     -searchStrategy %d, -adaptSearchStrategy 0, -adaptSearchStrategyParams 0\n",
                                  INP_STRATEGY_NULL,
                                  SEARCH_STRATEGY_NULL);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n");
    printf("c                     -prepStrategy %d\n",
    		                      PREP_STRATEGY_NULL);
}

void guide_preponly_apply(int argc, char** argv){
	//This guide will enforce that only preprocessing is performed.

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
	//PREP
	GUIDANCE_FORBID(guidance_prepStrategy, param_prepStrategy, PREP_STRATEGY_NULL);

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

void guide_preponly_enforceAfterAdaptation(){
	param_inpStrategy = INP_STRATEGY_NULL;
	param_searchStrategy = SEARCH_STRATEGY_NULL;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   inpStrategy = INP_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_NULL\n");
	#endif
}
